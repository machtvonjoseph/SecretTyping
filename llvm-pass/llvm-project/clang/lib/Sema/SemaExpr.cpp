//===--- SemaExpr.cpp - Semantic Analysis for Expressions -----------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
//  This file implements semantic analysis for expressions.
//
//===----------------------------------------------------------------------===//

#include "TreeTransform.h"
#include "UsedDeclVisitor.h"
#include "clang/AST/ASTConsumer.h"
#include "clang/AST/ASTContext.h"
#include "clang/AST/ASTLambda.h"
#include "clang/AST/ASTMutationListener.h"
#include "clang/AST/CXXInheritance.h"
#include "clang/AST/DeclObjC.h"
#include "clang/AST/DeclTemplate.h"
#include "clang/AST/EvaluatedExprVisitor.h"
#include "clang/AST/Expr.h"
#include "clang/AST/ExprCXX.h"
#include "clang/AST/ExprObjC.h"
#include "clang/AST/ExprOpenMP.h"
#include "clang/AST/OperationKinds.h"
#include "clang/AST/ParentMapContext.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/AST/Type.h"
#include "clang/AST/TypeLoc.h"
#include "clang/Basic/Builtins.h"
#include "clang/Basic/DiagnosticSema.h"
#include "clang/Basic/PartialDiagnostic.h"
#include "clang/Basic/SourceManager.h"
#include "clang/Basic/Specifiers.h"
#include "clang/Basic/TargetInfo.h"
#include "clang/Lex/LiteralSupport.h"
#include "clang/Lex/Preprocessor.h"
#include "clang/Sema/AnalysisBasedWarnings.h"
#include "clang/Sema/DeclSpec.h"
#include "clang/Sema/DelayedDiagnostic.h"
#include "clang/Sema/Designator.h"
#include "clang/Sema/EnterExpressionEvaluationContext.h"
#include "clang/Sema/Initialization.h"
#include "clang/Sema/Lookup.h"
#include "clang/Sema/Overload.h"
#include "clang/Sema/ParsedTemplate.h"
#include "clang/Sema/Scope.h"
#include "clang/Sema/ScopeInfo.h"
#include "clang/Sema/SemaFixItUtils.h"
#include "clang/Sema/SemaInternal.h"
#include "clang/Sema/Template.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/ADT/StringExtras.h"
#include "llvm/Support/Casting.h"
#include "llvm/Support/ConvertUTF.h"
#include "llvm/Support/SaveAndRestore.h"
#include "llvm/Support/TypeSize.h"
#include <optional>

using namespace clang;
using namespace sema;

/// Determine whether the use of this declaration is valid, without
/// emitting diagnostics.
bool Sema::CanUseDecl(NamedDecl *D, bool TreatUnavailableAsInvalid) {
  // See if this is an auto-typed variable whose initializer we are parsing.
  if (ParsingInitForAutoVars.count(D))
    return false;

  // See if this is a deleted function.
  if (FunctionDecl *FD = dyn_cast<FunctionDecl>(D)) {
    if (FD->isDeleted())
      return false;

    // If the function has a deduced return type, and we can't deduce it,
    // then we can't use it either.
    if (getLangOpts().CPlusPlus14 && FD->getReturnType()->isUndeducedType() &&
        DeduceReturnType(FD, SourceLocation(), /*Diagnose*/ false))
      return false;

    // See if this is an aligned allocation/deallocation function that is
    // unavailable.
    if (TreatUnavailableAsInvalid &&
        isUnavailableAlignedAllocationFunction(*FD))
      return false;
  }

  // See if this function is unavailable.
  if (TreatUnavailableAsInvalid && D->getAvailability() == AR_Unavailable &&
      cast<Decl>(CurContext)->getAvailability() != AR_Unavailable)
    return false;

  if (isa<UnresolvedUsingIfExistsDecl>(D))
    return false;

  return true;
}

static void DiagnoseUnusedOfDecl(Sema &S, NamedDecl *D, SourceLocation Loc) {
  // Warn if this is used but marked unused.
  if (const auto *A = D->getAttr<UnusedAttr>()) {
    // [[maybe_unused]] should not diagnose uses, but __attribute__((unused))
    // should diagnose them.
    if (A->getSemanticSpelling() != UnusedAttr::CXX11_maybe_unused &&
        A->getSemanticSpelling() != UnusedAttr::C23_maybe_unused) {
      const Decl *DC = cast_or_null<Decl>(S.getCurObjCLexicalContext());
      if (DC && !DC->hasAttr<UnusedAttr>())
        S.Diag(Loc, diag::warn_used_but_marked_unused) << D;
    }
  }
}

/// Emit a note explaining that this function is deleted.
void Sema::NoteDeletedFunction(FunctionDecl *Decl) {
  assert(Decl && Decl->isDeleted());

  if (Decl->isDefaulted()) {
    // If the method was explicitly defaulted, point at that declaration.
    if (!Decl->isImplicit())
      Diag(Decl->getLocation(), diag::note_implicitly_deleted);

    // Try to diagnose why this special member function was implicitly
    // deleted. This might fail, if that reason no longer applies.
    DiagnoseDeletedDefaultedFunction(Decl);
    return;
  }

  auto *Ctor = dyn_cast<CXXConstructorDecl>(Decl);
  if (Ctor && Ctor->isInheritingConstructor())
    return NoteDeletedInheritingConstructor(Ctor);

  Diag(Decl->getLocation(), diag::note_availability_specified_here)
    << Decl << 1;
}

/// Determine whether a FunctionDecl was ever declared with an
/// explicit storage class.
static bool hasAnyExplicitStorageClass(const FunctionDecl *D) {
  for (auto *I : D->redecls()) {
    if (I->getStorageClass() != SC_None)
      return true;
  }
  return false;
}

/// Check whether we're in an extern inline function and referring to a
/// variable or function with internal linkage (C11 6.7.4p3).
///
/// This is only a warning because we used to silently accept this code, but
/// in many cases it will not behave correctly. This is not enabled in C++ mode
/// because the restriction language is a bit weaker (C++11 [basic.def.odr]p6)
/// and so while there may still be user mistakes, most of the time we can't
/// prove that there are errors.
static void diagnoseUseOfInternalDeclInInlineFunction(Sema &S,
                                                      const NamedDecl *D,
                                                      SourceLocation Loc) {
  // This is disabled under C++; there are too many ways for this to fire in
  // contexts where the warning is a false positive, or where it is technically
  // correct but benign.
  if (S.getLangOpts().CPlusPlus)
    return;

  // Check if this is an inlined function or method.
  FunctionDecl *Current = S.getCurFunctionDecl();
  if (!Current)
    return;
  if (!Current->isInlined())
    return;
  if (!Current->isExternallyVisible())
    return;

  // Check if the decl has internal linkage.
  if (D->getFormalLinkage() != InternalLinkage)
    return;

  // Downgrade from ExtWarn to Extension if
  //  (1) the supposedly external inline function is in the main file,
  //      and probably won't be included anywhere else.
  //  (2) the thing we're referencing is a pure function.
  //  (3) the thing we're referencing is another inline function.
  // This last can give us false negatives, but it's better than warning on
  // wrappers for simple C library functions.
  const FunctionDecl *UsedFn = dyn_cast<FunctionDecl>(D);
  bool DowngradeWarning = S.getSourceManager().isInMainFile(Loc);
  if (!DowngradeWarning && UsedFn)
    DowngradeWarning = UsedFn->isInlined() || UsedFn->hasAttr<ConstAttr>();

  S.Diag(Loc, DowngradeWarning ? diag::ext_internal_in_extern_inline_quiet
                               : diag::ext_internal_in_extern_inline)
    << /*IsVar=*/!UsedFn << D;

  S.MaybeSuggestAddingStaticToDecl(Current);

  S.Diag(D->getCanonicalDecl()->getLocation(), diag::note_entity_declared_at)
      << D;
}

void Sema::MaybeSuggestAddingStaticToDecl(const FunctionDecl *Cur) {
  const FunctionDecl *First = Cur->getFirstDecl();

  // Suggest "static" on the function, if possible.
  if (!hasAnyExplicitStorageClass(First)) {
    SourceLocation DeclBegin = First->getSourceRange().getBegin();
    Diag(DeclBegin, diag::note_convert_inline_to_static)
      << Cur << FixItHint::CreateInsertion(DeclBegin, "static ");
  }
}

/// Determine whether the use of this declaration is valid, and
/// emit any corresponding diagnostics.
///
/// This routine diagnoses various problems with referencing
/// declarations that can occur when using a declaration. For example,
/// it might warn if a deprecated or unavailable declaration is being
/// used, or produce an error (and return true) if a C++0x deleted
/// function is being used.
///
/// \returns true if there was an error (this declaration cannot be
/// referenced), false otherwise.
///
bool Sema::DiagnoseUseOfDecl(NamedDecl *D, ArrayRef<SourceLocation> Locs,
                             const ObjCInterfaceDecl *UnknownObjCClass,
                             bool ObjCPropertyAccess,
                             bool AvoidPartialAvailabilityChecks,
                             ObjCInterfaceDecl *ClassReceiver,
                             bool SkipTrailingRequiresClause) {
  SourceLocation Loc = Locs.front();
  if (getLangOpts().CPlusPlus && isa<FunctionDecl>(D)) {
    // If there were any diagnostics suppressed by template argument deduction,
    // emit them now.
    auto Pos = SuppressedDiagnostics.find(D->getCanonicalDecl());
    if (Pos != SuppressedDiagnostics.end()) {
      for (const PartialDiagnosticAt &Suppressed : Pos->second)
        Diag(Suppressed.first, Suppressed.second);

      // Clear out the list of suppressed diagnostics, so that we don't emit
      // them again for this specialization. However, we don't obsolete this
      // entry from the table, because we want to avoid ever emitting these
      // diagnostics again.
      Pos->second.clear();
    }

    // C++ [basic.start.main]p3:
    //   The function 'main' shall not be used within a program.
    if (cast<FunctionDecl>(D)->isMain())
      Diag(Loc, diag::ext_main_used);

    diagnoseUnavailableAlignedAllocation(*cast<FunctionDecl>(D), Loc);
  }

  // See if this is an auto-typed variable whose initializer we are parsing.
  if (ParsingInitForAutoVars.count(D)) {
    if (isa<BindingDecl>(D)) {
      Diag(Loc, diag::err_binding_cannot_appear_in_own_initializer)
        << D->getDeclName();
    } else {
      Diag(Loc, diag::err_auto_variable_cannot_appear_in_own_initializer)
        << D->getDeclName() << cast<VarDecl>(D)->getType();
    }
    return true;
  }

  if (FunctionDecl *FD = dyn_cast<FunctionDecl>(D)) {
    // See if this is a deleted function.
    if (FD->isDeleted()) {
      auto *Ctor = dyn_cast<CXXConstructorDecl>(FD);
      if (Ctor && Ctor->isInheritingConstructor())
        Diag(Loc, diag::err_deleted_inherited_ctor_use)
            << Ctor->getParent()
            << Ctor->getInheritedConstructor().getConstructor()->getParent();
      else
        Diag(Loc, diag::err_deleted_function_use);
      NoteDeletedFunction(FD);
      return true;
    }

    // [expr.prim.id]p4
    //   A program that refers explicitly or implicitly to a function with a
    //   trailing requires-clause whose constraint-expression is not satisfied,
    //   other than to declare it, is ill-formed. [...]
    //
    // See if this is a function with constraints that need to be satisfied.
    // Check this before deducing the return type, as it might instantiate the
    // definition.
    if (!SkipTrailingRequiresClause && FD->getTrailingRequiresClause()) {
      ConstraintSatisfaction Satisfaction;
      if (CheckFunctionConstraints(FD, Satisfaction, Loc,
                                   /*ForOverloadResolution*/ true))
        // A diagnostic will have already been generated (non-constant
        // constraint expression, for example)
        return true;
      if (!Satisfaction.IsSatisfied) {
        Diag(Loc,
             diag::err_reference_to_function_with_unsatisfied_constraints)
            << D;
        DiagnoseUnsatisfiedConstraint(Satisfaction);
        return true;
      }
    }

    // If the function has a deduced return type, and we can't deduce it,
    // then we can't use it either.
    if (getLangOpts().CPlusPlus14 && FD->getReturnType()->isUndeducedType() &&
        DeduceReturnType(FD, Loc))
      return true;

    if (getLangOpts().CUDA && !CheckCUDACall(Loc, FD))
      return true;

  }

  if (auto *MD = dyn_cast<CXXMethodDecl>(D)) {
    // Lambdas are only default-constructible or assignable in C++2a onwards.
    if (MD->getParent()->isLambda() &&
        ((isa<CXXConstructorDecl>(MD) &&
          cast<CXXConstructorDecl>(MD)->isDefaultConstructor()) ||
         MD->isCopyAssignmentOperator() || MD->isMoveAssignmentOperator())) {
      Diag(Loc, diag::warn_cxx17_compat_lambda_def_ctor_assign)
        << !isa<CXXConstructorDecl>(MD);
    }
  }

  auto getReferencedObjCProp = [](const NamedDecl *D) ->
                                      const ObjCPropertyDecl * {
    if (const auto *MD = dyn_cast<ObjCMethodDecl>(D))
      return MD->findPropertyDecl();
    return nullptr;
  };
  if (const ObjCPropertyDecl *ObjCPDecl = getReferencedObjCProp(D)) {
    if (diagnoseArgIndependentDiagnoseIfAttrs(ObjCPDecl, Loc))
      return true;
  } else if (diagnoseArgIndependentDiagnoseIfAttrs(D, Loc)) {
      return true;
  }

  // [OpenMP 4.0], 2.15 declare reduction Directive, Restrictions
  // Only the variables omp_in and omp_out are allowed in the combiner.
  // Only the variables omp_priv and omp_orig are allowed in the
  // initializer-clause.
  auto *DRD = dyn_cast<OMPDeclareReductionDecl>(CurContext);
  if (LangOpts.OpenMP && DRD && !CurContext->containsDecl(D) &&
      isa<VarDecl>(D)) {
    Diag(Loc, diag::err_omp_wrong_var_in_declare_reduction)
        << getCurFunction()->HasOMPDeclareReductionCombiner;
    Diag(D->getLocation(), diag::note_entity_declared_at) << D;
    return true;
  }

  // [OpenMP 5.0], 2.19.7.3. declare mapper Directive, Restrictions
  //  List-items in map clauses on this construct may only refer to the declared
  //  variable var and entities that could be referenced by a procedure defined
  //  at the same location.
  // [OpenMP 5.2] Also allow iterator declared variables.
  if (LangOpts.OpenMP && isa<VarDecl>(D) &&
      !isOpenMPDeclareMapperVarDeclAllowed(cast<VarDecl>(D))) {
    Diag(Loc, diag::err_omp_declare_mapper_wrong_var)
        << getOpenMPDeclareMapperVarName();
    Diag(D->getLocation(), diag::note_entity_declared_at) << D;
    return true;
  }

  if (const auto *EmptyD = dyn_cast<UnresolvedUsingIfExistsDecl>(D)) {
    Diag(Loc, diag::err_use_of_empty_using_if_exists);
    Diag(EmptyD->getLocation(), diag::note_empty_using_if_exists_here);
    return true;
  }

  DiagnoseAvailabilityOfDecl(D, Locs, UnknownObjCClass, ObjCPropertyAccess,
                             AvoidPartialAvailabilityChecks, ClassReceiver);

  DiagnoseUnusedOfDecl(*this, D, Loc);

  diagnoseUseOfInternalDeclInInlineFunction(*this, D, Loc);

  if (D->hasAttr<AvailableOnlyInDefaultEvalMethodAttr>()) {
    if (getLangOpts().getFPEvalMethod() !=
            LangOptions::FPEvalMethodKind::FEM_UnsetOnCommandLine &&
        PP.getLastFPEvalPragmaLocation().isValid() &&
        PP.getCurrentFPEvalMethod() != getLangOpts().getFPEvalMethod())
      Diag(D->getLocation(),
           diag::err_type_available_only_in_default_eval_method)
          << D->getName();
  }

  if (auto *VD = dyn_cast<ValueDecl>(D))
    checkTypeSupport(VD->getType(), Loc, VD);

  if (LangOpts.SYCLIsDevice ||
      (LangOpts.OpenMP && LangOpts.OpenMPIsTargetDevice)) {
    if (!Context.getTargetInfo().isTLSSupported())
      if (const auto *VD = dyn_cast<VarDecl>(D))
        if (VD->getTLSKind() != VarDecl::TLS_None)
          targetDiag(*Locs.begin(), diag::err_thread_unsupported);
  }

  if (isa<ParmVarDecl>(D) && isa<RequiresExprBodyDecl>(D->getDeclContext()) &&
      !isUnevaluatedContext()) {
    // C++ [expr.prim.req.nested] p3
    //   A local parameter shall only appear as an unevaluated operand
    //   (Clause 8) within the constraint-expression.
    Diag(Loc, diag::err_requires_expr_parameter_referenced_in_evaluated_context)
        << D;
    Diag(D->getLocation(), diag::note_entity_declared_at) << D;
    return true;
  }

  return false;
}

/// DiagnoseSentinelCalls - This routine checks whether a call or
/// message-send is to a declaration with the sentinel attribute, and
/// if so, it checks that the requirements of the sentinel are
/// satisfied.
void Sema::DiagnoseSentinelCalls(NamedDecl *D, SourceLocation Loc,
                                 ArrayRef<Expr *> Args) {
  const SentinelAttr *attr = D->getAttr<SentinelAttr>();
  if (!attr)
    return;

  // The number of formal parameters of the declaration.
  unsigned numFormalParams;

  // The kind of declaration.  This is also an index into a %select in
  // the diagnostic.
  enum CalleeType { CT_Function, CT_Method, CT_Block } calleeType;

  if (ObjCMethodDecl *MD = dyn_cast<ObjCMethodDecl>(D)) {
    numFormalParams = MD->param_size();
    calleeType = CT_Method;
  } else if (FunctionDecl *FD = dyn_cast<FunctionDecl>(D)) {
    numFormalParams = FD->param_size();
    calleeType = CT_Function;
  } else if (isa<VarDecl>(D)) {
    QualType type = cast<ValueDecl>(D)->getType();
    const FunctionType *fn = nullptr;
    if (const PointerType *ptr = type->getAs<PointerType>()) {
      fn = ptr->getPointeeType()->getAs<FunctionType>();
      if (!fn) return;
      calleeType = CT_Function;
    } else if (const BlockPointerType *ptr = type->getAs<BlockPointerType>()) {
      fn = ptr->getPointeeType()->castAs<FunctionType>();
      calleeType = CT_Block;
    } else {
      return;
    }

    if (const FunctionProtoType *proto = dyn_cast<FunctionProtoType>(fn)) {
      numFormalParams = proto->getNumParams();
    } else {
      numFormalParams = 0;
    }
  } else {
    return;
  }

  // "nullPos" is the number of formal parameters at the end which
  // effectively count as part of the variadic arguments.  This is
  // useful if you would prefer to not have *any* formal parameters,
  // but the language forces you to have at least one.
  unsigned nullPos = attr->getNullPos();
  assert((nullPos == 0 || nullPos == 1) && "invalid null position on sentinel");
  numFormalParams = (nullPos > numFormalParams ? 0 : numFormalParams - nullPos);

  // The number of arguments which should follow the sentinel.
  unsigned numArgsAfterSentinel = attr->getSentinel();

  // If there aren't enough arguments for all the formal parameters,
  // the sentinel, and the args after the sentinel, complain.
  if (Args.size() < numFormalParams + numArgsAfterSentinel + 1) {
    Diag(Loc, diag::warn_not_enough_argument) << D->getDeclName();
    Diag(D->getLocation(), diag::note_sentinel_here) << int(calleeType);
    return;
  }

  // Otherwise, find the sentinel expression.
  Expr *sentinelExpr = Args[Args.size() - numArgsAfterSentinel - 1];
  if (!sentinelExpr) return;
  if (sentinelExpr->isValueDependent()) return;
  if (Context.isSentinelNullExpr(sentinelExpr)) return;

  // Pick a reasonable string to insert.  Optimistically use 'nil', 'nullptr',
  // or 'NULL' if those are actually defined in the context.  Only use
  // 'nil' for ObjC methods, where it's much more likely that the
  // variadic arguments form a list of object pointers.
  SourceLocation MissingNilLoc = getLocForEndOfToken(sentinelExpr->getEndLoc());
  std::string NullValue;
  if (calleeType == CT_Method && PP.isMacroDefined("nil"))
    NullValue = "nil";
  else if (getLangOpts().CPlusPlus11)
    NullValue = "nullptr";
  else if (PP.isMacroDefined("NULL"))
    NullValue = "NULL";
  else
    NullValue = "(void*) 0";

  if (MissingNilLoc.isInvalid())
    Diag(Loc, diag::warn_missing_sentinel) << int(calleeType);
  else
    Diag(MissingNilLoc, diag::warn_missing_sentinel)
      << int(calleeType)
      << FixItHint::CreateInsertion(MissingNilLoc, ", " + NullValue);
  Diag(D->getLocation(), diag::note_sentinel_here) << int(calleeType);
}

SourceRange Sema::getExprRange(Expr *E) const {
  return E ? E->getSourceRange() : SourceRange();
}

//===----------------------------------------------------------------------===//
//  Standard Promotions and Conversions
//===----------------------------------------------------------------------===//

/// DefaultFunctionArrayConversion (C99 6.3.2.1p3, C99 6.3.2.1p4).
ExprResult Sema::DefaultFunctionArrayConversion(Expr *E, bool Diagnose) {
  // Handle any placeholder expressions which made it here.
  if (E->hasPlaceholderType()) {
    ExprResult result = CheckPlaceholderExpr(E);
    if (result.isInvalid()) return ExprError();
    E = result.get();
  }

  QualType Ty = E->getType();
  assert(!Ty.isNull() && "DefaultFunctionArrayConversion - missing type");

  if (Ty->isFunctionType()) {
    if (auto *DRE = dyn_cast<DeclRefExpr>(E->IgnoreParenCasts()))
      if (auto *FD = dyn_cast<FunctionDecl>(DRE->getDecl()))
        if (!checkAddressOfFunctionIsAvailable(FD, Diagnose, E->getExprLoc()))
          return ExprError();

    E = ImpCastExprToType(E, Context.getPointerType(Ty),
                          CK_FunctionToPointerDecay).get();
  } else if (Ty->isArrayType()) {
    // In C90 mode, arrays only promote to pointers if the array expression is
    // an lvalue.  The relevant legalese is C90 6.2.2.1p3: "an lvalue that has
    // type 'array of type' is converted to an expression that has type 'pointer
    // to type'...".  In C99 this was changed to: C99 6.3.2.1p3: "an expression
    // that has type 'array of type' ...".  The relevant change is "an lvalue"
    // (C90) to "an expression" (C99).
    //
    // C++ 4.2p1:
    // An lvalue or rvalue of type "array of N T" or "array of unknown bound of
    // T" can be converted to an rvalue of type "pointer to T".
    //
    if (getLangOpts().C99 || getLangOpts().CPlusPlus || E->isLValue()) {
      ExprResult Res = ImpCastExprToType(E, Context.getArrayDecayedType(Ty),
                                         CK_ArrayToPointerDecay);
      if (Res.isInvalid())
        return ExprError();
      E = Res.get();
    }
  }
  return E;
}

static void CheckForNullPointerDereference(Sema &S, Expr *E) {
  // Check to see if we are dereferencing a null pointer.  If so,
  // and if not volatile-qualified, this is undefined behavior that the
  // optimizer will delete, so warn about it.  People sometimes try to use this
  // to get a deterministic trap and are surprised by clang's behavior.  This
  // only handles the pattern "*null", which is a very syntactic check.
  const auto *UO = dyn_cast<UnaryOperator>(E->IgnoreParenCasts());
  if (UO && UO->getOpcode() == UO_Deref &&
      UO->getSubExpr()->getType()->isPointerType()) {
    const LangAS AS =
        UO->getSubExpr()->getType()->getPointeeType().getAddressSpace();
    if ((!isTargetAddressSpace(AS) ||
         (isTargetAddressSpace(AS) && toTargetAddressSpace(AS) == 0)) &&
        UO->getSubExpr()->IgnoreParenCasts()->isNullPointerConstant(
            S.Context, Expr::NPC_ValueDependentIsNotNull) &&
        !UO->getType().isVolatileQualified()) {
      S.DiagRuntimeBehavior(UO->getOperatorLoc(), UO,
                            S.PDiag(diag::warn_indirection_through_null)
                                << UO->getSubExpr()->getSourceRange());
      S.DiagRuntimeBehavior(UO->getOperatorLoc(), UO,
                            S.PDiag(diag::note_indirection_through_null));
    }
  }
}

static void DiagnoseDirectIsaAccess(Sema &S, const ObjCIvarRefExpr *OIRE,
                                    SourceLocation AssignLoc,
                                    const Expr* RHS) {
  const ObjCIvarDecl *IV = OIRE->getDecl();
  if (!IV)
    return;

  DeclarationName MemberName = IV->getDeclName();
  IdentifierInfo *Member = MemberName.getAsIdentifierInfo();
  if (!Member || !Member->isStr("isa"))
    return;

  const Expr *Base = OIRE->getBase();
  QualType BaseType = Base->getType();
  if (OIRE->isArrow())
    BaseType = BaseType->getPointeeType();
  if (const ObjCObjectType *OTy = BaseType->getAs<ObjCObjectType>())
    if (ObjCInterfaceDecl *IDecl = OTy->getInterface()) {
      ObjCInterfaceDecl *ClassDeclared = nullptr;
      ObjCIvarDecl *IV = IDecl->lookupInstanceVariable(Member, ClassDeclared);
      if (!ClassDeclared->getSuperClass()
          && (*ClassDeclared->ivar_begin()) == IV) {
        if (RHS) {
          NamedDecl *ObjectSetClass =
            S.LookupSingleName(S.TUScope,
                               &S.Context.Idents.get("object_setClass"),
                               SourceLocation(), S.LookupOrdinaryName);
          if (ObjectSetClass) {
            SourceLocation RHSLocEnd = S.getLocForEndOfToken(RHS->getEndLoc());
            S.Diag(OIRE->getExprLoc(), diag::warn_objc_isa_assign)
                << FixItHint::CreateInsertion(OIRE->getBeginLoc(),
                                              "object_setClass(")
                << FixItHint::CreateReplacement(
                       SourceRange(OIRE->getOpLoc(), AssignLoc), ",")
                << FixItHint::CreateInsertion(RHSLocEnd, ")");
          }
          else
            S.Diag(OIRE->getLocation(), diag::warn_objc_isa_assign);
        } else {
          NamedDecl *ObjectGetClass =
            S.LookupSingleName(S.TUScope,
                               &S.Context.Idents.get("object_getClass"),
                               SourceLocation(), S.LookupOrdinaryName);
          if (ObjectGetClass)
            S.Diag(OIRE->getExprLoc(), diag::warn_objc_isa_use)
                << FixItHint::CreateInsertion(OIRE->getBeginLoc(),
                                              "object_getClass(")
                << FixItHint::CreateReplacement(
                       SourceRange(OIRE->getOpLoc(), OIRE->getEndLoc()), ")");
          else
            S.Diag(OIRE->getLocation(), diag::warn_objc_isa_use);
        }
        S.Diag(IV->getLocation(), diag::note_ivar_decl);
      }
    }
}

ExprResult Sema::DefaultLvalueConversion(Expr *E) {
  // Handle any placeholder expressions which made it here.
  if (E->hasPlaceholderType()) {
    ExprResult result = CheckPlaceholderExpr(E);
    if (result.isInvalid()) return ExprError();
    E = result.get();
  }

  // C++ [conv.lval]p1:
  //   A glvalue of a non-function, non-array type T can be
  //   converted to a prvalue.
  if (!E->isGLValue()) return E;

  QualType T = E->getType();
  assert(!T.isNull() && "r-value conversion on typeless expression?");

  // lvalue-to-rvalue conversion cannot be applied to function or array types.
  if (T->isFunctionType() || T->isArrayType())
    return E;

  // We don't want to throw lvalue-to-rvalue casts on top of
  // expressions of certain types in C++.
  if (getLangOpts().CPlusPlus &&
      (E->getType() == Context.OverloadTy ||
       T->isDependentType() ||
       T->isRecordType()))
    return E;

  // The C standard is actually really unclear on this point, and
  // DR106 tells us what the result should be but not why.  It's
  // generally best to say that void types just doesn't undergo
  // lvalue-to-rvalue at all.  Note that expressions of unqualified
  // 'void' type are never l-values, but qualified void can be.
  if (T->isVoidType())
    return E;

  // OpenCL usually rejects direct accesses to values of 'half' type.
  if (getLangOpts().OpenCL &&
      !getOpenCLOptions().isAvailableOption("cl_khr_fp16", getLangOpts()) &&
      T->isHalfType()) {
    Diag(E->getExprLoc(), diag::err_opencl_half_load_store)
      << 0 << T;
    return ExprError();
  }

  CheckForNullPointerDereference(*this, E);
  if (const ObjCIsaExpr *OISA = dyn_cast<ObjCIsaExpr>(E->IgnoreParenCasts())) {
    NamedDecl *ObjectGetClass = LookupSingleName(TUScope,
                                     &Context.Idents.get("object_getClass"),
                                     SourceLocation(), LookupOrdinaryName);
    if (ObjectGetClass)
      Diag(E->getExprLoc(), diag::warn_objc_isa_use)
          << FixItHint::CreateInsertion(OISA->getBeginLoc(), "object_getClass(")
          << FixItHint::CreateReplacement(
                 SourceRange(OISA->getOpLoc(), OISA->getIsaMemberLoc()), ")");
    else
      Diag(E->getExprLoc(), diag::warn_objc_isa_use);
  }
  else if (const ObjCIvarRefExpr *OIRE =
            dyn_cast<ObjCIvarRefExpr>(E->IgnoreParenCasts()))
    DiagnoseDirectIsaAccess(*this, OIRE, SourceLocation(), /* Expr*/nullptr);

  // C++ [conv.lval]p1:
  //   [...] If T is a non-class type, the type of the prvalue is the
  //   cv-unqualified version of T. Otherwise, the type of the
  //   rvalue is T.
  //
  // C99 6.3.2.1p2:
  //   If the lvalue has qualified type, the value has the unqualified
  //   version of the type of the lvalue; otherwise, the value has the
  //   type of the lvalue.
  if (T.hasQualifiers())
    T = T.getUnqualifiedType();

  // Under the MS ABI, lock down the inheritance model now.
  if (T->isMemberPointerType() &&
      Context.getTargetInfo().getCXXABI().isMicrosoft())
    (void)isCompleteType(E->getExprLoc(), T);

  ExprResult Res = CheckLValueToRValueConversionOperand(E);
  if (Res.isInvalid())
    return Res;
  E = Res.get();

  // Loading a __weak object implicitly retains the value, so we need a cleanup to
  // balance that.
  if (E->getType().getObjCLifetime() == Qualifiers::OCL_Weak)
    Cleanup.setExprNeedsCleanups(true);

  if (E->getType().isDestructedType() == QualType::DK_nontrivial_c_struct)
    Cleanup.setExprNeedsCleanups(true);

  // C++ [conv.lval]p3:
  //   If T is cv std::nullptr_t, the result is a null pointer constant.
  CastKind CK = T->isNullPtrType() ? CK_NullToPointer : CK_LValueToRValue;
  Res = ImplicitCastExpr::Create(Context, T, CK, E, nullptr, VK_PRValue,
                                 CurFPFeatureOverrides());

  // C11 6.3.2.1p2:
  //   ... if the lvalue has atomic type, the value has the non-atomic version
  //   of the type of the lvalue ...
  if (const AtomicType *Atomic = T->getAs<AtomicType>()) {
    T = Atomic->getValueType().getUnqualifiedType();
    Res = ImplicitCastExpr::Create(Context, T, CK_AtomicToNonAtomic, Res.get(),
                                   nullptr, VK_PRValue, FPOptionsOverride());
  }

  return Res;
}

ExprResult Sema::DefaultFunctionArrayLvalueConversion(Expr *E, bool Diagnose) {
  ExprResult Res = DefaultFunctionArrayConversion(E, Diagnose);
  if (Res.isInvalid())
    return ExprError();
  Res = DefaultLvalueConversion(Res.get());
  if (Res.isInvalid())
    return ExprError();
  return Res;
}

/// CallExprUnaryConversions - a special case of an unary conversion
/// performed on a function designator of a call expression.
ExprResult Sema::CallExprUnaryConversions(Expr *E) {
  QualType Ty = E->getType();
  ExprResult Res = E;
  // Only do implicit cast for a function type, but not for a pointer
  // to function type.
  if (Ty->isFunctionType()) {
    Res = ImpCastExprToType(E, Context.getPointerType(Ty),
                            CK_FunctionToPointerDecay);
    if (Res.isInvalid())
      return ExprError();
  }
  Res = DefaultLvalueConversion(Res.get());
  if (Res.isInvalid())
    return ExprError();
  return Res.get();
}

/// UsualUnaryConversions - Performs various conversions that are common to most
/// operators (C99 6.3). The conversions of array and function types are
/// sometimes suppressed. For example, the array->pointer conversion doesn't
/// apply if the array is an argument to the sizeof or address (&) operators.
/// In these instances, this routine should *not* be called.
ExprResult Sema::UsualUnaryConversions(Expr *E) {
  // First, convert to an r-value.
  ExprResult Res = DefaultFunctionArrayLvalueConversion(E);
  if (Res.isInvalid())
    return ExprError();
  E = Res.get();

  QualType Ty = E->getType();
  assert(!Ty.isNull() && "UsualUnaryConversions - missing type");

  LangOptions::FPEvalMethodKind EvalMethod = CurFPFeatures.getFPEvalMethod();
  if (EvalMethod != LangOptions::FEM_Source && Ty->isFloatingType() &&
      (getLangOpts().getFPEvalMethod() !=
           LangOptions::FPEvalMethodKind::FEM_UnsetOnCommandLine ||
       PP.getLastFPEvalPragmaLocation().isValid())) {
    switch (EvalMethod) {
    default:
      llvm_unreachable("Unrecognized float evaluation method");
      break;
    case LangOptions::FEM_UnsetOnCommandLine:
      llvm_unreachable("Float evaluation method should be set by now");
      break;
    case LangOptions::FEM_Double:
      if (Context.getFloatingTypeOrder(Context.DoubleTy, Ty) > 0)
        // Widen the expression to double.
        return Ty->isComplexType()
                   ? ImpCastExprToType(E,
                                       Context.getComplexType(Context.DoubleTy),
                                       CK_FloatingComplexCast)
                   : ImpCastExprToType(E, Context.DoubleTy, CK_FloatingCast);
      break;
    case LangOptions::FEM_Extended:
      if (Context.getFloatingTypeOrder(Context.LongDoubleTy, Ty) > 0)
        // Widen the expression to long double.
        return Ty->isComplexType()
                   ? ImpCastExprToType(
                         E, Context.getComplexType(Context.LongDoubleTy),
                         CK_FloatingComplexCast)
                   : ImpCastExprToType(E, Context.LongDoubleTy,
                                       CK_FloatingCast);
      break;
    }
  }

  // Half FP have to be promoted to float unless it is natively supported
  if (Ty->isHalfType() && !getLangOpts().NativeHalfType)
    return ImpCastExprToType(Res.get(), Context.FloatTy, CK_FloatingCast);

  // Try to perform integral promotions if the object has a theoretically
  // promotable type.
  if (Ty->isIntegralOrUnscopedEnumerationType()) {
    // C99 6.3.1.1p2:
    //
    //   The following may be used in an expression wherever an int or
    //   unsigned int may be used:
    //     - an object or expression with an integer type whose integer
    //       conversion rank is less than or equal to the rank of int
    //       and unsigned int.
    //     - A bit-field of type _Bool, int, signed int, or unsigned int.
    //
    //   If an int can represent all values of the original type, the
    //   value is converted to an int; otherwise, it is converted to an
    //   unsigned int. These are called the integer promotions. All
    //   other types are unchanged by the integer promotions.

    QualType PTy = Context.isPromotableBitField(E);
    if (!PTy.isNull()) {
      E = ImpCastExprToType(E, PTy, CK_IntegralCast).get();
      return E;
    }
    if (Context.isPromotableIntegerType(Ty)) {
      QualType PT = Context.getPromotedIntegerType(Ty);
      E = ImpCastExprToType(E, PT, CK_IntegralCast).get();
      return E;
    }
  }
  return E;
}

/// DefaultArgumentPromotion (C99 6.5.2.2p6). Used for function calls that
/// do not have a prototype. Arguments that have type float or __fp16
/// are promoted to double. All other argument types are converted by
/// UsualUnaryConversions().
ExprResult Sema::DefaultArgumentPromotion(Expr *E) {
  QualType Ty = E->getType();
  assert(!Ty.isNull() && "DefaultArgumentPromotion - missing type");

  ExprResult Res = UsualUnaryConversions(E);
  if (Res.isInvalid())
    return ExprError();
  E = Res.get();

  // If this is a 'float'  or '__fp16' (CVR qualified or typedef)
  // promote to double.
  // Note that default argument promotion applies only to float (and
  // half/fp16); it does not apply to _Float16.
  const BuiltinType *BTy = Ty->getAs<BuiltinType>();
  if (BTy && (BTy->getKind() == BuiltinType::Half ||
              BTy->getKind() == BuiltinType::Float)) {
    if (getLangOpts().OpenCL &&
        !getOpenCLOptions().isAvailableOption("cl_khr_fp64", getLangOpts())) {
      if (BTy->getKind() == BuiltinType::Half) {
        E = ImpCastExprToType(E, Context.FloatTy, CK_FloatingCast).get();
      }
    } else {
      E = ImpCastExprToType(E, Context.DoubleTy, CK_FloatingCast).get();
    }
  }
  if (BTy &&
      getLangOpts().getExtendIntArgs() ==
          LangOptions::ExtendArgsKind::ExtendTo64 &&
      Context.getTargetInfo().supportsExtendIntArgs() && Ty->isIntegerType() &&
      Context.getTypeSizeInChars(BTy) <
          Context.getTypeSizeInChars(Context.LongLongTy)) {
    E = (Ty->isUnsignedIntegerType())
            ? ImpCastExprToType(E, Context.UnsignedLongLongTy, CK_IntegralCast)
                  .get()
            : ImpCastExprToType(E, Context.LongLongTy, CK_IntegralCast).get();
    assert(8 == Context.getTypeSizeInChars(Context.LongLongTy).getQuantity() &&
           "Unexpected typesize for LongLongTy");
  }

  // C++ performs lvalue-to-rvalue conversion as a default argument
  // promotion, even on class types, but note:
  //   C++11 [conv.lval]p2:
  //     When an lvalue-to-rvalue conversion occurs in an unevaluated
  //     operand or a subexpression thereof the value contained in the
  //     referenced object is not accessed. Otherwise, if the glvalue
  //     has a class type, the conversion copy-initializes a temporary
  //     of type T from the glvalue and the result of the conversion
  //     is a prvalue for the temporary.
  // FIXME: add some way to gate this entire thing for correctness in
  // potentially potentially evaluated contexts.
  if (getLangOpts().CPlusPlus && E->isGLValue() && !isUnevaluatedContext()) {
    ExprResult Temp = PerformCopyInitialization(
                       InitializedEntity::InitializeTemporary(E->getType()),
                                                E->getExprLoc(), E);
    if (Temp.isInvalid())
      return ExprError();
    E = Temp.get();
  }

  return E;
}

/// Determine the degree of POD-ness for an expression.
/// Incomplete types are considered POD, since this check can be performed
/// when we're in an unevaluated context.
Sema::VarArgKind Sema::isValidVarArgType(const QualType &Ty) {
  if (Ty->isIncompleteType()) {
    // C++11 [expr.call]p7:
    //   After these conversions, if the argument does not have arithmetic,
    //   enumeration, pointer, pointer to member, or class type, the program
    //   is ill-formed.
    //
    // Since we've already performed array-to-pointer and function-to-pointer
    // decay, the only such type in C++ is cv void. This also handles
    // initializer lists as variadic arguments.
    if (Ty->isVoidType())
      return VAK_Invalid;

    if (Ty->isObjCObjectType())
      return VAK_Invalid;
    return VAK_Valid;
  }

  if (Ty.isDestructedType() == QualType::DK_nontrivial_c_struct)
    return VAK_Invalid;

  if (Context.getTargetInfo().getTriple().isWasm() &&
      Ty.isWebAssemblyReferenceType()) {
    return VAK_Invalid;
  }

  if (Ty.isCXX98PODType(Context))
    return VAK_Valid;

  // C++11 [expr.call]p7:
  //   Passing a potentially-evaluated argument of class type (Clause 9)
  //   having a non-trivial copy constructor, a non-trivial move constructor,
  //   or a non-trivial destructor, with no corresponding parameter,
  //   is conditionally-supported with implementation-defined semantics.
  if (getLangOpts().CPlusPlus11 && !Ty->isDependentType())
    if (CXXRecordDecl *Record = Ty->getAsCXXRecordDecl())
      if (!Record->hasNonTrivialCopyConstructor() &&
          !Record->hasNonTrivialMoveConstructor() &&
          !Record->hasNonTrivialDestructor())
        return VAK_ValidInCXX11;

  if (getLangOpts().ObjCAutoRefCount && Ty->isObjCLifetimeType())
    return VAK_Valid;

  if (Ty->isObjCObjectType())
    return VAK_Invalid;

  if (getLangOpts().MSVCCompat)
    return VAK_MSVCUndefined;

  // FIXME: In C++11, these cases are conditionally-supported, meaning we're
  // permitted to reject them. We should consider doing so.
  return VAK_Undefined;
}

void Sema::checkVariadicArgument(const Expr *E, VariadicCallType CT) {
  // Don't allow one to pass an Objective-C interface to a vararg.
  const QualType &Ty = E->getType();
  VarArgKind VAK = isValidVarArgType(Ty);

  // Complain about passing non-POD types through varargs.
  switch (VAK) {
  case VAK_ValidInCXX11:
    DiagRuntimeBehavior(
        E->getBeginLoc(), nullptr,
        PDiag(diag::warn_cxx98_compat_pass_non_pod_arg_to_vararg) << Ty << CT);
    [[fallthrough]];
  case VAK_Valid:
    if (Ty->isRecordType()) {
      // This is unlikely to be what the user intended. If the class has a
      // 'c_str' member function, the user probably meant to call that.
      DiagRuntimeBehavior(E->getBeginLoc(), nullptr,
                          PDiag(diag::warn_pass_class_arg_to_vararg)
                              << Ty << CT << hasCStrMethod(E) << ".c_str()");
    }
    break;

  case VAK_Undefined:
  case VAK_MSVCUndefined:
    DiagRuntimeBehavior(E->getBeginLoc(), nullptr,
                        PDiag(diag::warn_cannot_pass_non_pod_arg_to_vararg)
                            << getLangOpts().CPlusPlus11 << Ty << CT);
    break;

  case VAK_Invalid:
    if (Ty.isDestructedType() == QualType::DK_nontrivial_c_struct)
      Diag(E->getBeginLoc(),
           diag::err_cannot_pass_non_trivial_c_struct_to_vararg)
          << Ty << CT;
    else if (Ty->isObjCObjectType())
      DiagRuntimeBehavior(E->getBeginLoc(), nullptr,
                          PDiag(diag::err_cannot_pass_objc_interface_to_vararg)
                              << Ty << CT);
    else
      Diag(E->getBeginLoc(), diag::err_cannot_pass_to_vararg)
          << isa<InitListExpr>(E) << Ty << CT;
    break;
  }
}

/// DefaultVariadicArgumentPromotion - Like DefaultArgumentPromotion, but
/// will create a trap if the resulting type is not a POD type.
ExprResult Sema::DefaultVariadicArgumentPromotion(Expr *E, VariadicCallType CT,
                                                  FunctionDecl *FDecl) {
  if (const BuiltinType *PlaceholderTy = E->getType()->getAsPlaceholderType()) {
    // Strip the unbridged-cast placeholder expression off, if applicable.
    if (PlaceholderTy->getKind() == BuiltinType::ARCUnbridgedCast &&
        (CT == VariadicMethod ||
         (FDecl && FDecl->hasAttr<CFAuditedTransferAttr>()))) {
      E = stripARCUnbridgedCast(E);

    // Otherwise, do normal placeholder checking.
    } else {
      ExprResult ExprRes = CheckPlaceholderExpr(E);
      if (ExprRes.isInvalid())
        return ExprError();
      E = ExprRes.get();
    }
  }

  ExprResult ExprRes = DefaultArgumentPromotion(E);
  if (ExprRes.isInvalid())
    return ExprError();

  // Copy blocks to the heap.
  if (ExprRes.get()->getType()->isBlockPointerType())
    maybeExtendBlockObject(ExprRes);

  E = ExprRes.get();

  // Diagnostics regarding non-POD argument types are
  // emitted along with format string checking in Sema::CheckFunctionCall().
  if (isValidVarArgType(E->getType()) == VAK_Undefined) {
    // Turn this into a trap.
    CXXScopeSpec SS;
    SourceLocation TemplateKWLoc;
    UnqualifiedId Name;
    Name.setIdentifier(PP.getIdentifierInfo("__builtin_trap"),
                       E->getBeginLoc());
    ExprResult TrapFn = ActOnIdExpression(TUScope, SS, TemplateKWLoc, Name,
                                          /*HasTrailingLParen=*/true,
                                          /*IsAddressOfOperand=*/false);
    if (TrapFn.isInvalid())
      return ExprError();

    ExprResult Call = BuildCallExpr(TUScope, TrapFn.get(), E->getBeginLoc(),
                                    std::nullopt, E->getEndLoc());
    if (Call.isInvalid())
      return ExprError();

    ExprResult Comma =
        ActOnBinOp(TUScope, E->getBeginLoc(), tok::comma, Call.get(), E);
    if (Comma.isInvalid())
      return ExprError();
    return Comma.get();
  }

  if (!getLangOpts().CPlusPlus &&
      RequireCompleteType(E->getExprLoc(), E->getType(),
                          diag::err_call_incomplete_argument))
    return ExprError();

  return E;
}

/// Converts an integer to complex float type.  Helper function of
/// UsualArithmeticConversions()
///
/// \return false if the integer expression is an integer type and is
/// successfully converted to the complex type.
static bool handleIntegerToComplexFloatConversion(Sema &S, ExprResult &IntExpr,
                                                  ExprResult &ComplexExpr,
                                                  QualType IntTy,
                                                  QualType ComplexTy,
                                                  bool SkipCast) {
  if (IntTy->isComplexType() || IntTy->isRealFloatingType()) return true;
  if (SkipCast) return false;
  if (IntTy->isIntegerType()) {
    QualType fpTy = ComplexTy->castAs<ComplexType>()->getElementType();
    IntExpr = S.ImpCastExprToType(IntExpr.get(), fpTy, CK_IntegralToFloating);
    IntExpr = S.ImpCastExprToType(IntExpr.get(), ComplexTy,
                                  CK_FloatingRealToComplex);
  } else {
    assert(IntTy->isComplexIntegerType());
    IntExpr = S.ImpCastExprToType(IntExpr.get(), ComplexTy,
                                  CK_IntegralComplexToFloatingComplex);
  }
  return false;
}

// This handles complex/complex, complex/float, or float/complex.
// When both operands are complex, the shorter operand is converted to the
// type of the longer, and that is the type of the result. This corresponds
// to what is done when combining two real floating-point operands.
// The fun begins when size promotion occur across type domains.
// From H&S 6.3.4: When one operand is complex and the other is a real
// floating-point type, the less precise type is converted, within it's
// real or complex domain, to the precision of the other type. For example,
// when combining a "long double" with a "double _Complex", the
// "double _Complex" is promoted to "long double _Complex".
static QualType handleComplexFloatConversion(Sema &S, ExprResult &Shorter,
                                             QualType ShorterType,
                                             QualType LongerType,
                                             bool PromotePrecision) {
  bool LongerIsComplex = isa<ComplexType>(LongerType.getCanonicalType());
  QualType Result =
      LongerIsComplex ? LongerType : S.Context.getComplexType(LongerType);

  if (PromotePrecision) {
    if (isa<ComplexType>(ShorterType.getCanonicalType())) {
      Shorter =
          S.ImpCastExprToType(Shorter.get(), Result, CK_FloatingComplexCast);
    } else {
      if (LongerIsComplex)
        LongerType = LongerType->castAs<ComplexType>()->getElementType();
      Shorter = S.ImpCastExprToType(Shorter.get(), LongerType, CK_FloatingCast);
    }
  }
  return Result;
}

/// Handle arithmetic conversion with complex types.  Helper function of
/// UsualArithmeticConversions()
static QualType handleComplexConversion(Sema &S, ExprResult &LHS,
                                        ExprResult &RHS, QualType LHSType,
                                        QualType RHSType, bool IsCompAssign) {
  // if we have an integer operand, the result is the complex type.
  if (!handleIntegerToComplexFloatConversion(S, RHS, LHS, RHSType, LHSType,
                                             /*SkipCast=*/false))
    return LHSType;
  if (!handleIntegerToComplexFloatConversion(S, LHS, RHS, LHSType, RHSType,
                                             /*SkipCast=*/IsCompAssign))
    return RHSType;

  // Compute the rank of the two types, regardless of whether they are complex.
  int Order = S.Context.getFloatingTypeOrder(LHSType, RHSType);
  if (Order < 0)
    // Promote the precision of the LHS if not an assignment.
    return handleComplexFloatConversion(S, LHS, LHSType, RHSType,
                                        /*PromotePrecision=*/!IsCompAssign);
  // Promote the precision of the RHS unless it is already the same as the LHS.
  return handleComplexFloatConversion(S, RHS, RHSType, LHSType,
                                      /*PromotePrecision=*/Order > 0);
}

/// Handle arithmetic conversion from integer to float.  Helper function
/// of UsualArithmeticConversions()
static QualType handleIntToFloatConversion(Sema &S, ExprResult &FloatExpr,
                                           ExprResult &IntExpr,
                                           QualType FloatTy, QualType IntTy,
                                           bool ConvertFloat, bool ConvertInt) {
  if (IntTy->isIntegerType()) {
    if (ConvertInt)
      // Convert intExpr to the lhs floating point type.
      IntExpr = S.ImpCastExprToType(IntExpr.get(), FloatTy,
                                    CK_IntegralToFloating);
    return FloatTy;
  }

  // Convert both sides to the appropriate complex float.
  assert(IntTy->isComplexIntegerType());
  QualType result = S.Context.getComplexType(FloatTy);

  // _Complex int -> _Complex float
  if (ConvertInt)
    IntExpr = S.ImpCastExprToType(IntExpr.get(), result,
                                  CK_IntegralComplexToFloatingComplex);

  // float -> _Complex float
  if (ConvertFloat)
    FloatExpr = S.ImpCastExprToType(FloatExpr.get(), result,
                                    CK_FloatingRealToComplex);

  return result;
}

/// Handle arithmethic conversion with floating point types.  Helper
/// function of UsualArithmeticConversions()
static QualType handleFloatConversion(Sema &S, ExprResult &LHS,
                                      ExprResult &RHS, QualType LHSType,
                                      QualType RHSType, bool IsCompAssign) {
  bool LHSFloat = LHSType->isRealFloatingType();
  bool RHSFloat = RHSType->isRealFloatingType();

  // N1169 4.1.4: If one of the operands has a floating type and the other
  //              operand has a fixed-point type, the fixed-point operand
  //              is converted to the floating type [...]
  if (LHSType->isFixedPointType() || RHSType->isFixedPointType()) {
    if (LHSFloat)
      RHS = S.ImpCastExprToType(RHS.get(), LHSType, CK_FixedPointToFloating);
    else if (!IsCompAssign)
      LHS = S.ImpCastExprToType(LHS.get(), RHSType, CK_FixedPointToFloating);
    return LHSFloat ? LHSType : RHSType;
  }

  // If we have two real floating types, convert the smaller operand
  // to the bigger result.
  if (LHSFloat && RHSFloat) {
    int order = S.Context.getFloatingTypeOrder(LHSType, RHSType);
    if (order > 0) {
      RHS = S.ImpCastExprToType(RHS.get(), LHSType, CK_FloatingCast);
      return LHSType;
    }

    assert(order < 0 && "illegal float comparison");
    if (!IsCompAssign)
      LHS = S.ImpCastExprToType(LHS.get(), RHSType, CK_FloatingCast);
    return RHSType;
  }

  if (LHSFloat) {
    // Half FP has to be promoted to float unless it is natively supported
    if (LHSType->isHalfType() && !S.getLangOpts().NativeHalfType)
      LHSType = S.Context.FloatTy;

    return handleIntToFloatConversion(S, LHS, RHS, LHSType, RHSType,
                                      /*ConvertFloat=*/!IsCompAssign,
                                      /*ConvertInt=*/ true);
  }
  assert(RHSFloat);
  return handleIntToFloatConversion(S, RHS, LHS, RHSType, LHSType,
                                    /*ConvertFloat=*/ true,
                                    /*ConvertInt=*/!IsCompAssign);
}

/// Diagnose attempts to convert between __float128, __ibm128 and
/// long double if there is no support for such conversion.
/// Helper function of UsualArithmeticConversions().
static bool unsupportedTypeConversion(const Sema &S, QualType LHSType,
                                      QualType RHSType) {
  // No issue if either is not a floating point type.
  if (!LHSType->isFloatingType() || !RHSType->isFloatingType())
    return false;

  // No issue if both have the same 128-bit float semantics.
  auto *LHSComplex = LHSType->getAs<ComplexType>();
  auto *RHSComplex = RHSType->getAs<ComplexType>();

  QualType LHSElem = LHSComplex ? LHSComplex->getElementType() : LHSType;
  QualType RHSElem = RHSComplex ? RHSComplex->getElementType() : RHSType;

  const llvm::fltSemantics &LHSSem = S.Context.getFloatTypeSemantics(LHSElem);
  const llvm::fltSemantics &RHSSem = S.Context.getFloatTypeSemantics(RHSElem);

  if ((&LHSSem != &llvm::APFloat::PPCDoubleDouble() ||
       &RHSSem != &llvm::APFloat::IEEEquad()) &&
      (&LHSSem != &llvm::APFloat::IEEEquad() ||
       &RHSSem != &llvm::APFloat::PPCDoubleDouble()))
    return false;

  return true;
}

typedef ExprResult PerformCastFn(Sema &S, Expr *operand, QualType toType);

namespace {
/// These helper callbacks are placed in an anonymous namespace to
/// permit their use as function template parameters.
ExprResult doIntegralCast(Sema &S, Expr *op, QualType toType) {
  return S.ImpCastExprToType(op, toType, CK_IntegralCast);
}

ExprResult doComplexIntegralCast(Sema &S, Expr *op, QualType toType) {
  return S.ImpCastExprToType(op, S.Context.getComplexType(toType),
                             CK_IntegralComplexCast);
}
}

/// Handle integer arithmetic conversions.  Helper function of
/// UsualArithmeticConversions()
template <PerformCastFn doLHSCast, PerformCastFn doRHSCast>
static QualType handleIntegerConversion(Sema &S, ExprResult &LHS,
                                        ExprResult &RHS, QualType LHSType,
                                        QualType RHSType, bool IsCompAssign) {
  // The rules for this case are in C99 6.3.1.8
  int order = S.Context.getIntegerTypeOrder(LHSType, RHSType);
  bool LHSSigned = LHSType->hasSignedIntegerRepresentation();
  bool RHSSigned = RHSType->hasSignedIntegerRepresentation();
  if (LHSSigned == RHSSigned) {
    // Same signedness; use the higher-ranked type
    if (order >= 0) {
      RHS = (*doRHSCast)(S, RHS.get(), LHSType);
      return LHSType;
    } else if (!IsCompAssign)
      LHS = (*doLHSCast)(S, LHS.get(), RHSType);
    return RHSType;
  } else if (order != (LHSSigned ? 1 : -1)) {
    // The unsigned type has greater than or equal rank to the
    // signed type, so use the unsigned type
    if (RHSSigned) {
      RHS = (*doRHSCast)(S, RHS.get(), LHSType);
      return LHSType;
    } else if (!IsCompAssign)
      LHS = (*doLHSCast)(S, LHS.get(), RHSType);
    return RHSType;
  } else if (S.Context.getIntWidth(LHSType) != S.Context.getIntWidth(RHSType)) {
    // The two types are different widths; if we are here, that
    // means the signed type is larger than the unsigned type, so
    // use the signed type.
    if (LHSSigned) {
      RHS = (*doRHSCast)(S, RHS.get(), LHSType);
      return LHSType;
    } else if (!IsCompAssign)
      LHS = (*doLHSCast)(S, LHS.get(), RHSType);
    return RHSType;
  } else {
    // The signed type is higher-ranked than the unsigned type,
    // but isn't actually any bigger (like unsigned int and long
    // on most 32-bit systems).  Use the unsigned type corresponding
    // to the signed type.
    QualType result =
      S.Context.getCorrespondingUnsignedType(LHSSigned ? LHSType : RHSType);
    RHS = (*doRHSCast)(S, RHS.get(), result);
    if (!IsCompAssign)
      LHS = (*doLHSCast)(S, LHS.get(), result);
    return result;
  }
}

/// Handle conversions with GCC complex int extension.  Helper function
/// of UsualArithmeticConversions()
static QualType handleComplexIntConversion(Sema &S, ExprResult &LHS,
                                           ExprResult &RHS, QualType LHSType,
                                           QualType RHSType,
                                           bool IsCompAssign) {
  const ComplexType *LHSComplexInt = LHSType->getAsComplexIntegerType();
  const ComplexType *RHSComplexInt = RHSType->getAsComplexIntegerType();

  if (LHSComplexInt && RHSComplexInt) {
    QualType LHSEltType = LHSComplexInt->getElementType();
    QualType RHSEltType = RHSComplexInt->getElementType();
    QualType ScalarType =
      handleIntegerConversion<doComplexIntegralCast, doComplexIntegralCast>
        (S, LHS, RHS, LHSEltType, RHSEltType, IsCompAssign);

    return S.Context.getComplexType(ScalarType);
  }

  if (LHSComplexInt) {
    QualType LHSEltType = LHSComplexInt->getElementType();
    QualType ScalarType =
      handleIntegerConversion<doComplexIntegralCast, doIntegralCast>
        (S, LHS, RHS, LHSEltType, RHSType, IsCompAssign);
    QualType ComplexType = S.Context.getComplexType(ScalarType);
    RHS = S.ImpCastExprToType(RHS.get(), ComplexType,
                              CK_IntegralRealToComplex);

    return ComplexType;
  }

  assert(RHSComplexInt);

  QualType RHSEltType = RHSComplexInt->getElementType();
  QualType ScalarType =
    handleIntegerConversion<doIntegralCast, doComplexIntegralCast>
      (S, LHS, RHS, LHSType, RHSEltType, IsCompAssign);
  QualType ComplexType = S.Context.getComplexType(ScalarType);

  if (!IsCompAssign)
    LHS = S.ImpCastExprToType(LHS.get(), ComplexType,
                              CK_IntegralRealToComplex);
  return ComplexType;
}

/// Return the rank of a given fixed point or integer type. The value itself
/// doesn't matter, but the values must be increasing with proper increasing
/// rank as described in N1169 4.1.1.
static unsigned GetFixedPointRank(QualType Ty) {
  const auto *BTy = Ty->getAs<BuiltinType>();
  assert(BTy && "Expected a builtin type.");

  switch (BTy->getKind()) {
  case BuiltinType::ShortFract:
  case BuiltinType::UShortFract:
  case BuiltinType::SatShortFract:
  case BuiltinType::SatUShortFract:
    return 1;
  case BuiltinType::Fract:
  case BuiltinType::UFract:
  case BuiltinType::SatFract:
  case BuiltinType::SatUFract:
    return 2;
  case BuiltinType::LongFract:
  case BuiltinType::ULongFract:
  case BuiltinType::SatLongFract:
  case BuiltinType::SatULongFract:
    return 3;
  case BuiltinType::ShortAccum:
  case BuiltinType::UShortAccum:
  case BuiltinType::SatShortAccum:
  case BuiltinType::SatUShortAccum:
    return 4;
  case BuiltinType::Accum:
  case BuiltinType::UAccum:
  case BuiltinType::SatAccum:
  case BuiltinType::SatUAccum:
    return 5;
  case BuiltinType::LongAccum:
  case BuiltinType::ULongAccum:
  case BuiltinType::SatLongAccum:
  case BuiltinType::SatULongAccum:
    return 6;
  default:
    if (BTy->isInteger())
      return 0;
    llvm_unreachable("Unexpected fixed point or integer type");
  }
}

/// handleFixedPointConversion - Fixed point operations between fixed
/// point types and integers or other fixed point types do not fall under
/// usual arithmetic conversion since these conversions could result in loss
/// of precsision (N1169 4.1.4). These operations should be calculated with
/// the full precision of their result type (N1169 4.1.6.2.1).
static QualType handleFixedPointConversion(Sema &S, QualType LHSTy,
                                           QualType RHSTy) {
  assert((LHSTy->isFixedPointType() || RHSTy->isFixedPointType()) &&
         "Expected at least one of the operands to be a fixed point type");
  assert((LHSTy->isFixedPointOrIntegerType() ||
          RHSTy->isFixedPointOrIntegerType()) &&
         "Special fixed point arithmetic operation conversions are only "
         "applied to ints or other fixed point types");

  // If one operand has signed fixed-point type and the other operand has
  // unsigned fixed-point type, then the unsigned fixed-point operand is
  // converted to its corresponding signed fixed-point type and the resulting
  // type is the type of the converted operand.
  if (RHSTy->isSignedFixedPointType() && LHSTy->isUnsignedFixedPointType())
    LHSTy = S.Context.getCorrespondingSignedFixedPointType(LHSTy);
  else if (RHSTy->isUnsignedFixedPointType() && LHSTy->isSignedFixedPointType())
    RHSTy = S.Context.getCorrespondingSignedFixedPointType(RHSTy);

  // The result type is the type with the highest rank, whereby a fixed-point
  // conversion rank is always greater than an integer conversion rank; if the
  // type of either of the operands is a saturating fixedpoint type, the result
  // type shall be the saturating fixed-point type corresponding to the type
  // with the highest rank; the resulting value is converted (taking into
  // account rounding and overflow) to the precision of the resulting type.
  // Same ranks between signed and unsigned types are resolved earlier, so both
  // types are either signed or both unsigned at this point.
  unsigned LHSTyRank = GetFixedPointRank(LHSTy);
  unsigned RHSTyRank = GetFixedPointRank(RHSTy);

  QualType ResultTy = LHSTyRank > RHSTyRank ? LHSTy : RHSTy;

  if (LHSTy->isSaturatedFixedPointType() || RHSTy->isSaturatedFixedPointType())
    ResultTy = S.Context.getCorrespondingSaturatedType(ResultTy);

  return ResultTy;
}

/// Check that the usual arithmetic conversions can be performed on this pair of
/// expressions that might be of enumeration type.
static void checkEnumArithmeticConversions(Sema &S, Expr *LHS, Expr *RHS,
                                           SourceLocation Loc,
                                           Sema::ArithConvKind ACK) {
  // C++2a [expr.arith.conv]p1:
  //   If one operand is of enumeration type and the other operand is of a
  //   different enumeration type or a floating-point type, this behavior is
  //   deprecated ([depr.arith.conv.enum]).
  //
  // Warn on this in all language modes. Produce a deprecation warning in C++20.
  // Eventually we will presumably reject these cases (in C++23 onwards?).
  QualType L = LHS->getType(), R = RHS->getType();
  bool LEnum = L->isUnscopedEnumerationType(),
       REnum = R->isUnscopedEnumerationType();
  bool IsCompAssign = ACK == Sema::ACK_CompAssign;
  if ((!IsCompAssign && LEnum && R->isFloatingType()) ||
      (REnum && L->isFloatingType())) {
    S.Diag(Loc, S.getLangOpts().CPlusPlus20
                    ? diag::warn_arith_conv_enum_float_cxx20
                    : diag::warn_arith_conv_enum_float)
        << LHS->getSourceRange() << RHS->getSourceRange()
        << (int)ACK << LEnum << L << R;
  } else if (!IsCompAssign && LEnum && REnum &&
             !S.Context.hasSameUnqualifiedType(L, R)) {
    unsigned DiagID;
    if (!L->castAs<EnumType>()->getDecl()->hasNameForLinkage() ||
        !R->castAs<EnumType>()->getDecl()->hasNameForLinkage()) {
      // If either enumeration type is unnamed, it's less likely that the
      // user cares about this, but this situation is still deprecated in
      // C++2a. Use a different warning group.
      DiagID = S.getLangOpts().CPlusPlus20
                    ? diag::warn_arith_conv_mixed_anon_enum_types_cxx20
                    : diag::warn_arith_conv_mixed_anon_enum_types;
    } else if (ACK == Sema::ACK_Conditional) {
      // Conditional expressions are separated out because they have
      // historically had a different warning flag.
      DiagID = S.getLangOpts().CPlusPlus20
                   ? diag::warn_conditional_mixed_enum_types_cxx20
                   : diag::warn_conditional_mixed_enum_types;
    } else if (ACK == Sema::ACK_Comparison) {
      // Comparison expressions are separated out because they have
      // historically had a different warning flag.
      DiagID = S.getLangOpts().CPlusPlus20
                   ? diag::warn_comparison_mixed_enum_types_cxx20
                   : diag::warn_comparison_mixed_enum_types;
    } else {
      DiagID = S.getLangOpts().CPlusPlus20
                   ? diag::warn_arith_conv_mixed_enum_types_cxx20
                   : diag::warn_arith_conv_mixed_enum_types;
    }
    S.Diag(Loc, DiagID) << LHS->getSourceRange() << RHS->getSourceRange()
                        << (int)ACK << L << R;
  }
}

/// UsualArithmeticConversions - Performs various conversions that are common to
/// binary operators (C99 6.3.1.8). If both operands aren't arithmetic, this
/// routine returns the first non-arithmetic type found. The client is
/// responsible for emitting appropriate error diagnostics.
QualType Sema::UsualArithmeticConversions(ExprResult &LHS, ExprResult &RHS,
                                          SourceLocation Loc,
                                          ArithConvKind ACK) {
  checkEnumArithmeticConversions(*this, LHS.get(), RHS.get(), Loc, ACK);

  if (ACK != ACK_CompAssign) {
    LHS = UsualUnaryConversions(LHS.get());
    if (LHS.isInvalid())
      return QualType();
  }

  RHS = UsualUnaryConversions(RHS.get());
  if (RHS.isInvalid())
    return QualType();

  // For conversion purposes, we ignore any qualifiers.
  // For example, "const float" and "float" are equivalent.
  QualType LHSType = LHS.get()->getType().getUnqualifiedType();
  QualType RHSType = RHS.get()->getType().getUnqualifiedType();

  // For conversion purposes, we ignore any atomic qualifier on the LHS.
  if (const AtomicType *AtomicLHS = LHSType->getAs<AtomicType>())
    LHSType = AtomicLHS->getValueType();

  // If both types are identical, no conversion is needed.
  if (Context.hasSameType(LHSType, RHSType))
    return Context.getCommonSugaredType(LHSType, RHSType);

  // If either side is a non-arithmetic type (e.g. a pointer), we are done.
  // The caller can deal with this (e.g. pointer + int).
  if (!LHSType->isArithmeticType() || !RHSType->isArithmeticType())
    return QualType();

  // Apply unary and bitfield promotions to the LHS's type.
  QualType LHSUnpromotedType = LHSType;
  if (Context.isPromotableIntegerType(LHSType))
    LHSType = Context.getPromotedIntegerType(LHSType);
  QualType LHSBitfieldPromoteTy = Context.isPromotableBitField(LHS.get());
  if (!LHSBitfieldPromoteTy.isNull())
    LHSType = LHSBitfieldPromoteTy;
  if (LHSType != LHSUnpromotedType && ACK != ACK_CompAssign)
    LHS = ImpCastExprToType(LHS.get(), LHSType, CK_IntegralCast);

  // If both types are identical, no conversion is needed.
  if (Context.hasSameType(LHSType, RHSType))
    return Context.getCommonSugaredType(LHSType, RHSType);

  // At this point, we have two different arithmetic types.

  // Diagnose attempts to convert between __ibm128, __float128 and long double
  // where such conversions currently can't be handled.
  if (unsupportedTypeConversion(*this, LHSType, RHSType))
    return QualType();

  // Handle complex types first (C99 6.3.1.8p1).
  if (LHSType->isComplexType() || RHSType->isComplexType())
    return handleComplexConversion(*this, LHS, RHS, LHSType, RHSType,
                                   ACK == ACK_CompAssign);

  // Now handle "real" floating types (i.e. float, double, long double).
  if (LHSType->isRealFloatingType() || RHSType->isRealFloatingType())
    return handleFloatConversion(*this, LHS, RHS, LHSType, RHSType,
                                 ACK == ACK_CompAssign);

  // Handle GCC complex int extension.
  if (LHSType->isComplexIntegerType() || RHSType->isComplexIntegerType())
    return handleComplexIntConversion(*this, LHS, RHS, LHSType, RHSType,
                                      ACK == ACK_CompAssign);

  if (LHSType->isFixedPointType() || RHSType->isFixedPointType())
    return handleFixedPointConversion(*this, LHSType, RHSType);

  // Finally, we have two differing integer types.
  return handleIntegerConversion<doIntegralCast, doIntegralCast>
           (*this, LHS, RHS, LHSType, RHSType, ACK == ACK_CompAssign);
}

//===----------------------------------------------------------------------===//
//  Semantic Analysis for various Expression Types
//===----------------------------------------------------------------------===//


ExprResult Sema::ActOnGenericSelectionExpr(
    SourceLocation KeyLoc, SourceLocation DefaultLoc, SourceLocation RParenLoc,
    bool PredicateIsExpr, void *ControllingExprOrType,
    ArrayRef<ParsedType> ArgTypes, ArrayRef<Expr *> ArgExprs) {
  unsigned NumAssocs = ArgTypes.size();
  assert(NumAssocs == ArgExprs.size());

  TypeSourceInfo **Types = new TypeSourceInfo*[NumAssocs];
  for (unsigned i = 0; i < NumAssocs; ++i) {
    if (ArgTypes[i])
      (void) GetTypeFromParser(ArgTypes[i], &Types[i]);
    else
      Types[i] = nullptr;
  }

  // If we have a controlling type, we need to convert it from a parsed type
  // into a semantic type and then pass that along.
  if (!PredicateIsExpr) {
    TypeSourceInfo *ControllingType;
    (void)GetTypeFromParser(ParsedType::getFromOpaquePtr(ControllingExprOrType),
                            &ControllingType);
    assert(ControllingType && "couldn't get the type out of the parser");
    ControllingExprOrType = ControllingType;
  }

  ExprResult ER = CreateGenericSelectionExpr(
      KeyLoc, DefaultLoc, RParenLoc, PredicateIsExpr, ControllingExprOrType,
      llvm::ArrayRef(Types, NumAssocs), ArgExprs);
  delete [] Types;
  return ER;
}

ExprResult Sema::CreateGenericSelectionExpr(
    SourceLocation KeyLoc, SourceLocation DefaultLoc, SourceLocation RParenLoc,
    bool PredicateIsExpr, void *ControllingExprOrType,
    ArrayRef<TypeSourceInfo *> Types, ArrayRef<Expr *> Exprs) {
  unsigned NumAssocs = Types.size();
  assert(NumAssocs == Exprs.size());
  assert(ControllingExprOrType &&
         "Must have either a controlling expression or a controlling type");

  Expr *ControllingExpr = nullptr;
  TypeSourceInfo *ControllingType = nullptr;
  if (PredicateIsExpr) {
    // Decay and strip qualifiers for the controlling expression type, and
    // handle placeholder type replacement. See committee discussion from WG14
    // DR423.
    EnterExpressionEvaluationContext Unevaluated(
        *this, Sema::ExpressionEvaluationContext::Unevaluated);
    ExprResult R = DefaultFunctionArrayLvalueConversion(
        reinterpret_cast<Expr *>(ControllingExprOrType));
    if (R.isInvalid())
      return ExprError();
    ControllingExpr = R.get();
  } else {
    // The extension form uses the type directly rather than converting it.
    ControllingType = reinterpret_cast<TypeSourceInfo *>(ControllingExprOrType);
    if (!ControllingType)
      return ExprError();
  }

  bool TypeErrorFound = false,
       IsResultDependent = ControllingExpr
                               ? ControllingExpr->isTypeDependent()
                               : ControllingType->getType()->isDependentType(),
       ContainsUnexpandedParameterPack =
           ControllingExpr
               ? ControllingExpr->containsUnexpandedParameterPack()
               : ControllingType->getType()->containsUnexpandedParameterPack();

  // The controlling expression is an unevaluated operand, so side effects are
  // likely unintended.
  if (!inTemplateInstantiation() && !IsResultDependent && ControllingExpr &&
      ControllingExpr->HasSideEffects(Context, false))
    Diag(ControllingExpr->getExprLoc(),
         diag::warn_side_effects_unevaluated_context);

  for (unsigned i = 0; i < NumAssocs; ++i) {
    if (Exprs[i]->containsUnexpandedParameterPack())
      ContainsUnexpandedParameterPack = true;

    if (Types[i]) {
      if (Types[i]->getType()->containsUnexpandedParameterPack())
        ContainsUnexpandedParameterPack = true;

      if (Types[i]->getType()->isDependentType()) {
        IsResultDependent = true;
      } else {
        // We relax the restriction on use of incomplete types and non-object
        // types with the type-based extension of _Generic. Allowing incomplete
        // objects means those can be used as "tags" for a type-safe way to map
        // to a value. Similarly, matching on function types rather than
        // function pointer types can be useful. However, the restriction on VM
        // types makes sense to retain as there are open questions about how
        // the selection can be made at compile time.
        //
        // C11 6.5.1.1p2 "The type name in a generic association shall specify a
        // complete object type other than a variably modified type."
        unsigned D = 0;
        if (ControllingExpr && Types[i]->getType()->isIncompleteType())
          D = diag::err_assoc_type_incomplete;
        else if (ControllingExpr && !Types[i]->getType()->isObjectType())
          D = diag::err_assoc_type_nonobject;
        else if (Types[i]->getType()->isVariablyModifiedType())
          D = diag::err_assoc_type_variably_modified;
        else if (ControllingExpr) {
          // Because the controlling expression undergoes lvalue conversion,
          // array conversion, and function conversion, an association which is
          // of array type, function type, or is qualified can never be
          // reached. We will warn about this so users are less surprised by
          // the unreachable association. However, we don't have to handle
          // function types; that's not an object type, so it's handled above.
          //
          // The logic is somewhat different for C++ because C++ has different
          // lvalue to rvalue conversion rules than C. [conv.lvalue]p1 says,
          // If T is a non-class type, the type of the prvalue is the cv-
          // unqualified version of T. Otherwise, the type of the prvalue is T.
          // The result of these rules is that all qualified types in an
          // association in C are unreachable, and in C++, only qualified non-
          // class types are unreachable.
          //
          // NB: this does not apply when the first operand is a type rather
          // than an expression, because the type form does not undergo
          // conversion.
          unsigned Reason = 0;
          QualType QT = Types[i]->getType();
          if (QT->isArrayType())
            Reason = 1;
          else if (QT.hasQualifiers() &&
                   (!LangOpts.CPlusPlus || !QT->isRecordType()))
            Reason = 2;

          if (Reason)
            Diag(Types[i]->getTypeLoc().getBeginLoc(),
                 diag::warn_unreachable_association)
                << QT << (Reason - 1);
        }

        if (D != 0) {
          Diag(Types[i]->getTypeLoc().getBeginLoc(), D)
            << Types[i]->getTypeLoc().getSourceRange()
            << Types[i]->getType();
          TypeErrorFound = true;
        }

        // C11 6.5.1.1p2 "No two generic associations in the same generic
        // selection shall specify compatible types."
        for (unsigned j = i+1; j < NumAssocs; ++j)
          if (Types[j] && !Types[j]->getType()->isDependentType() &&
              Context.typesAreCompatible(Types[i]->getType(),
                                         Types[j]->getType())) {
            Diag(Types[j]->getTypeLoc().getBeginLoc(),
                 diag::err_assoc_compatible_types)
              << Types[j]->getTypeLoc().getSourceRange()
              << Types[j]->getType()
              << Types[i]->getType();
            Diag(Types[i]->getTypeLoc().getBeginLoc(),
                 diag::note_compat_assoc)
              << Types[i]->getTypeLoc().getSourceRange()
              << Types[i]->getType();
            TypeErrorFound = true;
          }
      }
    }
  }
  if (TypeErrorFound)
    return ExprError();

  // If we determined that the generic selection is result-dependent, don't
  // try to compute the result expression.
  if (IsResultDependent) {
    if (ControllingExpr)
      return GenericSelectionExpr::Create(Context, KeyLoc, ControllingExpr,
                                          Types, Exprs, DefaultLoc, RParenLoc,
                                          ContainsUnexpandedParameterPack);
    return GenericSelectionExpr::Create(Context, KeyLoc, ControllingType, Types,
                                        Exprs, DefaultLoc, RParenLoc,
                                        ContainsUnexpandedParameterPack);
  }

  SmallVector<unsigned, 1> CompatIndices;
  unsigned DefaultIndex = -1U;
  // Look at the canonical type of the controlling expression in case it was a
  // deduced type like __auto_type. However, when issuing diagnostics, use the
  // type the user wrote in source rather than the canonical one.
  for (unsigned i = 0; i < NumAssocs; ++i) {
    if (!Types[i])
      DefaultIndex = i;
    else if (ControllingExpr &&
             Context.typesAreCompatible(
                 ControllingExpr->getType().getCanonicalType(),
                 Types[i]->getType()))
      CompatIndices.push_back(i);
    else if (ControllingType &&
             Context.typesAreCompatible(
                 ControllingType->getType().getCanonicalType(),
                 Types[i]->getType()))
      CompatIndices.push_back(i);
  }

  auto GetControllingRangeAndType = [](Expr *ControllingExpr,
                                       TypeSourceInfo *ControllingType) {
    // We strip parens here because the controlling expression is typically
    // parenthesized in macro definitions.
    if (ControllingExpr)
      ControllingExpr = ControllingExpr->IgnoreParens();

    SourceRange SR = ControllingExpr
                         ? ControllingExpr->getSourceRange()
                         : ControllingType->getTypeLoc().getSourceRange();
    QualType QT = ControllingExpr ? ControllingExpr->getType()
                                  : ControllingType->getType();

    return std::make_pair(SR, QT);
  };

  // C11 6.5.1.1p2 "The controlling expression of a generic selection shall have
  // type compatible with at most one of the types named in its generic
  // association list."
  if (CompatIndices.size() > 1) {
    auto P = GetControllingRangeAndType(ControllingExpr, ControllingType);
    SourceRange SR = P.first;
    Diag(SR.getBegin(), diag::err_generic_sel_multi_match)
        << SR << P.second << (unsigned)CompatIndices.size();
    for (unsigned I : CompatIndices) {
      Diag(Types[I]->getTypeLoc().getBeginLoc(),
           diag::note_compat_assoc)
        << Types[I]->getTypeLoc().getSourceRange()
        << Types[I]->getType();
    }
    return ExprError();
  }

  // C11 6.5.1.1p2 "If a generic selection has no default generic association,
  // its controlling expression shall have type compatible with exactly one of
  // the types named in its generic association list."
  if (DefaultIndex == -1U && CompatIndices.size() == 0) {
    auto P = GetControllingRangeAndType(ControllingExpr, ControllingType);
    SourceRange SR = P.first;
    Diag(SR.getBegin(), diag::err_generic_sel_no_match) << SR << P.second;
    return ExprError();
  }

  // C11 6.5.1.1p3 "If a generic selection has a generic association with a
  // type name that is compatible with the type of the controlling expression,
  // then the result expression of the generic selection is the expression
  // in that generic association. Otherwise, the result expression of the
  // generic selection is the expression in the default generic association."
  unsigned ResultIndex =
    CompatIndices.size() ? CompatIndices[0] : DefaultIndex;

  if (ControllingExpr) {
    return GenericSelectionExpr::Create(
        Context, KeyLoc, ControllingExpr, Types, Exprs, DefaultLoc, RParenLoc,
        ContainsUnexpandedParameterPack, ResultIndex);
  }
  return GenericSelectionExpr::Create(
      Context, KeyLoc, ControllingType, Types, Exprs, DefaultLoc, RParenLoc,
      ContainsUnexpandedParameterPack, ResultIndex);
}

static PredefinedExpr::IdentKind getPredefinedExprKind(tok::TokenKind Kind) {
  switch (Kind) {
  default:
    llvm_unreachable("unexpected TokenKind");
  case tok::kw___func__:
    return PredefinedExpr::Func; // [C99 6.4.2.2]
  case tok::kw___FUNCTION__:
    return PredefinedExpr::Function;
  case tok::kw___FUNCDNAME__:
    return PredefinedExpr::FuncDName; // [MS]
  case tok::kw___FUNCSIG__:
    return PredefinedExpr::FuncSig; // [MS]
  case tok::kw_L__FUNCTION__:
    return PredefinedExpr::LFunction; // [MS]
  case tok::kw_L__FUNCSIG__:
    return PredefinedExpr::LFuncSig; // [MS]
  case tok::kw___PRETTY_FUNCTION__:
    return PredefinedExpr::PrettyFunction; // [GNU]
  }
}

/// getUDSuffixLoc - Create a SourceLocation for a ud-suffix, given the
/// location of the token and the offset of the ud-suffix within it.
static SourceLocation getUDSuffixLoc(Sema &S, SourceLocation TokLoc,
                                     unsigned Offset) {
  return Lexer::AdvanceToTokenCharacter(TokLoc, Offset, S.getSourceManager(),
                                        S.getLangOpts());
}

/// BuildCookedLiteralOperatorCall - A user-defined literal was found. Look up
/// the corresponding cooked (non-raw) literal operator, and build a call to it.
static ExprResult BuildCookedLiteralOperatorCall(Sema &S, Scope *Scope,
                                                 IdentifierInfo *UDSuffix,
                                                 SourceLocation UDSuffixLoc,
                                                 ArrayRef<Expr*> Args,
                                                 SourceLocation LitEndLoc) {
  assert(Args.size() <= 2 && "too many arguments for literal operator");

  QualType ArgTy[2];
  for (unsigned ArgIdx = 0; ArgIdx != Args.size(); ++ArgIdx) {
    ArgTy[ArgIdx] = Args[ArgIdx]->getType();
    if (ArgTy[ArgIdx]->isArrayType())
      ArgTy[ArgIdx] = S.Context.getArrayDecayedType(ArgTy[ArgIdx]);
  }

  DeclarationName OpName =
    S.Context.DeclarationNames.getCXXLiteralOperatorName(UDSuffix);
  DeclarationNameInfo OpNameInfo(OpName, UDSuffixLoc);
  OpNameInfo.setCXXLiteralOperatorNameLoc(UDSuffixLoc);

  LookupResult R(S, OpName, UDSuffixLoc, Sema::LookupOrdinaryName);
  if (S.LookupLiteralOperator(Scope, R, llvm::ArrayRef(ArgTy, Args.size()),
                              /*AllowRaw*/ false, /*AllowTemplate*/ false,
                              /*AllowStringTemplatePack*/ false,
                              /*DiagnoseMissing*/ true) == Sema::LOLR_Error)
    return ExprError();

  return S.BuildLiteralOperatorCall(R, OpNameInfo, Args, LitEndLoc);
}

ExprResult Sema::ActOnUnevaluatedStringLiteral(ArrayRef<Token> StringToks) {
  // StringToks needs backing storage as it doesn't hold array elements itself
  std::vector<Token> ExpandedToks;
  if (getLangOpts().MicrosoftExt)
    StringToks = ExpandedToks = ExpandFunctionLocalPredefinedMacros(StringToks);

  StringLiteralParser Literal(StringToks, PP,
                              StringLiteralEvalMethod::Unevaluated);
  if (Literal.hadError)
    return ExprError();

  SmallVector<SourceLocation, 4> StringTokLocs;
  for (const Token &Tok : StringToks)
    StringTokLocs.push_back(Tok.getLocation());

  StringLiteral *Lit = StringLiteral::Create(
      Context, Literal.GetString(), StringLiteral::Unevaluated, false, {},
      &StringTokLocs[0], StringTokLocs.size());

  if (!Literal.getUDSuffix().empty()) {
    SourceLocation UDSuffixLoc =
        getUDSuffixLoc(*this, StringTokLocs[Literal.getUDSuffixToken()],
                       Literal.getUDSuffixOffset());
    return ExprError(Diag(UDSuffixLoc, diag::err_invalid_string_udl));
  }

  return Lit;
}

std::vector<Token>
Sema::ExpandFunctionLocalPredefinedMacros(ArrayRef<Token> Toks) {
  // MSVC treats some predefined identifiers (e.g. __FUNCTION__) as function
  // local macros that expand to string literals that may be concatenated.
  // These macros are expanded here (in Sema), because StringLiteralParser
  // (in Lex) doesn't know the enclosing function (because it hasn't been
  // parsed yet).
  assert(getLangOpts().MicrosoftExt);

  // Note: Although function local macros are defined only inside functions,
  // we ensure a valid `CurrentDecl` even outside of a function. This allows
  // expansion of macros into empty string literals without additional checks.
  Decl *CurrentDecl = getCurLocalScopeDecl();
  if (!CurrentDecl)
    CurrentDecl = Context.getTranslationUnitDecl();

  std::vector<Token> ExpandedToks;
  ExpandedToks.reserve(Toks.size());
  for (const Token &Tok : Toks) {
    if (!isFunctionLocalStringLiteralMacro(Tok.getKind(), getLangOpts())) {
      assert(tok::isStringLiteral(Tok.getKind()));
      ExpandedToks.emplace_back(Tok);
      continue;
    }
    if (isa<TranslationUnitDecl>(CurrentDecl))
      Diag(Tok.getLocation(), diag::ext_predef_outside_function);
    // Stringify predefined expression
    Diag(Tok.getLocation(), diag::ext_string_literal_from_predefined)
        << Tok.getKind();
    SmallString<64> Str;
    llvm::raw_svector_ostream OS(Str);
    Token &Exp = ExpandedToks.emplace_back();
    Exp.startToken();
    if (Tok.getKind() == tok::kw_L__FUNCTION__ ||
        Tok.getKind() == tok::kw_L__FUNCSIG__) {
      OS << 'L';
      Exp.setKind(tok::wide_string_literal);
    } else {
      Exp.setKind(tok::string_literal);
    }
    OS << '"'
       << Lexer::Stringify(PredefinedExpr::ComputeName(
              getPredefinedExprKind(Tok.getKind()), CurrentDecl))
       << '"';
    PP.CreateString(OS.str(), Exp, Tok.getLocation(), Tok.getEndLoc());
  }
  return ExpandedToks;
}

/// ActOnStringLiteral - The specified tokens were lexed as pasted string
/// fragments (e.g. "foo" "bar" L"baz").  The result string has to handle string
/// concatenation ([C99 5.1.1.2, translation phase #6]), so it may come from
/// multiple tokens.  However, the common case is that StringToks points to one
/// string.
///
ExprResult
Sema::ActOnStringLiteral(ArrayRef<Token> StringToks, Scope *UDLScope) {
  assert(!StringToks.empty() && "Must have at least one string!");

  // StringToks needs backing storage as it doesn't hold array elements itself
  std::vector<Token> ExpandedToks;
  if (getLangOpts().MicrosoftExt)
    StringToks = ExpandedToks = ExpandFunctionLocalPredefinedMacros(StringToks);

  StringLiteralParser Literal(StringToks, PP);
  if (Literal.hadError)
    return ExprError();

  SmallVector<SourceLocation, 4> StringTokLocs;
  for (const Token &Tok : StringToks)
    StringTokLocs.push_back(Tok.getLocation());

  QualType CharTy = Context.CharTy;
  StringLiteral::StringKind Kind = StringLiteral::Ordinary;
  if (Literal.isWide()) {
    CharTy = Context.getWideCharType();
    Kind = StringLiteral::Wide;
  } else if (Literal.isUTF8()) {
    if (getLangOpts().Char8)
      CharTy = Context.Char8Ty;
    Kind = StringLiteral::UTF8;
  } else if (Literal.isUTF16()) {
    CharTy = Context.Char16Ty;
    Kind = StringLiteral::UTF16;
  } else if (Literal.isUTF32()) {
    CharTy = Context.Char32Ty;
    Kind = StringLiteral::UTF32;
  } else if (Literal.isPascal()) {
    CharTy = Context.UnsignedCharTy;
  }

  // Warn on initializing an array of char from a u8 string literal; this
  // becomes ill-formed in C++2a.
  if (getLangOpts().CPlusPlus && !getLangOpts().CPlusPlus20 &&
      !getLangOpts().Char8 && Kind == StringLiteral::UTF8) {
    Diag(StringTokLocs.front(), diag::warn_cxx20_compat_utf8_string);

    // Create removals for all 'u8' prefixes in the string literal(s). This
    // ensures C++2a compatibility (but may change the program behavior when
    // built by non-Clang compilers for which the execution character set is
    // not always UTF-8).
    auto RemovalDiag = PDiag(diag::note_cxx20_compat_utf8_string_remove_u8);
    SourceLocation RemovalDiagLoc;
    for (const Token &Tok : StringToks) {
      if (Tok.getKind() == tok::utf8_string_literal) {
        if (RemovalDiagLoc.isInvalid())
          RemovalDiagLoc = Tok.getLocation();
        RemovalDiag << FixItHint::CreateRemoval(CharSourceRange::getCharRange(
            Tok.getLocation(),
            Lexer::AdvanceToTokenCharacter(Tok.getLocation(), 2,
                                           getSourceManager(), getLangOpts())));
      }
    }
    Diag(RemovalDiagLoc, RemovalDiag);
  }

  QualType StrTy =
      Context.getStringLiteralArrayType(CharTy, Literal.GetNumStringChars());

  // Pass &StringTokLocs[0], StringTokLocs.size() to factory!
  StringLiteral *Lit = StringLiteral::Create(Context, Literal.GetString(),
                                             Kind, Literal.Pascal, StrTy,
                                             &StringTokLocs[0],
                                             StringTokLocs.size());
  if (Literal.getUDSuffix().empty())
    return Lit;

  // We're building a user-defined literal.
  IdentifierInfo *UDSuffix = &Context.Idents.get(Literal.getUDSuffix());
  SourceLocation UDSuffixLoc =
    getUDSuffixLoc(*this, StringTokLocs[Literal.getUDSuffixToken()],
                   Literal.getUDSuffixOffset());

  // Make sure we're allowed user-defined literals here.
  if (!UDLScope)
    return ExprError(Diag(UDSuffixLoc, diag::err_invalid_string_udl));

  // C++11 [lex.ext]p5: The literal L is treated as a call of the form
  //   operator "" X (str, len)
  QualType SizeType = Context.getSizeType();

  DeclarationName OpName =
    Context.DeclarationNames.getCXXLiteralOperatorName(UDSuffix);
  DeclarationNameInfo OpNameInfo(OpName, UDSuffixLoc);
  OpNameInfo.setCXXLiteralOperatorNameLoc(UDSuffixLoc);

  QualType ArgTy[] = {
    Context.getArrayDecayedType(StrTy), SizeType
  };

  LookupResult R(*this, OpName, UDSuffixLoc, LookupOrdinaryName);
  switch (LookupLiteralOperator(UDLScope, R, ArgTy,
                                /*AllowRaw*/ false, /*AllowTemplate*/ true,
                                /*AllowStringTemplatePack*/ true,
                                /*DiagnoseMissing*/ true, Lit)) {

  case LOLR_Cooked: {
    llvm::APInt Len(Context.getIntWidth(SizeType), Literal.GetNumStringChars());
    IntegerLiteral *LenArg = IntegerLiteral::Create(Context, Len, SizeType,
                                                    StringTokLocs[0]);
    Expr *Args[] = { Lit, LenArg };

    return BuildLiteralOperatorCall(R, OpNameInfo, Args, StringTokLocs.back());
  }

  case LOLR_Template: {
    TemplateArgumentListInfo ExplicitArgs;
    TemplateArgument Arg(Lit);
    TemplateArgumentLocInfo ArgInfo(Lit);
    ExplicitArgs.addArgument(TemplateArgumentLoc(Arg, ArgInfo));
    return BuildLiteralOperatorCall(R, OpNameInfo, std::nullopt,
                                    StringTokLocs.back(), &ExplicitArgs);
  }

  case LOLR_StringTemplatePack: {
    TemplateArgumentListInfo ExplicitArgs;

    unsigned CharBits = Context.getIntWidth(CharTy);
    bool CharIsUnsigned = CharTy->isUnsignedIntegerType();
    llvm::APSInt Value(CharBits, CharIsUnsigned);

    TemplateArgument TypeArg(CharTy);
    TemplateArgumentLocInfo TypeArgInfo(Context.getTrivialTypeSourceInfo(CharTy));
    ExplicitArgs.addArgument(TemplateArgumentLoc(TypeArg, TypeArgInfo));

    for (unsigned I = 0, N = Lit->getLength(); I != N; ++I) {
      Value = Lit->getCodeUnit(I);
      TemplateArgument Arg(Context, Value, CharTy);
      TemplateArgumentLocInfo ArgInfo;
      ExplicitArgs.addArgument(TemplateArgumentLoc(Arg, ArgInfo));
    }
    return BuildLiteralOperatorCall(R, OpNameInfo, std::nullopt,
                                    StringTokLocs.back(), &ExplicitArgs);
  }
  case LOLR_Raw:
  case LOLR_ErrorNoDiagnostic:
    llvm_unreachable("unexpected literal operator lookup result");
  case LOLR_Error:
    return ExprError();
  }
  llvm_unreachable("unexpected literal operator lookup result");
}

DeclRefExpr *
Sema::BuildDeclRefExpr(ValueDecl *D, QualType Ty, ExprValueKind VK,
                       SourceLocation Loc,
                       const CXXScopeSpec *SS) {
  DeclarationNameInfo NameInfo(D->getDeclName(), Loc);
  return BuildDeclRefExpr(D, Ty, VK, NameInfo, SS);
}

DeclRefExpr *
Sema::BuildDeclRefExpr(ValueDecl *D, QualType Ty, ExprValueKind VK,
                       const DeclarationNameInfo &NameInfo,
                       const CXXScopeSpec *SS, NamedDecl *FoundD,
                       SourceLocation TemplateKWLoc,
                       const TemplateArgumentListInfo *TemplateArgs) {
  NestedNameSpecifierLoc NNS =
      SS ? SS->getWithLocInContext(Context) : NestedNameSpecifierLoc();
  return BuildDeclRefExpr(D, Ty, VK, NameInfo, NNS, FoundD, TemplateKWLoc,
                          TemplateArgs);
}

// CUDA/HIP: Check whether a captured reference variable is referencing a
// host variable in a device or host device lambda.
static bool isCapturingReferenceToHostVarInCUDADeviceLambda(const Sema &S,
                                                            VarDecl *VD) {
  if (!S.getLangOpts().CUDA || !VD->hasInit())
    return false;
  assert(VD->getType()->isReferenceType());

  // Check whether the reference variable is referencing a host variable.
  auto *DRE = dyn_cast<DeclRefExpr>(VD->getInit());
  if (!DRE)
    return false;
  auto *Referee = dyn_cast<VarDecl>(DRE->getDecl());
  if (!Referee || !Referee->hasGlobalStorage() ||
      Referee->hasAttr<CUDADeviceAttr>())
    return false;

  // Check whether the current function is a device or host device lambda.
  // Check whether the reference variable is a capture by getDeclContext()
  // since refersToEnclosingVariableOrCapture() is not ready at this point.
  auto *MD = dyn_cast_or_null<CXXMethodDecl>(S.CurContext);
  if (MD && MD->getParent()->isLambda() &&
      MD->getOverloadedOperator() == OO_Call && MD->hasAttr<CUDADeviceAttr>() &&
      VD->getDeclContext() != MD)
    return true;

  return false;
}

NonOdrUseReason Sema::getNonOdrUseReasonInCurrentContext(ValueDecl *D) {
  // A declaration named in an unevaluated operand never constitutes an odr-use.
  if (isUnevaluatedContext())
    return NOUR_Unevaluated;

  // C++2a [basic.def.odr]p4:
  //   A variable x whose name appears as a potentially-evaluated expression e
  //   is odr-used by e unless [...] x is a reference that is usable in
  //   constant expressions.
  // CUDA/HIP:
  //   If a reference variable referencing a host variable is captured in a
  //   device or host device lambda, the value of the referee must be copied
  //   to the capture and the reference variable must be treated as odr-use
  //   since the value of the referee is not known at compile time and must
  //   be loaded from the captured.
  if (VarDecl *VD = dyn_cast<VarDecl>(D)) {
    if (VD->getType()->isReferenceType() &&
        !(getLangOpts().OpenMP && isOpenMPCapturedDecl(D)) &&
        !isCapturingReferenceToHostVarInCUDADeviceLambda(*this, VD) &&
        VD->isUsableInConstantExpressions(Context))
      return NOUR_Constant;
  }

  // All remaining non-variable cases constitute an odr-use. For variables, we
  // need to wait and see how the expression is used.
  return NOUR_None;
}

/// BuildDeclRefExpr - Build an expression that references a
/// declaration that does not require a closure capture.
DeclRefExpr *
Sema::BuildDeclRefExpr(ValueDecl *D, QualType Ty, ExprValueKind VK,
                       const DeclarationNameInfo &NameInfo,
                       NestedNameSpecifierLoc NNS, NamedDecl *FoundD,
                       SourceLocation TemplateKWLoc,
                       const TemplateArgumentListInfo *TemplateArgs) {
  bool RefersToCapturedVariable = isa<VarDecl, BindingDecl>(D) &&
                                  NeedToCaptureVariable(D, NameInfo.getLoc());

  DeclRefExpr *E = DeclRefExpr::Create(
      Context, NNS, TemplateKWLoc, D, RefersToCapturedVariable, NameInfo, Ty,
      VK, FoundD, TemplateArgs, getNonOdrUseReasonInCurrentContext(D));
  MarkDeclRefReferenced(E);

  // C++ [except.spec]p17:
  //   An exception-specification is considered to be needed when:
  //   - in an expression, the function is the unique lookup result or
  //     the selected member of a set of overloaded functions.
  //
  // We delay doing this until after we've built the function reference and
  // marked it as used so that:
  //  a) if the function is defaulted, we get errors from defining it before /
  //     instead of errors from computing its exception specification, and
  //  b) if the function is a defaulted comparison, we can use the body we
  //     build when defining it as input to the exception specification
  //     computation rather than computing a new body.
  if (const auto *FPT = Ty->getAs<FunctionProtoType>()) {
    if (isUnresolvedExceptionSpec(FPT->getExceptionSpecType())) {
      if (const auto *NewFPT = ResolveExceptionSpec(NameInfo.getLoc(), FPT))
        E->setType(Context.getQualifiedType(NewFPT, Ty.getQualifiers()));
    }
  }

  if (getLangOpts().ObjCWeak && isa<VarDecl>(D) &&
      Ty.getObjCLifetime() == Qualifiers::OCL_Weak && !isUnevaluatedContext() &&
      !Diags.isIgnored(diag::warn_arc_repeated_use_of_weak, E->getBeginLoc()))
    getCurFunction()->recordUseOfWeak(E);

  const auto *FD = dyn_cast<FieldDecl>(D);
  if (const auto *IFD = dyn_cast<IndirectFieldDecl>(D))
    FD = IFD->getAnonField();
  if (FD) {
    UnusedPrivateFields.remove(FD);
    // Just in case we're building an illegal pointer-to-member.
    if (FD->isBitField())
      E->setObjectKind(OK_BitField);
  }

  // C++ [expr.prim]/8: The expression [...] is a bit-field if the identifier
  // designates a bit-field.
  if (const auto *BD = dyn_cast<BindingDecl>(D))
    if (const auto *BE = BD->getBinding())
      E->setObjectKind(BE->getObjectKind());

  return E;
}

/// Decomposes the given name into a DeclarationNameInfo, its location, and
/// possibly a list of template arguments.
///
/// If this produces template arguments, it is permitted to call
/// DecomposeTemplateName.
///
/// This actually loses a lot of source location information for
/// non-standard name kinds; we should consider preserving that in
/// some way.
void
Sema::DecomposeUnqualifiedId(const UnqualifiedId &Id,
                             TemplateArgumentListInfo &Buffer,
                             DeclarationNameInfo &NameInfo,
                             const TemplateArgumentListInfo *&TemplateArgs) {
  if (Id.getKind() == UnqualifiedIdKind::IK_TemplateId) {
    Buffer.setLAngleLoc(Id.TemplateId->LAngleLoc);
    Buffer.setRAngleLoc(Id.TemplateId->RAngleLoc);

    ASTTemplateArgsPtr TemplateArgsPtr(Id.TemplateId->getTemplateArgs(),
                                       Id.TemplateId->NumArgs);
    translateTemplateArguments(TemplateArgsPtr, Buffer);

    TemplateName TName = Id.TemplateId->Template.get();
    SourceLocation TNameLoc = Id.TemplateId->TemplateNameLoc;
    NameInfo = Context.getNameForTemplate(TName, TNameLoc);
    TemplateArgs = &Buffer;
  } else {
    NameInfo = GetNameFromUnqualifiedId(Id);
    TemplateArgs = nullptr;
  }
}

static void emitEmptyLookupTypoDiagnostic(
    const TypoCorrection &TC, Sema &SemaRef, const CXXScopeSpec &SS,
    DeclarationName Typo, SourceLocation TypoLoc, ArrayRef<Expr *> Args,
    unsigned DiagnosticID, unsigned DiagnosticSuggestID) {
  DeclContext *Ctx =
      SS.isEmpty() ? nullptr : SemaRef.computeDeclContext(SS, false);
  if (!TC) {
    // Emit a special diagnostic for failed member lookups.
    // FIXME: computing the declaration context might fail here (?)
    if (Ctx)
      SemaRef.Diag(TypoLoc, diag::err_no_member) << Typo << Ctx
                                                 << SS.getRange();
    else
      SemaRef.Diag(TypoLoc, DiagnosticID) << Typo;
    return;
  }

  std::string CorrectedStr = TC.getAsString(SemaRef.getLangOpts());
  bool DroppedSpecifier =
      TC.WillReplaceSpecifier() && Typo.getAsString() == CorrectedStr;
  unsigned NoteID = TC.getCorrectionDeclAs<ImplicitParamDecl>()
                        ? diag::note_implicit_param_decl
                        : diag::note_previous_decl;
  if (!Ctx)
    SemaRef.diagnoseTypo(TC, SemaRef.PDiag(DiagnosticSuggestID) << Typo,
                         SemaRef.PDiag(NoteID));
  else
    SemaRef.diagnoseTypo(TC, SemaRef.PDiag(diag::err_no_member_suggest)
                                 << Typo << Ctx << DroppedSpecifier
                                 << SS.getRange(),
                         SemaRef.PDiag(NoteID));
}

/// Diagnose a lookup that found results in an enclosing class during error
/// recovery. This usually indicates that the results were found in a dependent
/// base class that could not be searched as part of a template definition.
/// Always issues a diagnostic (though this may be only a warning in MS
/// compatibility mode).
///
/// Return \c true if the error is unrecoverable, or \c false if the caller
/// should attempt to recover using these lookup results.
bool Sema::DiagnoseDependentMemberLookup(const LookupResult &R) {
  // During a default argument instantiation the CurContext points
  // to a CXXMethodDecl; but we can't apply a this-> fixit inside a
  // function parameter list, hence add an explicit check.
  bool isDefaultArgument =
      !CodeSynthesisContexts.empty() &&
      CodeSynthesisContexts.back().Kind ==
          CodeSynthesisContext::DefaultFunctionArgumentInstantiation;
  const auto *CurMethod = dyn_cast<CXXMethodDecl>(CurContext);
  bool isInstance = CurMethod && CurMethod->isInstance() &&
                    R.getNamingClass() == CurMethod->getParent() &&
                    !isDefaultArgument;

  // There are two ways we can find a class-scope declaration during template
  // instantiation that we did not find in the template definition: if it is a
  // member of a dependent base class, or if it is declared after the point of
  // use in the same class. Distinguish these by comparing the class in which
  // the member was found to the naming class of the lookup.
  unsigned DiagID = diag::err_found_in_dependent_base;
  unsigned NoteID = diag::note_member_declared_at;
  if (R.getRepresentativeDecl()->getDeclContext()->Equals(R.getNamingClass())) {
    DiagID = getLangOpts().MSVCCompat ? diag::ext_found_later_in_class
                                      : diag::err_found_later_in_class;
  } else if (getLangOpts().MSVCCompat) {
    DiagID = diag::ext_found_in_dependent_base;
    NoteID = diag::note_dependent_member_use;
  }

  if (isInstance) {
    // Give a code modification hint to insert 'this->'.
    Diag(R.getNameLoc(), DiagID)
        << R.getLookupName()
        << FixItHint::CreateInsertion(R.getNameLoc(), "this->");
    CheckCXXThisCapture(R.getNameLoc());
  } else {
    // FIXME: Add a FixItHint to insert 'Base::' or 'Derived::' (assuming
    // they're not shadowed).
    Diag(R.getNameLoc(), DiagID) << R.getLookupName();
  }

  for (const NamedDecl *D : R)
    Diag(D->getLocation(), NoteID);

  // Return true if we are inside a default argument instantiation
  // and the found name refers to an instance member function, otherwise
  // the caller will try to create an implicit member call and this is wrong
  // for default arguments.
  //
  // FIXME: Is this special case necessary? We could allow the caller to
  // diagnose this.
  if (isDefaultArgument && ((*R.begin())->isCXXInstanceMember())) {
    Diag(R.getNameLoc(), diag::err_member_call_without_object);
    return true;
  }

  // Tell the callee to try to recover.
  return false;
}

/// Diagnose an empty lookup.
///
/// \return false if new lookup candidates were found
bool Sema::DiagnoseEmptyLookup(Scope *S, CXXScopeSpec &SS, LookupResult &R,
                               CorrectionCandidateCallback &CCC,
                               TemplateArgumentListInfo *ExplicitTemplateArgs,
                               ArrayRef<Expr *> Args, TypoExpr **Out) {
  DeclarationName Name = R.getLookupName();

  unsigned diagnostic = diag::err_undeclared_var_use;
  unsigned diagnostic_suggest = diag::err_undeclared_var_use_suggest;
  if (Name.getNameKind() == DeclarationName::CXXOperatorName ||
      Name.getNameKind() == DeclarationName::CXXLiteralOperatorName ||
      Name.getNameKind() == DeclarationName::CXXConversionFunctionName) {
    diagnostic = diag::err_undeclared_use;
    diagnostic_suggest = diag::err_undeclared_use_suggest;
  }

  // If the original lookup was an unqualified lookup, fake an
  // unqualified lookup.  This is useful when (for example) the
  // original lookup would not have found something because it was a
  // dependent name.
  DeclContext *DC = SS.isEmpty() ? CurContext : nullptr;
  while (DC) {
    if (isa<CXXRecordDecl>(DC)) {
      LookupQualifiedName(R, DC);

      if (!R.empty()) {
        // Don't give errors about ambiguities in this lookup.
        R.suppressDiagnostics();

        // If there's a best viable function among the results, only mention
        // that one in the notes.
        OverloadCandidateSet Candidates(R.getNameLoc(),
                                        OverloadCandidateSet::CSK_Normal);
        AddOverloadedCallCandidates(R, ExplicitTemplateArgs, Args, Candidates);
        OverloadCandidateSet::iterator Best;
        if (Candidates.BestViableFunction(*this, R.getNameLoc(), Best) ==
            OR_Success) {
          R.clear();
          R.addDecl(Best->FoundDecl.getDecl(), Best->FoundDecl.getAccess());
          R.resolveKind();
        }

        return DiagnoseDependentMemberLookup(R);
      }

      R.clear();
    }

    DC = DC->getLookupParent();
  }

  // We didn't find anything, so try to correct for a typo.
  TypoCorrection Corrected;
  if (S && Out) {
    SourceLocation TypoLoc = R.getNameLoc();
    assert(!ExplicitTemplateArgs &&
           "Diagnosing an empty lookup with explicit template args!");
    *Out = CorrectTypoDelayed(
        R.getLookupNameInfo(), R.getLookupKind(), S, &SS, CCC,
        [=](const TypoCorrection &TC) {
          emitEmptyLookupTypoDiagnostic(TC, *this, SS, Name, TypoLoc, Args,
                                        diagnostic, diagnostic_suggest);
        },
        nullptr, CTK_ErrorRecovery);
    if (*Out)
      return true;
  } else if (S &&
             (Corrected = CorrectTypo(R.getLookupNameInfo(), R.getLookupKind(),
                                      S, &SS, CCC, CTK_ErrorRecovery))) {
    std::string CorrectedStr(Corrected.getAsString(getLangOpts()));
    bool DroppedSpecifier =
        Corrected.WillReplaceSpecifier() && Name.getAsString() == CorrectedStr;
    R.setLookupName(Corrected.getCorrection());

    bool AcceptableWithRecovery = false;
    bool AcceptableWithoutRecovery = false;
    NamedDecl *ND = Corrected.getFoundDecl();
    if (ND) {
      if (Corrected.isOverloaded()) {
        OverloadCandidateSet OCS(R.getNameLoc(),
                                 OverloadCandidateSet::CSK_Normal);
        OverloadCandidateSet::iterator Best;
        for (NamedDecl *CD : Corrected) {
          if (FunctionTemplateDecl *FTD =
                   dyn_cast<FunctionTemplateDecl>(CD))
            AddTemplateOverloadCandidate(
                FTD, DeclAccessPair::make(FTD, AS_none), ExplicitTemplateArgs,
                Args, OCS);
          else if (FunctionDecl *FD = dyn_cast<FunctionDecl>(CD))
            if (!ExplicitTemplateArgs || ExplicitTemplateArgs->size() == 0)
              AddOverloadCandidate(FD, DeclAccessPair::make(FD, AS_none),
                                   Args, OCS);
        }
        switch (OCS.BestViableFunction(*this, R.getNameLoc(), Best)) {
        case OR_Success:
          ND = Best->FoundDecl;
          Corrected.setCorrectionDecl(ND);
          break;
        default:
          // FIXME: Arbitrarily pick the first declaration for the note.
          Corrected.setCorrectionDecl(ND);
          break;
        }
      }
      R.addDecl(ND);
      if (getLangOpts().CPlusPlus && ND->isCXXClassMember()) {
        CXXRecordDecl *Record = nullptr;
        if (Corrected.getCorrectionSpecifier()) {
          const Type *Ty = Corrected.getCorrectionSpecifier()->getAsType();
          Record = Ty->getAsCXXRecordDecl();
        }
        if (!Record)
          Record = cast<CXXRecordDecl>(
              ND->getDeclContext()->getRedeclContext());
        R.setNamingClass(Record);
      }

      auto *UnderlyingND = ND->getUnderlyingDecl();
      AcceptableWithRecovery = isa<ValueDecl>(UnderlyingND) ||
                               isa<FunctionTemplateDecl>(UnderlyingND);
      // FIXME: If we ended up with a typo for a type name or
      // Objective-C class name, we're in trouble because the parser
      // is in the wrong place to recover. Suggest the typo
      // correction, but don't make it a fix-it since we're not going
      // to recover well anyway.
      AcceptableWithoutRecovery = isa<TypeDecl>(UnderlyingND) ||
                                  getAsTypeTemplateDecl(UnderlyingND) ||
                                  isa<ObjCInterfaceDecl>(UnderlyingND);
    } else {
      // FIXME: We found a keyword. Suggest it, but don't provide a fix-it
      // because we aren't able to recover.
      AcceptableWithoutRecovery = true;
    }

    if (AcceptableWithRecovery || AcceptableWithoutRecovery) {
      unsigned NoteID = Corrected.getCorrectionDeclAs<ImplicitParamDecl>()
                            ? diag::note_implicit_param_decl
                            : diag::note_previous_decl;
      if (SS.isEmpty())
        diagnoseTypo(Corrected, PDiag(diagnostic_suggest) << Name,
                     PDiag(NoteID), AcceptableWithRecovery);
      else
        diagnoseTypo(Corrected, PDiag(diag::err_no_member_suggest)
                                  << Name << computeDeclContext(SS, false)
                                  << DroppedSpecifier << SS.getRange(),
                     PDiag(NoteID), AcceptableWithRecovery);

      // Tell the callee whether to try to recover.
      return !AcceptableWithRecovery;
    }
  }
  R.clear();

  // Emit a special diagnostic for failed member lookups.
  // FIXME: computing the declaration context might fail here (?)
  if (!SS.isEmpty()) {
    Diag(R.getNameLoc(), diag::err_no_member)
      << Name << computeDeclContext(SS, false)
      << SS.getRange();
    return true;
  }

  // Give up, we can't recover.
  Diag(R.getNameLoc(), diagnostic) << Name;
  return true;
}

/// In Microsoft mode, if we are inside a template class whose parent class has
/// dependent base classes, and we can't resolve an unqualified identifier, then
/// assume the identifier is a member of a dependent base class.  We can only
/// recover successfully in static methods, instance methods, and other contexts
/// where 'this' is available.  This doesn't precisely match MSVC's
/// instantiation model, but it's close enough.
static Expr *
recoverFromMSUnqualifiedLookup(Sema &S, ASTContext &Context,
                               DeclarationNameInfo &NameInfo,
                               SourceLocation TemplateKWLoc,
                               const TemplateArgumentListInfo *TemplateArgs) {
  // Only try to recover from lookup into dependent bases in static methods or
  // contexts where 'this' is available.
  QualType ThisType = S.getCurrentThisType();
  const CXXRecordDecl *RD = nullptr;
  if (!ThisType.isNull())
    RD = ThisType->getPointeeType()->getAsCXXRecordDecl();
  else if (auto *MD = dyn_cast<CXXMethodDecl>(S.CurContext))
    RD = MD->getParent();
  if (!RD || !RD->hasAnyDependentBases())
    return nullptr;

  // Diagnose this as unqualified lookup into a dependent base class.  If 'this'
  // is available, suggest inserting 'this->' as a fixit.
  SourceLocation Loc = NameInfo.getLoc();
  auto DB = S.Diag(Loc, diag::ext_undeclared_unqual_id_with_dependent_base);
  DB << NameInfo.getName() << RD;

  if (!ThisType.isNull()) {
    DB << FixItHint::CreateInsertion(Loc, "this->");
    return CXXDependentScopeMemberExpr::Create(
        Context, /*This=*/nullptr, ThisType, /*IsArrow=*/true,
        /*Op=*/SourceLocation(), NestedNameSpecifierLoc(), TemplateKWLoc,
        /*FirstQualifierFoundInScope=*/nullptr, NameInfo, TemplateArgs);
  }

  // Synthesize a fake NNS that points to the derived class.  This will
  // perform name lookup during template instantiation.
  CXXScopeSpec SS;
  auto *NNS =
      NestedNameSpecifier::Create(Context, nullptr, true, RD->getTypeForDecl());
  SS.MakeTrivial(Context, NNS, SourceRange(Loc, Loc));
  return DependentScopeDeclRefExpr::Create(
      Context, SS.getWithLocInContext(Context), TemplateKWLoc, NameInfo,
      TemplateArgs);
}

ExprResult
Sema::ActOnIdExpression(Scope *S, CXXScopeSpec &SS,
                        SourceLocation TemplateKWLoc, UnqualifiedId &Id,
                        bool HasTrailingLParen, bool IsAddressOfOperand,
                        CorrectionCandidateCallback *CCC,
                        bool IsInlineAsmIdentifier, Token *KeywordReplacement) {
  assert(!(IsAddressOfOperand && HasTrailingLParen) &&
         "cannot be direct & operand and have a trailing lparen");
  if (SS.isInvalid())
    return ExprError();

  TemplateArgumentListInfo TemplateArgsBuffer;

  // Decompose the UnqualifiedId into the following data.
  DeclarationNameInfo NameInfo;
  const TemplateArgumentListInfo *TemplateArgs;
  DecomposeUnqualifiedId(Id, TemplateArgsBuffer, NameInfo, TemplateArgs);

  DeclarationName Name = NameInfo.getName();
  IdentifierInfo *II = Name.getAsIdentifierInfo();
  SourceLocation NameLoc = NameInfo.getLoc();

  if (II && II->isEditorPlaceholder()) {
    // FIXME: When typed placeholders are supported we can create a typed
    // placeholder expression node.
    return ExprError();
  }

  // C++ [temp.dep.expr]p3:
  //   An id-expression is type-dependent if it contains:
  //     -- an identifier that was declared with a dependent type,
  //        (note: handled after lookup)
  //     -- a template-id that is dependent,
  //        (note: handled in BuildTemplateIdExpr)
  //     -- a conversion-function-id that specifies a dependent type,
  //     -- a nested-name-specifier that contains a class-name that
  //        names a dependent type.
  // Determine whether this is a member of an unknown specialization;
  // we need to handle these differently.
  bool DependentID = false;
  if (Name.getNameKind() == DeclarationName::CXXConversionFunctionName &&
      Name.getCXXNameType()->isDependentType()) {
    DependentID = true;
  } else if (SS.isSet()) {
    if (DeclContext *DC = computeDeclContext(SS, false)) {
      if (RequireCompleteDeclContext(SS, DC))
        return ExprError();
    } else {
      DependentID = true;
    }
  }

  if (DependentID)
    return ActOnDependentIdExpression(SS, TemplateKWLoc, NameInfo,
                                      IsAddressOfOperand, TemplateArgs);

  // Perform the required lookup.
  LookupResult R(*this, NameInfo,
                 (Id.getKind() == UnqualifiedIdKind::IK_ImplicitSelfParam)
                     ? LookupObjCImplicitSelfParam
                     : LookupOrdinaryName);
  if (TemplateKWLoc.isValid() || TemplateArgs) {
    // Lookup the template name again to correctly establish the context in
    // which it was found. This is really unfortunate as we already did the
    // lookup to determine that it was a template name in the first place. If
    // this becomes a performance hit, we can work harder to preserve those
    // results until we get here but it's likely not worth it.
    bool MemberOfUnknownSpecialization;
    AssumedTemplateKind AssumedTemplate;
    if (LookupTemplateName(R, S, SS, QualType(), /*EnteringContext=*/false,
                           MemberOfUnknownSpecialization, TemplateKWLoc,
                           &AssumedTemplate))
      return ExprError();

    if (MemberOfUnknownSpecialization ||
        (R.getResultKind() == LookupResult::NotFoundInCurrentInstantiation))
      return ActOnDependentIdExpression(SS, TemplateKWLoc, NameInfo,
                                        IsAddressOfOperand, TemplateArgs);
  } else {
    bool IvarLookupFollowUp = II && !SS.isSet() && getCurMethodDecl();
    LookupParsedName(R, S, &SS, !IvarLookupFollowUp);

    // If the result might be in a dependent base class, this is a dependent
    // id-expression.
    if (R.getResultKind() == LookupResult::NotFoundInCurrentInstantiation)
      return ActOnDependentIdExpression(SS, TemplateKWLoc, NameInfo,
                                        IsAddressOfOperand, TemplateArgs);

    // If this reference is in an Objective-C method, then we need to do
    // some special Objective-C lookup, too.
    if (IvarLookupFollowUp) {
      ExprResult E(LookupInObjCMethod(R, S, II, true));
      if (E.isInvalid())
        return ExprError();

      if (Expr *Ex = E.getAs<Expr>())
        return Ex;
    }
  }

  if (R.isAmbiguous())
    return ExprError();

  // This could be an implicitly declared function reference if the language
  // mode allows it as a feature.
  if (R.empty() && HasTrailingLParen && II &&
      getLangOpts().implicitFunctionsAllowed()) {
    NamedDecl *D = ImplicitlyDefineFunction(NameLoc, *II, S);
    if (D) R.addDecl(D);
  }

  // Determine whether this name might be a candidate for
  // argument-dependent lookup.
  bool ADL = UseArgumentDependentLookup(SS, R, HasTrailingLParen);

  if (R.empty() && !ADL) {
    if (SS.isEmpty() && getLangOpts().MSVCCompat) {
      if (Expr *E = recoverFromMSUnqualifiedLookup(*this, Context, NameInfo,
                                                   TemplateKWLoc, TemplateArgs))
        return E;
    }

    // Don't diagnose an empty lookup for inline assembly.
    if (IsInlineAsmIdentifier)
      return ExprError();

    // If this name wasn't predeclared and if this is not a function
    // call, diagnose the problem.
    TypoExpr *TE = nullptr;
    DefaultFilterCCC DefaultValidator(II, SS.isValid() ? SS.getScopeRep()
                                                       : nullptr);
    DefaultValidator.IsAddressOfOperand = IsAddressOfOperand;
    assert((!CCC || CCC->IsAddressOfOperand == IsAddressOfOperand) &&
           "Typo correction callback misconfigured");
    if (CCC) {
      // Make sure the callback knows what the typo being diagnosed is.
      CCC->setTypoName(II);
      if (SS.isValid())
        CCC->setTypoNNS(SS.getScopeRep());
    }
    // FIXME: DiagnoseEmptyLookup produces bad diagnostics if we're looking for
    // a template name, but we happen to have always already looked up the name
    // before we get here if it must be a template name.
    if (DiagnoseEmptyLookup(S, SS, R, CCC ? *CCC : DefaultValidator, nullptr,
                            std::nullopt, &TE)) {
      if (TE && KeywordReplacement) {
        auto &State = getTypoExprState(TE);
        auto BestTC = State.Consumer->getNextCorrection();
        if (BestTC.isKeyword()) {
          auto *II = BestTC.getCorrectionAsIdentifierInfo();
          if (State.DiagHandler)
            State.DiagHandler(BestTC);
          KeywordReplacement->startToken();
          KeywordReplacement->setKind(II->getTokenID());
          KeywordReplacement->setIdentifierInfo(II);
          KeywordReplacement->setLocation(BestTC.getCorrectionRange().getBegin());
          // Clean up the state associated with the TypoExpr, since it has
          // now been diagnosed (without a call to CorrectDelayedTyposInExpr).
          clearDelayedTypo(TE);
          // Signal that a correction to a keyword was performed by returning a
          // valid-but-null ExprResult.
          return (Expr*)nullptr;
        }
        State.Consumer->resetCorrectionStream();
      }
      return TE ? TE : ExprError();
    }

    assert(!R.empty() &&
           "DiagnoseEmptyLookup returned false but added no results");

    // If we found an Objective-C instance variable, let
    // LookupInObjCMethod build the appropriate expression to
    // reference the ivar.
    if (ObjCIvarDecl *Ivar = R.getAsSingle<ObjCIvarDecl>()) {
      R.clear();
      ExprResult E(LookupInObjCMethod(R, S, Ivar->getIdentifier()));
      // In a hopelessly buggy code, Objective-C instance variable
      // lookup fails and no expression will be built to reference it.
      if (!E.isInvalid() && !E.get())
        return ExprError();
      return E;
    }
  }

  // This is guaranteed from this point on.
  assert(!R.empty() || ADL);

  // Check whether this might be a C++ implicit instance member access.
  // C++ [class.mfct.non-static]p3:
  //   When an id-expression that is not part of a class member access
  //   syntax and not used to form a pointer to member is used in the
  //   body of a non-static member function of class X, if name lookup
  //   resolves the name in the id-expression to a non-static non-type
  //   member of some class C, the id-expression is transformed into a
  //   class member access expression using (*this) as the
  //   postfix-expression to the left of the . operator.
  //
  // But we don't actually need to do this for '&' operands if R
  // resolved to a function or overloaded function set, because the
  // expression is ill-formed if it actually works out to be a
  // non-static member function:
  //
  // C++ [expr.ref]p4:
  //   Otherwise, if E1.E2 refers to a non-static member function. . .
  //   [t]he expression can be used only as the left-hand operand of a
  //   member function call.
  //
  // There are other safeguards against such uses, but it's important
  // to get this right here so that we don't end up making a
  // spuriously dependent expression if we're inside a dependent
  // instance method.
  if (!R.empty() && (*R.begin())->isCXXClassMember()) {
    bool MightBeImplicitMember;
    if (!IsAddressOfOperand)
      MightBeImplicitMember = true;
    else if (!SS.isEmpty())
      MightBeImplicitMember = false;
    else if (R.isOverloadedResult())
      MightBeImplicitMember = false;
    else if (R.isUnresolvableResult())
      MightBeImplicitMember = true;
    else
      MightBeImplicitMember = isa<FieldDecl>(R.getFoundDecl()) ||
                              isa<IndirectFieldDecl>(R.getFoundDecl()) ||
                              isa<MSPropertyDecl>(R.getFoundDecl());

    if (MightBeImplicitMember)
      return BuildPossibleImplicitMemberExpr(SS, TemplateKWLoc,
                                             R, TemplateArgs, S);
  }

  if (TemplateArgs || TemplateKWLoc.isValid()) {

    // In C++1y, if this is a variable template id, then check it
    // in BuildTemplateIdExpr().
    // The single lookup result must be a variable template declaration.
    if (Id.getKind() == UnqualifiedIdKind::IK_TemplateId && Id.TemplateId &&
        Id.TemplateId->Kind == TNK_Var_template) {
      assert(R.getAsSingle<VarTemplateDecl>() &&
             "There should only be one declaration found.");
    }

    return BuildTemplateIdExpr(SS, TemplateKWLoc, R, ADL, TemplateArgs);
  }

  return BuildDeclarationNameExpr(SS, R, ADL);
}

/// BuildQualifiedDeclarationNameExpr - Build a C++ qualified
/// declaration name, generally during template instantiation.
/// There's a large number of things which don't need to be done along
/// this path.
ExprResult Sema::BuildQualifiedDeclarationNameExpr(
    CXXScopeSpec &SS, const DeclarationNameInfo &NameInfo,
    bool IsAddressOfOperand, const Scope *S, TypeSourceInfo **RecoveryTSI) {
  if (NameInfo.getName().isDependentName())
    return BuildDependentDeclRefExpr(SS, /*TemplateKWLoc=*/SourceLocation(),
                                     NameInfo, /*TemplateArgs=*/nullptr);

  DeclContext *DC = computeDeclContext(SS, false);
  if (!DC)
    return BuildDependentDeclRefExpr(SS, /*TemplateKWLoc=*/SourceLocation(),
                                     NameInfo, /*TemplateArgs=*/nullptr);

  if (RequireCompleteDeclContext(SS, DC))
    return ExprError();

  LookupResult R(*this, NameInfo, LookupOrdinaryName);
  LookupQualifiedName(R, DC);

  if (R.isAmbiguous())
    return ExprError();

  if (R.getResultKind() == LookupResult::NotFoundInCurrentInstantiation)
    return BuildDependentDeclRefExpr(SS, /*TemplateKWLoc=*/SourceLocation(),
                                     NameInfo, /*TemplateArgs=*/nullptr);

  if (R.empty()) {
    // Don't diagnose problems with invalid record decl, the secondary no_member
    // diagnostic during template instantiation is likely bogus, e.g. if a class
    // is invalid because it's derived from an invalid base class, then missing
    // members were likely supposed to be inherited.
    if (const auto *CD = dyn_cast<CXXRecordDecl>(DC))
      if (CD->isInvalidDecl())
        return ExprError();
    Diag(NameInfo.getLoc(), diag::err_no_member)
      << NameInfo.getName() << DC << SS.getRange();
    return ExprError();
  }

  if (const TypeDecl *TD = R.getAsSingle<TypeDecl>()) {
    // Diagnose a missing typename if this resolved unambiguously to a type in
    // a dependent context.  If we can recover with a type, downgrade this to
    // a warning in Microsoft compatibility mode.
    unsigned DiagID = diag::err_typename_missing;
    if (RecoveryTSI && getLangOpts().MSVCCompat)
      DiagID = diag::ext_typename_missing;
    SourceLocation Loc = SS.getBeginLoc();
    auto D = Diag(Loc, DiagID);
    D << SS.getScopeRep() << NameInfo.getName().getAsString()
      << SourceRange(Loc, NameInfo.getEndLoc());

    // Don't recover if the caller isn't expecting us to or if we're in a SFINAE
    // context.
    if (!RecoveryTSI)
      return ExprError();

    // Only issue the fixit if we're prepared to recover.
    D << FixItHint::CreateInsertion(Loc, "typename ");

    // Recover by pretending this was an elaborated type.
    QualType Ty = Context.getTypeDeclType(TD);
    TypeLocBuilder TLB;
    TLB.pushTypeSpec(Ty).setNameLoc(NameInfo.getLoc());

    QualType ET = getElaboratedType(ETK_None, SS, Ty);
    ElaboratedTypeLoc QTL = TLB.push<ElaboratedTypeLoc>(ET);
    QTL.setElaboratedKeywordLoc(SourceLocation());
    QTL.setQualifierLoc(SS.getWithLocInContext(Context));

    *RecoveryTSI = TLB.getTypeSourceInfo(Context, ET);

    return ExprEmpty();
  }

  // Defend against this resolving to an implicit member access. We usually
  // won't get here if this might be a legitimate a class member (we end up in
  // BuildMemberReferenceExpr instead), but this can be valid if we're forming
  // a pointer-to-member or in an unevaluated context in C++11.
  if (!R.empty() && (*R.begin())->isCXXClassMember() && !IsAddressOfOperand)
    return BuildPossibleImplicitMemberExpr(SS,
                                           /*TemplateKWLoc=*/SourceLocation(),
                                           R, /*TemplateArgs=*/nullptr, S);

  return BuildDeclarationNameExpr(SS, R, /* ADL */ false);
}

/// The parser has read a name in, and Sema has detected that we're currently
/// inside an ObjC method. Perform some additional checks and determine if we
/// should form a reference to an ivar.
///
/// Ideally, most of this would be done by lookup, but there's
/// actually quite a lot of extra work involved.
DeclResult Sema::LookupIvarInObjCMethod(LookupResult &Lookup, Scope *S,
                                        IdentifierInfo *II) {
  SourceLocation Loc = Lookup.getNameLoc();
  ObjCMethodDecl *CurMethod = getCurMethodDecl();

  // Check for error condition which is already reported.
  if (!CurMethod)
    return DeclResult(true);

  // There are two cases to handle here.  1) scoped lookup could have failed,
  // in which case we should look for an ivar.  2) scoped lookup could have
  // found a decl, but that decl is outside the current instance method (i.e.
  // a global variable).  In these two cases, we do a lookup for an ivar with
  // this name, if the lookup sucedes, we replace it our current decl.

  // If we're in a class method, we don't normally want to look for
  // ivars.  But if we don't find anything else, and there's an
  // ivar, that's an error.
  bool IsClassMethod = CurMethod->isClassMethod();

  bool LookForIvars;
  if (Lookup.empty())
    LookForIvars = true;
  else if (IsClassMethod)
    LookForIvars = false;
  else
    LookForIvars = (Lookup.isSingleResult() &&
                    Lookup.getFoundDecl()->isDefinedOutsideFunctionOrMethod());
  ObjCInterfaceDecl *IFace = nullptr;
  if (LookForIvars) {
    IFace = CurMethod->getClassInterface();
    ObjCInterfaceDecl *ClassDeclared;
    ObjCIvarDecl *IV = nullptr;
    if (IFace && (IV = IFace->lookupInstanceVariable(II, ClassDeclared))) {
      // Diagnose using an ivar in a class method.
      if (IsClassMethod) {
        Diag(Loc, diag::err_ivar_use_in_class_method) << IV->getDeclName();
        return DeclResult(true);
      }

      // Diagnose the use of an ivar outside of the declaring class.
      if (IV->getAccessControl() == ObjCIvarDecl::Private &&
          !declaresSameEntity(ClassDeclared, IFace) &&
          !getLangOpts().DebuggerSupport)
        Diag(Loc, diag::err_private_ivar_access) << IV->getDeclName();

      // Success.
      return IV;
    }
  } else if (CurMethod->isInstanceMethod()) {
    // We should warn if a local variable hides an ivar.
    if (ObjCInterfaceDecl *IFace = CurMethod->getClassInterface()) {
      ObjCInterfaceDecl *ClassDeclared;
      if (ObjCIvarDecl *IV = IFace->lookupInstanceVariable(II, ClassDeclared)) {
        if (IV->getAccessControl() != ObjCIvarDecl::Private ||
            declaresSameEntity(IFace, ClassDeclared))
          Diag(Loc, diag::warn_ivar_use_hidden) << IV->getDeclName();
      }
    }
  } else if (Lookup.isSingleResult() &&
             Lookup.getFoundDecl()->isDefinedOutsideFunctionOrMethod()) {
    // If accessing a stand-alone ivar in a class method, this is an error.
    if (const ObjCIvarDecl *IV =
            dyn_cast<ObjCIvarDecl>(Lookup.getFoundDecl())) {
      Diag(Loc, diag::err_ivar_use_in_class_method) << IV->getDeclName();
      return DeclResult(true);
    }
  }

  // Didn't encounter an error, didn't find an ivar.
  return DeclResult(false);
}

ExprResult Sema::BuildIvarRefExpr(Scope *S, SourceLocation Loc,
                                  ObjCIvarDecl *IV) {
  ObjCMethodDecl *CurMethod = getCurMethodDecl();
  assert(CurMethod && CurMethod->isInstanceMethod() &&
         "should not reference ivar from this context");

  ObjCInterfaceDecl *IFace = CurMethod->getClassInterface();
  assert(IFace && "should not reference ivar from this context");

  // If we're referencing an invalid decl, just return this as a silent
  // error node.  The error diagnostic was already emitted on the decl.
  if (IV->isInvalidDecl())
    return ExprError();

  // Check if referencing a field with __attribute__((deprecated)).
  if (DiagnoseUseOfDecl(IV, Loc))
    return ExprError();

  // FIXME: This should use a new expr for a direct reference, don't
  // turn this into Self->ivar, just return a BareIVarExpr or something.
  IdentifierInfo &II = Context.Idents.get("self");
  UnqualifiedId SelfName;
  SelfName.setImplicitSelfParam(&II);
  CXXScopeSpec SelfScopeSpec;
  SourceLocation TemplateKWLoc;
  ExprResult SelfExpr =
      ActOnIdExpression(S, SelfScopeSpec, TemplateKWLoc, SelfName,
                        /*HasTrailingLParen=*/false,
                        /*IsAddressOfOperand=*/false);
  if (SelfExpr.isInvalid())
    return ExprError();

  SelfExpr = DefaultLvalueConversion(SelfExpr.get());
  if (SelfExpr.isInvalid())
    return ExprError();

  MarkAnyDeclReferenced(Loc, IV, true);

  ObjCMethodFamily MF = CurMethod->getMethodFamily();
  if (MF != OMF_init && MF != OMF_dealloc && MF != OMF_finalize &&
      !IvarBacksCurrentMethodAccessor(IFace, CurMethod, IV))
    Diag(Loc, diag::warn_direct_ivar_access) << IV->getDeclName();

  ObjCIvarRefExpr *Result = new (Context)
      ObjCIvarRefExpr(IV, IV->getUsageType(SelfExpr.get()->getType()), Loc,
                      IV->getLocation(), SelfExpr.get(), true, true);

  if (IV->getType().getObjCLifetime() == Qualifiers::OCL_Weak) {
    if (!isUnevaluatedContext() &&
        !Diags.isIgnored(diag::warn_arc_repeated_use_of_weak, Loc))
      getCurFunction()->recordUseOfWeak(Result);
  }
  if (getLangOpts().ObjCAutoRefCount && !isUnevaluatedContext())
    if (const BlockDecl *BD = CurContext->getInnermostBlockDecl())
      ImplicitlyRetainedSelfLocs.push_back({Loc, BD});

  return Result;
}

/// The parser has read a name in, and Sema has detected that we're currently
/// inside an ObjC method. Perform some additional checks and determine if we
/// should form a reference to an ivar. If so, build an expression referencing
/// that ivar.
ExprResult
Sema::LookupInObjCMethod(LookupResult &Lookup, Scope *S,
                         IdentifierInfo *II, bool AllowBuiltinCreation) {
  // FIXME: Integrate this lookup step into LookupParsedName.
  DeclResult Ivar = LookupIvarInObjCMethod(Lookup, S, II);
  if (Ivar.isInvalid())
    return ExprError();
  if (Ivar.isUsable())
    return BuildIvarRefExpr(S, Lookup.getNameLoc(),
                            cast<ObjCIvarDecl>(Ivar.get()));

  if (Lookup.empty() && II && AllowBuiltinCreation)
    LookupBuiltin(Lookup);

  // Sentinel value saying that we didn't do anything special.
  return ExprResult(false);
}

/// Cast a base object to a member's actual type.
///
/// There are two relevant checks:
///
/// C++ [class.access.base]p7:
///
///   If a class member access operator [...] is used to access a non-static
///   data member or non-static member function, the reference is ill-formed if
///   the left operand [...] cannot be implicitly converted to a pointer to the
///   naming class of the right operand.
///
/// C++ [expr.ref]p7:
///
///   If E2 is a non-static data member or a non-static member function, the
///   program is ill-formed if the class of which E2 is directly a member is an
///   ambiguous base (11.8) of the naming class (11.9.3) of E2.
///
/// Note that the latter check does not consider access; the access of the
/// "real" base class is checked as appropriate when checking the access of the
/// member name.
ExprResult
Sema::PerformObjectMemberConversion(Expr *From,
                                    NestedNameSpecifier *Qualifier,
                                    NamedDecl *FoundDecl,
                                    NamedDecl *Member) {
  const auto *RD = dyn_cast<CXXRecordDecl>(Member->getDeclContext());
  if (!RD)
    return From;

  QualType DestRecordType;
  QualType DestType;
  QualType FromRecordType;
  QualType FromType = From->getType();
  bool PointerConversions = false;
  if (isa<FieldDecl>(Member)) {
    DestRecordType = Context.getCanonicalType(Context.getTypeDeclType(RD));
    auto FromPtrType = FromType->getAs<PointerType>();
    DestRecordType = Context.getAddrSpaceQualType(
        DestRecordType, FromPtrType
                            ? FromType->getPointeeType().getAddressSpace()
                            : FromType.getAddressSpace());

    if (FromPtrType) {
      DestType = Context.getPointerType(DestRecordType);
      FromRecordType = FromPtrType->getPointeeType();
      PointerConversions = true;
    } else {
      DestType = DestRecordType;
      FromRecordType = FromType;
    }
  } else if (const auto *Method = dyn_cast<CXXMethodDecl>(Member)) {
    if (Method->isStatic())
      return From;

    DestType = Method->getThisType();
    DestRecordType = DestType->getPointeeType();

    if (FromType->getAs<PointerType>()) {
      FromRecordType = FromType->getPointeeType();
      PointerConversions = true;
    } else {
      FromRecordType = FromType;
      DestType = DestRecordType;
    }

    LangAS FromAS = FromRecordType.getAddressSpace();
    LangAS DestAS = DestRecordType.getAddressSpace();
    if (FromAS != DestAS) {
      QualType FromRecordTypeWithoutAS =
          Context.removeAddrSpaceQualType(FromRecordType);
      QualType FromTypeWithDestAS =
          Context.getAddrSpaceQualType(FromRecordTypeWithoutAS, DestAS);
      if (PointerConversions)
        FromTypeWithDestAS = Context.getPointerType(FromTypeWithDestAS);
      From = ImpCastExprToType(From, FromTypeWithDestAS,
                               CK_AddressSpaceConversion, From->getValueKind())
                 .get();
    }
  } else {
    // No conversion necessary.
    return From;
  }

  if (DestType->isDependentType() || FromType->isDependentType())
    return From;

  // If the unqualified types are the same, no conversion is necessary.
  if (Context.hasSameUnqualifiedType(FromRecordType, DestRecordType))
    return From;

  SourceRange FromRange = From->getSourceRange();
  SourceLocation FromLoc = FromRange.getBegin();

  ExprValueKind VK = From->getValueKind();

  // C++ [class.member.lookup]p8:
  //   [...] Ambiguities can often be resolved by qualifying a name with its
  //   class name.
  //
  // If the member was a qualified name and the qualified referred to a
  // specific base subobject type, we'll cast to that intermediate type
  // first and then to the object in which the member is declared. That allows
  // one to resolve ambiguities in, e.g., a diamond-shaped hierarchy such as:
  //
  //   class Base { public: int x; };
  //   class Derived1 : public Base { };
  //   class Derived2 : public Base { };
  //   class VeryDerived : public Derived1, public Derived2 { void f(); };
  //
  //   void VeryDerived::f() {
  //     x = 17; // error: ambiguous base subobjects
  //     Derived1::x = 17; // okay, pick the Base subobject of Derived1
  //   }
  if (Qualifier && Qualifier->getAsType()) {
    QualType QType = QualType(Qualifier->getAsType(), 0);
    assert(QType->isRecordType() && "lookup done with non-record type");

    QualType QRecordType = QualType(QType->castAs<RecordType>(), 0);

    // In C++98, the qualifier type doesn't actually have to be a base
    // type of the object type, in which case we just ignore it.
    // Otherwise build the appropriate casts.
    if (IsDerivedFrom(FromLoc, FromRecordType, QRecordType)) {
      CXXCastPath BasePath;
      if (CheckDerivedToBaseConversion(FromRecordType, QRecordType,
                                       FromLoc, FromRange, &BasePath))
        return ExprError();

      if (PointerConversions)
        QType = Context.getPointerType(QType);
      From = ImpCastExprToType(From, QType, CK_UncheckedDerivedToBase,
                               VK, &BasePath).get();

      FromType = QType;
      FromRecordType = QRecordType;

      // If the qualifier type was the same as the destination type,
      // we're done.
      if (Context.hasSameUnqualifiedType(FromRecordType, DestRecordType))
        return From;
    }
  }

  CXXCastPath BasePath;
  if (CheckDerivedToBaseConversion(FromRecordType, DestRecordType,
                                   FromLoc, FromRange, &BasePath,
                                   /*IgnoreAccess=*/true))
    return ExprError();

  return ImpCastExprToType(From, DestType, CK_UncheckedDerivedToBase,
                           VK, &BasePath);
}

bool Sema::UseArgumentDependentLookup(const CXXScopeSpec &SS,
                                      const LookupResult &R,
                                      bool HasTrailingLParen) {
  // Only when used directly as the postfix-expression of a call.
  if (!HasTrailingLParen)
    return false;

  // Never if a scope specifier was provided.
  if (SS.isSet())
    return false;

  // Only in C++ or ObjC++.
  if (!getLangOpts().CPlusPlus)
    return false;

  // Turn off ADL when we find certain kinds of declarations during
  // normal lookup:
  for (const NamedDecl *D : R) {
    // C++0x [basic.lookup.argdep]p3:
    //     -- a declaration of a class member
    // Since using decls preserve this property, we check this on the
    // original decl.
    if (D->isCXXClassMember())
      return false;

    // C++0x [basic.lookup.argdep]p3:
    //     -- a block-scope function declaration that is not a
    //        using-declaration
    // NOTE: we also trigger this for function templates (in fact, we
    // don't check the decl type at all, since all other decl types
    // turn off ADL anyway).
    if (isa<UsingShadowDecl>(D))
      D = cast<UsingShadowDecl>(D)->getTargetDecl();
    else if (D->getLexicalDeclContext()->isFunctionOrMethod())
      return false;

    // C++0x [basic.lookup.argdep]p3:
    //     -- a declaration that is neither a function or a function
    //        template
    // And also for builtin functions.
    if (const auto *FDecl = dyn_cast<FunctionDecl>(D)) {
      // But also builtin functions.
      if (FDecl->getBuiltinID() && FDecl->isImplicit())
        return false;
    } else if (!isa<FunctionTemplateDecl>(D))
      return false;
  }

  return true;
}


/// Diagnoses obvious problems with the use of the given declaration
/// as an expression.  This is only actually called for lookups that
/// were not overloaded, and it doesn't promise that the declaration
/// will in fact be used.
static bool CheckDeclInExpr(Sema &S, SourceLocation Loc, NamedDecl *D,
                            bool AcceptInvalid) {
  if (D->isInvalidDecl() && !AcceptInvalid)
    return true;

  if (isa<TypedefNameDecl>(D)) {
    S.Diag(Loc, diag::err_unexpected_typedef) << D->getDeclName();
    return true;
  }

  if (isa<ObjCInterfaceDecl>(D)) {
    S.Diag(Loc, diag::err_unexpected_interface) << D->getDeclName();
    return true;
  }

  if (isa<NamespaceDecl>(D)) {
    S.Diag(Loc, diag::err_unexpected_namespace) << D->getDeclName();
    return true;
  }

  return false;
}

// Certain multiversion types should be treated as overloaded even when there is
// only one result.
static bool ShouldLookupResultBeMultiVersionOverload(const LookupResult &R) {
  assert(R.isSingleResult() && "Expected only a single result");
  const auto *FD = dyn_cast<FunctionDecl>(R.getFoundDecl());
  return FD &&
         (FD->isCPUDispatchMultiVersion() || FD->isCPUSpecificMultiVersion());
}

ExprResult Sema::BuildDeclarationNameExpr(const CXXScopeSpec &SS,
                                          LookupResult &R, bool NeedsADL,
                                          bool AcceptInvalidDecl) {
  // If this is a single, fully-resolved result and we don't need ADL,
  // just build an ordinary singleton decl ref.
  if (!NeedsADL && R.isSingleResult() &&
      !R.getAsSingle<FunctionTemplateDecl>() &&
      !ShouldLookupResultBeMultiVersionOverload(R))
    return BuildDeclarationNameExpr(SS, R.getLookupNameInfo(), R.getFoundDecl(),
                                    R.getRepresentativeDecl(), nullptr,
                                    AcceptInvalidDecl);

  // We only need to check the declaration if there's exactly one
  // result, because in the overloaded case the results can only be
  // functions and function templates.
  if (R.isSingleResult() && !ShouldLookupResultBeMultiVersionOverload(R) &&
      CheckDeclInExpr(*this, R.getNameLoc(), R.getFoundDecl(),
                      AcceptInvalidDecl))
    return ExprError();

  // Otherwise, just build an unresolved lookup expression.  Suppress
  // any lookup-related diagnostics; we'll hash these out later, when
  // we've picked a target.
  R.suppressDiagnostics();

  UnresolvedLookupExpr *ULE
    = UnresolvedLookupExpr::Create(Context, R.getNamingClass(),
                                   SS.getWithLocInContext(Context),
                                   R.getLookupNameInfo(),
                                   NeedsADL, R.isOverloadedResult(),
                                   R.begin(), R.end());

  return ULE;
}

static void diagnoseUncapturableValueReferenceOrBinding(Sema &S,
                                                        SourceLocation loc,
                                                        ValueDecl *var);

/// Complete semantic analysis for a reference to the given declaration.
ExprResult Sema::BuildDeclarationNameExpr(
    const CXXScopeSpec &SS, const DeclarationNameInfo &NameInfo, NamedDecl *D,
    NamedDecl *FoundD, const TemplateArgumentListInfo *TemplateArgs,
    bool AcceptInvalidDecl) {
  assert(D && "Cannot refer to a NULL declaration");
  assert(!isa<FunctionTemplateDecl>(D) &&
         "Cannot refer unambiguously to a function template");

  SourceLocation Loc = NameInfo.getLoc();
  if (CheckDeclInExpr(*this, Loc, D, AcceptInvalidDecl)) {
    // Recovery from invalid cases (e.g. D is an invalid Decl).
    // We use the dependent type for the RecoveryExpr to prevent bogus follow-up
    // diagnostics, as invalid decls use int as a fallback type.
    return CreateRecoveryExpr(NameInfo.getBeginLoc(), NameInfo.getEndLoc(), {});
  }

  if (TemplateDecl *Template = dyn_cast<TemplateDecl>(D)) {
    // Specifically diagnose references to class templates that are missing
    // a template argument list.
    diagnoseMissingTemplateArguments(TemplateName(Template), Loc);
    return ExprError();
  }

  // Make sure that we're referring to a value.
  if (!isa<ValueDecl, UnresolvedUsingIfExistsDecl>(D)) {
    Diag(Loc, diag::err_ref_non_value) << D << SS.getRange();
    Diag(D->getLocation(), diag::note_declared_at);
    return ExprError();
  }

  // Check whether this declaration can be used. Note that we suppress
  // this check when we're going to perform argument-dependent lookup
  // on this function name, because this might not be the function
  // that overload resolution actually selects.
  if (DiagnoseUseOfDecl(D, Loc))
    return ExprError();

  auto *VD = cast<ValueDecl>(D);

  // Only create DeclRefExpr's for valid Decl's.
  if (VD->isInvalidDecl() && !AcceptInvalidDecl)
    return ExprError();

  // Handle members of anonymous structs and unions.  If we got here,
  // and the reference is to a class member indirect field, then this
  // must be the subject of a pointer-to-member expression.
  if (auto *IndirectField = dyn_cast<IndirectFieldDecl>(VD);
      IndirectField && !IndirectField->isCXXClassMember())
    return BuildAnonymousStructUnionMemberReference(SS, NameInfo.getLoc(),
                                                    IndirectField);

  QualType type = VD->getType();
  if (type.isNull())
    return ExprError();
  ExprValueKind valueKind = VK_PRValue;

  // In 'T ...V;', the type of the declaration 'V' is 'T...', but the type of
  // a reference to 'V' is simply (unexpanded) 'T'. The type, like the value,
  // is expanded by some outer '...' in the context of the use.
  type = type.getNonPackExpansionType();

  switch (D->getKind()) {
    // Ignore all the non-ValueDecl kinds.
#define ABSTRACT_DECL(kind)
#define VALUE(type, base)
#define DECL(type, base) case Decl::type:
#include "clang/AST/DeclNodes.inc"
    llvm_unreachable("invalid value decl kind");

  // These shouldn't make it here.
  case Decl::ObjCAtDefsField:
    llvm_unreachable("forming non-member reference to ivar?");

  // Enum constants are always r-values and never references.
  // Unresolved using declarations are dependent.
  case Decl::EnumConstant:
  case Decl::UnresolvedUsingValue:
  case Decl::OMPDeclareReduction:
  case Decl::OMPDeclareMapper:
    valueKind = VK_PRValue;
    break;

  // Fields and indirect fields that got here must be for
  // pointer-to-member expressions; we just call them l-values for
  // internal consistency, because this subexpression doesn't really
  // exist in the high-level semantics.
  case Decl::Field:
  case Decl::IndirectField:
  case Decl::ObjCIvar:
    assert(getLangOpts().CPlusPlus && "building reference to field in C?");

    // These can't have reference type in well-formed programs, but
    // for internal consistency we do this anyway.
    type = type.getNonReferenceType();
    valueKind = VK_LValue;
    break;

  // Non-type template parameters are either l-values or r-values
  // depending on the type.
  case Decl::NonTypeTemplateParm: {
    if (const ReferenceType *reftype = type->getAs<ReferenceType>()) {
      type = reftype->getPointeeType();
      valueKind = VK_LValue; // even if the parameter is an r-value reference
      break;
    }

    // [expr.prim.id.unqual]p2:
    //   If the entity is a template parameter object for a template
    //   parameter of type T, the type of the expression is const T.
    //   [...] The expression is an lvalue if the entity is a [...] template
    //   parameter object.
    if (type->isRecordType()) {
      type = type.getUnqualifiedType().withConst();
      valueKind = VK_LValue;
      break;
    }

    // For non-references, we need to strip qualifiers just in case
    // the template parameter was declared as 'const int' or whatever.
    valueKind = VK_PRValue;
    type = type.getUnqualifiedType();
    break;
  }

  case Decl::Var:
  case Decl::VarTemplateSpecialization:
  case Decl::VarTemplatePartialSpecialization:
  case Decl::Decomposition:
  case Decl::OMPCapturedExpr:
    // In C, "extern void blah;" is valid and is an r-value.
    if (!getLangOpts().CPlusPlus && !type.hasQualifiers() &&
        type->isVoidType()) {
      valueKind = VK_PRValue;
      break;
    }
    [[fallthrough]];

  case Decl::ImplicitParam:
  case Decl::ParmVar: {
    // These are always l-values.
    valueKind = VK_LValue;
    type = type.getNonReferenceType();

    // FIXME: Does the addition of const really only apply in
    // potentially-evaluated contexts? Since the variable isn't actually
    // captured in an unevaluated context, it seems that the answer is no.
    if (!isUnevaluatedContext()) {
      QualType CapturedType = getCapturedDeclRefType(cast<VarDecl>(VD), Loc);
      if (!CapturedType.isNull())
        type = CapturedType;
    }

    break;
  }

  case Decl::Binding:
    // These are always lvalues.
    valueKind = VK_LValue;
    type = type.getNonReferenceType();
    break;

  case Decl::Function: {
    if (unsigned BID = cast<FunctionDecl>(VD)->getBuiltinID()) {
      if (!Context.BuiltinInfo.isDirectlyAddressable(BID)) {
        type = Context.BuiltinFnTy;
        valueKind = VK_PRValue;
        break;
      }
    }

    const FunctionType *fty = type->castAs<FunctionType>();

    // If we're referring to a function with an __unknown_anytype
    // result type, make the entire expression __unknown_anytype.
    if (fty->getReturnType() == Context.UnknownAnyTy) {
      type = Context.UnknownAnyTy;
      valueKind = VK_PRValue;
      break;
    }

    // Functions are l-values in C++.
    if (getLangOpts().CPlusPlus) {
      valueKind = VK_LValue;
      break;
    }

    // C99 DR 316 says that, if a function type comes from a
    // function definition (without a prototype), that type is only
    // used for checking compatibility. Therefore, when referencing
    // the function, we pretend that we don't have the full function
    // type.
    if (!cast<FunctionDecl>(VD)->hasPrototype() && isa<FunctionProtoType>(fty))
      type = Context.getFunctionNoProtoType(fty->getReturnType(),
                                            fty->getExtInfo());

    // Functions are r-values in C.
    valueKind = VK_PRValue;
    break;
  }

  case Decl::CXXDeductionGuide:
    llvm_unreachable("building reference to deduction guide");

  case Decl::MSProperty:
  case Decl::MSGuid:
  case Decl::TemplateParamObject:
    // FIXME: Should MSGuidDecl and template parameter objects be subject to
    // capture in OpenMP, or duplicated between host and device?
    valueKind = VK_LValue;
    break;

  case Decl::UnnamedGlobalConstant:
    valueKind = VK_LValue;
    break;

  case Decl::CXXMethod:
    // If we're referring to a method with an __unknown_anytype
    // result type, make the entire expression __unknown_anytype.
    // This should only be possible with a type written directly.
    if (const FunctionProtoType *proto =
            dyn_cast<FunctionProtoType>(VD->getType()))
      if (proto->getReturnType() == Context.UnknownAnyTy) {
        type = Context.UnknownAnyTy;
        valueKind = VK_PRValue;
        break;
      }

    // C++ methods are l-values if static, r-values if non-static.
    if (cast<CXXMethodDecl>(VD)->isStatic()) {
      valueKind = VK_LValue;
      break;
    }
    [[fallthrough]];

  case Decl::CXXConversion:
  case Decl::CXXDestructor:
  case Decl::CXXConstructor:
    valueKind = VK_PRValue;
    break;
  }

  auto *E =
      BuildDeclRefExpr(VD, type, valueKind, NameInfo, &SS, FoundD,
                       /*FIXME: TemplateKWLoc*/ SourceLocation(), TemplateArgs);
  // Clang AST consumers assume a DeclRefExpr refers to a valid decl. We
  // wrap a DeclRefExpr referring to an invalid decl with a dependent-type
  // RecoveryExpr to avoid follow-up semantic analysis (thus prevent bogus
  // diagnostics).
  if (VD->isInvalidDecl() && E)
    return CreateRecoveryExpr(E->getBeginLoc(), E->getEndLoc(), {E});
  return E;
}

static void ConvertUTF8ToWideString(unsigned CharByteWidth, StringRef Source,
                                    SmallString<32> &Target) {
  Target.resize(CharByteWidth * (Source.size() + 1));
  char *ResultPtr = &Target[0];
  const llvm::UTF8 *ErrorPtr;
  bool success =
      llvm::ConvertUTF8toWide(CharByteWidth, Source, ResultPtr, ErrorPtr);
  (void)success;
  assert(success);
  Target.resize(ResultPtr - &Target[0]);
}

ExprResult Sema::BuildPredefinedExpr(SourceLocation Loc,
                                     PredefinedExpr::IdentKind IK) {
  Decl *currentDecl = getCurLocalScopeDecl();
  if (!currentDecl) {
    Diag(Loc, diag::ext_predef_outside_function);
    currentDecl = Context.getTranslationUnitDecl();
  }

  QualType ResTy;
  StringLiteral *SL = nullptr;
  if (cast<DeclContext>(currentDecl)->isDependentContext())
    ResTy = Context.DependentTy;
  else {
    // Pre-defined identifiers are of type char[x], where x is the length of
    // the string.
    auto Str = PredefinedExpr::ComputeName(IK, currentDecl);
    unsigned Length = Str.length();

    llvm::APInt LengthI(32, Length + 1);
    if (IK == PredefinedExpr::LFunction || IK == PredefinedExpr::LFuncSig) {
      ResTy =
          Context.adjustStringLiteralBaseType(Context.WideCharTy.withConst());
      SmallString<32> RawChars;
      ConvertUTF8ToWideString(Context.getTypeSizeInChars(ResTy).getQuantity(),
                              Str, RawChars);
      ResTy = Context.getConstantArrayType(ResTy, LengthI, nullptr,
                                           ArrayType::Normal,
                                           /*IndexTypeQuals*/ 0);
      SL = StringLiteral::Create(Context, RawChars, StringLiteral::Wide,
                                 /*Pascal*/ false, ResTy, Loc);
    } else {
      ResTy = Context.adjustStringLiteralBaseType(Context.CharTy.withConst());
      ResTy = Context.getConstantArrayType(ResTy, LengthI, nullptr,
                                           ArrayType::Normal,
                                           /*IndexTypeQuals*/ 0);
      SL = StringLiteral::Create(Context, Str, StringLiteral::Ordinary,
                                 /*Pascal*/ false, ResTy, Loc);
    }
  }

  return PredefinedExpr::Create(Context, Loc, ResTy, IK, LangOpts.MicrosoftExt,
                                SL);
}

ExprResult Sema::BuildSYCLUniqueStableNameExpr(SourceLocation OpLoc,
                                               SourceLocation LParen,
                                               SourceLocation RParen,
                                               TypeSourceInfo *TSI) {
  return SYCLUniqueStableNameExpr::Create(Context, OpLoc, LParen, RParen, TSI);
}

ExprResult Sema::ActOnSYCLUniqueStableNameExpr(SourceLocation OpLoc,
                                               SourceLocation LParen,
                                               SourceLocation RParen,
                                               ParsedType ParsedTy) {
  TypeSourceInfo *TSI = nullptr;
  QualType Ty = GetTypeFromParser(ParsedTy, &TSI);

  if (Ty.isNull())
    return ExprError();
  if (!TSI)
    TSI = Context.getTrivialTypeSourceInfo(Ty, LParen);

  return BuildSYCLUniqueStableNameExpr(OpLoc, LParen, RParen, TSI);
}

ExprResult Sema::ActOnPredefinedExpr(SourceLocation Loc, tok::TokenKind Kind) {
  return BuildPredefinedExpr(Loc, getPredefinedExprKind(Kind));
}

ExprResult Sema::ActOnCharacterConstant(const Token &Tok, Scope *UDLScope) {
  SmallString<16> CharBuffer;
  bool Invalid = false;
  StringRef ThisTok = PP.getSpelling(Tok, CharBuffer, &Invalid);
  if (Invalid)
    return ExprError();

  CharLiteralParser Literal(ThisTok.begin(), ThisTok.end(), Tok.getLocation(),
                            PP, Tok.getKind());
  if (Literal.hadError())
    return ExprError();

  QualType Ty;
  if (Literal.isWide())
    Ty = Context.WideCharTy; // L'x' -> wchar_t in C and C++.
  else if (Literal.isUTF8() && getLangOpts().C23)
    Ty = Context.UnsignedCharTy; // u8'x' -> unsigned char in C23
  else if (Literal.isUTF8() && getLangOpts().Char8)
    Ty = Context.Char8Ty; // u8'x' -> char8_t when it exists.
  else if (Literal.isUTF16())
    Ty = Context.Char16Ty; // u'x' -> char16_t in C11 and C++11.
  else if (Literal.isUTF32())
    Ty = Context.Char32Ty; // U'x' -> char32_t in C11 and C++11.
  else if (!getLangOpts().CPlusPlus || Literal.isMultiChar())
    Ty = Context.IntTy;   // 'x' -> int in C, 'wxyz' -> int in C++.
  else
    Ty = Context.CharTy; // 'x' -> char in C++;
                         // u8'x' -> char in C11-C17 and in C++ without char8_t.

  CharacterLiteral::CharacterKind Kind = CharacterLiteral::Ascii;
  if (Literal.isWide())
    Kind = CharacterLiteral::Wide;
  else if (Literal.isUTF16())
    Kind = CharacterLiteral::UTF16;
  else if (Literal.isUTF32())
    Kind = CharacterLiteral::UTF32;
  else if (Literal.isUTF8())
    Kind = CharacterLiteral::UTF8;

  Expr *Lit = new (Context) CharacterLiteral(Literal.getValue(), Kind, Ty,
                                             Tok.getLocation());

  if (Literal.getUDSuffix().empty())
    return Lit;

  // We're building a user-defined literal.
  IdentifierInfo *UDSuffix = &Context.Idents.get(Literal.getUDSuffix());
  SourceLocation UDSuffixLoc =
    getUDSuffixLoc(*this, Tok.getLocation(), Literal.getUDSuffixOffset());

  // Make sure we're allowed user-defined literals here.
  if (!UDLScope)
    return ExprError(Diag(UDSuffixLoc, diag::err_invalid_character_udl));

  // C++11 [lex.ext]p6: The literal L is treated as a call of the form
  //   operator "" X (ch)
  return BuildCookedLiteralOperatorCall(*this, UDLScope, UDSuffix, UDSuffixLoc,
                                        Lit, Tok.getLocation());
}

ExprResult Sema::ActOnIntegerConstant(SourceLocation Loc, uint64_t Val) {
  unsigned IntSize = Context.getTargetInfo().getIntWidth();
  return IntegerLiteral::Create(Context, llvm::APInt(IntSize, Val),
                                Context.IntTy, Loc);
}

static Expr *BuildFloatingLiteral(Sema &S, NumericLiteralParser &Literal,
                                  QualType Ty, SourceLocation Loc) {
  const llvm::fltSemantics &Format = S.Context.getFloatTypeSemantics(Ty);

  using llvm::APFloat;
  APFloat Val(Format);

  APFloat::opStatus result = Literal.GetFloatValue(Val);

  // Overflow is always an error, but underflow is only an error if
  // we underflowed to zero (APFloat reports denormals as underflow).
  if ((result & APFloat::opOverflow) ||
      ((result & APFloat::opUnderflow) && Val.isZero())) {
    unsigned diagnostic;
    SmallString<20> buffer;
    if (result & APFloat::opOverflow) {
      diagnostic = diag::warn_float_overflow;
      APFloat::getLargest(Format).toString(buffer);
    } else {
      diagnostic = diag::warn_float_underflow;
      APFloat::getSmallest(Format).toString(buffer);
    }

    S.Diag(Loc, diagnostic)
      << Ty
      << StringRef(buffer.data(), buffer.size());
  }

  bool isExact = (result == APFloat::opOK);
  return FloatingLiteral::Create(S.Context, Val, isExact, Ty, Loc);
}

bool Sema::CheckLoopHintExpr(Expr *E, SourceLocation Loc) {
  assert(E && "Invalid expression");

  if (E->isValueDependent())
    return false;

  QualType QT = E->getType();
  if (!QT->isIntegerType() || QT->isBooleanType() || QT->isCharType()) {
    Diag(E->getExprLoc(), diag::err_pragma_loop_invalid_argument_type) << QT;
    return true;
  }

  llvm::APSInt ValueAPS;
  ExprResult R = VerifyIntegerConstantExpression(E, &ValueAPS);

  if (R.isInvalid())
    return true;

  bool ValueIsPositive = ValueAPS.isStrictlyPositive();
  if (!ValueIsPositive || ValueAPS.getActiveBits() > 31) {
    Diag(E->getExprLoc(), diag::err_pragma_loop_invalid_argument_value)
        << toString(ValueAPS, 10) << ValueIsPositive;
    return true;
  }

  return false;
}

ExprResult Sema::ActOnNumericConstant(const Token &Tok, Scope *UDLScope) {
  // Fast path for a single digit (which is quite common).  A single digit
  // cannot have a trigraph, escaped newline, radix prefix, or suffix.
  if (Tok.getLength() == 1) {
    const char Val = PP.getSpellingOfSingleCharacterNumericConstant(Tok);
    return ActOnIntegerConstant(Tok.getLocation(), Val-'0');
  }

  SmallString<128> SpellingBuffer;
  // NumericLiteralParser wants to overread by one character.  Add padding to
  // the buffer in case the token is copied to the buffer.  If getSpelling()
  // returns a StringRef to the memory buffer, it should have a null char at
  // the EOF, so it is also safe.
  SpellingBuffer.resize(Tok.getLength() + 1);

  // Get the spelling of the token, which eliminates trigraphs, etc.
  bool Invalid = false;
  StringRef TokSpelling = PP.getSpelling(Tok, SpellingBuffer, &Invalid);
  if (Invalid)
    return ExprError();

  NumericLiteralParser Literal(TokSpelling, Tok.getLocation(),
                               PP.getSourceManager(), PP.getLangOpts(),
                               PP.getTargetInfo(), PP.getDiagnostics());
  if (Literal.hadError)
    return ExprError();

  if (Literal.hasUDSuffix()) {
    // We're building a user-defined literal.
    const IdentifierInfo *UDSuffix = &Context.Idents.get(Literal.getUDSuffix());
    SourceLocation UDSuffixLoc =
      getUDSuffixLoc(*this, Tok.getLocation(), Literal.getUDSuffixOffset());

    // Make sure we're allowed user-defined literals here.
    if (!UDLScope)
      return ExprError(Diag(UDSuffixLoc, diag::err_invalid_numeric_udl));

    QualType CookedTy;
    if (Literal.isFloatingLiteral()) {
      // C++11 [lex.ext]p4: If S contains a literal operator with parameter type
      // long double, the literal is treated as a call of the form
      //   operator "" X (f L)
      CookedTy = Context.LongDoubleTy;
    } else {
      // C++11 [lex.ext]p3: If S contains a literal operator with parameter type
      // unsigned long long, the literal is treated as a call of the form
      //   operator "" X (n ULL)
      CookedTy = Context.UnsignedLongLongTy;
    }

    DeclarationName OpName =
      Context.DeclarationNames.getCXXLiteralOperatorName(UDSuffix);
    DeclarationNameInfo OpNameInfo(OpName, UDSuffixLoc);
    OpNameInfo.setCXXLiteralOperatorNameLoc(UDSuffixLoc);

    SourceLocation TokLoc = Tok.getLocation();

    // Perform literal operator lookup to determine if we're building a raw
    // literal or a cooked one.
    LookupResult R(*this, OpName, UDSuffixLoc, LookupOrdinaryName);
    switch (LookupLiteralOperator(UDLScope, R, CookedTy,
                                  /*AllowRaw*/ true, /*AllowTemplate*/ true,
                                  /*AllowStringTemplatePack*/ false,
                                  /*DiagnoseMissing*/ !Literal.isImaginary)) {
    case LOLR_ErrorNoDiagnostic:
      // Lookup failure for imaginary constants isn't fatal, there's still the
      // GNU extension producing _Complex types.
      break;
    case LOLR_Error:
      return ExprError();
    case LOLR_Cooked: {
      Expr *Lit;
      if (Literal.isFloatingLiteral()) {
        Lit = BuildFloatingLiteral(*this, Literal, CookedTy, Tok.getLocation());
      } else {
        llvm::APInt ResultVal(Context.getTargetInfo().getLongLongWidth(), 0);
        if (Literal.GetIntegerValue(ResultVal))
          Diag(Tok.getLocation(), diag::err_integer_literal_too_large)
              << /* Unsigned */ 1;
        Lit = IntegerLiteral::Create(Context, ResultVal, CookedTy,
                                     Tok.getLocation());
      }
      return BuildLiteralOperatorCall(R, OpNameInfo, Lit, TokLoc);
    }

    case LOLR_Raw: {
      // C++11 [lit.ext]p3, p4: If S contains a raw literal operator, the
      // literal is treated as a call of the form
      //   operator "" X ("n")
      unsigned Length = Literal.getUDSuffixOffset();
      QualType StrTy = Context.getConstantArrayType(
          Context.adjustStringLiteralBaseType(Context.CharTy.withConst()),
          llvm::APInt(32, Length + 1), nullptr, ArrayType::Normal, 0);
      Expr *Lit =
          StringLiteral::Create(Context, StringRef(TokSpelling.data(), Length),
                                StringLiteral::Ordinary,
                                /*Pascal*/ false, StrTy, &TokLoc, 1);
      return BuildLiteralOperatorCall(R, OpNameInfo, Lit, TokLoc);
    }

    case LOLR_Template: {
      // C++11 [lit.ext]p3, p4: Otherwise (S contains a literal operator
      // template), L is treated as a call fo the form
      //   operator "" X <'c1', 'c2', ... 'ck'>()
      // where n is the source character sequence c1 c2 ... ck.
      TemplateArgumentListInfo ExplicitArgs;
      unsigned CharBits = Context.getIntWidth(Context.CharTy);
      bool CharIsUnsigned = Context.CharTy->isUnsignedIntegerType();
      llvm::APSInt Value(CharBits, CharIsUnsigned);
      for (unsigned I = 0, N = Literal.getUDSuffixOffset(); I != N; ++I) {
        Value = TokSpelling[I];
        TemplateArgument Arg(Context, Value, Context.CharTy);
        TemplateArgumentLocInfo ArgInfo;
        ExplicitArgs.addArgument(TemplateArgumentLoc(Arg, ArgInfo));
      }
      return BuildLiteralOperatorCall(R, OpNameInfo, std::nullopt, TokLoc,
                                      &ExplicitArgs);
    }
    case LOLR_StringTemplatePack:
      llvm_unreachable("unexpected literal operator lookup result");
    }
  }

  Expr *Res;

  if (Literal.isFixedPointLiteral()) {
    QualType Ty;

    if (Literal.isAccum) {
      if (Literal.isHalf) {
        Ty = Context.ShortAccumTy;
      } else if (Literal.isLong) {
        Ty = Context.LongAccumTy;
      } else {
        Ty = Context.AccumTy;
      }
    } else if (Literal.isFract) {
      if (Literal.isHalf) {
        Ty = Context.ShortFractTy;
      } else if (Literal.isLong) {
        Ty = Context.LongFractTy;
      } else {
        Ty = Context.FractTy;
      }
    }

    if (Literal.isUnsigned) Ty = Context.getCorrespondingUnsignedType(Ty);

    bool isSigned = !Literal.isUnsigned;
    unsigned scale = Context.getFixedPointScale(Ty);
    unsigned bit_width = Context.getTypeInfo(Ty).Width;

    llvm::APInt Val(bit_width, 0, isSigned);
    bool Overflowed = Literal.GetFixedPointValue(Val, scale);
    bool ValIsZero = Val.isZero() && !Overflowed;

    auto MaxVal = Context.getFixedPointMax(Ty).getValue();
    if (Literal.isFract && Val == MaxVal + 1 && !ValIsZero)
      // Clause 6.4.4 - The value of a constant shall be in the range of
      // representable values for its type, with exception for constants of a
      // fract type with a value of exactly 1; such a constant shall denote
      // the maximal value for the type.
      --Val;
    else if (Val.ugt(MaxVal) || Overflowed)
      Diag(Tok.getLocation(), diag::err_too_large_for_fixed_point);

    Res = FixedPointLiteral::CreateFromRawInt(Context, Val, Ty,
                                              Tok.getLocation(), scale);
  } else if (Literal.isFloatingLiteral()) {
    QualType Ty;
    if (Literal.isHalf){
      if (getOpenCLOptions().isAvailableOption("cl_khr_fp16", getLangOpts()))
        Ty = Context.HalfTy;
      else {
        Diag(Tok.getLocation(), diag::err_half_const_requires_fp16);
        return ExprError();
      }
    } else if (Literal.isFloat)
      Ty = Context.FloatTy;
    else if (Literal.isLong)
      Ty = Context.LongDoubleTy;
    else if (Literal.isFloat16)
      Ty = Context.Float16Ty;
    else if (Literal.isFloat128)
      Ty = Context.Float128Ty;
    else
      Ty = Context.DoubleTy;

    Res = BuildFloatingLiteral(*this, Literal, Ty, Tok.getLocation());

    if (Ty == Context.DoubleTy) {
      if (getLangOpts().SinglePrecisionConstants) {
        if (Ty->castAs<BuiltinType>()->getKind() != BuiltinType::Float) {
          Res = ImpCastExprToType(Res, Context.FloatTy, CK_FloatingCast).get();
        }
      } else if (getLangOpts().OpenCL && !getOpenCLOptions().isAvailableOption(
                                             "cl_khr_fp64", getLangOpts())) {
        // Impose single-precision float type when cl_khr_fp64 is not enabled.
        Diag(Tok.getLocation(), diag::warn_double_const_requires_fp64)
            << (getLangOpts().getOpenCLCompatibleVersion() >= 300);
        Res = ImpCastExprToType(Res, Context.FloatTy, CK_FloatingCast).get();
      }
    }
  } else if (!Literal.isIntegerLiteral()) {
    return ExprError();
  } else {
    QualType Ty;

    // 'z/uz' literals are a C++23 feature.
    if (Literal.isSizeT)
      Diag(Tok.getLocation(), getLangOpts().CPlusPlus
                                  ? getLangOpts().CPlusPlus23
                                        ? diag::warn_cxx20_compat_size_t_suffix
                                        : diag::ext_cxx23_size_t_suffix
                                  : diag::err_cxx23_size_t_suffix);

    // 'wb/uwb' literals are a C23 feature. We support _BitInt as a type in C++,
    // but we do not currently support the suffix in C++ mode because it's not
    // entirely clear whether WG21 will prefer this suffix to return a library
    // type such as std::bit_int instead of returning a _BitInt.
    if (Literal.isBitInt && !getLangOpts().CPlusPlus)
      PP.Diag(Tok.getLocation(), getLangOpts().C23
                                     ? diag::warn_c23_compat_bitint_suffix
                                     : diag::ext_c23_bitint_suffix);

    // Get the value in the widest-possible width. What is "widest" depends on
    // whether the literal is a bit-precise integer or not. For a bit-precise
    // integer type, try to scan the source to determine how many bits are
    // needed to represent the value. This may seem a bit expensive, but trying
    // to get the integer value from an overly-wide APInt is *extremely*
    // expensive, so the naive approach of assuming
    // llvm::IntegerType::MAX_INT_BITS is a big performance hit.
    unsigned BitsNeeded =
        Literal.isBitInt ? llvm::APInt::getSufficientBitsNeeded(
                               Literal.getLiteralDigits(), Literal.getRadix())
                         : Context.getTargetInfo().getIntMaxTWidth();
    llvm::APInt ResultVal(BitsNeeded, 0);

    if (Literal.GetIntegerValue(ResultVal)) {
      // If this value didn't fit into uintmax_t, error and force to ull.
      Diag(Tok.getLocation(), diag::err_integer_literal_too_large)
          << /* Unsigned */ 1;
      Ty = Context.UnsignedLongLongTy;
      assert(Context.getTypeSize(Ty) == ResultVal.getBitWidth() &&
             "long long is not intmax_t?");
    } else {
      // If this value fits into a ULL, try to figure out what else it fits into
      // according to the rules of C99 6.4.4.1p5.

      // Octal, Hexadecimal, and integers with a U suffix are allowed to
      // be an unsigned int.
      bool AllowUnsigned = Literal.isUnsigned || Literal.getRadix() != 10;

      // Check from smallest to largest, picking the smallest type we can.
      unsigned Width = 0;

      // Microsoft specific integer suffixes are explicitly sized.
      if (Literal.MicrosoftInteger) {
        if (Literal.MicrosoftInteger == 8 && !Literal.isUnsigned) {
          Width = 8;
          Ty = Context.CharTy;
        } else {
          Width = Literal.MicrosoftInteger;
          Ty = Context.getIntTypeForBitwidth(Width,
                                             /*Signed=*/!Literal.isUnsigned);
        }
      }

      // Bit-precise integer literals are automagically-sized based on the
      // width required by the literal.
      if (Literal.isBitInt) {
        // The signed version has one more bit for the sign value. There are no
        // zero-width bit-precise integers, even if the literal value is 0.
        Width = std::max(ResultVal.getActiveBits(), 1u) +
                (Literal.isUnsigned ? 0u : 1u);

        // Diagnose if the width of the constant is larger than BITINT_MAXWIDTH,
        // and reset the type to the largest supported width.
        unsigned int MaxBitIntWidth =
            Context.getTargetInfo().getMaxBitIntWidth();
        if (Width > MaxBitIntWidth) {
          Diag(Tok.getLocation(), diag::err_integer_literal_too_large)
              << Literal.isUnsigned;
          Width = MaxBitIntWidth;
        }

        // Reset the result value to the smaller APInt and select the correct
        // type to be used. Note, we zext even for signed values because the
        // literal itself is always an unsigned value (a preceeding - is a
        // unary operator, not part of the literal).
        ResultVal = ResultVal.zextOrTrunc(Width);
        Ty = Context.getBitIntType(Literal.isUnsigned, Width);
      }

      // Check C++23 size_t literals.
      if (Literal.isSizeT) {
        assert(!Literal.MicrosoftInteger &&
               "size_t literals can't be Microsoft literals");
        unsigned SizeTSize = Context.getTargetInfo().getTypeWidth(
            Context.getTargetInfo().getSizeType());

        // Does it fit in size_t?
        if (ResultVal.isIntN(SizeTSize)) {
          // Does it fit in ssize_t?
          if (!Literal.isUnsigned && ResultVal[SizeTSize - 1] == 0)
            Ty = Context.getSignedSizeType();
          else if (AllowUnsigned)
            Ty = Context.getSizeType();
          Width = SizeTSize;
        }
      }

      if (Ty.isNull() && !Literal.isLong && !Literal.isLongLong &&
          !Literal.isSizeT) {
        // Are int/unsigned possibilities?
        unsigned IntSize = Context.getTargetInfo().getIntWidth();

        // Does it fit in a unsigned int?
        if (ResultVal.isIntN(IntSize)) {
          // Does it fit in a signed int?
          if (!Literal.isUnsigned && ResultVal[IntSize-1] == 0)
            Ty = Context.IntTy;
          else if (AllowUnsigned)
            Ty = Context.UnsignedIntTy;
          Width = IntSize;
        }
      }

      // Are long/unsigned long possibilities?
      if (Ty.isNull() && !Literal.isLongLong && !Literal.isSizeT) {
        unsigned LongSize = Context.getTargetInfo().getLongWidth();

        // Does it fit in a unsigned long?
        if (ResultVal.isIntN(LongSize)) {
          // Does it fit in a signed long?
          if (!Literal.isUnsigned && ResultVal[LongSize-1] == 0)
            Ty = Context.LongTy;
          else if (AllowUnsigned)
            Ty = Context.UnsignedLongTy;
          // Check according to the rules of C90 6.1.3.2p5. C++03 [lex.icon]p2
          // is compatible.
          else if (!getLangOpts().C99 && !getLangOpts().CPlusPlus11) {
            const unsigned LongLongSize =
                Context.getTargetInfo().getLongLongWidth();
            Diag(Tok.getLocation(),
                 getLangOpts().CPlusPlus
                     ? Literal.isLong
                           ? diag::warn_old_implicitly_unsigned_long_cxx
                           : /*C++98 UB*/ diag::
                                 ext_old_implicitly_unsigned_long_cxx
                     : diag::warn_old_implicitly_unsigned_long)
                << (LongLongSize > LongSize ? /*will have type 'long long'*/ 0
                                            : /*will be ill-formed*/ 1);
            Ty = Context.UnsignedLongTy;
          }
          Width = LongSize;
        }
      }

      // Check long long if needed.
      if (Ty.isNull() && !Literal.isSizeT) {
        unsigned LongLongSize = Context.getTargetInfo().getLongLongWidth();

        // Does it fit in a unsigned long long?
        if (ResultVal.isIntN(LongLongSize)) {
          // Does it fit in a signed long long?
          // To be compatible with MSVC, hex integer literals ending with the
          // LL or i64 suffix are always signed in Microsoft mode.
          if (!Literal.isUnsigned && (ResultVal[LongLongSize-1] == 0 ||
              (getLangOpts().MSVCCompat && Literal.isLongLong)))
            Ty = Context.LongLongTy;
          else if (AllowUnsigned)
            Ty = Context.UnsignedLongLongTy;
          Width = LongLongSize;

          // 'long long' is a C99 or C++11 feature, whether the literal
          // explicitly specified 'long long' or we needed the extra width.
          if (getLangOpts().CPlusPlus)
            Diag(Tok.getLocation(), getLangOpts().CPlusPlus11
                                        ? diag::warn_cxx98_compat_longlong
                                        : diag::ext_cxx11_longlong);
          else if (!getLangOpts().C99)
            Diag(Tok.getLocation(), diag::ext_c99_longlong);
        }
      }

      // If we still couldn't decide a type, we either have 'size_t' literal
      // that is out of range, or a decimal literal that does not fit in a
      // signed long long and has no U suffix.
      if (Ty.isNull()) {
        if (Literal.isSizeT)
          Diag(Tok.getLocation(), diag::err_size_t_literal_too_large)
              << Literal.isUnsigned;
        else
          Diag(Tok.getLocation(),
               diag::ext_integer_literal_too_large_for_signed);
        Ty = Context.UnsignedLongLongTy;
        Width = Context.getTargetInfo().getLongLongWidth();
      }

      if (ResultVal.getBitWidth() != Width)
        ResultVal = ResultVal.trunc(Width);
    }
    Res = IntegerLiteral::Create(Context, ResultVal, Ty, Tok.getLocation());
  }

  // If this is an imaginary literal, create the ImaginaryLiteral wrapper.
  if (Literal.isImaginary) {
    Res = new (Context) ImaginaryLiteral(Res,
                                        Context.getComplexType(Res->getType()));

    Diag(Tok.getLocation(), diag::ext_imaginary_constant);
  }
  return Res;
}

ExprResult Sema::ActOnParenExpr(SourceLocation L, SourceLocation R, Expr *E) {
  assert(E && "ActOnParenExpr() missing expr");
  QualType ExprTy = E->getType();
  if (getLangOpts().ProtectParens && CurFPFeatures.getAllowFPReassociate() &&
      !E->isLValue() && ExprTy->hasFloatingRepresentation())
    return BuildBuiltinCallExpr(R, Builtin::BI__arithmetic_fence, E);
  return new (Context) ParenExpr(L, R, E);
}

static bool CheckVecStepTraitOperandType(Sema &S, QualType T,
                                         SourceLocation Loc,
                                         SourceRange ArgRange) {
  // [OpenCL 1.1 6.11.12] "The vec_step built-in function takes a built-in
  // scalar or vector data type argument..."
  // Every built-in scalar type (OpenCL 1.1 6.1.1) is either an arithmetic
  // type (C99 6.2.5p18) or void.
  if (!(T->isArithmeticType() || T->isVoidType() || T->isVectorType())) {
    S.Diag(Loc, diag::err_vecstep_non_scalar_vector_type)
      << T << ArgRange;
    return true;
  }

  assert((T->isVoidType() || !T->isIncompleteType()) &&
         "Scalar types should always be complete");
  return false;
}

static bool CheckExtensionTraitOperandType(Sema &S, QualType T,
                                           SourceLocation Loc,
                                           SourceRange ArgRange,
                                           UnaryExprOrTypeTrait TraitKind) {
  // Invalid types must be hard errors for SFINAE in C++.
  if (S.LangOpts.CPlusPlus)
    return true;

  // C99 6.5.3.4p1:
  if (T->isFunctionType() &&
      (TraitKind == UETT_SizeOf || TraitKind == UETT_AlignOf ||
       TraitKind == UETT_PreferredAlignOf)) {
    // sizeof(function)/alignof(function) is allowed as an extension.
    S.Diag(Loc, diag::ext_sizeof_alignof_function_type)
        << getTraitSpelling(TraitKind) << ArgRange;
    return false;
  }

  // Allow sizeof(void)/alignof(void) as an extension, unless in OpenCL where
  // this is an error (OpenCL v1.1 s6.3.k)
  if (T->isVoidType()) {
    unsigned DiagID = S.LangOpts.OpenCL ? diag::err_opencl_sizeof_alignof_type
                                        : diag::ext_sizeof_alignof_void_type;
    S.Diag(Loc, DiagID) << getTraitSpelling(TraitKind) << ArgRange;
    return false;
  }

  return true;
}

static bool CheckObjCTraitOperandConstraints(Sema &S, QualType T,
                                             SourceLocation Loc,
                                             SourceRange ArgRange,
                                             UnaryExprOrTypeTrait TraitKind) {
  // Reject sizeof(interface) and sizeof(interface<proto>) if the
  // runtime doesn't allow it.
  if (!S.LangOpts.ObjCRuntime.allowsSizeofAlignof() && T->isObjCObjectType()) {
    S.Diag(Loc, diag::err_sizeof_nonfragile_interface)
      << T << (TraitKind == UETT_SizeOf)
      << ArgRange;
    return true;
  }

  return false;
}

/// Check whether E is a pointer from a decayed array type (the decayed
/// pointer type is equal to T) and emit a warning if it is.
static void warnOnSizeofOnArrayDecay(Sema &S, SourceLocation Loc, QualType T,
                                     const Expr *E) {
  // Don't warn if the operation changed the type.
  if (T != E->getType())
    return;

  // Now look for array decays.
  const auto *ICE = dyn_cast<ImplicitCastExpr>(E);
  if (!ICE || ICE->getCastKind() != CK_ArrayToPointerDecay)
    return;

  S.Diag(Loc, diag::warn_sizeof_array_decay) << ICE->getSourceRange()
                                             << ICE->getType()
                                             << ICE->getSubExpr()->getType();
}

/// Check the constraints on expression operands to unary type expression
/// and type traits.
///
/// Completes any types necessary and validates the constraints on the operand
/// expression. The logic mostly mirrors the type-based overload, but may modify
/// the expression as it completes the type for that expression through template
/// instantiation, etc.
bool Sema::CheckUnaryExprOrTypeTraitOperand(Expr *E,
                                            UnaryExprOrTypeTrait ExprKind) {
  QualType ExprTy = E->getType();
  assert(!ExprTy->isReferenceType());

  bool IsUnevaluatedOperand =
      (ExprKind == UETT_SizeOf || ExprKind == UETT_AlignOf ||
       ExprKind == UETT_PreferredAlignOf || ExprKind == UETT_VecStep);
  if (IsUnevaluatedOperand) {
    ExprResult Result = CheckUnevaluatedOperand(E);
    if (Result.isInvalid())
      return true;
    E = Result.get();
  }

  // The operand for sizeof and alignof is in an unevaluated expression context,
  // so side effects could result in unintended consequences.
  // Exclude instantiation-dependent expressions, because 'sizeof' is sometimes
  // used to build SFINAE gadgets.
  // FIXME: Should we consider instantiation-dependent operands to 'alignof'?
  if (IsUnevaluatedOperand && !inTemplateInstantiation() &&
      !E->isInstantiationDependent() &&
      !E->getType()->isVariableArrayType() &&
      E->HasSideEffects(Context, false))
    Diag(E->getExprLoc(), diag::warn_side_effects_unevaluated_context);

  if (ExprKind == UETT_VecStep)
    return CheckVecStepTraitOperandType(*this, ExprTy, E->getExprLoc(),
                                        E->getSourceRange());

  // Explicitly list some types as extensions.
  if (!CheckExtensionTraitOperandType(*this, ExprTy, E->getExprLoc(),
                                      E->getSourceRange(), ExprKind))
    return false;

  // WebAssembly tables are always illegal operands to unary expressions and
  // type traits.
  if (Context.getTargetInfo().getTriple().isWasm() &&
      E->getType()->isWebAssemblyTableType()) {
    Diag(E->getExprLoc(), diag::err_wasm_table_invalid_uett_operand)
        << getTraitSpelling(ExprKind);
    return true;
  }

  // 'alignof' applied to an expression only requires the base element type of
  // the expression to be complete. 'sizeof' requires the expression's type to
  // be complete (and will attempt to complete it if it's an array of unknown
  // bound).
  if (ExprKind == UETT_AlignOf || ExprKind == UETT_PreferredAlignOf) {
    if (RequireCompleteSizedType(
            E->getExprLoc(), Context.getBaseElementType(E->getType()),
            diag::err_sizeof_alignof_incomplete_or_sizeless_type,
            getTraitSpelling(ExprKind), E->getSourceRange()))
      return true;
  } else {
    if (RequireCompleteSizedExprType(
            E, diag::err_sizeof_alignof_incomplete_or_sizeless_type,
            getTraitSpelling(ExprKind), E->getSourceRange()))
      return true;
  }

  // Completing the expression's type may have changed it.
  ExprTy = E->getType();
  assert(!ExprTy->isReferenceType());

  if (ExprTy->isFunctionType()) {
    Diag(E->getExprLoc(), diag::err_sizeof_alignof_function_type)
        << getTraitSpelling(ExprKind) << E->getSourceRange();
    return true;
  }

  if (CheckObjCTraitOperandConstraints(*this, ExprTy, E->getExprLoc(),
                                       E->getSourceRange(), ExprKind))
    return true;

  if (ExprKind == UETT_SizeOf) {
    if (const auto *DeclRef = dyn_cast<DeclRefExpr>(E->IgnoreParens())) {
      if (const auto *PVD = dyn_cast<ParmVarDecl>(DeclRef->getFoundDecl())) {
        QualType OType = PVD->getOriginalType();
        QualType Type = PVD->getType();
        if (Type->isPointerType() && OType->isArrayType()) {
          Diag(E->getExprLoc(), diag::warn_sizeof_array_param)
            << Type << OType;
          Diag(PVD->getLocation(), diag::note_declared_at);
        }
      }
    }

    // Warn on "sizeof(array op x)" and "sizeof(x op array)", where the array
    // decays into a pointer and returns an unintended result. This is most
    // likely a typo for "sizeof(array) op x".
    if (const auto *BO = dyn_cast<BinaryOperator>(E->IgnoreParens())) {
      warnOnSizeofOnArrayDecay(*this, BO->getOperatorLoc(), BO->getType(),
                               BO->getLHS());
      warnOnSizeofOnArrayDecay(*this, BO->getOperatorLoc(), BO->getType(),
                               BO->getRHS());
    }
  }

  return false;
}

static bool CheckAlignOfExpr(Sema &S, Expr *E, UnaryExprOrTypeTrait ExprKind) {
  // Cannot know anything else if the expression is dependent.
  if (E->isTypeDependent())
    return false;

  if (E->getObjectKind() == OK_BitField) {
    S.Diag(E->getExprLoc(), diag::err_sizeof_alignof_typeof_bitfield)
       << 1 << E->getSourceRange();
    return true;
  }

  ValueDecl *D = nullptr;
  Expr *Inner = E->IgnoreParens();
  if (DeclRefExpr *DRE = dyn_cast<DeclRefExpr>(Inner)) {
    D = DRE->getDecl();
  } else if (MemberExpr *ME = dyn_cast<MemberExpr>(Inner)) {
    D = ME->getMemberDecl();
  }

  // If it's a field, require the containing struct to have a
  // complete definition so that we can compute the layout.
  //
  // This can happen in C++11 onwards, either by naming the member
  // in a way that is not transformed into a member access expression
  // (in an unevaluated operand, for instance), or by naming the member
  // in a trailing-return-type.
  //
  // For the record, since __alignof__ on expressions is a GCC
  // extension, GCC seems to permit this but always gives the
  // nonsensical answer 0.
  //
  // We don't really need the layout here --- we could instead just
  // directly check for all the appropriate alignment-lowing
  // attributes --- but that would require duplicating a lot of
  // logic that just isn't worth duplicating for such a marginal
  // use-case.
  if (FieldDecl *FD = dyn_cast_or_null<FieldDecl>(D)) {
    // Fast path this check, since we at least know the record has a
    // definition if we can find a member of it.
    if (!FD->getParent()->isCompleteDefinition()) {
      S.Diag(E->getExprLoc(), diag::err_alignof_member_of_incomplete_type)
        << E->getSourceRange();
      return true;
    }

    // Otherwise, if it's a field, and the field doesn't have
    // reference type, then it must have a complete type (or be a
    // flexible array member, which we explicitly want to
    // white-list anyway), which makes the following checks trivial.
    if (!FD->getType()->isReferenceType())
      return false;
  }

  return S.CheckUnaryExprOrTypeTraitOperand(E, ExprKind);
}

bool Sema::CheckVecStepExpr(Expr *E) {
  E = E->IgnoreParens();

  // Cannot know anything else if the expression is dependent.
  if (E->isTypeDependent())
    return false;

  return CheckUnaryExprOrTypeTraitOperand(E, UETT_VecStep);
}

static void captureVariablyModifiedType(ASTContext &Context, QualType T,
                                        CapturingScopeInfo *CSI) {
  assert(T->isVariablyModifiedType());
  assert(CSI != nullptr);

  // We're going to walk down into the type and look for VLA expressions.
  do {
    const Type *Ty = T.getTypePtr();
    switch (Ty->getTypeClass()) {
#define TYPE(Class, Base)
#define ABSTRACT_TYPE(Class, Base)
#define NON_CANONICAL_TYPE(Class, Base)
#define DEPENDENT_TYPE(Class, Base) case Type::Class:
#define NON_CANONICAL_UNLESS_DEPENDENT_TYPE(Class, Base)
#include "clang/AST/TypeNodes.inc"
      T = QualType();
      break;
    // These types are never variably-modified.
    case Type::Builtin:
    case Type::Complex:
    case Type::Vector:
    case Type::ExtVector:
    case Type::ConstantMatrix:
    case Type::Record:
    case Type::Enum:
    case Type::TemplateSpecialization:
    case Type::ObjCObject:
    case Type::ObjCInterface:
    case Type::ObjCObjectPointer:
    case Type::ObjCTypeParam:
    case Type::Pipe:
    case Type::BitInt:
      llvm_unreachable("type class is never variably-modified!");
    case Type::Elaborated:
      T = cast<ElaboratedType>(Ty)->getNamedType();
      break;
    case Type::Adjusted:
      T = cast<AdjustedType>(Ty)->getOriginalType();
      break;
    case Type::Decayed:
      T = cast<DecayedType>(Ty)->getPointeeType();
      break;
    case Type::Pointer:
      T = cast<PointerType>(Ty)->getPointeeType();
      break;
    case Type::BlockPointer:
      T = cast<BlockPointerType>(Ty)->getPointeeType();
      break;
    case Type::LValueReference:
    case Type::RValueReference:
      T = cast<ReferenceType>(Ty)->getPointeeType();
      break;
    case Type::MemberPointer:
      T = cast<MemberPointerType>(Ty)->getPointeeType();
      break;
    case Type::ConstantArray:
    case Type::IncompleteArray:
      // Losing element qualification here is fine.
      T = cast<ArrayType>(Ty)->getElementType();
      break;
    case Type::VariableArray: {
      // Losing element qualification here is fine.
      const VariableArrayType *VAT = cast<VariableArrayType>(Ty);

      // Unknown size indication requires no size computation.
      // Otherwise, evaluate and record it.
      auto Size = VAT->getSizeExpr();
      if (Size && !CSI->isVLATypeCaptured(VAT) &&
          (isa<CapturedRegionScopeInfo>(CSI) || isa<LambdaScopeInfo>(CSI)))
        CSI->addVLATypeCapture(Size->getExprLoc(), VAT, Context.getSizeType());

      T = VAT->getElementType();
      break;
    }
    case Type::FunctionProto:
    case Type::FunctionNoProto:
      T = cast<FunctionType>(Ty)->getReturnType();
      break;
    case Type::Paren:
    case Type::TypeOf:
    case Type::UnaryTransform:
    case Type::Attributed:
    case Type::BTFTagAttributed:
    case Type::SubstTemplateTypeParm:
    case Type::MacroQualified:
      // Keep walking after single level desugaring.
      T = T.getSingleStepDesugaredType(Context);
      break;
    case Type::Typedef:
      T = cast<TypedefType>(Ty)->desugar();
      break;
    case Type::Decltype:
      T = cast<DecltypeType>(Ty)->desugar();
      break;
    case Type::Using:
      T = cast<UsingType>(Ty)->desugar();
      break;
    case Type::Auto:
    case Type::DeducedTemplateSpecialization:
      T = cast<DeducedType>(Ty)->getDeducedType();
      break;
    case Type::TypeOfExpr:
      T = cast<TypeOfExprType>(Ty)->getUnderlyingExpr()->getType();
      break;
    case Type::Atomic:
      T = cast<AtomicType>(Ty)->getValueType();
      break;
    }
  } while (!T.isNull() && T->isVariablyModifiedType());
}

/// Check the constraints on operands to unary expression and type
/// traits.
///
/// This will complete any types necessary, and validate the various constraints
/// on those operands.
///
/// The UsualUnaryConversions() function is *not* called by this routine.
/// C99 6.3.2.1p[2-4] all state:
///   Except when it is the operand of the sizeof operator ...
///
/// C++ [expr.sizeof]p4
///   The lvalue-to-rvalue, array-to-pointer, and function-to-pointer
///   standard conversions are not applied to the operand of sizeof.
///
/// This policy is followed for all of the unary trait expressions.
bool Sema::CheckUnaryExprOrTypeTraitOperand(QualType ExprType,
                                            SourceLocation OpLoc,
                                            SourceRange ExprRange,
                                            UnaryExprOrTypeTrait ExprKind,
                                            StringRef KWName) {
  if (ExprType->isDependentType())
    return false;

  // C++ [expr.sizeof]p2:
  //     When applied to a reference or a reference type, the result
  //     is the size of the referenced type.
  // C++11 [expr.alignof]p3:
  //     When alignof is applied to a reference type, the result
  //     shall be the alignment of the referenced type.
  if (const ReferenceType *Ref = ExprType->getAs<ReferenceType>())
    ExprType = Ref->getPointeeType();

  // C11 6.5.3.4/3, C++11 [expr.alignof]p3:
  //   When alignof or _Alignof is applied to an array type, the result
  //   is the alignment of the element type.
  if (ExprKind == UETT_AlignOf || ExprKind == UETT_PreferredAlignOf ||
      ExprKind == UETT_OpenMPRequiredSimdAlign)
    ExprType = Context.getBaseElementType(ExprType);

  if (ExprKind == UETT_VecStep)
    return CheckVecStepTraitOperandType(*this, ExprType, OpLoc, ExprRange);

  // Explicitly list some types as extensions.
  if (!CheckExtensionTraitOperandType(*this, ExprType, OpLoc, ExprRange,
                                      ExprKind))
    return false;

  if (RequireCompleteSizedType(
          OpLoc, ExprType, diag::err_sizeof_alignof_incomplete_or_sizeless_type,
          KWName, ExprRange))
    return true;

  if (ExprType->isFunctionType()) {
    Diag(OpLoc, diag::err_sizeof_alignof_function_type) << KWName << ExprRange;
    return true;
  }

  // WebAssembly tables are always illegal operands to unary expressions and
  // type traits.
  if (Context.getTargetInfo().getTriple().isWasm() &&
      ExprType->isWebAssemblyTableType()) {
    Diag(OpLoc, diag::err_wasm_table_invalid_uett_operand)
        << getTraitSpelling(ExprKind);
    return true;
  }

  if (CheckObjCTraitOperandConstraints(*this, ExprType, OpLoc, ExprRange,
                                       ExprKind))
    return true;

  if (ExprType->isVariablyModifiedType() && FunctionScopes.size() > 1) {
    if (auto *TT = ExprType->getAs<TypedefType>()) {
      for (auto I = FunctionScopes.rbegin(),
                E = std::prev(FunctionScopes.rend());
           I != E; ++I) {
        auto *CSI = dyn_cast<CapturingScopeInfo>(*I);
        if (CSI == nullptr)
          break;
        DeclContext *DC = nullptr;
        if (auto *LSI = dyn_cast<LambdaScopeInfo>(CSI))
          DC = LSI->CallOperator;
        else if (auto *CRSI = dyn_cast<CapturedRegionScopeInfo>(CSI))
          DC = CRSI->TheCapturedDecl;
        else if (auto *BSI = dyn_cast<BlockScopeInfo>(CSI))
          DC = BSI->TheDecl;
        if (DC) {
          if (DC->containsDecl(TT->getDecl()))
            break;
          captureVariablyModifiedType(Context, ExprType, CSI);
        }
      }
    }
  }

  return false;
}

/// Build a sizeof or alignof expression given a type operand.
ExprResult Sema::CreateUnaryExprOrTypeTraitExpr(TypeSourceInfo *TInfo,
                                                SourceLocation OpLoc,
                                                UnaryExprOrTypeTrait ExprKind,
                                                SourceRange R) {
  if (!TInfo)
    return ExprError();

  QualType T = TInfo->getType();

  if (!T->isDependentType() &&
      CheckUnaryExprOrTypeTraitOperand(T, OpLoc, R, ExprKind,
                                       getTraitSpelling(ExprKind)))
    return ExprError();

  // Adds overload of TransformToPotentiallyEvaluated for TypeSourceInfo to
  // properly deal with VLAs in nested calls of sizeof and typeof.
  if (isUnevaluatedContext() && ExprKind == UETT_SizeOf &&
      TInfo->getType()->isVariablyModifiedType())
    TInfo = TransformToPotentiallyEvaluated(TInfo);

  // C99 6.5.3.4p4: the type (an unsigned integer type) is size_t.
  return new (Context) UnaryExprOrTypeTraitExpr(
      ExprKind, TInfo, Context.getSizeType(), OpLoc, R.getEnd());
}

/// Build a sizeof or alignof expression given an expression
/// operand.
ExprResult
Sema::CreateUnaryExprOrTypeTraitExpr(Expr *E, SourceLocation OpLoc,
                                     UnaryExprOrTypeTrait ExprKind) {
  ExprResult PE = CheckPlaceholderExpr(E);
  if (PE.isInvalid())
    return ExprError();

  E = PE.get();

  // Verify that the operand is valid.
  bool isInvalid = false;
  if (E->isTypeDependent()) {
    // Delay type-checking for type-dependent expressions.
  } else if (ExprKind == UETT_AlignOf || ExprKind == UETT_PreferredAlignOf) {
    isInvalid = CheckAlignOfExpr(*this, E, ExprKind);
  } else if (ExprKind == UETT_VecStep) {
    isInvalid = CheckVecStepExpr(E);
  } else if (ExprKind == UETT_OpenMPRequiredSimdAlign) {
      Diag(E->getExprLoc(), diag::err_openmp_default_simd_align_expr);
      isInvalid = true;
  } else if (E->refersToBitField()) {  // C99 6.5.3.4p1.
    Diag(E->getExprLoc(), diag::err_sizeof_alignof_typeof_bitfield) << 0;
    isInvalid = true;
  } else {
    isInvalid = CheckUnaryExprOrTypeTraitOperand(E, UETT_SizeOf);
  }

  if (isInvalid)
    return ExprError();

  if (ExprKind == UETT_SizeOf && E->getType()->isVariableArrayType()) {
    PE = TransformToPotentiallyEvaluated(E);
    if (PE.isInvalid()) return ExprError();
    E = PE.get();
  }

  // C99 6.5.3.4p4: the type (an unsigned integer type) is size_t.
  return new (Context) UnaryExprOrTypeTraitExpr(
      ExprKind, E, Context.getSizeType(), OpLoc, E->getSourceRange().getEnd());
}

/// ActOnUnaryExprOrTypeTraitExpr - Handle @c sizeof(type) and @c sizeof @c
/// expr and the same for @c alignof and @c __alignof
/// Note that the ArgRange is invalid if isType is false.
ExprResult
Sema::ActOnUnaryExprOrTypeTraitExpr(SourceLocation OpLoc,
                                    UnaryExprOrTypeTrait ExprKind, bool IsType,
                                    void *TyOrEx, SourceRange ArgRange) {
  // If error parsing type, ignore.
  if (!TyOrEx) return ExprError();

  if (IsType) {
    TypeSourceInfo *TInfo;
    (void) GetTypeFromParser(ParsedType::getFromOpaquePtr(TyOrEx), &TInfo);
    return CreateUnaryExprOrTypeTraitExpr(TInfo, OpLoc, ExprKind, ArgRange);
  }

  Expr *ArgEx = (Expr *)TyOrEx;
  ExprResult Result = CreateUnaryExprOrTypeTraitExpr(ArgEx, OpLoc, ExprKind);
  return Result;
}

bool Sema::CheckAlignasTypeArgument(StringRef KWName, TypeSourceInfo *TInfo,
                                    SourceLocation OpLoc, SourceRange R) {
  if (!TInfo)
    return true;
  return CheckUnaryExprOrTypeTraitOperand(TInfo->getType(), OpLoc, R,
                                          UETT_AlignOf, KWName);
}

/// ActOnAlignasTypeArgument - Handle @c alignas(type-id) and @c
/// _Alignas(type-name) .
/// [dcl.align] An alignment-specifier of the form
/// alignas(type-id) has the same effect as alignas(alignof(type-id)).
///
/// [N1570 6.7.5] _Alignas(type-name) is equivalent to
/// _Alignas(_Alignof(type-name)).
bool Sema::ActOnAlignasTypeArgument(StringRef KWName, ParsedType Ty,
                                    SourceLocation OpLoc, SourceRange R) {
  TypeSourceInfo *TInfo;
  (void)GetTypeFromParser(ParsedType::getFromOpaquePtr(Ty.getAsOpaquePtr()),
                          &TInfo);
  return CheckAlignasTypeArgument(KWName, TInfo, OpLoc, R);
}

static QualType CheckRealImagOperand(Sema &S, ExprResult &V, SourceLocation Loc,
                                     bool IsReal) {
  if (V.get()->isTypeDependent())
    return S.Context.DependentTy;

  // _Real and _Imag are only l-values for normal l-values.
  if (V.get()->getObjectKind() != OK_Ordinary) {
    V = S.DefaultLvalueConversion(V.get());
    if (V.isInvalid())
      return QualType();
  }

  // These operators return the element type of a complex type.
  if (const ComplexType *CT = V.get()->getType()->getAs<ComplexType>())
    return CT->getElementType();

  // Otherwise they pass through real integer and floating point types here.
  if (V.get()->getType()->isArithmeticType())
    return V.get()->getType();

  // Test for placeholders.
  ExprResult PR = S.CheckPlaceholderExpr(V.get());
  if (PR.isInvalid()) return QualType();
  if (PR.get() != V.get()) {
    V = PR;
    return CheckRealImagOperand(S, V, Loc, IsReal);
  }

  // Reject anything else.
  S.Diag(Loc, diag::err_realimag_invalid_type) << V.get()->getType()
    << (IsReal ? "__real" : "__imag");
  return QualType();
}



ExprResult
Sema::ActOnPostfixUnaryOp(Scope *S, SourceLocation OpLoc,
                          tok::TokenKind Kind, Expr *Input) {
  UnaryOperatorKind Opc;
  switch (Kind) {
  default: llvm_unreachable("Unknown unary op!");
  case tok::plusplus:   Opc = UO_PostInc; break;
  case tok::minusminus: Opc = UO_PostDec; break;
  }

  // Since this might is a postfix expression, get rid of ParenListExprs.
  ExprResult Result = MaybeConvertParenListExprToParenExpr(S, Input);
  if (Result.isInvalid()) return ExprError();
  Input = Result.get();

  return BuildUnaryOp(S, OpLoc, Opc, Input);
}

/// Diagnose if arithmetic on the given ObjC pointer is illegal.
///
/// \return true on error
static bool checkArithmeticOnObjCPointer(Sema &S,
                                         SourceLocation opLoc,
                                         Expr *op) {
  assert(op->getType()->isObjCObjectPointerType());
  if (S.LangOpts.ObjCRuntime.allowsPointerArithmetic() &&
      !S.LangOpts.ObjCSubscriptingLegacyRuntime)
    return false;

  S.Diag(opLoc, diag::err_arithmetic_nonfragile_interface)
    << op->getType()->castAs<ObjCObjectPointerType>()->getPointeeType()
    << op->getSourceRange();
  return true;
}

static bool isMSPropertySubscriptExpr(Sema &S, Expr *Base) {
  auto *BaseNoParens = Base->IgnoreParens();
  if (auto *MSProp = dyn_cast<MSPropertyRefExpr>(BaseNoParens))
    return MSProp->getPropertyDecl()->getType()->isArrayType();
  return isa<MSPropertySubscriptExpr>(BaseNoParens);
}

// Returns the type used for LHS[RHS], given one of LHS, RHS is type-dependent.
// Typically this is DependentTy, but can sometimes be more precise.
//
// There are cases when we could determine a non-dependent type:
//  - LHS and RHS may have non-dependent types despite being type-dependent
//    (e.g. unbounded array static members of the current instantiation)
//  - one may be a dependent-sized array with known element type
//  - one may be a dependent-typed valid index (enum in current instantiation)
//
// We *always* return a dependent type, in such cases it is DependentTy.
// This avoids creating type-dependent expressions with non-dependent types.
// FIXME: is this important to avoid? See https://reviews.llvm.org/D107275
static QualType getDependentArraySubscriptType(Expr *LHS, Expr *RHS,
                                               const ASTContext &Ctx) {
  assert(LHS->isTypeDependent() || RHS->isTypeDependent());
  QualType LTy = LHS->getType(), RTy = RHS->getType();
  QualType Result = Ctx.DependentTy;
  if (RTy->isIntegralOrUnscopedEnumerationType()) {
    if (const PointerType *PT = LTy->getAs<PointerType>())
      Result = PT->getPointeeType();
    else if (const ArrayType *AT = LTy->getAsArrayTypeUnsafe())
      Result = AT->getElementType();
  } else if (LTy->isIntegralOrUnscopedEnumerationType()) {
    if (const PointerType *PT = RTy->getAs<PointerType>())
      Result = PT->getPointeeType();
    else if (const ArrayType *AT = RTy->getAsArrayTypeUnsafe())
      Result = AT->getElementType();
  }
  // Ensure we return a dependent type.
  return Result->isDependentType() ? Result : Ctx.DependentTy;
}

static bool checkArgsForPlaceholders(Sema &S, MultiExprArg args);

ExprResult Sema::ActOnArraySubscriptExpr(Scope *S, Expr *base,
                                         SourceLocation lbLoc,
                                         MultiExprArg ArgExprs,
                                         SourceLocation rbLoc) {

  if (base && !base->getType().isNull() &&
      base->hasPlaceholderType(BuiltinType::OMPArraySection))
    return ActOnOMPArraySectionExpr(base, lbLoc, ArgExprs.front(), SourceLocation(),
                                    SourceLocation(), /*Length*/ nullptr,
                                    /*Stride=*/nullptr, rbLoc);

  // Since this might be a postfix expression, get rid of ParenListExprs.
  if (isa<ParenListExpr>(base)) {
    ExprResult result = MaybeConvertParenListExprToParenExpr(S, base);
    if (result.isInvalid())
      return ExprError();
    base = result.get();
  }

  // Check if base and idx form a MatrixSubscriptExpr.
  //
  // Helper to check for comma expressions, which are not allowed as indices for
  // matrix subscript expressions.
  auto CheckAndReportCommaError = [this, base, rbLoc](Expr *E) {
    if (isa<BinaryOperator>(E) && cast<BinaryOperator>(E)->isCommaOp()) {
      Diag(E->getExprLoc(), diag::err_matrix_subscript_comma)
          << SourceRange(base->getBeginLoc(), rbLoc);
      return true;
    }
    return false;
  };
  // The matrix subscript operator ([][])is considered a single operator.
  // Separating the index expressions by parenthesis is not allowed.
  if (base && !base->getType().isNull() &&
      base->hasPlaceholderType(BuiltinType::IncompleteMatrixIdx) &&
      !isa<MatrixSubscriptExpr>(base)) {
    Diag(base->getExprLoc(), diag::err_matrix_separate_incomplete_index)
        << SourceRange(base->getBeginLoc(), rbLoc);
    return ExprError();
  }
  // If the base is a MatrixSubscriptExpr, try to create a new
  // MatrixSubscriptExpr.
  auto *matSubscriptE = dyn_cast<MatrixSubscriptExpr>(base);
  if (matSubscriptE) {
    assert(ArgExprs.size() == 1);
    if (CheckAndReportCommaError(ArgExprs.front()))
      return ExprError();

    assert(matSubscriptE->isIncomplete() &&
           "base has to be an incomplete matrix subscript");
    return CreateBuiltinMatrixSubscriptExpr(matSubscriptE->getBase(),
                                            matSubscriptE->getRowIdx(),
                                            ArgExprs.front(), rbLoc);
  }
  if (base->getType()->isWebAssemblyTableType()) {
    Diag(base->getExprLoc(), diag::err_wasm_table_art)
        << SourceRange(base->getBeginLoc(), rbLoc) << 3;
    return ExprError();
  }

  // Handle any non-overload placeholder types in the base and index
  // expressions.  We can't handle overloads here because the other
  // operand might be an overloadable type, in which case the overload
  // resolution for the operator overload should get the first crack
  // at the overload.
  bool IsMSPropertySubscript = false;
  if (base->getType()->isNonOverloadPlaceholderType()) {
    IsMSPropertySubscript = isMSPropertySubscriptExpr(*this, base);
    if (!IsMSPropertySubscript) {
      ExprResult result = CheckPlaceholderExpr(base);
      if (result.isInvalid())
        return ExprError();
      base = result.get();
    }
  }

  // If the base is a matrix type, try to create a new MatrixSubscriptExpr.
  if (base->getType()->isMatrixType()) {
    assert(ArgExprs.size() == 1);
    if (CheckAndReportCommaError(ArgExprs.front()))
      return ExprError();

    return CreateBuiltinMatrixSubscriptExpr(base, ArgExprs.front(), nullptr,
                                            rbLoc);
  }

  if (ArgExprs.size() == 1 && getLangOpts().CPlusPlus20) {
    Expr *idx = ArgExprs[0];
    if ((isa<BinaryOperator>(idx) && cast<BinaryOperator>(idx)->isCommaOp()) ||
        (isa<CXXOperatorCallExpr>(idx) &&
         cast<CXXOperatorCallExpr>(idx)->getOperator() == OO_Comma)) {
      Diag(idx->getExprLoc(), diag::warn_deprecated_comma_subscript)
          << SourceRange(base->getBeginLoc(), rbLoc);
    }
  }

  if (ArgExprs.size() == 1 &&
      ArgExprs[0]->getType()->isNonOverloadPlaceholderType()) {
    ExprResult result = CheckPlaceholderExpr(ArgExprs[0]);
    if (result.isInvalid())
      return ExprError();
    ArgExprs[0] = result.get();
  } else {
    if (checkArgsForPlaceholders(*this, ArgExprs))
      return ExprError();
  }

  // Build an unanalyzed expression if either operand is type-dependent.
  if (getLangOpts().CPlusPlus && ArgExprs.size() == 1 &&
      (base->isTypeDependent() ||
       Expr::hasAnyTypeDependentArguments(ArgExprs)) &&
      !isa<PackExpansionExpr>(ArgExprs[0])) {
    return new (Context) ArraySubscriptExpr(
        base, ArgExprs.front(),
        getDependentArraySubscriptType(base, ArgExprs.front(), getASTContext()),
        VK_LValue, OK_Ordinary, rbLoc);
  }

  // MSDN, property (C++)
  // https://msdn.microsoft.com/en-us/library/yhfk0thd(v=vs.120).aspx
  // This attribute can also be used in the declaration of an empty array in a
  // class or structure definition. For example:
  // __declspec(property(get=GetX, put=PutX)) int x[];
  // The above statement indicates that x[] can be used with one or more array
  // indices. In this case, i=p->x[a][b] will be turned into i=p->GetX(a, b),
  // and p->x[a][b] = i will be turned into p->PutX(a, b, i);
  if (IsMSPropertySubscript) {
    assert(ArgExprs.size() == 1);
    // Build MS property subscript expression if base is MS property reference
    // or MS property subscript.
    return new (Context)
        MSPropertySubscriptExpr(base, ArgExprs.front(), Context.PseudoObjectTy,
                                VK_LValue, OK_Ordinary, rbLoc);
  }

  // Use C++ overloaded-operator rules if either operand has record
  // type.  The spec says to do this if either type is *overloadable*,
  // but enum types can't declare subscript operators or conversion
  // operators, so there's nothing interesting for overload resolution
  // to do if there aren't any record types involved.
  //
  // ObjC pointers have their own subscripting logic that is not tied
  // to overload resolution and so should not take this path.
  if (getLangOpts().CPlusPlus && !base->getType()->isObjCObjectPointerType() &&
      ((base->getType()->isRecordType() ||
        (ArgExprs.size() != 1 || isa<PackExpansionExpr>(ArgExprs[0]) ||
         ArgExprs[0]->getType()->isRecordType())))) {
    return CreateOverloadedArraySubscriptExpr(lbLoc, rbLoc, base, ArgExprs);
  }

  ExprResult Res =
      CreateBuiltinArraySubscriptExpr(base, lbLoc, ArgExprs.front(), rbLoc);

  if (!Res.isInvalid() && isa<ArraySubscriptExpr>(Res.get()))
    CheckSubscriptAccessOfNoDeref(cast<ArraySubscriptExpr>(Res.get()));

  return Res;
}

ExprResult Sema::tryConvertExprToType(Expr *E, QualType Ty) {
  InitializedEntity Entity = InitializedEntity::InitializeTemporary(Ty);
  InitializationKind Kind =
      InitializationKind::CreateCopy(E->getBeginLoc(), SourceLocation());
  InitializationSequence InitSeq(*this, Entity, Kind, E);
  return InitSeq.Perform(*this, Entity, Kind, E);
}

ExprResult Sema::CreateBuiltinMatrixSubscriptExpr(Expr *Base, Expr *RowIdx,
                                                  Expr *ColumnIdx,
                                                  SourceLocation RBLoc) {
  ExprResult BaseR = CheckPlaceholderExpr(Base);
  if (BaseR.isInvalid())
    return BaseR;
  Base = BaseR.get();

  ExprResult RowR = CheckPlaceholderExpr(RowIdx);
  if (RowR.isInvalid())
    return RowR;
  RowIdx = RowR.get();

  if (!ColumnIdx)
    return new (Context) MatrixSubscriptExpr(
        Base, RowIdx, ColumnIdx, Context.IncompleteMatrixIdxTy, RBLoc);

  // Build an unanalyzed expression if any of the operands is type-dependent.
  if (Base->isTypeDependent() || RowIdx->isTypeDependent() ||
      ColumnIdx->isTypeDependent())
    return new (Context) MatrixSubscriptExpr(Base, RowIdx, ColumnIdx,
                                             Context.DependentTy, RBLoc);

  ExprResult ColumnR = CheckPlaceholderExpr(ColumnIdx);
  if (ColumnR.isInvalid())
    return ColumnR;
  ColumnIdx = ColumnR.get();

  // Check that IndexExpr is an integer expression. If it is a constant
  // expression, check that it is less than Dim (= the number of elements in the
  // corresponding dimension).
  auto IsIndexValid = [&](Expr *IndexExpr, unsigned Dim,
                          bool IsColumnIdx) -> Expr * {
    if (!IndexExpr->getType()->isIntegerType() &&
        !IndexExpr->isTypeDependent()) {
      Diag(IndexExpr->getBeginLoc(), diag::err_matrix_index_not_integer)
          << IsColumnIdx;
      return nullptr;
    }

    if (std::optional<llvm::APSInt> Idx =
            IndexExpr->getIntegerConstantExpr(Context)) {
      if ((*Idx < 0 || *Idx >= Dim)) {
        Diag(IndexExpr->getBeginLoc(), diag::err_matrix_index_outside_range)
            << IsColumnIdx << Dim;
        return nullptr;
      }
    }

    ExprResult ConvExpr =
        tryConvertExprToType(IndexExpr, Context.getSizeType());
    assert(!ConvExpr.isInvalid() &&
           "should be able to convert any integer type to size type");
    return ConvExpr.get();
  };

  auto *MTy = Base->getType()->getAs<ConstantMatrixType>();
  RowIdx = IsIndexValid(RowIdx, MTy->getNumRows(), false);
  ColumnIdx = IsIndexValid(ColumnIdx, MTy->getNumColumns(), true);
  if (!RowIdx || !ColumnIdx)
    return ExprError();

  return new (Context) MatrixSubscriptExpr(Base, RowIdx, ColumnIdx,
                                           MTy->getElementType(), RBLoc);
}

void Sema::CheckAddressOfNoDeref(const Expr *E) {
  ExpressionEvaluationContextRecord &LastRecord = ExprEvalContexts.back();
  const Expr *StrippedExpr = E->IgnoreParenImpCasts();

  // For expressions like `&(*s).b`, the base is recorded and what should be
  // checked.
  const MemberExpr *Member = nullptr;
  while ((Member = dyn_cast<MemberExpr>(StrippedExpr)) && !Member->isArrow())
    StrippedExpr = Member->getBase()->IgnoreParenImpCasts();

  LastRecord.PossibleDerefs.erase(StrippedExpr);
}

void Sema::CheckSubscriptAccessOfNoDeref(const ArraySubscriptExpr *E) {
  if (isUnevaluatedContext())
    return;

  QualType ResultTy = E->getType();
  ExpressionEvaluationContextRecord &LastRecord = ExprEvalContexts.back();

  // Bail if the element is an array since it is not memory access.
  if (isa<ArrayType>(ResultTy))
    return;

  if (ResultTy->hasAttr(attr::NoDeref)) {
    LastRecord.PossibleDerefs.insert(E);
    return;
  }

  // Check if the base type is a pointer to a member access of a struct
  // marked with noderef.
  const Expr *Base = E->getBase();
  QualType BaseTy = Base->getType();
  if (!(isa<ArrayType>(BaseTy) || isa<PointerType>(BaseTy)))
    // Not a pointer access
    return;

  const MemberExpr *Member = nullptr;
  while ((Member = dyn_cast<MemberExpr>(Base->IgnoreParenCasts())) &&
         Member->isArrow())
    Base = Member->getBase();

  if (const auto *Ptr = dyn_cast<PointerType>(Base->getType())) {
    if (Ptr->getPointeeType()->hasAttr(attr::NoDeref))
      LastRecord.PossibleDerefs.insert(E);
  }
}

ExprResult Sema::ActOnOMPArraySectionExpr(Expr *Base, SourceLocation LBLoc,
                                          Expr *LowerBound,
                                          SourceLocation ColonLocFirst,
                                          SourceLocation ColonLocSecond,
                                          Expr *Length, Expr *Stride,
                                          SourceLocation RBLoc) {
  if (Base->hasPlaceholderType() &&
      !Base->hasPlaceholderType(BuiltinType::OMPArraySection)) {
    ExprResult Result = CheckPlaceholderExpr(Base);
    if (Result.isInvalid())
      return ExprError();
    Base = Result.get();
  }
  if (LowerBound && LowerBound->getType()->isNonOverloadPlaceholderType()) {
    ExprResult Result = CheckPlaceholderExpr(LowerBound);
    if (Result.isInvalid())
      return ExprError();
    Result = DefaultLvalueConversion(Result.get());
    if (Result.isInvalid())
      return ExprError();
    LowerBound = Result.get();
  }
  if (Length && Length->getType()->isNonOverloadPlaceholderType()) {
    ExprResult Result = CheckPlaceholderExpr(Length);
    if (Result.isInvalid())
      return ExprError();
    Result = DefaultLvalueConversion(Result.get());
    if (Result.isInvalid())
      return ExprError();
    Length = Result.get();
  }
  if (Stride && Stride->getType()->isNonOverloadPlaceholderType()) {
    ExprResult Result = CheckPlaceholderExpr(Stride);
    if (Result.isInvalid())
      return ExprError();
    Result = DefaultLvalueConversion(Result.get());
    if (Result.isInvalid())
      return ExprError();
    Stride = Result.get();
  }

  // Build an unanalyzed expression if either operand is type-dependent.
  if (Base->isTypeDependent() ||
      (LowerBound &&
       (LowerBound->isTypeDependent() || LowerBound->isValueDependent())) ||
      (Length && (Length->isTypeDependent() || Length->isValueDependent())) ||
      (Stride && (Stride->isTypeDependent() || Stride->isValueDependent()))) {
    return new (Context) OMPArraySectionExpr(
        Base, LowerBound, Length, Stride, Context.DependentTy, VK_LValue,
        OK_Ordinary, ColonLocFirst, ColonLocSecond, RBLoc);
  }

  // Perform default conversions.
  QualType OriginalTy = OMPArraySectionExpr::getBaseOriginalType(Base);
  QualType ResultTy;
  if (OriginalTy->isAnyPointerType()) {
    ResultTy = OriginalTy->getPointeeType();
  } else if (OriginalTy->isArrayType()) {
    ResultTy = OriginalTy->getAsArrayTypeUnsafe()->getElementType();
  } else {
    return ExprError(
        Diag(Base->getExprLoc(), diag::err_omp_typecheck_section_value)
        << Base->getSourceRange());
  }
  // C99 6.5.2.1p1
  if (LowerBound) {
    auto Res = PerformOpenMPImplicitIntegerConversion(LowerBound->getExprLoc(),
                                                      LowerBound);
    if (Res.isInvalid())
      return ExprError(Diag(LowerBound->getExprLoc(),
                            diag::err_omp_typecheck_section_not_integer)
                       << 0 << LowerBound->getSourceRange());
    LowerBound = Res.get();

    if (LowerBound->getType()->isSpecificBuiltinType(BuiltinType::Char_S) ||
        LowerBound->getType()->isSpecificBuiltinType(BuiltinType::Char_U))
      Diag(LowerBound->getExprLoc(), diag::warn_omp_section_is_char)
          << 0 << LowerBound->getSourceRange();
  }
  if (Length) {
    auto Res =
        PerformOpenMPImplicitIntegerConversion(Length->getExprLoc(), Length);
    if (Res.isInvalid())
      return ExprError(Diag(Length->getExprLoc(),
                            diag::err_omp_typecheck_section_not_integer)
                       << 1 << Length->getSourceRange());
    Length = Res.get();

    if (Length->getType()->isSpecificBuiltinType(BuiltinType::Char_S) ||
        Length->getType()->isSpecificBuiltinType(BuiltinType::Char_U))
      Diag(Length->getExprLoc(), diag::warn_omp_section_is_char)
          << 1 << Length->getSourceRange();
  }
  if (Stride) {
    ExprResult Res =
        PerformOpenMPImplicitIntegerConversion(Stride->getExprLoc(), Stride);
    if (Res.isInvalid())
      return ExprError(Diag(Stride->getExprLoc(),
                            diag::err_omp_typecheck_section_not_integer)
                       << 1 << Stride->getSourceRange());
    Stride = Res.get();

    if (Stride->getType()->isSpecificBuiltinType(BuiltinType::Char_S) ||
        Stride->getType()->isSpecificBuiltinType(BuiltinType::Char_U))
      Diag(Stride->getExprLoc(), diag::warn_omp_section_is_char)
          << 1 << Stride->getSourceRange();
  }

  // C99 6.5.2.1p1: "shall have type "pointer to *object* type". Similarly,
  // C++ [expr.sub]p1: The type "T" shall be a completely-defined object
  // type. Note that functions are not objects, and that (in C99 parlance)
  // incomplete types are not object types.
  if (ResultTy->isFunctionType()) {
    Diag(Base->getExprLoc(), diag::err_omp_section_function_type)
        << ResultTy << Base->getSourceRange();
    return ExprError();
  }

  if (RequireCompleteType(Base->getExprLoc(), ResultTy,
                          diag::err_omp_section_incomplete_type, Base))
    return ExprError();

  if (LowerBound && !OriginalTy->isAnyPointerType()) {
    Expr::EvalResult Result;
    if (LowerBound->EvaluateAsInt(Result, Context)) {
      // OpenMP 5.0, [2.1.5 Array Sections]
      // The array section must be a subset of the original array.
      llvm::APSInt LowerBoundValue = Result.Val.getInt();
      if (LowerBoundValue.isNegative()) {
        Diag(LowerBound->getExprLoc(), diag::err_omp_section_not_subset_of_array)
            << LowerBound->getSourceRange();
        return ExprError();
      }
    }
  }

  if (Length) {
    Expr::EvalResult Result;
    if (Length->EvaluateAsInt(Result, Context)) {
      // OpenMP 5.0, [2.1.5 Array Sections]
      // The length must evaluate to non-negative integers.
      llvm::APSInt LengthValue = Result.Val.getInt();
      if (LengthValue.isNegative()) {
        Diag(Length->getExprLoc(), diag::err_omp_section_length_negative)
            << toString(LengthValue, /*Radix=*/10, /*Signed=*/true)
            << Length->getSourceRange();
        return ExprError();
      }
    }
  } else if (ColonLocFirst.isValid() &&
             (OriginalTy.isNull() || (!OriginalTy->isConstantArrayType() &&
                                      !OriginalTy->isVariableArrayType()))) {
    // OpenMP 5.0, [2.1.5 Array Sections]
    // When the size of the array dimension is not known, the length must be
    // specified explicitly.
    Diag(ColonLocFirst, diag::err_omp_section_length_undefined)
        << (!OriginalTy.isNull() && OriginalTy->isArrayType());
    return ExprError();
  }

  if (Stride) {
    Expr::EvalResult Result;
    if (Stride->EvaluateAsInt(Result, Context)) {
      // OpenMP 5.0, [2.1.5 Array Sections]
      // The stride must evaluate to a positive integer.
      llvm::APSInt StrideValue = Result.Val.getInt();
      if (!StrideValue.isStrictlyPositive()) {
        Diag(Stride->getExprLoc(), diag::err_omp_section_stride_non_positive)
            << toString(StrideValue, /*Radix=*/10, /*Signed=*/true)
            << Stride->getSourceRange();
        return ExprError();
      }
    }
  }

  if (!Base->hasPlaceholderType(BuiltinType::OMPArraySection)) {
    ExprResult Result = DefaultFunctionArrayLvalueConversion(Base);
    if (Result.isInvalid())
      return ExprError();
    Base = Result.get();
  }
  return new (Context) OMPArraySectionExpr(
      Base, LowerBound, Length, Stride, Context.OMPArraySectionTy, VK_LValue,
      OK_Ordinary, ColonLocFirst, ColonLocSecond, RBLoc);
}

ExprResult Sema::ActOnOMPArrayShapingExpr(Expr *Base, SourceLocation LParenLoc,
                                          SourceLocation RParenLoc,
                                          ArrayRef<Expr *> Dims,
                                          ArrayRef<SourceRange> Brackets) {
  if (Base->hasPlaceholderType()) {
    ExprResult Result = CheckPlaceholderExpr(Base);
    if (Result.isInvalid())
      return ExprError();
    Result = DefaultLvalueConversion(Result.get());
    if (Result.isInvalid())
      return ExprError();
    Base = Result.get();
  }
  QualType BaseTy = Base->getType();
  // Delay analysis of the types/expressions if instantiation/specialization is
  // required.
  if (!BaseTy->isPointerType() && Base->isTypeDependent())
    return OMPArrayShapingExpr::Create(Context, Context.DependentTy, Base,
                                       LParenLoc, RParenLoc, Dims, Brackets);
  if (!BaseTy->isPointerType() ||
      (!Base->isTypeDependent() &&
       BaseTy->getPointeeType()->isIncompleteType()))
    return ExprError(Diag(Base->getExprLoc(),
                          diag::err_omp_non_pointer_type_array_shaping_base)
                     << Base->getSourceRange());

  SmallVector<Expr *, 4> NewDims;
  bool ErrorFound = false;
  for (Expr *Dim : Dims) {
    if (Dim->hasPlaceholderType()) {
      ExprResult Result = CheckPlaceholderExpr(Dim);
      if (Result.isInvalid()) {
        ErrorFound = true;
        continue;
      }
      Result = DefaultLvalueConversion(Result.get());
      if (Result.isInvalid()) {
        ErrorFound = true;
        continue;
      }
      Dim = Result.get();
    }
    if (!Dim->isTypeDependent()) {
      ExprResult Result =
          PerformOpenMPImplicitIntegerConversion(Dim->getExprLoc(), Dim);
      if (Result.isInvalid()) {
        ErrorFound = true;
        Diag(Dim->getExprLoc(), diag::err_omp_typecheck_shaping_not_integer)
            << Dim->getSourceRange();
        continue;
      }
      Dim = Result.get();
      Expr::EvalResult EvResult;
      if (!Dim->isValueDependent() && Dim->EvaluateAsInt(EvResult, Context)) {
        // OpenMP 5.0, [2.1.4 Array Shaping]
        // Each si is an integral type expression that must evaluate to a
        // positive integer.
        llvm::APSInt Value = EvResult.Val.getInt();
        if (!Value.isStrictlyPositive()) {
          Diag(Dim->getExprLoc(), diag::err_omp_shaping_dimension_not_positive)
              << toString(Value, /*Radix=*/10, /*Signed=*/true)
              << Dim->getSourceRange();
          ErrorFound = true;
          continue;
        }
      }
    }
    NewDims.push_back(Dim);
  }
  if (ErrorFound)
    return ExprError();
  return OMPArrayShapingExpr::Create(Context, Context.OMPArrayShapingTy, Base,
                                     LParenLoc, RParenLoc, NewDims, Brackets);
}

ExprResult Sema::ActOnOMPIteratorExpr(Scope *S, SourceLocation IteratorKwLoc,
                                      SourceLocation LLoc, SourceLocation RLoc,
                                      ArrayRef<OMPIteratorData> Data) {
  SmallVector<OMPIteratorExpr::IteratorDefinition, 4> ID;
  bool IsCorrect = true;
  for (const OMPIteratorData &D : Data) {
    TypeSourceInfo *TInfo = nullptr;
    SourceLocation StartLoc;
    QualType DeclTy;
    if (!D.Type.getAsOpaquePtr()) {
      // OpenMP 5.0, 2.1.6 Iterators
      // In an iterator-specifier, if the iterator-type is not specified then
      // the type of that iterator is of int type.
      DeclTy = Context.IntTy;
      StartLoc = D.DeclIdentLoc;
    } else {
      DeclTy = GetTypeFromParser(D.Type, &TInfo);
      StartLoc = TInfo->getTypeLoc().getBeginLoc();
    }

    bool IsDeclTyDependent = DeclTy->isDependentType() ||
                             DeclTy->containsUnexpandedParameterPack() ||
                             DeclTy->isInstantiationDependentType();
    if (!IsDeclTyDependent) {
      if (!DeclTy->isIntegralType(Context) && !DeclTy->isAnyPointerType()) {
        // OpenMP 5.0, 2.1.6 Iterators, Restrictions, C/C++
        // The iterator-type must be an integral or pointer type.
        Diag(StartLoc, diag::err_omp_iterator_not_integral_or_pointer)
            << DeclTy;
        IsCorrect = false;
        continue;
      }
      if (DeclTy.isConstant(Context)) {
        // OpenMP 5.0, 2.1.6 Iterators, Restrictions, C/C++
        // The iterator-type must not be const qualified.
        Diag(StartLoc, diag::err_omp_iterator_not_integral_or_pointer)
            << DeclTy;
        IsCorrect = false;
        continue;
      }
    }

    // Iterator declaration.
    assert(D.DeclIdent && "Identifier expected.");
    // Always try to create iterator declarator to avoid extra error messages
    // about unknown declarations use.
    auto *VD = VarDecl::Create(Context, CurContext, StartLoc, D.DeclIdentLoc,
                               D.DeclIdent, DeclTy, TInfo, SC_None);
    VD->setImplicit();
    if (S) {
      // Check for conflicting previous declaration.
      DeclarationNameInfo NameInfo(VD->getDeclName(), D.DeclIdentLoc);
      LookupResult Previous(*this, NameInfo, LookupOrdinaryName,
                            ForVisibleRedeclaration);
      Previous.suppressDiagnostics();
      LookupName(Previous, S);

      FilterLookupForScope(Previous, CurContext, S, /*ConsiderLinkage=*/false,
                           /*AllowInlineNamespace=*/false);
      if (!Previous.empty()) {
        NamedDecl *Old = Previous.getRepresentativeDecl();
        Diag(D.DeclIdentLoc, diag::err_redefinition) << VD->getDeclName();
        Diag(Old->getLocation(), diag::note_previous_definition);
      } else {
        PushOnScopeChains(VD, S);
      }
    } else {
      CurContext->addDecl(VD);
    }

    /// Act on the iterator variable declaration.
    ActOnOpenMPIteratorVarDecl(VD);

    Expr *Begin = D.Range.Begin;
    if (!IsDeclTyDependent && Begin && !Begin->isTypeDependent()) {
      ExprResult BeginRes =
          PerformImplicitConversion(Begin, DeclTy, AA_Converting);
      Begin = BeginRes.get();
    }
    Expr *End = D.Range.End;
    if (!IsDeclTyDependent && End && !End->isTypeDependent()) {
      ExprResult EndRes = PerformImplicitConversion(End, DeclTy, AA_Converting);
      End = EndRes.get();
    }
    Expr *Step = D.Range.Step;
    if (!IsDeclTyDependent && Step && !Step->isTypeDependent()) {
      if (!Step->getType()->isIntegralType(Context)) {
        Diag(Step->getExprLoc(), diag::err_omp_iterator_step_not_integral)
            << Step << Step->getSourceRange();
        IsCorrect = false;
        continue;
      }
      std::optional<llvm::APSInt> Result =
          Step->getIntegerConstantExpr(Context);
      // OpenMP 5.0, 2.1.6 Iterators, Restrictions
      // If the step expression of a range-specification equals zero, the
      // behavior is unspecified.
      if (Result && Result->isZero()) {
        Diag(Step->getExprLoc(), diag::err_omp_iterator_step_constant_zero)
            << Step << Step->getSourceRange();
        IsCorrect = false;
        continue;
      }
    }
    if (!Begin || !End || !IsCorrect) {
      IsCorrect = false;
      continue;
    }
    OMPIteratorExpr::IteratorDefinition &IDElem = ID.emplace_back();
    IDElem.IteratorDecl = VD;
    IDElem.AssignmentLoc = D.AssignLoc;
    IDElem.Range.Begin = Begin;
    IDElem.Range.End = End;
    IDElem.Range.Step = Step;
    IDElem.ColonLoc = D.ColonLoc;
    IDElem.SecondColonLoc = D.SecColonLoc;
  }
  if (!IsCorrect) {
    // Invalidate all created iterator declarations if error is found.
    for (const OMPIteratorExpr::IteratorDefinition &D : ID) {
      if (Decl *ID = D.IteratorDecl)
        ID->setInvalidDecl();
    }
    return ExprError();
  }
  SmallVector<OMPIteratorHelperData, 4> Helpers;
  if (!CurContext->isDependentContext()) {
    // Build number of ityeration for each iteration range.
    // Ni = ((Stepi > 0) ? ((Endi + Stepi -1 - Begini)/Stepi) :
    // ((Begini-Stepi-1-Endi) / -Stepi);
    for (OMPIteratorExpr::IteratorDefinition &D : ID) {
      // (Endi - Begini)
      ExprResult Res = CreateBuiltinBinOp(D.AssignmentLoc, BO_Sub, D.Range.End,
                                          D.Range.Begin);
      if(!Res.isUsable()) {
        IsCorrect = false;
        continue;
      }
      ExprResult St, St1;
      if (D.Range.Step) {
        St = D.Range.Step;
        // (Endi - Begini) + Stepi
        Res = CreateBuiltinBinOp(D.AssignmentLoc, BO_Add, Res.get(), St.get());
        if (!Res.isUsable()) {
          IsCorrect = false;
          continue;
        }
        // (Endi - Begini) + Stepi - 1
        Res =
            CreateBuiltinBinOp(D.AssignmentLoc, BO_Sub, Res.get(),
                               ActOnIntegerConstant(D.AssignmentLoc, 1).get());
        if (!Res.isUsable()) {
          IsCorrect = false;
          continue;
        }
        // ((Endi - Begini) + Stepi - 1) / Stepi
        Res = CreateBuiltinBinOp(D.AssignmentLoc, BO_Div, Res.get(), St.get());
        if (!Res.isUsable()) {
          IsCorrect = false;
          continue;
        }
        St1 = CreateBuiltinUnaryOp(D.AssignmentLoc, UO_Minus, D.Range.Step);
        // (Begini - Endi)
        ExprResult Res1 = CreateBuiltinBinOp(D.AssignmentLoc, BO_Sub,
                                             D.Range.Begin, D.Range.End);
        if (!Res1.isUsable()) {
          IsCorrect = false;
          continue;
        }
        // (Begini - Endi) - Stepi
        Res1 =
            CreateBuiltinBinOp(D.AssignmentLoc, BO_Add, Res1.get(), St1.get());
        if (!Res1.isUsable()) {
          IsCorrect = false;
          continue;
        }
        // (Begini - Endi) - Stepi - 1
        Res1 =
            CreateBuiltinBinOp(D.AssignmentLoc, BO_Sub, Res1.get(),
                               ActOnIntegerConstant(D.AssignmentLoc, 1).get());
        if (!Res1.isUsable()) {
          IsCorrect = false;
          continue;
        }
        // ((Begini - Endi) - Stepi - 1) / (-Stepi)
        Res1 =
            CreateBuiltinBinOp(D.AssignmentLoc, BO_Div, Res1.get(), St1.get());
        if (!Res1.isUsable()) {
          IsCorrect = false;
          continue;
        }
        // Stepi > 0.
        ExprResult CmpRes =
            CreateBuiltinBinOp(D.AssignmentLoc, BO_GT, D.Range.Step,
                               ActOnIntegerConstant(D.AssignmentLoc, 0).get());
        if (!CmpRes.isUsable()) {
          IsCorrect = false;
          continue;
        }
        Res = ActOnConditionalOp(D.AssignmentLoc, D.AssignmentLoc, CmpRes.get(),
                                 Res.get(), Res1.get());
        if (!Res.isUsable()) {
          IsCorrect = false;
          continue;
        }
      }
      Res = ActOnFinishFullExpr(Res.get(), /*DiscardedValue=*/false);
      if (!Res.isUsable()) {
        IsCorrect = false;
        continue;
      }

      // Build counter update.
      // Build counter.
      auto *CounterVD =
          VarDecl::Create(Context, CurContext, D.IteratorDecl->getBeginLoc(),
                          D.IteratorDecl->getBeginLoc(), nullptr,
                          Res.get()->getType(), nullptr, SC_None);
      CounterVD->setImplicit();
      ExprResult RefRes =
          BuildDeclRefExpr(CounterVD, CounterVD->getType(), VK_LValue,
                           D.IteratorDecl->getBeginLoc());
      // Build counter update.
      // I = Begini + counter * Stepi;
      ExprResult UpdateRes;
      if (D.Range.Step) {
        UpdateRes = CreateBuiltinBinOp(
            D.AssignmentLoc, BO_Mul,
            DefaultLvalueConversion(RefRes.get()).get(), St.get());
      } else {
        UpdateRes = DefaultLvalueConversion(RefRes.get());
      }
      if (!UpdateRes.isUsable()) {
        IsCorrect = false;
        continue;
      }
      UpdateRes = CreateBuiltinBinOp(D.AssignmentLoc, BO_Add, D.Range.Begin,
                                     UpdateRes.get());
      if (!UpdateRes.isUsable()) {
        IsCorrect = false;
        continue;
      }
      ExprResult VDRes =
          BuildDeclRefExpr(cast<VarDecl>(D.IteratorDecl),
                           cast<VarDecl>(D.IteratorDecl)->getType(), VK_LValue,
                           D.IteratorDecl->getBeginLoc());
      UpdateRes = CreateBuiltinBinOp(D.AssignmentLoc, BO_Assign, VDRes.get(),
                                     UpdateRes.get());
      if (!UpdateRes.isUsable()) {
        IsCorrect = false;
        continue;
      }
      UpdateRes =
          ActOnFinishFullExpr(UpdateRes.get(), /*DiscardedValue=*/true);
      if (!UpdateRes.isUsable()) {
        IsCorrect = false;
        continue;
      }
      ExprResult CounterUpdateRes =
          CreateBuiltinUnaryOp(D.AssignmentLoc, UO_PreInc, RefRes.get());
      if (!CounterUpdateRes.isUsable()) {
        IsCorrect = false;
        continue;
      }
      CounterUpdateRes =
          ActOnFinishFullExpr(CounterUpdateRes.get(), /*DiscardedValue=*/true);
      if (!CounterUpdateRes.isUsable()) {
        IsCorrect = false;
        continue;
      }
      OMPIteratorHelperData &HD = Helpers.emplace_back();
      HD.CounterVD = CounterVD;
      HD.Upper = Res.get();
      HD.Update = UpdateRes.get();
      HD.CounterUpdate = CounterUpdateRes.get();
    }
  } else {
    Helpers.assign(ID.size(), {});
  }
  if (!IsCorrect) {
    // Invalidate all created iterator declarations if error is found.
    for (const OMPIteratorExpr::IteratorDefinition &D : ID) {
      if (Decl *ID = D.IteratorDecl)
        ID->setInvalidDecl();
    }
    return ExprError();
  }
  return OMPIteratorExpr::Create(Context, Context.OMPIteratorTy, IteratorKwLoc,
                                 LLoc, RLoc, ID, Helpers);
}

ExprResult
Sema::CreateBuiltinArraySubscriptExpr(Expr *Base, SourceLocation LLoc,
                                      Expr *Idx, SourceLocation RLoc) {
  Expr *LHSExp = Base;
  Expr *RHSExp = Idx;

  ExprValueKind VK = VK_LValue;
  ExprObjectKind OK = OK_Ordinary;

  // Per C++ core issue 1213, the result is an xvalue if either operand is
  // a non-lvalue array, and an lvalue otherwise.
  if (getLangOpts().CPlusPlus11) {
    for (auto *Op : {LHSExp, RHSExp}) {
      Op = Op->IgnoreImplicit();
      if (Op->getType()->isArrayType() && !Op->isLValue())
        VK = VK_XValue;
    }
  }

  // Perform default conversions.
  if (!LHSExp->getType()->getAs<VectorType>()) {
    ExprResult Result = DefaultFunctionArrayLvalueConversion(LHSExp);
    if (Result.isInvalid())
      return ExprError();
    LHSExp = Result.get();
  }
  ExprResult Result = DefaultFunctionArrayLvalueConversion(RHSExp);
  if (Result.isInvalid())
    return ExprError();
  RHSExp = Result.get();

  QualType LHSTy = LHSExp->getType(), RHSTy = RHSExp->getType();

  // C99 6.5.2.1p2: the expression e1[e2] is by definition precisely equivalent
  // to the expression *((e1)+(e2)). This means the array "Base" may actually be
  // in the subscript position. As a result, we need to derive the array base
  // and index from the expression types.
  Expr *BaseExpr, *IndexExpr;
  QualType ResultType;
  if (LHSTy->isDependentType() || RHSTy->isDependentType()) {
    BaseExpr = LHSExp;
    IndexExpr = RHSExp;
    ResultType =
        getDependentArraySubscriptType(LHSExp, RHSExp, getASTContext());
  } else if (const PointerType *PTy = LHSTy->getAs<PointerType>()) {
    BaseExpr = LHSExp;
    IndexExpr = RHSExp;
    ResultType = PTy->getPointeeType();
  } else if (const ObjCObjectPointerType *PTy =
               LHSTy->getAs<ObjCObjectPointerType>()) {
    BaseExpr = LHSExp;
    IndexExpr = RHSExp;

    // Use custom logic if this should be the pseudo-object subscript
    // expression.
    if (!LangOpts.isSubscriptPointerArithmetic())
      return BuildObjCSubscriptExpression(RLoc, BaseExpr, IndexExpr, nullptr,
                                          nullptr);

    ResultType = PTy->getPointeeType();
  } else if (const PointerType *PTy = RHSTy->getAs<PointerType>()) {
     // Handle the uncommon case of "123[Ptr]".
    BaseExpr = RHSExp;
    IndexExpr = LHSExp;
    ResultType = PTy->getPointeeType();
  } else if (const ObjCObjectPointerType *PTy =
               RHSTy->getAs<ObjCObjectPointerType>()) {
     // Handle the uncommon case of "123[Ptr]".
    BaseExpr = RHSExp;
    IndexExpr = LHSExp;
    ResultType = PTy->getPointeeType();
    if (!LangOpts.isSubscriptPointerArithmetic()) {
      Diag(LLoc, diag::err_subscript_nonfragile_interface)
        << ResultType << BaseExpr->getSourceRange();
      return ExprError();
    }
  } else if (const VectorType *VTy = LHSTy->getAs<VectorType>()) {
    BaseExpr = LHSExp;    // vectors: V[123]
    IndexExpr = RHSExp;
    // We apply C++ DR1213 to vector subscripting too.
    if (getLangOpts().CPlusPlus11 && LHSExp->isPRValue()) {
      ExprResult Materialized = TemporaryMaterializationConversion(LHSExp);
      if (Materialized.isInvalid())
        return ExprError();
      LHSExp = Materialized.get();
    }
    VK = LHSExp->getValueKind();
    if (VK != VK_PRValue)
      OK = OK_VectorComponent;

    ResultType = VTy->getElementType();
    QualType BaseType = BaseExpr->getType();
    Qualifiers BaseQuals = BaseType.getQualifiers();
    Qualifiers MemberQuals = ResultType.getQualifiers();
    Qualifiers Combined = BaseQuals + MemberQuals;
    if (Combined != MemberQuals)
      ResultType = Context.getQualifiedType(ResultType, Combined);
  } else if (LHSTy->isBuiltinType() &&
             LHSTy->getAs<BuiltinType>()->isSveVLSBuiltinType()) {
    const BuiltinType *BTy = LHSTy->getAs<BuiltinType>();
    if (BTy->isSVEBool())
      return ExprError(Diag(LLoc, diag::err_subscript_svbool_t)
                       << LHSExp->getSourceRange() << RHSExp->getSourceRange());

    BaseExpr = LHSExp;
    IndexExpr = RHSExp;
    if (getLangOpts().CPlusPlus11 && LHSExp->isPRValue()) {
      ExprResult Materialized = TemporaryMaterializationConversion(LHSExp);
      if (Materialized.isInvalid())
        return ExprError();
      LHSExp = Materialized.get();
    }
    VK = LHSExp->getValueKind();
    if (VK != VK_PRValue)
      OK = OK_VectorComponent;

    ResultType = BTy->getSveEltType(Context);

    QualType BaseType = BaseExpr->getType();
    Qualifiers BaseQuals = BaseType.getQualifiers();
    Qualifiers MemberQuals = ResultType.getQualifiers();
    Qualifiers Combined = BaseQuals + MemberQuals;
    if (Combined != MemberQuals)
      ResultType = Context.getQualifiedType(ResultType, Combined);
  } else if (LHSTy->isArrayType()) {
    // If we see an array that wasn't promoted by
    // DefaultFunctionArrayLvalueConversion, it must be an array that
    // wasn't promoted because of the C90 rule that doesn't
    // allow promoting non-lvalue arrays.  Warn, then
    // force the promotion here.
    Diag(LHSExp->getBeginLoc(), diag::ext_subscript_non_lvalue)
        << LHSExp->getSourceRange();
    LHSExp = ImpCastExprToType(LHSExp, Context.getArrayDecayedType(LHSTy),
                               CK_ArrayToPointerDecay).get();
    LHSTy = LHSExp->getType();

    BaseExpr = LHSExp;
    IndexExpr = RHSExp;
    ResultType = LHSTy->castAs<PointerType>()->getPointeeType();
  } else if (RHSTy->isArrayType()) {
    // Same as previous, except for 123[f().a] case
    Diag(RHSExp->getBeginLoc(), diag::ext_subscript_non_lvalue)
        << RHSExp->getSourceRange();
    RHSExp = ImpCastExprToType(RHSExp, Context.getArrayDecayedType(RHSTy),
                               CK_ArrayToPointerDecay).get();
    RHSTy = RHSExp->getType();

    BaseExpr = RHSExp;
    IndexExpr = LHSExp;
    ResultType = RHSTy->castAs<PointerType>()->getPointeeType();
  } else {
    return ExprError(Diag(LLoc, diag::err_typecheck_subscript_value)
       << LHSExp->getSourceRange() << RHSExp->getSourceRange());
  }
  // C99 6.5.2.1p1
  if (!IndexExpr->getType()->isIntegerType() && !IndexExpr->isTypeDependent())
    return ExprError(Diag(LLoc, diag::err_typecheck_subscript_not_integer)
                     << IndexExpr->getSourceRange());

  if ((IndexExpr->getType()->isSpecificBuiltinType(BuiltinType::Char_S) ||
       IndexExpr->getType()->isSpecificBuiltinType(BuiltinType::Char_U))
         && !IndexExpr->isTypeDependent())
    Diag(LLoc, diag::warn_subscript_is_char) << IndexExpr->getSourceRange();

  // C99 6.5.2.1p1: "shall have type "pointer to *object* type". Similarly,
  // C++ [expr.sub]p1: The type "T" shall be a completely-defined object
  // type. Note that Functions are not objects, and that (in C99 parlance)
  // incomplete types are not object types.
  if (ResultType->isFunctionType()) {
    Diag(BaseExpr->getBeginLoc(), diag::err_subscript_function_type)
        << ResultType << BaseExpr->getSourceRange();
    return ExprError();
  }

  if (ResultType->isVoidType() && !getLangOpts().CPlusPlus) {
    // GNU extension: subscripting on pointer to void
    Diag(LLoc, diag::ext_gnu_subscript_void_type)
      << BaseExpr->getSourceRange();

    // C forbids expressions of unqualified void type from being l-values.
    // See IsCForbiddenLValueType.
    if (!ResultType.hasQualifiers())
      VK = VK_PRValue;
  } else if (!ResultType->isDependentType() &&
             !ResultType.isWebAssemblyReferenceType() &&
             RequireCompleteSizedType(
                 LLoc, ResultType,
                 diag::err_subscript_incomplete_or_sizeless_type, BaseExpr))
    return ExprError();

  assert(VK == VK_PRValue || LangOpts.CPlusPlus ||
         !ResultType.isCForbiddenLValueType());

  if (LHSExp->IgnoreParenImpCasts()->getType()->isVariablyModifiedType() &&
      FunctionScopes.size() > 1) {
    if (auto *TT =
            LHSExp->IgnoreParenImpCasts()->getType()->getAs<TypedefType>()) {
      for (auto I = FunctionScopes.rbegin(),
                E = std::prev(FunctionScopes.rend());
           I != E; ++I) {
        auto *CSI = dyn_cast<CapturingScopeInfo>(*I);
        if (CSI == nullptr)
          break;
        DeclContext *DC = nullptr;
        if (auto *LSI = dyn_cast<LambdaScopeInfo>(CSI))
          DC = LSI->CallOperator;
        else if (auto *CRSI = dyn_cast<CapturedRegionScopeInfo>(CSI))
          DC = CRSI->TheCapturedDecl;
        else if (auto *BSI = dyn_cast<BlockScopeInfo>(CSI))
          DC = BSI->TheDecl;
        if (DC) {
          if (DC->containsDecl(TT->getDecl()))
            break;
          captureVariablyModifiedType(
              Context, LHSExp->IgnoreParenImpCasts()->getType(), CSI);
        }
      }
    }
  }

  return new (Context)
      ArraySubscriptExpr(LHSExp, RHSExp, ResultType, VK, OK, RLoc);
}

bool Sema::CheckCXXDefaultArgExpr(SourceLocation CallLoc, FunctionDecl *FD,
                                  ParmVarDecl *Param, Expr *RewrittenInit,
                                  bool SkipImmediateInvocations) {
  if (Param->hasUnparsedDefaultArg()) {
    assert(!RewrittenInit && "Should not have a rewritten init expression yet");
    // If we've already cleared out the location for the default argument,
    // that means we're parsing it right now.
    if (!UnparsedDefaultArgLocs.count(Param)) {
      Diag(Param->getBeginLoc(), diag::err_recursive_default_argument) << FD;
      Diag(CallLoc, diag::note_recursive_default_argument_used_here);
      Param->setInvalidDecl();
      return true;
    }

    Diag(CallLoc, diag::err_use_of_default_argument_to_function_declared_later)
        << FD << cast<CXXRecordDecl>(FD->getDeclContext());
    Diag(UnparsedDefaultArgLocs[Param],
         diag::note_default_argument_declared_here);
    return true;
  }

  if (Param->hasUninstantiatedDefaultArg()) {
    assert(!RewrittenInit && "Should not have a rewitten init expression yet");
    if (InstantiateDefaultArgument(CallLoc, FD, Param))
      return true;
  }

  Expr *Init = RewrittenInit ? RewrittenInit : Param->getInit();
  assert(Init && "default argument but no initializer?");

  // If the default expression creates temporaries, we need to
  // push them to the current stack of expression temporaries so they'll
  // be properly destroyed.
  // FIXME: We should really be rebuilding the default argument with new
  // bound temporaries; see the comment in PR5810.
  // We don't need to do that with block decls, though, because
  // blocks in default argument expression can never capture anything.
  if (auto *InitWithCleanup = dyn_cast<ExprWithCleanups>(Init)) {
    // Set the "needs cleanups" bit regardless of whether there are
    // any explicit objects.
    Cleanup.setExprNeedsCleanups(InitWithCleanup->cleanupsHaveSideEffects());
    // Append all the objects to the cleanup list.  Right now, this
    // should always be a no-op, because blocks in default argument
    // expressions should never be able to capture anything.
    assert(!InitWithCleanup->getNumObjects() &&
           "default argument expression has capturing blocks?");
  }
  // C++ [expr.const]p15.1:
  //   An expression or conversion is in an immediate function context if it is
  //   potentially evaluated and [...] its innermost enclosing non-block scope
  //   is a function parameter scope of an immediate function.
  EnterExpressionEvaluationContext EvalContext(
      *this,
      FD->isImmediateFunction()
          ? ExpressionEvaluationContext::ImmediateFunctionContext
          : ExpressionEvaluationContext::PotentiallyEvaluated,
      Param);
  ExprEvalContexts.back().IsCurrentlyCheckingDefaultArgumentOrInitializer =
      SkipImmediateInvocations;
  runWithSufficientStackSpace(CallLoc, [&] {
    MarkDeclarationsReferencedInExpr(Init, /*SkipLocalVariables=*/true);
  });
  return false;
}

struct ImmediateCallVisitor : public RecursiveASTVisitor<ImmediateCallVisitor> {
  const ASTContext &Context;
  ImmediateCallVisitor(const ASTContext &Ctx) : Context(Ctx) {}

  bool HasImmediateCalls = false;
  bool shouldVisitImplicitCode() const { return true; }

  bool VisitCallExpr(CallExpr *E) {
    if (const FunctionDecl *FD = E->getDirectCallee())
      HasImmediateCalls |= FD->isImmediateFunction();
    return RecursiveASTVisitor<ImmediateCallVisitor>::VisitStmt(E);
  }

  // SourceLocExpr are not immediate invocations
  // but CXXDefaultInitExpr/CXXDefaultArgExpr containing a SourceLocExpr
  // need to be rebuilt so that they refer to the correct SourceLocation and
  // DeclContext.
  bool VisitSourceLocExpr(SourceLocExpr *E) {
    HasImmediateCalls = true;
    return RecursiveASTVisitor<ImmediateCallVisitor>::VisitStmt(E);
  }

  // A nested lambda might have parameters with immediate invocations
  // in their default arguments.
  // The compound statement is not visited (as it does not constitute a
  // subexpression).
  // FIXME: We should consider visiting and transforming captures
  // with init expressions.
  bool VisitLambdaExpr(LambdaExpr *E) {
    return VisitCXXMethodDecl(E->getCallOperator());
  }

  // Blocks don't support default parameters, and, as for lambdas,
  // we don't consider their body a subexpression.
  bool VisitBlockDecl(BlockDecl *B) { return false; }

  bool VisitCompoundStmt(CompoundStmt *B) { return false; }

  bool VisitCXXDefaultArgExpr(CXXDefaultArgExpr *E) {
    return TraverseStmt(E->getExpr());
  }

  bool VisitCXXDefaultInitExpr(CXXDefaultInitExpr *E) {
    return TraverseStmt(E->getExpr());
  }
};

struct EnsureImmediateInvocationInDefaultArgs
    : TreeTransform<EnsureImmediateInvocationInDefaultArgs> {
  EnsureImmediateInvocationInDefaultArgs(Sema &SemaRef)
      : TreeTransform(SemaRef) {}

  // Lambda can only have immediate invocations in the default
  // args of their parameters, which is transformed upon calling the closure.
  // The body is not a subexpression, so we have nothing to do.
  // FIXME: Immediate calls in capture initializers should be transformed.
  ExprResult TransformLambdaExpr(LambdaExpr *E) { return E; }
  ExprResult TransformBlockExpr(BlockExpr *E) { return E; }

  // Make sure we don't rebuild the this pointer as it would
  // cause it to incorrectly point it to the outermost class
  // in the case of nested struct initialization.
  ExprResult TransformCXXThisExpr(CXXThisExpr *E) { return E; }
};

ExprResult Sema::BuildCXXDefaultArgExpr(SourceLocation CallLoc,
                                        FunctionDecl *FD, ParmVarDecl *Param,
                                        Expr *Init) {
  assert(Param->hasDefaultArg() && "can't build nonexistent default arg");

  bool NestedDefaultChecking = isCheckingDefaultArgumentOrInitializer();

  std::optional<ExpressionEvaluationContextRecord::InitializationContext>
      InitializationContext =
          OutermostDeclarationWithDelayedImmediateInvocations();
  if (!InitializationContext.has_value())
    InitializationContext.emplace(CallLoc, Param, CurContext);

  if (!Init && !Param->hasUnparsedDefaultArg()) {
    // Mark that we are replacing a default argument first.
    // If we are instantiating a template we won't have to
    // retransform immediate calls.
    // C++ [expr.const]p15.1:
    //   An expression or conversion is in an immediate function context if it
    //   is potentially evaluated and [...] its innermost enclosing non-block
    //   scope is a function parameter scope of an immediate function.
    EnterExpressionEvaluationContext EvalContext(
        *this,
        FD->isImmediateFunction()
            ? ExpressionEvaluationContext::ImmediateFunctionContext
            : ExpressionEvaluationContext::PotentiallyEvaluated,
        Param);

    if (Param->hasUninstantiatedDefaultArg()) {
      if (InstantiateDefaultArgument(CallLoc, FD, Param))
        return ExprError();
    }
    // CWG2631
    // An immediate invocation that is not evaluated where it appears is
    // evaluated and checked for whether it is a constant expression at the
    // point where the enclosing initializer is used in a function call.
    ImmediateCallVisitor V(getASTContext());
    if (!NestedDefaultChecking)
      V.TraverseDecl(Param);
    if (V.HasImmediateCalls) {
      ExprEvalContexts.back().DelayedDefaultInitializationContext = {
          CallLoc, Param, CurContext};
      EnsureImmediateInvocationInDefaultArgs Immediate(*this);
      ExprResult Res;
      runWithSufficientStackSpace(CallLoc, [&] {
        Res = Immediate.TransformInitializer(Param->getInit(),
                                             /*NotCopy=*/false);
      });
      if (Res.isInvalid())
        return ExprError();
      Res = ConvertParamDefaultArgument(Param, Res.get(),
                                        Res.get()->getBeginLoc());
      if (Res.isInvalid())
        return ExprError();
      Init = Res.get();
    }
  }

  if (CheckCXXDefaultArgExpr(
          CallLoc, FD, Param, Init,
          /*SkipImmediateInvocations=*/NestedDefaultChecking))
    return ExprError();

  return CXXDefaultArgExpr::Create(Context, InitializationContext->Loc, Param,
                                   Init, InitializationContext->Context);
}

ExprResult Sema::BuildCXXDefaultInitExpr(SourceLocation Loc, FieldDecl *Field) {
  assert(Field->hasInClassInitializer());

  // If we might have already tried and failed to instantiate, don't try again.
  if (Field->isInvalidDecl())
    return ExprError();

  CXXThisScopeRAII This(*this, Field->getParent(), Qualifiers());

  auto *ParentRD = cast<CXXRecordDecl>(Field->getParent());

  std::optional<ExpressionEvaluationContextRecord::InitializationContext>
      InitializationContext =
          OutermostDeclarationWithDelayedImmediateInvocations();
  if (!InitializationContext.has_value())
    InitializationContext.emplace(Loc, Field, CurContext);

  Expr *Init = nullptr;

  bool NestedDefaultChecking = isCheckingDefaultArgumentOrInitializer();

  EnterExpressionEvaluationContext EvalContext(
      *this, ExpressionEvaluationContext::PotentiallyEvaluated, Field);

  if (!Field->getInClassInitializer()) {
    // Maybe we haven't instantiated the in-class initializer. Go check the
    // pattern FieldDecl to see if it has one.
    if (isTemplateInstantiation(ParentRD->getTemplateSpecializationKind())) {
      CXXRecordDecl *ClassPattern = ParentRD->getTemplateInstantiationPattern();
      DeclContext::lookup_result Lookup =
          ClassPattern->lookup(Field->getDeclName());

      FieldDecl *Pattern = nullptr;
      for (auto *L : Lookup) {
        if ((Pattern = dyn_cast<FieldDecl>(L)))
          break;
      }
      assert(Pattern && "We must have set the Pattern!");
      if (!Pattern->hasInClassInitializer() ||
          InstantiateInClassInitializer(Loc, Field, Pattern,
                                        getTemplateInstantiationArgs(Field))) {
        Field->setInvalidDecl();
        return ExprError();
      }
    }
  }

  // CWG2631
  // An immediate invocation that is not evaluated where it appears is
  // evaluated and checked for whether it is a constant expression at the
  // point where the enclosing initializer is used in a [...] a constructor
  // definition, or an aggregate initialization.
  ImmediateCallVisitor V(getASTContext());
  if (!NestedDefaultChecking)
    V.TraverseDecl(Field);
  if (V.HasImmediateCalls) {
    ExprEvalContexts.back().DelayedDefaultInitializationContext = {Loc, Field,
                                                                   CurContext};
    ExprEvalContexts.back().IsCurrentlyCheckingDefaultArgumentOrInitializer =
        NestedDefaultChecking;

    EnsureImmediateInvocationInDefaultArgs Immediate(*this);
    ExprResult Res;
    runWithSufficientStackSpace(Loc, [&] {
      Res = Immediate.TransformInitializer(Field->getInClassInitializer(),
                                           /*CXXDirectInit=*/false);
    });
    if (!Res.isInvalid())
      Res = ConvertMemberDefaultInitExpression(Field, Res.get(), Loc);
    if (Res.isInvalid()) {
      Field->setInvalidDecl();
      return ExprError();
    }
    Init = Res.get();
  }

  if (Field->getInClassInitializer()) {
    Expr *E = Init ? Init : Field->getInClassInitializer();
    if (!NestedDefaultChecking)
      runWithSufficientStackSpace(Loc, [&] {
        MarkDeclarationsReferencedInExpr(E, /*SkipLocalVariables=*/false);
      });
    // C++11 [class.base.init]p7:
    //   The initialization of each base and member constitutes a
    //   full-expression.
    ExprResult Res = ActOnFinishFullExpr(E, /*DiscardedValue=*/false);
    if (Res.isInvalid()) {
      Field->setInvalidDecl();
      return ExprError();
    }
    Init = Res.get();

    return CXXDefaultInitExpr::Create(Context, InitializationContext->Loc,
                                      Field, InitializationContext->Context,
                                      Init);
  }

  // DR1351:
  //   If the brace-or-equal-initializer of a non-static data member
  //   invokes a defaulted default constructor of its class or of an
  //   enclosing class in a potentially evaluated subexpression, the
  //   program is ill-formed.
  //
  // This resolution is unworkable: the exception specification of the
  // default constructor can be needed in an unevaluated context, in
  // particular, in the operand of a noexcept-expression, and we can be
  // unable to compute an exception specification for an enclosed class.
  //
  // Any attempt to resolve the exception specification of a defaulted default
  // constructor before the initializer is lexically complete will ultimately
  // come here at which point we can diagnose it.
  RecordDecl *OutermostClass = ParentRD->getOuterLexicalRecordContext();
  Diag(Loc, diag::err_default_member_initializer_not_yet_parsed)
      << OutermostClass << Field;
  Diag(Field->getEndLoc(),
       diag::note_default_member_initializer_not_yet_parsed);
  // Recover by marking the field invalid, unless we're in a SFINAE context.
  if (!isSFINAEContext())
    Field->setInvalidDecl();
  return ExprError();
}

Sema::VariadicCallType
Sema::getVariadicCallType(FunctionDecl *FDecl, const FunctionProtoType *Proto,
                          Expr *Fn) {
  if (Proto && Proto->isVariadic()) {
    if (isa_and_nonnull<CXXConstructorDecl>(FDecl))
      return VariadicConstructor;
    else if (Fn && Fn->getType()->isBlockPointerType())
      return VariadicBlock;
    else if (FDecl) {
      if (CXXMethodDecl *Method = dyn_cast_or_null<CXXMethodDecl>(FDecl))
        if (Method->isInstance())
          return VariadicMethod;
    } else if (Fn && Fn->getType() == Context.BoundMemberTy)
      return VariadicMethod;
    return VariadicFunction;
  }
  return VariadicDoesNotApply;
}

namespace {
class FunctionCallCCC final : public FunctionCallFilterCCC {
public:
  FunctionCallCCC(Sema &SemaRef, const IdentifierInfo *FuncName,
                  unsigned NumArgs, MemberExpr *ME)
      : FunctionCallFilterCCC(SemaRef, NumArgs, false, ME),
        FunctionName(FuncName) {}

  bool ValidateCandidate(const TypoCorrection &candidate) override {
    if (!candidate.getCorrectionSpecifier() ||
        candidate.getCorrectionAsIdentifierInfo() != FunctionName) {
      return false;
    }

    return FunctionCallFilterCCC::ValidateCandidate(candidate);
  }

  std::unique_ptr<CorrectionCandidateCallback> clone() override {
    return std::make_unique<FunctionCallCCC>(*this);
  }

private:
  const IdentifierInfo *const FunctionName;
};
}

static TypoCorrection TryTypoCorrectionForCall(Sema &S, Expr *Fn,
                                               FunctionDecl *FDecl,
                                               ArrayRef<Expr *> Args) {
  MemberExpr *ME = dyn_cast<MemberExpr>(Fn);
  DeclarationName FuncName = FDecl->getDeclName();
  SourceLocation NameLoc = ME ? ME->getMemberLoc() : Fn->getBeginLoc();

  FunctionCallCCC CCC(S, FuncName.getAsIdentifierInfo(), Args.size(), ME);
  if (TypoCorrection Corrected = S.CorrectTypo(
          DeclarationNameInfo(FuncName, NameLoc), Sema::LookupOrdinaryName,
          S.getScopeForContext(S.CurContext), nullptr, CCC,
          Sema::CTK_ErrorRecovery)) {
    if (NamedDecl *ND = Corrected.getFoundDecl()) {
      if (Corrected.isOverloaded()) {
        OverloadCandidateSet OCS(NameLoc, OverloadCandidateSet::CSK_Normal);
        OverloadCandidateSet::iterator Best;
        for (NamedDecl *CD : Corrected) {
          if (FunctionDecl *FD = dyn_cast<FunctionDecl>(CD))
            S.AddOverloadCandidate(FD, DeclAccessPair::make(FD, AS_none), Args,
                                   OCS);
        }
        switch (OCS.BestViableFunction(S, NameLoc, Best)) {
        case OR_Success:
          ND = Best->FoundDecl;
          Corrected.setCorrectionDecl(ND);
          break;
        default:
          break;
        }
      }
      ND = ND->getUnderlyingDecl();
      if (isa<ValueDecl>(ND) || isa<FunctionTemplateDecl>(ND))
        return Corrected;
    }
  }
  return TypoCorrection();
}

/// ConvertArgumentsForCall - Converts the arguments specified in
/// Args/NumArgs to the parameter types of the function FDecl with
/// function prototype Proto. Call is the call expression itself, and
/// Fn is the function expression. For a C++ member function, this
/// routine does not attempt to convert the object argument. Returns
/// true if the call is ill-formed.
bool
Sema::ConvertArgumentsForCall(CallExpr *Call, Expr *Fn,
                              FunctionDecl *FDecl,
                              const FunctionProtoType *Proto,
                              ArrayRef<Expr *> Args,
                              SourceLocation RParenLoc,
                              bool IsExecConfig) {
  // Bail out early if calling a builtin with custom typechecking.
  if (FDecl)
    if (unsigned ID = FDecl->getBuiltinID())
      if (Context.BuiltinInfo.hasCustomTypechecking(ID))
        return false;

  // C99 6.5.2.2p7 - the arguments are implicitly converted, as if by
  // assignment, to the types of the corresponding parameter, ...
  unsigned NumParams = Proto->getNumParams();
  bool Invalid = false;
  unsigned MinArgs = FDecl ? FDecl->getMinRequiredArguments() : NumParams;
  unsigned FnKind = Fn->getType()->isBlockPointerType()
                       ? 1 /* block */
                       : (IsExecConfig ? 3 /* kernel function (exec config) */
                                       : 0 /* function */);

  // If too few arguments are available (and we don't have default
  // arguments for the remaining parameters), don't make the call.
  if (Args.size() < NumParams) {
    if (Args.size() < MinArgs) {
      TypoCorrection TC;
      if (FDecl && (TC = TryTypoCorrectionForCall(*this, Fn, FDecl, Args))) {
        unsigned diag_id =
            MinArgs == NumParams && !Proto->isVariadic()
                ? diag::err_typecheck_call_too_few_args_suggest
                : diag::err_typecheck_call_too_few_args_at_least_suggest;
        diagnoseTypo(TC, PDiag(diag_id) << FnKind << MinArgs
                                        << static_cast<unsigned>(Args.size())
                                        << TC.getCorrectionRange());
      } else if (MinArgs == 1 && FDecl && FDecl->getParamDecl(0)->getDeclName())
        Diag(RParenLoc,
             MinArgs == NumParams && !Proto->isVariadic()
                 ? diag::err_typecheck_call_too_few_args_one
                 : diag::err_typecheck_call_too_few_args_at_least_one)
            << FnKind << FDecl->getParamDecl(0) << Fn->getSourceRange();
      else
        Diag(RParenLoc, MinArgs == NumParams && !Proto->isVariadic()
                            ? diag::err_typecheck_call_too_few_args
                            : diag::err_typecheck_call_too_few_args_at_least)
            << FnKind << MinArgs << static_cast<unsigned>(Args.size())
            << Fn->getSourceRange();

      // Emit the location of the prototype.
      if (!TC && FDecl && !FDecl->getBuiltinID() && !IsExecConfig)
        Diag(FDecl->getLocation(), diag::note_callee_decl)
            << FDecl << FDecl->getParametersSourceRange();

      return true;
    }
    // We reserve space for the default arguments when we create
    // the call expression, before calling ConvertArgumentsForCall.
    assert((Call->getNumArgs() == NumParams) &&
           "We should have reserved space for the default arguments before!");
  }

  // If too many are passed and not variadic, error on the extras and drop
  // them.
  if (Args.size() > NumParams) {
    if (!Proto->isVariadic()) {
      TypoCorrection TC;
      if (FDecl && (TC = TryTypoCorrectionForCall(*this, Fn, FDecl, Args))) {
        unsigned diag_id =
            MinArgs == NumParams && !Proto->isVariadic()
                ? diag::err_typecheck_call_too_many_args_suggest
                : diag::err_typecheck_call_too_many_args_at_most_suggest;
        diagnoseTypo(TC, PDiag(diag_id) << FnKind << NumParams
                                        << static_cast<unsigned>(Args.size())
                                        << TC.getCorrectionRange());
      } else if (NumParams == 1 && FDecl &&
                 FDecl->getParamDecl(0)->getDeclName())
        Diag(Args[NumParams]->getBeginLoc(),
             MinArgs == NumParams
                 ? diag::err_typecheck_call_too_many_args_one
                 : diag::err_typecheck_call_too_many_args_at_most_one)
            << FnKind << FDecl->getParamDecl(0)
            << static_cast<unsigned>(Args.size()) << Fn->getSourceRange()
            << SourceRange(Args[NumParams]->getBeginLoc(),
                           Args.back()->getEndLoc());
      else
        Diag(Args[NumParams]->getBeginLoc(),
             MinArgs == NumParams
                 ? diag::err_typecheck_call_too_many_args
                 : diag::err_typecheck_call_too_many_args_at_most)
            << FnKind << NumParams << static_cast<unsigned>(Args.size())
            << Fn->getSourceRange()
            << SourceRange(Args[NumParams]->getBeginLoc(),
                           Args.back()->getEndLoc());

      // Emit the location of the prototype.
      if (!TC && FDecl && !FDecl->getBuiltinID() && !IsExecConfig)
        Diag(FDecl->getLocation(), diag::note_callee_decl)
            << FDecl << FDecl->getParametersSourceRange();

      // This deletes the extra arguments.
      Call->shrinkNumArgs(NumParams);
      return true;
    }
  }
  SmallVector<Expr *, 8> AllArgs;
  VariadicCallType CallType = getVariadicCallType(FDecl, Proto, Fn);

  Invalid = GatherArgumentsForCall(Call->getBeginLoc(), FDecl, Proto, 0, Args,
                                   AllArgs, CallType);
  if (Invalid)
    return true;
  unsigned TotalNumArgs = AllArgs.size();
  for (unsigned i = 0; i < TotalNumArgs; ++i)
    Call->setArg(i, AllArgs[i]);

  Call->computeDependence();
  return false;
}

bool Sema::GatherArgumentsForCall(SourceLocation CallLoc, FunctionDecl *FDecl,
                                  const FunctionProtoType *Proto,
                                  unsigned FirstParam, ArrayRef<Expr *> Args,
                                  SmallVectorImpl<Expr *> &AllArgs,
                                  VariadicCallType CallType, bool AllowExplicit,
                                  bool IsListInitialization) {
  unsigned NumParams = Proto->getNumParams();
  bool Invalid = false;
  size_t ArgIx = 0;
  // Continue to check argument types (even if we have too few/many args).
  for (unsigned i = FirstParam; i < NumParams; i++) {
    QualType ProtoArgType = Proto->getParamType(i);

    Expr *Arg;
    ParmVarDecl *Param = FDecl ? FDecl->getParamDecl(i) : nullptr;
    if (ArgIx < Args.size()) {
      Arg = Args[ArgIx++];

      if (RequireCompleteType(Arg->getBeginLoc(), ProtoArgType,
                              diag::err_call_incomplete_argument, Arg))
        return true;

      // Strip the unbridged-cast placeholder expression off, if applicable.
      bool CFAudited = false;
      if (Arg->getType() == Context.ARCUnbridgedCastTy &&
          FDecl && FDecl->hasAttr<CFAuditedTransferAttr>() &&
          (!Param || !Param->hasAttr<CFConsumedAttr>()))
        Arg = stripARCUnbridgedCast(Arg);
      else if (getLangOpts().ObjCAutoRefCount &&
               FDecl && FDecl->hasAttr<CFAuditedTransferAttr>() &&
               (!Param || !Param->hasAttr<CFConsumedAttr>()))
        CFAudited = true;

      if (Proto->getExtParameterInfo(i).isNoEscape() &&
          ProtoArgType->isBlockPointerType())
        if (auto *BE = dyn_cast<BlockExpr>(Arg->IgnoreParenNoopCasts(Context)))
          BE->getBlockDecl()->setDoesNotEscape();

      InitializedEntity Entity =
          Param ? InitializedEntity::InitializeParameter(Context, Param,
                                                         ProtoArgType)
                : InitializedEntity::InitializeParameter(
                      Context, ProtoArgType, Proto->isParamConsumed(i));

      // Remember that parameter belongs to a CF audited API.
      if (CFAudited)
        Entity.setParameterCFAudited();

      ExprResult ArgE = PerformCopyInitialization(
          Entity, SourceLocation(), Arg, IsListInitialization, AllowExplicit);
      if (ArgE.isInvalid())
        return true;

      Arg = ArgE.getAs<Expr>();
    } else {
      assert(Param && "can't use default arguments without a known callee");

      ExprResult ArgExpr = BuildCXXDefaultArgExpr(CallLoc, FDecl, Param);
      if (ArgExpr.isInvalid())
        return true;

      Arg = ArgExpr.getAs<Expr>();
    }

    // Check for array bounds violations for each argument to the call. This
    // check only triggers warnings when the argument isn't a more complex Expr
    // with its own checking, such as a BinaryOperator.
    CheckArrayAccess(Arg);

    // Check for violations of C99 static array rules (C99 6.7.5.3p7).
    CheckStaticArrayArgument(CallLoc, Param, Arg);

    AllArgs.push_back(Arg);
  }

  // If this is a variadic call, handle args passed through "...".
  if (CallType != VariadicDoesNotApply) {
    // Assume that extern "C" functions with variadic arguments that
    // return __unknown_anytype aren't *really* variadic.
    if (Proto->getReturnType() == Context.UnknownAnyTy && FDecl &&
        FDecl->isExternC()) {
      for (Expr *A : Args.slice(ArgIx)) {
        QualType paramType; // ignored
        ExprResult arg = checkUnknownAnyArg(CallLoc, A, paramType);
        Invalid |= arg.isInvalid();
        AllArgs.push_back(arg.get());
      }

    // Otherwise do argument promotion, (C99 6.5.2.2p7).
    } else {
      for (Expr *A : Args.slice(ArgIx)) {
        ExprResult Arg = DefaultVariadicArgumentPromotion(A, CallType, FDecl);
        Invalid |= Arg.isInvalid();
        AllArgs.push_back(Arg.get());
      }
    }

    // Check for array bounds violations.
    for (Expr *A : Args.slice(ArgIx))
      CheckArrayAccess(A);
  }
  return Invalid;
}

static void DiagnoseCalleeStaticArrayParam(Sema &S, ParmVarDecl *PVD) {
  TypeLoc TL = PVD->getTypeSourceInfo()->getTypeLoc();
  if (DecayedTypeLoc DTL = TL.getAs<DecayedTypeLoc>())
    TL = DTL.getOriginalLoc();
  if (ArrayTypeLoc ATL = TL.getAs<ArrayTypeLoc>())
    S.Diag(PVD->getLocation(), diag::note_callee_static_array)
      << ATL.getLocalSourceRange();
}

/// CheckStaticArrayArgument - If the given argument corresponds to a static
/// array parameter, check that it is non-null, and that if it is formed by
/// array-to-pointer decay, the underlying array is sufficiently large.
///
/// C99 6.7.5.3p7: If the keyword static also appears within the [ and ] of the
/// array type derivation, then for each call to the function, the value of the
/// corresponding actual argument shall provide access to the first element of
/// an array with at least as many elements as specified by the size expression.
void
Sema::CheckStaticArrayArgument(SourceLocation CallLoc,
                               ParmVarDecl *Param,
                               const Expr *ArgExpr) {
  // Static array parameters are not supported in C++.
  if (!Param || getLangOpts().CPlusPlus)
    return;

  QualType OrigTy = Param->getOriginalType();

  const ArrayType *AT = Context.getAsArrayType(OrigTy);
  if (!AT || AT->getSizeModifier() != ArrayType::Static)
    return;

  if (ArgExpr->isNullPointerConstant(Context,
                                     Expr::NPC_NeverValueDependent)) {
    Diag(CallLoc, diag::warn_null_arg) << ArgExpr->getSourceRange();
    DiagnoseCalleeStaticArrayParam(*this, Param);
    return;
  }

  const ConstantArrayType *CAT = dyn_cast<ConstantArrayType>(AT);
  if (!CAT)
    return;

  const ConstantArrayType *ArgCAT =
    Context.getAsConstantArrayType(ArgExpr->IgnoreParenCasts()->getType());
  if (!ArgCAT)
    return;

  if (getASTContext().hasSameUnqualifiedType(CAT->getElementType(),
                                             ArgCAT->getElementType())) {
    if (ArgCAT->getSize().ult(CAT->getSize())) {
      Diag(CallLoc, diag::warn_static_array_too_small)
          << ArgExpr->getSourceRange()
          << (unsigned)ArgCAT->getSize().getZExtValue()
          << (unsigned)CAT->getSize().getZExtValue() << 0;
      DiagnoseCalleeStaticArrayParam(*this, Param);
    }
    return;
  }

  std::optional<CharUnits> ArgSize =
      getASTContext().getTypeSizeInCharsIfKnown(ArgCAT);
  std::optional<CharUnits> ParmSize =
      getASTContext().getTypeSizeInCharsIfKnown(CAT);
  if (ArgSize && ParmSize && *ArgSize < *ParmSize) {
    Diag(CallLoc, diag::warn_static_array_too_small)
        << ArgExpr->getSourceRange() << (unsigned)ArgSize->getQuantity()
        << (unsigned)ParmSize->getQuantity() << 1;
    DiagnoseCalleeStaticArrayParam(*this, Param);
  }
}

/// Given a function expression of unknown-any type, try to rebuild it
/// to have a function type.
static ExprResult rebuildUnknownAnyFunction(Sema &S, Expr *fn);

/// Is the given type a placeholder that we need to lower out
/// immediately during argument processing?
static bool isPlaceholderToRemoveAsArg(QualType type) {
  // Placeholders are never sugared.
  const BuiltinType *placeholder = dyn_cast<BuiltinType>(type);
  if (!placeholder) return false;

  switch (placeholder->getKind()) {
  // Ignore all the non-placeholder types.
#define IMAGE_TYPE(ImgType, Id, SingletonId, Access, Suffix) \
  case BuiltinType::Id:
#include "clang/Basic/OpenCLImageTypes.def"
#define EXT_OPAQUE_TYPE(ExtType, Id, Ext) \
  case BuiltinType::Id:
#include "clang/Basic/OpenCLExtensionTypes.def"
  // In practice we'll never use this, since all SVE types are sugared
  // via TypedefTypes rather than exposed directly as BuiltinTypes.
#define SVE_TYPE(Name, Id, SingletonId) \
  case BuiltinType::Id:
#include "clang/Basic/AArch64SVEACLETypes.def"
#define PPC_VECTOR_TYPE(Name, Id, Size) \
  case BuiltinType::Id:
#include "clang/Basic/PPCTypes.def"
#define RVV_TYPE(Name, Id, SingletonId) case BuiltinType::Id:
#include "clang/Basic/RISCVVTypes.def"
#define WASM_TYPE(Name, Id, SingletonId) case BuiltinType::Id:
#include "clang/Basic/WebAssemblyReferenceTypes.def"
#define PLACEHOLDER_TYPE(ID, SINGLETON_ID)
#define BUILTIN_TYPE(ID, SINGLETON_ID) case BuiltinType::ID:
#include "clang/AST/BuiltinTypes.def"
    return false;

  // We cannot lower out overload sets; they might validly be resolved
  // by the call machinery.
  case BuiltinType::Overload:
    return false;

  // Unbridged casts in ARC can be handled in some call positions and
  // should be left in place.
  case BuiltinType::ARCUnbridgedCast:
    return false;

  // Pseudo-objects should be converted as soon as possible.
  case BuiltinType::PseudoObject:
    return true;

  // The debugger mode could theoretically but currently does not try
  // to resolve unknown-typed arguments based on known parameter types.
  case BuiltinType::UnknownAny:
    return true;

  // These are always invalid as call arguments and should be reported.
  case BuiltinType::BoundMember:
  case BuiltinType::BuiltinFn:
  case BuiltinType::IncompleteMatrixIdx:
  case BuiltinType::OMPArraySection:
  case BuiltinType::OMPArrayShaping:
  case BuiltinType::OMPIterator:
    return true;

  }
  llvm_unreachable("bad builtin type kind");
}

/// Check an argument list for placeholders that we won't try to
/// handle later.
static bool checkArgsForPlaceholders(Sema &S, MultiExprArg args) {
  // Apply this processing to all the arguments at once instead of
  // dying at the first failure.
  bool hasInvalid = false;
  for (size_t i = 0, e = args.size(); i != e; i++) {
    if (isPlaceholderToRemoveAsArg(args[i]->getType())) {
      ExprResult result = S.CheckPlaceholderExpr(args[i]);
      if (result.isInvalid()) hasInvalid = true;
      else args[i] = result.get();
    }
  }
  return hasInvalid;
}

/// If a builtin function has a pointer argument with no explicit address
/// space, then it should be able to accept a pointer to any address
/// space as input.  In order to do this, we need to replace the
/// standard builtin declaration with one that uses the same address space
/// as the call.
///
/// \returns nullptr If this builtin is not a candidate for a rewrite i.e.
///                  it does not contain any pointer arguments without
///                  an address space qualifer.  Otherwise the rewritten
///                  FunctionDecl is returned.
/// TODO: Handle pointer return types.
static FunctionDecl *rewriteBuiltinFunctionDecl(Sema *Sema, ASTContext &Context,
                                                FunctionDecl *FDecl,
                                                MultiExprArg ArgExprs) {

  QualType DeclType = FDecl->getType();
  const FunctionProtoType *FT = dyn_cast<FunctionProtoType>(DeclType);

  if (!Context.BuiltinInfo.hasPtrArgsOrResult(FDecl->getBuiltinID()) || !FT ||
      ArgExprs.size() < FT->getNumParams())
    return nullptr;

  bool NeedsNewDecl = false;
  unsigned i = 0;
  SmallVector<QualType, 8> OverloadParams;

  for (QualType ParamType : FT->param_types()) {

    // Convert array arguments to pointer to simplify type lookup.
    ExprResult ArgRes =
        Sema->DefaultFunctionArrayLvalueConversion(ArgExprs[i++]);
    if (ArgRes.isInvalid())
      return nullptr;
    Expr *Arg = ArgRes.get();
    QualType ArgType = Arg->getType();
    if (!ParamType->isPointerType() || ParamType.hasAddressSpace() ||
        !ArgType->isPointerType() ||
        !ArgType->getPointeeType().hasAddressSpace() ||
        isPtrSizeAddressSpace(ArgType->getPointeeType().getAddressSpace())) {
      OverloadParams.push_back(ParamType);
      continue;
    }

    QualType PointeeType = ParamType->getPointeeType();
    if (PointeeType.hasAddressSpace())
      continue;

    NeedsNewDecl = true;
    LangAS AS = ArgType->getPointeeType().getAddressSpace();

    PointeeType = Context.getAddrSpaceQualType(PointeeType, AS);
    OverloadParams.push_back(Context.getPointerType(PointeeType));
  }

  if (!NeedsNewDecl)
    return nullptr;

  FunctionProtoType::ExtProtoInfo EPI;
  EPI.Variadic = FT->isVariadic();
  QualType OverloadTy = Context.getFunctionType(FT->getReturnType(),
                                                OverloadParams, EPI);
  DeclContext *Parent = FDecl->getParent();
  FunctionDecl *OverloadDecl = FunctionDecl::Create(
      Context, Parent, FDecl->getLocation(), FDecl->getLocation(),
      FDecl->getIdentifier(), OverloadTy,
      /*TInfo=*/nullptr, SC_Extern, Sema->getCurFPFeatures().isFPConstrained(),
      false,
      /*hasPrototype=*/true);
  SmallVector<ParmVarDecl*, 16> Params;
  FT = cast<FunctionProtoType>(OverloadTy);
  for (unsigned i = 0, e = FT->getNumParams(); i != e; ++i) {
    QualType ParamType = FT->getParamType(i);
    ParmVarDecl *Parm =
        ParmVarDecl::Create(Context, OverloadDecl, SourceLocation(),
                                SourceLocation(), nullptr, ParamType,
                                /*TInfo=*/nullptr, SC_None, nullptr);
    Parm->setScopeInfo(0, i);
    Params.push_back(Parm);
  }
  OverloadDecl->setParams(Params);
  Sema->mergeDeclAttributes(OverloadDecl, FDecl);
  return OverloadDecl;
}

static void checkDirectCallValidity(Sema &S, const Expr *Fn,
                                    FunctionDecl *Callee,
                                    MultiExprArg ArgExprs) {
  // `Callee` (when called with ArgExprs) may be ill-formed. enable_if (and
  // similar attributes) really don't like it when functions are called with an
  // invalid number of args.
  if (S.TooManyArguments(Callee->getNumParams(), ArgExprs.size(),
                         /*PartialOverloading=*/false) &&
      !Callee->isVariadic())
    return;
  if (Callee->getMinRequiredArguments() > ArgExprs.size())
    return;

  if (const EnableIfAttr *Attr =
          S.CheckEnableIf(Callee, Fn->getBeginLoc(), ArgExprs, true)) {
    S.Diag(Fn->getBeginLoc(),
           isa<CXXMethodDecl>(Callee)
               ? diag::err_ovl_no_viable_member_function_in_call
               : diag::err_ovl_no_viable_function_in_call)
        << Callee << Callee->getSourceRange();
    S.Diag(Callee->getLocation(),
           diag::note_ovl_candidate_disabled_by_function_cond_attr)
        << Attr->getCond()->getSourceRange() << Attr->getMessage();
    return;
  }
}

static bool enclosingClassIsRelatedToClassInWhichMembersWereFound(
    const UnresolvedMemberExpr *const UME, Sema &S) {

  const auto GetFunctionLevelDCIfCXXClass =
      [](Sema &S) -> const CXXRecordDecl * {
    const DeclContext *const DC = S.getFunctionLevelDeclContext();
    if (!DC || !DC->getParent())
      return nullptr;

    // If the call to some member function was made from within a member
    // function body 'M' return return 'M's parent.
    if (const auto *MD = dyn_cast<CXXMethodDecl>(DC))
      return MD->getParent()->getCanonicalDecl();
    // else the call was made from within a default member initializer of a
    // class, so return the class.
    if (const auto *RD = dyn_cast<CXXRecordDecl>(DC))
      return RD->getCanonicalDecl();
    return nullptr;
  };
  // If our DeclContext is neither a member function nor a class (in the
  // case of a lambda in a default member initializer), we can't have an
  // enclosing 'this'.

  const CXXRecordDecl *const CurParentClass = GetFunctionLevelDCIfCXXClass(S);
  if (!CurParentClass)
    return false;

  // The naming class for implicit member functions call is the class in which
  // name lookup starts.
  const CXXRecordDecl *const NamingClass =
      UME->getNamingClass()->getCanonicalDecl();
  assert(NamingClass && "Must have naming class even for implicit access");

  // If the unresolved member functions were found in a 'naming class' that is
  // related (either the same or derived from) to the class that contains the
  // member function that itself contained the implicit member access.

  return CurParentClass == NamingClass ||
         CurParentClass->isDerivedFrom(NamingClass);
}

static void
tryImplicitlyCaptureThisIfImplicitMemberFunctionAccessWithDependentArgs(
    Sema &S, const UnresolvedMemberExpr *const UME, SourceLocation CallLoc) {

  if (!UME)
    return;

  LambdaScopeInfo *const CurLSI = S.getCurLambda();
  // Only try and implicitly capture 'this' within a C++ Lambda if it hasn't
  // already been captured, or if this is an implicit member function call (if
  // it isn't, an attempt to capture 'this' should already have been made).
  if (!CurLSI || CurLSI->ImpCaptureStyle == CurLSI->ImpCap_None ||
      !UME->isImplicitAccess() || CurLSI->isCXXThisCaptured())
    return;

  // Check if the naming class in which the unresolved members were found is
  // related (same as or is a base of) to the enclosing class.

  if (!enclosingClassIsRelatedToClassInWhichMembersWereFound(UME, S))
    return;


  DeclContext *EnclosingFunctionCtx = S.CurContext->getParent()->getParent();
  // If the enclosing function is not dependent, then this lambda is
  // capture ready, so if we can capture this, do so.
  if (!EnclosingFunctionCtx->isDependentContext()) {
    // If the current lambda and all enclosing lambdas can capture 'this' -
    // then go ahead and capture 'this' (since our unresolved overload set
    // contains at least one non-static member function).
    if (!S.CheckCXXThisCapture(CallLoc, /*Explcit*/ false, /*Diagnose*/ false))
      S.CheckCXXThisCapture(CallLoc);
  } else if (S.CurContext->isDependentContext()) {
    // ... since this is an implicit member reference, that might potentially
    // involve a 'this' capture, mark 'this' for potential capture in
    // enclosing lambdas.
    if (CurLSI->ImpCaptureStyle != CurLSI->ImpCap_None)
      CurLSI->addPotentialThisCapture(CallLoc);
  }
}

// Once a call is fully resolved, warn for unqualified calls to specific
// C++ standard functions, like move and forward.
static void DiagnosedUnqualifiedCallsToStdFunctions(Sema &S, CallExpr *Call) {
  // We are only checking unary move and forward so exit early here.
  if (Call->getNumArgs() != 1)
    return;

  Expr *E = Call->getCallee()->IgnoreParenImpCasts();
  if (!E || isa<UnresolvedLookupExpr>(E))
    return;
  DeclRefExpr *DRE = dyn_cast_or_null<DeclRefExpr>(E);
  if (!DRE || !DRE->getLocation().isValid())
    return;

  if (DRE->getQualifier())
    return;

  const FunctionDecl *FD = Call->getDirectCallee();
  if (!FD)
    return;

  // Only warn for some functions deemed more frequent or problematic.
  unsigned BuiltinID = FD->getBuiltinID();
  if (BuiltinID != Builtin::BImove && BuiltinID != Builtin::BIforward)
    return;

  S.Diag(DRE->getLocation(), diag::warn_unqualified_call_to_std_cast_function)
      << FD->getQualifiedNameAsString()
      << FixItHint::CreateInsertion(DRE->getLocation(), "std::");
}

ExprResult Sema::ActOnCallExpr(Scope *Scope, Expr *Fn, SourceLocation LParenLoc,
                               MultiExprArg ArgExprs, SourceLocation RParenLoc,
                               Expr *ExecConfig) {
  ExprResult Call =
      BuildCallExpr(Scope, Fn, LParenLoc, ArgExprs, RParenLoc, ExecConfig,
                    /*IsExecConfig=*/false, /*AllowRecovery=*/true);
  if (Call.isInvalid())
    return Call;

  // Diagnose uses of the C++20 "ADL-only template-id call" feature in earlier
  // language modes.
  if (auto *ULE = dyn_cast<UnresolvedLookupExpr>(Fn)) {
    if (ULE->hasExplicitTemplateArgs() &&
        ULE->decls_begin() == ULE->decls_end()) {
      Diag(Fn->getExprLoc(), getLangOpts().CPlusPlus20
                                 ? diag::warn_cxx17_compat_adl_only_template_id
                                 : diag::ext_adl_only_template_id)
          << ULE->getName();
    }
  }

  if (LangOpts.OpenMP)
    Call = ActOnOpenMPCall(Call, Scope, LParenLoc, ArgExprs, RParenLoc,
                           ExecConfig);
  if (LangOpts.CPlusPlus) {
    CallExpr *CE = dyn_cast<CallExpr>(Call.get());
    if (CE)
      DiagnosedUnqualifiedCallsToStdFunctions(*this, CE);
  }
  return Call;
}

/// BuildCallExpr - Handle a call to Fn with the specified array of arguments.
/// This provides the location of the left/right parens and a list of comma
/// locations.
ExprResult Sema::BuildCallExpr(Scope *Scope, Expr *Fn, SourceLocation LParenLoc,
                               MultiExprArg ArgExprs, SourceLocation RParenLoc,
                               Expr *ExecConfig, bool IsExecConfig,
                               bool AllowRecovery) {
  // Since this might be a postfix expression, get rid of ParenListExprs.
  ExprResult Result = MaybeConvertParenListExprToParenExpr(Scope, Fn);
  if (Result.isInvalid()) return ExprError();
  Fn = Result.get();

  if (checkArgsForPlaceholders(*this, ArgExprs))
    return ExprError();

  if (getLangOpts().CPlusPlus) {
    // If this is a pseudo-destructor expression, build the call immediately.
    if (isa<CXXPseudoDestructorExpr>(Fn)) {
      if (!ArgExprs.empty()) {
        // Pseudo-destructor calls should not have any arguments.
        Diag(Fn->getBeginLoc(), diag::err_pseudo_dtor_call_with_args)
            << FixItHint::CreateRemoval(
                   SourceRange(ArgExprs.front()->getBeginLoc(),
                               ArgExprs.back()->getEndLoc()));
      }

      return CallExpr::Create(Context, Fn, /*Args=*/{}, Context.VoidTy,
                              VK_PRValue, RParenLoc, CurFPFeatureOverrides());
    }
    if (Fn->getType() == Context.PseudoObjectTy) {
      ExprResult result = CheckPlaceholderExpr(Fn);
      if (result.isInvalid()) return ExprError();
      Fn = result.get();
    }

    // Determine whether this is a dependent call inside a C++ template,
    // in which case we won't do any semantic analysis now.
    if (Fn->isTypeDependent() || Expr::hasAnyTypeDependentArguments(ArgExprs)) {
      if (ExecConfig) {
        return CUDAKernelCallExpr::Create(Context, Fn,
                                          cast<CallExpr>(ExecConfig), ArgExprs,
                                          Context.DependentTy, VK_PRValue,
                                          RParenLoc, CurFPFeatureOverrides());
      } else {

        tryImplicitlyCaptureThisIfImplicitMemberFunctionAccessWithDependentArgs(
            *this, dyn_cast<UnresolvedMemberExpr>(Fn->IgnoreParens()),
            Fn->getBeginLoc());

        return CallExpr::Create(Context, Fn, ArgExprs, Context.DependentTy,
                                VK_PRValue, RParenLoc, CurFPFeatureOverrides());
      }
    }

    // Determine whether this is a call to an object (C++ [over.call.object]).
    if (Fn->getType()->isRecordType())
      return BuildCallToObjectOfClassType(Scope, Fn, LParenLoc, ArgExprs,
                                          RParenLoc);

    if (Fn->getType() == Context.UnknownAnyTy) {
      ExprResult result = rebuildUnknownAnyFunction(*this, Fn);
      if (result.isInvalid()) return ExprError();
      Fn = result.get();
    }

    if (Fn->getType() == Context.BoundMemberTy) {
      return BuildCallToMemberFunction(Scope, Fn, LParenLoc, ArgExprs,
                                       RParenLoc, ExecConfig, IsExecConfig,
                                       AllowRecovery);
    }
  }

  // Check for overloaded calls.  This can happen even in C due to extensions.
  if (Fn->getType() == Context.OverloadTy) {
    OverloadExpr::FindResult find = OverloadExpr::find(Fn);

    // We aren't supposed to apply this logic if there's an '&' involved.
    if (!find.HasFormOfMemberPointer) {
      if (Expr::hasAnyTypeDependentArguments(ArgExprs))
        return CallExpr::Create(Context, Fn, ArgExprs, Context.DependentTy,
                                VK_PRValue, RParenLoc, CurFPFeatureOverrides());
      OverloadExpr *ovl = find.Expression;
      if (UnresolvedLookupExpr *ULE = dyn_cast<UnresolvedLookupExpr>(ovl))
        return BuildOverloadedCallExpr(
            Scope, Fn, ULE, LParenLoc, ArgExprs, RParenLoc, ExecConfig,
            /*AllowTypoCorrection=*/true, find.IsAddressOfOperand);
      return BuildCallToMemberFunction(Scope, Fn, LParenLoc, ArgExprs,
                                       RParenLoc, ExecConfig, IsExecConfig,
                                       AllowRecovery);
    }
  }

  // If we're directly calling a function, get the appropriate declaration.
  if (Fn->getType() == Context.UnknownAnyTy) {
    ExprResult result = rebuildUnknownAnyFunction(*this, Fn);
    if (result.isInvalid()) return ExprError();
    Fn = result.get();
  }

  Expr *NakedFn = Fn->IgnoreParens();

  bool CallingNDeclIndirectly = false;
  NamedDecl *NDecl = nullptr;
  if (UnaryOperator *UnOp = dyn_cast<UnaryOperator>(NakedFn)) {
    if (UnOp->getOpcode() == UO_AddrOf) {
      CallingNDeclIndirectly = true;
      NakedFn = UnOp->getSubExpr()->IgnoreParens();
    }
  }

  if (auto *DRE = dyn_cast<DeclRefExpr>(NakedFn)) {
    NDecl = DRE->getDecl();

    FunctionDecl *FDecl = dyn_cast<FunctionDecl>(NDecl);
    if (FDecl && FDecl->getBuiltinID()) {
      // Rewrite the function decl for this builtin by replacing parameters
      // with no explicit address space with the address space of the arguments
      // in ArgExprs.
      if ((FDecl =
               rewriteBuiltinFunctionDecl(this, Context, FDecl, ArgExprs))) {
        NDecl = FDecl;
        Fn = DeclRefExpr::Create(
            Context, FDecl->getQualifierLoc(), SourceLocation(), FDecl, false,
            SourceLocation(), FDecl->getType(), Fn->getValueKind(), FDecl,
            nullptr, DRE->isNonOdrUse());
      }
    }
  } else if (auto *ME = dyn_cast<MemberExpr>(NakedFn))
    NDecl = ME->getMemberDecl();

  if (FunctionDecl *FD = dyn_cast_or_null<FunctionDecl>(NDecl)) {
    if (CallingNDeclIndirectly && !checkAddressOfFunctionIsAvailable(
                                      FD, /*Complain=*/true, Fn->getBeginLoc()))
      return ExprError();

    checkDirectCallValidity(*this, Fn, FD, ArgExprs);

    // If this expression is a call to a builtin function in HIP device
    // compilation, allow a pointer-type argument to default address space to be
    // passed as a pointer-type parameter to a non-default address space.
    // If Arg is declared in the default address space and Param is declared
    // in a non-default address space, perform an implicit address space cast to
    // the parameter type.
    if (getLangOpts().HIP && getLangOpts().CUDAIsDevice && FD &&
        FD->getBuiltinID()) {
      for (unsigned Idx = 0; Idx < FD->param_size(); ++Idx) {
        ParmVarDecl *Param = FD->getParamDecl(Idx);
        if (!ArgExprs[Idx] || !Param || !Param->getType()->isPointerType() ||
            !ArgExprs[Idx]->getType()->isPointerType())
          continue;

        auto ParamAS = Param->getType()->getPointeeType().getAddressSpace();
        auto ArgTy = ArgExprs[Idx]->getType();
        auto ArgPtTy = ArgTy->getPointeeType();
        auto ArgAS = ArgPtTy.getAddressSpace();

        // Add address space cast if target address spaces are different
        bool NeedImplicitASC =
          ParamAS != LangAS::Default &&       // Pointer params in generic AS don't need special handling.
          ( ArgAS == LangAS::Default  ||      // We do allow implicit conversion from generic AS
                                              // or from specific AS which has target AS matching that of Param.
          getASTContext().getTargetAddressSpace(ArgAS) == getASTContext().getTargetAddressSpace(ParamAS));
        if (!NeedImplicitASC)
          continue;

        // First, ensure that the Arg is an RValue.
        if (ArgExprs[Idx]->isGLValue()) {
          ArgExprs[Idx] = ImplicitCastExpr::Create(
              Context, ArgExprs[Idx]->getType(), CK_NoOp, ArgExprs[Idx],
              nullptr, VK_PRValue, FPOptionsOverride());
        }

        // Construct a new arg type with address space of Param
        Qualifiers ArgPtQuals = ArgPtTy.getQualifiers();
        ArgPtQuals.setAddressSpace(ParamAS);
        auto NewArgPtTy =
            Context.getQualifiedType(ArgPtTy.getUnqualifiedType(), ArgPtQuals);
        auto NewArgTy =
            Context.getQualifiedType(Context.getPointerType(NewArgPtTy),
                                     ArgTy.getQualifiers());

        // Finally perform an implicit address space cast
        ArgExprs[Idx] = ImpCastExprToType(ArgExprs[Idx], NewArgTy,
                                          CK_AddressSpaceConversion)
                            .get();
      }
    }
  }

  if (Context.isDependenceAllowed() &&
      (Fn->isTypeDependent() || Expr::hasAnyTypeDependentArguments(ArgExprs))) {
    assert(!getLangOpts().CPlusPlus);
    assert((Fn->containsErrors() ||
            llvm::any_of(ArgExprs,
                         [](clang::Expr *E) { return E->containsErrors(); })) &&
           "should only occur in error-recovery path.");
    return CallExpr::Create(Context, Fn, ArgExprs, Context.DependentTy,
                            VK_PRValue, RParenLoc, CurFPFeatureOverrides());
  }
  return BuildResolvedCallExpr(Fn, NDecl, LParenLoc, ArgExprs, RParenLoc,
                               ExecConfig, IsExecConfig);
}

/// BuildBuiltinCallExpr - Create a call to a builtin function specified by Id
//  with the specified CallArgs
Expr *Sema::BuildBuiltinCallExpr(SourceLocation Loc, Builtin::ID Id,
                                 MultiExprArg CallArgs) {
  StringRef Name = Context.BuiltinInfo.getName(Id);
  LookupResult R(*this, &Context.Idents.get(Name), Loc,
                 Sema::LookupOrdinaryName);
  LookupName(R, TUScope, /*AllowBuiltinCreation=*/true);

  auto *BuiltInDecl = R.getAsSingle<FunctionDecl>();
  assert(BuiltInDecl && "failed to find builtin declaration");

  ExprResult DeclRef =
      BuildDeclRefExpr(BuiltInDecl, BuiltInDecl->getType(), VK_LValue, Loc);
  assert(DeclRef.isUsable() && "Builtin reference cannot fail");

  ExprResult Call =
      BuildCallExpr(/*Scope=*/nullptr, DeclRef.get(), Loc, CallArgs, Loc);

  assert(!Call.isInvalid() && "Call to builtin cannot fail!");
  return Call.get();
}

/// Parse a __builtin_astype expression.
///
/// __builtin_astype( value, dst type )
///
ExprResult Sema::ActOnAsTypeExpr(Expr *E, ParsedType ParsedDestTy,
                                 SourceLocation BuiltinLoc,
                                 SourceLocation RParenLoc) {
  QualType DstTy = GetTypeFromParser(ParsedDestTy);
  return BuildAsTypeExpr(E, DstTy, BuiltinLoc, RParenLoc);
}

/// Create a new AsTypeExpr node (bitcast) from the arguments.
ExprResult Sema::BuildAsTypeExpr(Expr *E, QualType DestTy,
                                 SourceLocation BuiltinLoc,
                                 SourceLocation RParenLoc) {
  ExprValueKind VK = VK_PRValue;
  ExprObjectKind OK = OK_Ordinary;
  QualType SrcTy = E->getType();
  if (!SrcTy->isDependentType() &&
      Context.getTypeSize(DestTy) != Context.getTypeSize(SrcTy))
    return ExprError(
        Diag(BuiltinLoc, diag::err_invalid_astype_of_different_size)
        << DestTy << SrcTy << E->getSourceRange());
  return new (Context) AsTypeExpr(E, DestTy, VK, OK, BuiltinLoc, RParenLoc);
}

/// ActOnConvertVectorExpr - create a new convert-vector expression from the
/// provided arguments.
///
/// __builtin_convertvector( value, dst type )
///
ExprResult Sema::ActOnConvertVectorExpr(Expr *E, ParsedType ParsedDestTy,
                                        SourceLocation BuiltinLoc,
                                        SourceLocation RParenLoc) {
  TypeSourceInfo *TInfo;
  GetTypeFromParser(ParsedDestTy, &TInfo);
  return SemaConvertVectorExpr(E, TInfo, BuiltinLoc, RParenLoc);
}

/// BuildResolvedCallExpr - Build a call to a resolved expression,
/// i.e. an expression not of \p OverloadTy.  The expression should
/// unary-convert to an expression of function-pointer or
/// block-pointer type.
///
/// \param NDecl the declaration being called, if available
ExprResult Sema::BuildResolvedCallExpr(Expr *Fn, NamedDecl *NDecl,
                                       SourceLocation LParenLoc,
                                       ArrayRef<Expr *> Args,
                                       SourceLocation RParenLoc, Expr *Config,
                                       bool IsExecConfig, ADLCallKind UsesADL) {
  FunctionDecl *FDecl = dyn_cast_or_null<FunctionDecl>(NDecl);
  unsigned BuiltinID = (FDecl ? FDecl->getBuiltinID() : 0);

  // Functions with 'interrupt' attribute cannot be called directly.
  if (FDecl && FDecl->hasAttr<AnyX86InterruptAttr>()) {
    Diag(Fn->getExprLoc(), diag::err_anyx86_interrupt_called);
    return ExprError();
  }

  // Interrupt handlers don't save off the VFP regs automatically on ARM,
  // so there's some risk when calling out to non-interrupt handler functions
  // that the callee might not preserve them. This is easy to diagnose here,
  // but can be very challenging to debug.
  // Likewise, X86 interrupt handlers may only call routines with attribute
  // no_caller_saved_registers since there is no efficient way to
  // save and restore the non-GPR state.
  if (auto *Caller = getCurFunctionDecl()) {
    if (Caller->hasAttr<ARMInterruptAttr>()) {
      bool VFP = Context.getTargetInfo().hasFeature("vfp");
      if (VFP && (!FDecl || !FDecl->hasAttr<ARMInterruptAttr>())) {
        Diag(Fn->getExprLoc(), diag::warn_arm_interrupt_calling_convention);
        if (FDecl)
          Diag(FDecl->getLocation(), diag::note_callee_decl) << FDecl;
      }
    }
    if (Caller->hasAttr<AnyX86InterruptAttr>() &&
        ((!FDecl || !FDecl->hasAttr<AnyX86NoCallerSavedRegistersAttr>()))) {
      Diag(Fn->getExprLoc(), diag::warn_anyx86_interrupt_regsave);
      if (FDecl)
        Diag(FDecl->getLocation(), diag::note_callee_decl) << FDecl;
    }
  }

  // Promote the function operand.
  // We special-case function promotion here because we only allow promoting
  // builtin functions to function pointers in the callee of a call.
  ExprResult Result;
  QualType ResultTy;
  if (BuiltinID &&
      Fn->getType()->isSpecificBuiltinType(BuiltinType::BuiltinFn)) {
    // Extract the return type from the (builtin) function pointer type.
    // FIXME Several builtins still have setType in
    // Sema::CheckBuiltinFunctionCall. One should review their definitions in
    // Builtins.def to ensure they are correct before removing setType calls.
    QualType FnPtrTy = Context.getPointerType(FDecl->getType());
    Result = ImpCastExprToType(Fn, FnPtrTy, CK_BuiltinFnToFnPtr).get();
    ResultTy = FDecl->getCallResultType();
  } else {
    Result = CallExprUnaryConversions(Fn);
    ResultTy = Context.BoolTy;
  }
  if (Result.isInvalid())
    return ExprError();
  Fn = Result.get();

  // Check for a valid function type, but only if it is not a builtin which
  // requires custom type checking. These will be handled by
  // CheckBuiltinFunctionCall below just after creation of the call expression.
  const FunctionType *FuncT = nullptr;
  if (!BuiltinID || !Context.BuiltinInfo.hasCustomTypechecking(BuiltinID)) {
  retry:
    if (const PointerType *PT = Fn->getType()->getAs<PointerType>()) {
      // C99 6.5.2.2p1 - "The expression that denotes the called function shall
      // have type pointer to function".
      FuncT = PT->getPointeeType()->getAs<FunctionType>();
      if (!FuncT)
        return ExprError(Diag(LParenLoc, diag::err_typecheck_call_not_function)
                         << Fn->getType() << Fn->getSourceRange());
    } else if (const BlockPointerType *BPT =
                   Fn->getType()->getAs<BlockPointerType>()) {
      FuncT = BPT->getPointeeType()->castAs<FunctionType>();
    } else {
      // Handle calls to expressions of unknown-any type.
      if (Fn->getType() == Context.UnknownAnyTy) {
        ExprResult rewrite = rebuildUnknownAnyFunction(*this, Fn);
        if (rewrite.isInvalid())
          return ExprError();
        Fn = rewrite.get();
        goto retry;
      }

      return ExprError(Diag(LParenLoc, diag::err_typecheck_call_not_function)
                       << Fn->getType() << Fn->getSourceRange());
    }
  }

  // Get the number of parameters in the function prototype, if any.
  // We will allocate space for max(Args.size(), NumParams) arguments
  // in the call expression.
  const auto *Proto = dyn_cast_or_null<FunctionProtoType>(FuncT);
  unsigned NumParams = Proto ? Proto->getNumParams() : 0;

  CallExpr *TheCall;
  if (Config) {
    assert(UsesADL == ADLCallKind::NotADL &&
           "CUDAKernelCallExpr should not use ADL");
    TheCall = CUDAKernelCallExpr::Create(Context, Fn, cast<CallExpr>(Config),
                                         Args, ResultTy, VK_PRValue, RParenLoc,
                                         CurFPFeatureOverrides(), NumParams);
  } else {
    TheCall =
        CallExpr::Create(Context, Fn, Args, ResultTy, VK_PRValue, RParenLoc,
                         CurFPFeatureOverrides(), NumParams, UsesADL);
  }

  if (!Context.isDependenceAllowed()) {
    // Forget about the nulled arguments since typo correction
    // do not handle them well.
    TheCall->shrinkNumArgs(Args.size());
    // C cannot always handle TypoExpr nodes in builtin calls and direct
    // function calls as their argument checking don't necessarily handle
    // dependent types properly, so make sure any TypoExprs have been
    // dealt with.
    ExprResult Result = CorrectDelayedTyposInExpr(TheCall);
    if (!Result.isUsable()) return ExprError();
    CallExpr *TheOldCall = TheCall;
    TheCall = dyn_cast<CallExpr>(Result.get());
    bool CorrectedTypos = TheCall != TheOldCall;
    if (!TheCall) return Result;
    Args = llvm::ArrayRef(TheCall->getArgs(), TheCall->getNumArgs());

    // A new call expression node was created if some typos were corrected.
    // However it may not have been constructed with enough storage. In this
    // case, rebuild the node with enough storage. The waste of space is
    // immaterial since this only happens when some typos were corrected.
    if (CorrectedTypos && Args.size() < NumParams) {
      if (Config)
        TheCall = CUDAKernelCallExpr::Create(
            Context, Fn, cast<CallExpr>(Config), Args, ResultTy, VK_PRValue,
            RParenLoc, CurFPFeatureOverrides(), NumParams);
      else
        TheCall =
            CallExpr::Create(Context, Fn, Args, ResultTy, VK_PRValue, RParenLoc,
                             CurFPFeatureOverrides(), NumParams, UsesADL);
    }
    // We can now handle the nulled arguments for the default arguments.
    TheCall->setNumArgsUnsafe(std::max<unsigned>(Args.size(), NumParams));
  }

  // Bail out early if calling a builtin with custom type checking.
  if (BuiltinID && Context.BuiltinInfo.hasCustomTypechecking(BuiltinID))
    return CheckBuiltinFunctionCall(FDecl, BuiltinID, TheCall);

  if (getLangOpts().CUDA) {
    if (Config) {
      // CUDA: Kernel calls must be to global functions
      if (FDecl && !FDecl->hasAttr<CUDAGlobalAttr>())
        return ExprError(Diag(LParenLoc,diag::err_kern_call_not_global_function)
            << FDecl << Fn->getSourceRange());

      // CUDA: Kernel function must have 'void' return type
      if (!FuncT->getReturnType()->isVoidType() &&
          !FuncT->getReturnType()->getAs<AutoType>() &&
          !FuncT->getReturnType()->isInstantiationDependentType())
        return ExprError(Diag(LParenLoc, diag::err_kern_type_not_void_return)
            << Fn->getType() << Fn->getSourceRange());
    } else {
      // CUDA: Calls to global functions must be configured
      if (FDecl && FDecl->hasAttr<CUDAGlobalAttr>())
        return ExprError(Diag(LParenLoc, diag::err_global_call_not_config)
            << FDecl << Fn->getSourceRange());
    }
  }

  // Check for a valid return type
  if (CheckCallReturnType(FuncT->getReturnType(), Fn->getBeginLoc(), TheCall,
                          FDecl))
    return ExprError();

  // We know the result type of the call, set it.
  TheCall->setType(FuncT->getCallResultType(Context));
  TheCall->setValueKind(Expr::getValueKindForType(FuncT->getReturnType()));

  // WebAssembly tables can't be used as arguments.
  if (Context.getTargetInfo().getTriple().isWasm()) {
    for (const Expr *Arg : Args) {
      if (Arg && Arg->getType()->isWebAssemblyTableType()) {
        return ExprError(Diag(Arg->getExprLoc(),
                              diag::err_wasm_table_as_function_parameter));
      }
    }
  }

  if (Proto) {
    if (ConvertArgumentsForCall(TheCall, Fn, FDecl, Proto, Args, RParenLoc,
                                IsExecConfig))
      return ExprError();
  } else {
    assert(isa<FunctionNoProtoType>(FuncT) && "Unknown FunctionType!");

    if (FDecl) {
      // Check if we have too few/too many template arguments, based
      // on our knowledge of the function definition.
      const FunctionDecl *Def = nullptr;
      if (FDecl->hasBody(Def) && Args.size() != Def->param_size()) {
        Proto = Def->getType()->getAs<FunctionProtoType>();
       if (!Proto || !(Proto->isVariadic() && Args.size() >= Def->param_size()))
          Diag(RParenLoc, diag::warn_call_wrong_number_of_arguments)
          << (Args.size() > Def->param_size()) << FDecl << Fn->getSourceRange();
      }

      // If the function we're calling isn't a function prototype, but we have
      // a function prototype from a prior declaratiom, use that prototype.
      if (!FDecl->hasPrototype())
        Proto = FDecl->getType()->getAs<FunctionProtoType>();
    }

    // If we still haven't found a prototype to use but there are arguments to
    // the call, diagnose this as calling a function without a prototype.
    // However, if we found a function declaration, check to see if
    // -Wdeprecated-non-prototype was disabled where the function was declared.
    // If so, we will silence the diagnostic here on the assumption that this
    // interface is intentional and the user knows what they're doing. We will
    // also silence the diagnostic if there is a function declaration but it
    // was implicitly defined (the user already gets diagnostics about the
    // creation of the implicit function declaration, so the additional warning
    // is not helpful).
    if (!Proto && !Args.empty() &&
        (!FDecl || (!FDecl->isImplicit() &&
                    !Diags.isIgnored(diag::warn_strict_uses_without_prototype,
                                     FDecl->getLocation()))))
      Diag(LParenLoc, diag::warn_strict_uses_without_prototype)
          << (FDecl != nullptr) << FDecl;

    // Promote the arguments (C99 6.5.2.2p6).
    for (unsigned i = 0, e = Args.size(); i != e; i++) {
      Expr *Arg = Args[i];

      if (Proto && i < Proto->getNumParams()) {
        InitializedEntity Entity = InitializedEntity::InitializeParameter(
            Context, Proto->getParamType(i), Proto->isParamConsumed(i));
        ExprResult ArgE =
            PerformCopyInitialization(Entity, SourceLocation(), Arg);
        if (ArgE.isInvalid())
          return true;

        Arg = ArgE.getAs<Expr>();

      } else {
        ExprResult ArgE = DefaultArgumentPromotion(Arg);

        if (ArgE.isInvalid())
          return true;

        Arg = ArgE.getAs<Expr>();
      }

      if (RequireCompleteType(Arg->getBeginLoc(), Arg->getType(),
                              diag::err_call_incomplete_argument, Arg))
        return ExprError();

      TheCall->setArg(i, Arg);
    }
    TheCall->computeDependence();
  }

  if (CXXMethodDecl *Method = dyn_cast_or_null<CXXMethodDecl>(FDecl))
    if (!Method->isStatic())
      return ExprError(Diag(LParenLoc, diag::err_member_call_without_object)
        << Fn->getSourceRange());

  // Check for sentinels
  if (NDecl)
    DiagnoseSentinelCalls(NDecl, LParenLoc, Args);

  // Warn for unions passing across security boundary (CMSE).
  if (FuncT != nullptr && FuncT->getCmseNSCallAttr()) {
    for (unsigned i = 0, e = Args.size(); i != e; i++) {
      if (const auto *RT =
              dyn_cast<RecordType>(Args[i]->getType().getCanonicalType())) {
        if (RT->getDecl()->isOrContainsUnion())
          Diag(Args[i]->getBeginLoc(), diag::warn_cmse_nonsecure_union)
              << 0 << i;
      }
    }
  }

  // Do special checking on direct calls to functions.
  if (FDecl) {
    if (CheckFunctionCall(FDecl, TheCall, Proto))
      return ExprError();

    checkFortifiedBuiltinMemoryFunction(FDecl, TheCall);

    if (BuiltinID)
      return CheckBuiltinFunctionCall(FDecl, BuiltinID, TheCall);
  } else if (NDecl) {
    if (CheckPointerCall(NDecl, TheCall, Proto))
      return ExprError();
  } else {
    if (CheckOtherCall(TheCall, Proto))
      return ExprError();
  }

  return CheckForImmediateInvocation(MaybeBindToTemporary(TheCall), FDecl);
}

ExprResult
Sema::ActOnCompoundLiteral(SourceLocation LParenLoc, ParsedType Ty,
                           SourceLocation RParenLoc, Expr *InitExpr) {
  assert(Ty && "ActOnCompoundLiteral(): missing type");
  assert(InitExpr && "ActOnCompoundLiteral(): missing expression");

  TypeSourceInfo *TInfo;
  QualType literalType = GetTypeFromParser(Ty, &TInfo);
  if (!TInfo)
    TInfo = Context.getTrivialTypeSourceInfo(literalType);

  return BuildCompoundLiteralExpr(LParenLoc, TInfo, RParenLoc, InitExpr);
}

ExprResult
Sema::BuildCompoundLiteralExpr(SourceLocation LParenLoc, TypeSourceInfo *TInfo,
                               SourceLocation RParenLoc, Expr *LiteralExpr) {
  QualType literalType = TInfo->getType();

  if (literalType->isArrayType()) {
    if (RequireCompleteSizedType(
            LParenLoc, Context.getBaseElementType(literalType),
            diag::err_array_incomplete_or_sizeless_type,
            SourceRange(LParenLoc, LiteralExpr->getSourceRange().getEnd())))
      return ExprError();
    if (literalType->isVariableArrayType()) {
      // C23 6.7.10p4: An entity of variable length array type shall not be
      // initialized except by an empty initializer.
      //
      // The C extension warnings are issued from ParseBraceInitializer() and
      // do not need to be issued here. However, we continue to issue an error
      // in the case there are initializers or we are compiling C++. We allow
      // use of VLAs in C++, but it's not clear we want to allow {} to zero
      // init a VLA in C++ in all cases (such as with non-trivial constructors).
      // FIXME: should we allow this construct in C++ when it makes sense to do
      // so?
      std::optional<unsigned> NumInits;
      if (const auto *ILE = dyn_cast<InitListExpr>(LiteralExpr))
        NumInits = ILE->getNumInits();
      if ((LangOpts.CPlusPlus || NumInits.value_or(0)) &&
          !tryToFixVariablyModifiedVarType(TInfo, literalType, LParenLoc,
                                           diag::err_variable_object_no_init))
        return ExprError();
    }
  } else if (!literalType->isDependentType() &&
             RequireCompleteType(LParenLoc, literalType,
               diag::err_typecheck_decl_incomplete_type,
               SourceRange(LParenLoc, LiteralExpr->getSourceRange().getEnd())))
    return ExprError();

  InitializedEntity Entity
    = InitializedEntity::InitializeCompoundLiteralInit(TInfo);
  InitializationKind Kind
    = InitializationKind::CreateCStyleCast(LParenLoc,
                                           SourceRange(LParenLoc, RParenLoc),
                                           /*InitList=*/true);
  InitializationSequence InitSeq(*this, Entity, Kind, LiteralExpr);
  ExprResult Result = InitSeq.Perform(*this, Entity, Kind, LiteralExpr,
                                      &literalType);
  if (Result.isInvalid())
    return ExprError();
  LiteralExpr = Result.get();

  bool isFileScope = !CurContext->isFunctionOrMethod();

  // In C, compound literals are l-values for some reason.
  // For GCC compatibility, in C++, file-scope array compound literals with
  // constant initializers are also l-values, and compound literals are
  // otherwise prvalues.
  //
  // (GCC also treats C++ list-initialized file-scope array prvalues with
  // constant initializers as l-values, but that's non-conforming, so we don't
  // follow it there.)
  //
  // FIXME: It would be better to handle the lvalue cases as materializing and
  // lifetime-extending a temporary object, but our materialized temporaries
  // representation only supports lifetime extension from a variable, not "out
  // of thin air".
  // FIXME: For C++, we might want to instead lifetime-extend only if a pointer
  // is bound to the result of applying array-to-pointer decay to the compound
  // literal.
  // FIXME: GCC supports compound literals of reference type, which should
  // obviously have a value kind derived from the kind of reference involved.
  ExprValueKind VK =
      (getLangOpts().CPlusPlus && !(isFileScope && literalType->isArrayType()))
          ? VK_PRValue
          : VK_LValue;

  if (isFileScope)
    if (auto ILE = dyn_cast<InitListExpr>(LiteralExpr))
      for (unsigned i = 0, j = ILE->getNumInits(); i != j; i++) {
        Expr *Init = ILE->getInit(i);
        ILE->setInit(i, ConstantExpr::Create(Context, Init));
      }

  auto *E = new (Context) CompoundLiteralExpr(LParenLoc, TInfo, literalType,
                                              VK, LiteralExpr, isFileScope);
  if (isFileScope) {
    if (!LiteralExpr->isTypeDependent() &&
        !LiteralExpr->isValueDependent() &&
        !literalType->isDependentType()) // C99 6.5.2.5p3
      if (CheckForConstantInitializer(LiteralExpr, literalType))
        return ExprError();
  } else if (literalType.getAddressSpace() != LangAS::opencl_private &&
             literalType.getAddressSpace() != LangAS::Default) {
    // Embedded-C extensions to C99 6.5.2.5:
    //   "If the compound literal occurs inside the body of a function, the
    //   type name shall not be qualified by an address-space qualifier."
    Diag(LParenLoc, diag::err_compound_literal_with_address_space)
      << SourceRange(LParenLoc, LiteralExpr->getSourceRange().getEnd());
    return ExprError();
  }

  if (!isFileScope && !getLangOpts().CPlusPlus) {
    // Compound literals that have automatic storage duration are destroyed at
    // the end of the scope in C; in C++, they're just temporaries.

    // Emit diagnostics if it is or contains a C union type that is non-trivial
    // to destruct.
    if (E->getType().hasNonTrivialToPrimitiveDestructCUnion())
      checkNonTrivialCUnion(E->getType(), E->getExprLoc(),
                            NTCUC_CompoundLiteral, NTCUK_Destruct);

    // Diagnose jumps that enter or exit the lifetime of the compound literal.
    if (literalType.isDestructedType()) {
      Cleanup.setExprNeedsCleanups(true);
      ExprCleanupObjects.push_back(E);
      getCurFunction()->setHasBranchProtectedScope();
    }
  }

  if (E->getType().hasNonTrivialToPrimitiveDefaultInitializeCUnion() ||
      E->getType().hasNonTrivialToPrimitiveCopyCUnion())
    checkNonTrivialCUnionInInitializer(E->getInitializer(),
                                       E->getInitializer()->getExprLoc());

  return MaybeBindToTemporary(E);
}

ExprResult
Sema::ActOnInitList(SourceLocation LBraceLoc, MultiExprArg InitArgList,
                    SourceLocation RBraceLoc) {
  // Only produce each kind of designated initialization diagnostic once.
  SourceLocation FirstDesignator;
  bool DiagnosedArrayDesignator = false;
  bool DiagnosedNestedDesignator = false;
  bool DiagnosedMixedDesignator = false;

  // Check that any designated initializers are syntactically valid in the
  // current language mode.
  for (unsigned I = 0, E = InitArgList.size(); I != E; ++I) {
    if (auto *DIE = dyn_cast<DesignatedInitExpr>(InitArgList[I])) {
      if (FirstDesignator.isInvalid())
        FirstDesignator = DIE->getBeginLoc();

      if (!getLangOpts().CPlusPlus)
        break;

      if (!DiagnosedNestedDesignator && DIE->size() > 1) {
        DiagnosedNestedDesignator = true;
        Diag(DIE->getBeginLoc(), diag::ext_designated_init_nested)
          << DIE->getDesignatorsSourceRange();
      }

      for (auto &Desig : DIE->designators()) {
        if (!Desig.isFieldDesignator() && !DiagnosedArrayDesignator) {
          DiagnosedArrayDesignator = true;
          Diag(Desig.getBeginLoc(), diag::ext_designated_init_array)
            << Desig.getSourceRange();
        }
      }

      if (!DiagnosedMixedDesignator &&
          !isa<DesignatedInitExpr>(InitArgList[0])) {
        DiagnosedMixedDesignator = true;
        Diag(DIE->getBeginLoc(), diag::ext_designated_init_mixed)
          << DIE->getSourceRange();
        Diag(InitArgList[0]->getBeginLoc(), diag::note_designated_init_mixed)
          << InitArgList[0]->getSourceRange();
      }
    } else if (getLangOpts().CPlusPlus && !DiagnosedMixedDesignator &&
               isa<DesignatedInitExpr>(InitArgList[0])) {
      DiagnosedMixedDesignator = true;
      auto *DIE = cast<DesignatedInitExpr>(InitArgList[0]);
      Diag(DIE->getBeginLoc(), diag::ext_designated_init_mixed)
        << DIE->getSourceRange();
      Diag(InitArgList[I]->getBeginLoc(), diag::note_designated_init_mixed)
        << InitArgList[I]->getSourceRange();
    }
  }

  if (FirstDesignator.isValid()) {
    // Only diagnose designated initiaization as a C++20 extension if we didn't
    // already diagnose use of (non-C++20) C99 designator syntax.
    if (getLangOpts().CPlusPlus && !DiagnosedArrayDesignator &&
        !DiagnosedNestedDesignator && !DiagnosedMixedDesignator) {
      Diag(FirstDesignator, getLangOpts().CPlusPlus20
                                ? diag::warn_cxx17_compat_designated_init
                                : diag::ext_cxx_designated_init);
    } else if (!getLangOpts().CPlusPlus && !getLangOpts().C99) {
      Diag(FirstDesignator, diag::ext_designated_init);
    }
  }

  return BuildInitList(LBraceLoc, InitArgList, RBraceLoc);
}

ExprResult
Sema::BuildInitList(SourceLocation LBraceLoc, MultiExprArg InitArgList,
                    SourceLocation RBraceLoc) {
  // Semantic analysis for initializers is done by ActOnDeclarator() and
  // CheckInitializer() - it requires knowledge of the object being initialized.

  // Immediately handle non-overload placeholders.  Overloads can be
  // resolved contextually, but everything else here can't.
  for (unsigned I = 0, E = InitArgList.size(); I != E; ++I) {
    if (InitArgList[I]->getType()->isNonOverloadPlaceholderType()) {
      ExprResult result = CheckPlaceholderExpr(InitArgList[I]);

      // Ignore failures; dropping the entire initializer list because
      // of one failure would be terrible for indexing/etc.
      if (result.isInvalid()) continue;

      InitArgList[I] = result.get();
    }
  }

  InitListExpr *E = new (Context) InitListExpr(Context, LBraceLoc, InitArgList,
                                               RBraceLoc);
  E->setType(Context.VoidTy); // FIXME: just a place holder for now.
  return E;
}

/// Do an explicit extend of the given block pointer if we're in ARC.
void Sema::maybeExtendBlockObject(ExprResult &E) {
  assert(E.get()->getType()->isBlockPointerType());
  assert(E.get()->isPRValue());

  // Only do this in an r-value context.
  if (!getLangOpts().ObjCAutoRefCount) return;

  E = ImplicitCastExpr::Create(
      Context, E.get()->getType(), CK_ARCExtendBlockObject, E.get(),
      /*base path*/ nullptr, VK_PRValue, FPOptionsOverride());
  Cleanup.setExprNeedsCleanups(true);
}

/// Prepare a conversion of the given expression to an ObjC object
/// pointer type.
CastKind Sema::PrepareCastToObjCObjectPointer(ExprResult &E) {
  QualType type = E.get()->getType();
  if (type->isObjCObjectPointerType()) {
    return CK_BitCast;
  } else if (type->isBlockPointerType()) {
    maybeExtendBlockObject(E);
    return CK_BlockPointerToObjCPointerCast;
  } else {
    assert(type->isPointerType());
    return CK_CPointerToObjCPointerCast;
  }
}

/// Prepares for a scalar cast, performing all the necessary stages
/// except the final cast and returning the kind required.
CastKind Sema::PrepareScalarCast(ExprResult &Src, QualType DestTy) {
  // Both Src and Dest are scalar types, i.e. arithmetic or pointer.
  // Also, callers should have filtered out the invalid cases with
  // pointers.  Everything else should be possible.

  QualType SrcTy = Src.get()->getType();
  if (Context.hasSameUnqualifiedType(SrcTy, DestTy))
    return CK_NoOp;

  switch (Type::ScalarTypeKind SrcKind = SrcTy->getScalarTypeKind()) {
  case Type::STK_MemberPointer:
    llvm_unreachable("member pointer type in C");

  case Type::STK_CPointer:
  case Type::STK_BlockPointer:
  case Type::STK_ObjCObjectPointer:
    switch (DestTy->getScalarTypeKind()) {
    case Type::STK_CPointer: {
      LangAS SrcAS = SrcTy->getPointeeType().getAddressSpace();
      LangAS DestAS = DestTy->getPointeeType().getAddressSpace();
      if (SrcAS != DestAS)
        return CK_AddressSpaceConversion;
      if (Context.hasCvrSimilarType(SrcTy, DestTy))
        return CK_NoOp;
      return CK_BitCast;
    }
    case Type::STK_BlockPointer:
      return (SrcKind == Type::STK_BlockPointer
                ? CK_BitCast : CK_AnyPointerToBlockPointerCast);
    case Type::STK_ObjCObjectPointer:
      if (SrcKind == Type::STK_ObjCObjectPointer)
        return CK_BitCast;
      if (SrcKind == Type::STK_CPointer)
        return CK_CPointerToObjCPointerCast;
      maybeExtendBlockObject(Src);
      return CK_BlockPointerToObjCPointerCast;
    case Type::STK_Bool:
      return CK_PointerToBoolean;
    case Type::STK_Integral:
      return CK_PointerToIntegral;
    case Type::STK_Floating:
    case Type::STK_FloatingComplex:
    case Type::STK_IntegralComplex:
    case Type::STK_MemberPointer:
    case Type::STK_FixedPoint:
      llvm_unreachable("illegal cast from pointer");
    }
    llvm_unreachable("Should have returned before this");

  case Type::STK_FixedPoint:
    switch (DestTy->getScalarTypeKind()) {
    case Type::STK_FixedPoint:
      return CK_FixedPointCast;
    case Type::STK_Bool:
      return CK_FixedPointToBoolean;
    case Type::STK_Integral:
      return CK_FixedPointToIntegral;
    case Type::STK_Floating:
      return CK_FixedPointToFloating;
    case Type::STK_IntegralComplex:
    case Type::STK_FloatingComplex:
      Diag(Src.get()->getExprLoc(),
           diag::err_unimplemented_conversion_with_fixed_point_type)
          << DestTy;
      return CK_IntegralCast;
    case Type::STK_CPointer:
    case Type::STK_ObjCObjectPointer:
    case Type::STK_BlockPointer:
    case Type::STK_MemberPointer:
      llvm_unreachable("illegal cast to pointer type");
    }
    llvm_unreachable("Should have returned before this");

  case Type::STK_Bool: // casting from bool is like casting from an integer
  case Type::STK_Integral:
    switch (DestTy->getScalarTypeKind()) {
    case Type::STK_CPointer:
    case Type::STK_ObjCObjectPointer:
    case Type::STK_BlockPointer:
      if (Src.get()->isNullPointerConstant(Context,
                                           Expr::NPC_ValueDependentIsNull))
        return CK_NullToPointer;
      return CK_IntegralToPointer;
    case Type::STK_Bool:
      return CK_IntegralToBoolean;
    case Type::STK_Integral:
      return CK_IntegralCast;
    case Type::STK_Floating:
      return CK_IntegralToFloating;
    case Type::STK_IntegralComplex:
      Src = ImpCastExprToType(Src.get(),
                      DestTy->castAs<ComplexType>()->getElementType(),
                      CK_IntegralCast);
      return CK_IntegralRealToComplex;
    case Type::STK_FloatingComplex:
      Src = ImpCastExprToType(Src.get(),
                      DestTy->castAs<ComplexType>()->getElementType(),
                      CK_IntegralToFloating);
      return CK_FloatingRealToComplex;
    case Type::STK_MemberPointer:
      llvm_unreachable("member pointer type in C");
    case Type::STK_FixedPoint:
      return CK_IntegralToFixedPoint;
    }
    llvm_unreachable("Should have returned before this");

  case Type::STK_Floating:
    switch (DestTy->getScalarTypeKind()) {
    case Type::STK_Floating:
      return CK_FloatingCast;
    case Type::STK_Bool:
      return CK_FloatingToBoolean;
    case Type::STK_Integral:
      return CK_FloatingToIntegral;
    case Type::STK_FloatingComplex:
      Src = ImpCastExprToType(Src.get(),
                              DestTy->castAs<ComplexType>()->getElementType(),
                              CK_FloatingCast);
      return CK_FloatingRealToComplex;
    case Type::STK_IntegralComplex:
      Src = ImpCastExprToType(Src.get(),
                              DestTy->castAs<ComplexType>()->getElementType(),
                              CK_FloatingToIntegral);
      return CK_IntegralRealToComplex;
    case Type::STK_CPointer:
    case Type::STK_ObjCObjectPointer:
    case Type::STK_BlockPointer:
      llvm_unreachable("valid float->pointer cast?");
    case Type::STK_MemberPointer:
      llvm_unreachable("member pointer type in C");
    case Type::STK_FixedPoint:
      return CK_FloatingToFixedPoint;
    }
    llvm_unreachable("Should have returned before this");

  case Type::STK_FloatingComplex:
    switch (DestTy->getScalarTypeKind()) {
    case Type::STK_FloatingComplex:
      return CK_FloatingComplexCast;
    case Type::STK_IntegralComplex:
      return CK_FloatingComplexToIntegralComplex;
    case Type::STK_Floating: {
      QualType ET = SrcTy->castAs<ComplexType>()->getElementType();
      if (Context.hasSameType(ET, DestTy))
        return CK_FloatingComplexToReal;
      Src = ImpCastExprToType(Src.get(), ET, CK_FloatingComplexToReal);
      return CK_FloatingCast;
    }
    case Type::STK_Bool:
      return CK_FloatingComplexToBoolean;
    case Type::STK_Integral:
      Src = ImpCastExprToType(Src.get(),
                              SrcTy->castAs<ComplexType>()->getElementType(),
                              CK_FloatingComplexToReal);
      return CK_FloatingToIntegral;
    case Type::STK_CPointer:
    case Type::STK_ObjCObjectPointer:
    case Type::STK_BlockPointer:
      llvm_unreachable("valid complex float->pointer cast?");
    case Type::STK_MemberPointer:
      llvm_unreachable("member pointer type in C");
    case Type::STK_FixedPoint:
      Diag(Src.get()->getExprLoc(),
           diag::err_unimplemented_conversion_with_fixed_point_type)
          << SrcTy;
      return CK_IntegralCast;
    }
    llvm_unreachable("Should have returned before this");

  case Type::STK_IntegralComplex:
    switch (DestTy->getScalarTypeKind()) {
    case Type::STK_FloatingComplex:
      return CK_IntegralComplexToFloatingComplex;
    case Type::STK_IntegralComplex:
      return CK_IntegralComplexCast;
    case Type::STK_Integral: {
      QualType ET = SrcTy->castAs<ComplexType>()->getElementType();
      if (Context.hasSameType(ET, DestTy))
        return CK_IntegralComplexToReal;
      Src = ImpCastExprToType(Src.get(), ET, CK_IntegralComplexToReal);
      return CK_IntegralCast;
    }
    case Type::STK_Bool:
      return CK_IntegralComplexToBoolean;
    case Type::STK_Floating:
      Src = ImpCastExprToType(Src.get(),
                              SrcTy->castAs<ComplexType>()->getElementType(),
                              CK_IntegralComplexToReal);
      return CK_IntegralToFloating;
    case Type::STK_CPointer:
    case Type::STK_ObjCObjectPointer:
    case Type::STK_BlockPointer:
      llvm_unreachable("valid complex int->pointer cast?");
    case Type::STK_MemberPointer:
      llvm_unreachable("member pointer type in C");
    case Type::STK_FixedPoint:
      Diag(Src.get()->getExprLoc(),
           diag::err_unimplemented_conversion_with_fixed_point_type)
          << SrcTy;
      return CK_IntegralCast;
    }
    llvm_unreachable("Should have returned before this");
  }

  llvm_unreachable("Unhandled scalar cast");
}

static bool breakDownVectorType(QualType type, uint64_t &len,
                                QualType &eltType) {
  // Vectors are simple.
  if (const VectorType *vecType = type->getAs<VectorType>()) {
    len = vecType->getNumElements();
    eltType = vecType->getElementType();
    assert(eltType->isScalarType());
    return true;
  }

  // We allow lax conversion to and from non-vector types, but only if
  // they're real types (i.e. non-complex, non-pointer scalar types).
  if (!type->isRealType()) return false;

  len = 1;
  eltType = type;
  return true;
}

/// Are the two types SVE-bitcast-compatible types? I.e. is bitcasting from the
/// first SVE type (e.g. an SVE VLAT) to the second type (e.g. an SVE VLST)
/// allowed?
///
/// This will also return false if the two given types do not make sense from
/// the perspective of SVE bitcasts.
bool Sema::isValidSveBitcast(QualType srcTy, QualType destTy) {
  assert(srcTy->isVectorType() || destTy->isVectorType());

  auto ValidScalableConversion = [](QualType FirstType, QualType SecondType) {
    if (!FirstType->isSVESizelessBuiltinType())
      return false;

    const auto *VecTy = SecondType->getAs<VectorType>();
    return VecTy &&
           VecTy->getVectorKind() == VectorType::SveFixedLengthDataVector;
  };

  return ValidScalableConversion(srcTy, destTy) ||
         ValidScalableConversion(destTy, srcTy);
}

/// Are the two types RVV-bitcast-compatible types? I.e. is bitcasting from the
/// first RVV type (e.g. an RVV scalable type) to the second type (e.g. an RVV
/// VLS type) allowed?
///
/// This will also return false if the two given types do not make sense from
/// the perspective of RVV bitcasts.
bool Sema::isValidRVVBitcast(QualType srcTy, QualType destTy) {
  assert(srcTy->isVectorType() || destTy->isVectorType());

  auto ValidScalableConversion = [](QualType FirstType, QualType SecondType) {
    if (!FirstType->isRVVSizelessBuiltinType())
      return false;

    const auto *VecTy = SecondType->getAs<VectorType>();
    return VecTy &&
           VecTy->getVectorKind() == VectorType::RVVFixedLengthDataVector;
  };

  return ValidScalableConversion(srcTy, destTy) ||
         ValidScalableConversion(destTy, srcTy);
}

/// Are the two types matrix types and do they have the same dimensions i.e.
/// do they have the same number of rows and the same number of columns?
bool Sema::areMatrixTypesOfTheSameDimension(QualType srcTy, QualType destTy) {
  if (!destTy->isMatrixType() || !srcTy->isMatrixType())
    return false;

  const ConstantMatrixType *matSrcType = srcTy->getAs<ConstantMatrixType>();
  const ConstantMatrixType *matDestType = destTy->getAs<ConstantMatrixType>();

  return matSrcType->getNumRows() == matDestType->getNumRows() &&
         matSrcType->getNumColumns() == matDestType->getNumColumns();
}

bool Sema::areVectorTypesSameSize(QualType SrcTy, QualType DestTy) {
  assert(DestTy->isVectorType() || SrcTy->isVectorType());

  uint64_t SrcLen, DestLen;
  QualType SrcEltTy, DestEltTy;
  if (!breakDownVectorType(SrcTy, SrcLen, SrcEltTy))
    return false;
  if (!breakDownVectorType(DestTy, DestLen, DestEltTy))
    return false;

  // ASTContext::getTypeSize will return the size rounded up to a
  // power of 2, so instead of using that, we need to use the raw
  // element size multiplied by the element count.
  uint64_t SrcEltSize = Context.getTypeSize(SrcEltTy);
  uint64_t DestEltSize = Context.getTypeSize(DestEltTy);

  return (SrcLen * SrcEltSize == DestLen * DestEltSize);
}

// This returns true if at least one of the types is an altivec vector.
bool Sema::anyAltivecTypes(QualType SrcTy, QualType DestTy) {
  assert((DestTy->isVectorType() || SrcTy->isVectorType()) &&
         "expected at least one type to be a vector here");

  bool IsSrcTyAltivec =
      SrcTy->isVectorType() && ((SrcTy->castAs<VectorType>()->getVectorKind() ==
                                 VectorType::AltiVecVector) ||
                                (SrcTy->castAs<VectorType>()->getVectorKind() ==
                                 VectorType::AltiVecBool) ||
                                (SrcTy->castAs<VectorType>()->getVectorKind() ==
                                 VectorType::AltiVecPixel));

  bool IsDestTyAltivec = DestTy->isVectorType() &&
                         ((DestTy->castAs<VectorType>()->getVectorKind() ==
                           VectorType::AltiVecVector) ||
                          (DestTy->castAs<VectorType>()->getVectorKind() ==
                           VectorType::AltiVecBool) ||
                          (DestTy->castAs<VectorType>()->getVectorKind() ==
                           VectorType::AltiVecPixel));

  return (IsSrcTyAltivec || IsDestTyAltivec);
}

/// Are the two types lax-compatible vector types?  That is, given
/// that one of them is a vector, do they have equal storage sizes,
/// where the storage size is the number of elements times the element
/// size?
///
/// This will also return false if either of the types is neither a
/// vector nor a real type.
bool Sema::areLaxCompatibleVectorTypes(QualType srcTy, QualType destTy) {
  assert(destTy->isVectorType() || srcTy->isVectorType());

  // Disallow lax conversions between scalars and ExtVectors (these
  // conversions are allowed for other vector types because common headers
  // depend on them).  Most scalar OP ExtVector cases are handled by the
  // splat path anyway, which does what we want (convert, not bitcast).
  // What this rules out for ExtVectors is crazy things like char4*float.
  if (srcTy->isScalarType() && destTy->isExtVectorType()) return false;
  if (destTy->isScalarType() && srcTy->isExtVectorType()) return false;

  return areVectorTypesSameSize(srcTy, destTy);
}

/// Is this a legal conversion between two types, one of which is
/// known to be a vector type?
bool Sema::isLaxVectorConversion(QualType srcTy, QualType destTy) {
  assert(destTy->isVectorType() || srcTy->isVectorType());

  switch (Context.getLangOpts().getLaxVectorConversions()) {
  case LangOptions::LaxVectorConversionKind::None:
    return false;

  case LangOptions::LaxVectorConversionKind::Integer:
    if (!srcTy->isIntegralOrEnumerationType()) {
      auto *Vec = srcTy->getAs<VectorType>();
      if (!Vec || !Vec->getElementType()->isIntegralOrEnumerationType())
        return false;
    }
    if (!destTy->isIntegralOrEnumerationType()) {
      auto *Vec = destTy->getAs<VectorType>();
      if (!Vec || !Vec->getElementType()->isIntegralOrEnumerationType())
        return false;
    }
    // OK, integer (vector) -> integer (vector) bitcast.
    break;

    case LangOptions::LaxVectorConversionKind::All:
    break;
  }

  return areLaxCompatibleVectorTypes(srcTy, destTy);
}

bool Sema::CheckMatrixCast(SourceRange R, QualType DestTy, QualType SrcTy,
                           CastKind &Kind) {
  if (SrcTy->isMatrixType() && DestTy->isMatrixType()) {
    if (!areMatrixTypesOfTheSameDimension(SrcTy, DestTy)) {
      return Diag(R.getBegin(), diag::err_invalid_conversion_between_matrixes)
             << DestTy << SrcTy << R;
    }
  } else if (SrcTy->isMatrixType()) {
    return Diag(R.getBegin(),
                diag::err_invalid_conversion_between_matrix_and_type)
           << SrcTy << DestTy << R;
  } else if (DestTy->isMatrixType()) {
    return Diag(R.getBegin(),
                diag::err_invalid_conversion_between_matrix_and_type)
           << DestTy << SrcTy << R;
  }

  Kind = CK_MatrixCast;
  return false;
}

bool Sema::CheckVectorCast(SourceRange R, QualType VectorTy, QualType Ty,
                           CastKind &Kind) {
  assert(VectorTy->isVectorType() && "Not a vector type!");

  if (Ty->isVectorType() || Ty->isIntegralType(Context)) {
    if (!areLaxCompatibleVectorTypes(Ty, VectorTy))
      return Diag(R.getBegin(),
                  Ty->isVectorType() ?
                  diag::err_invalid_conversion_between_vectors :
                  diag::err_invalid_conversion_between_vector_and_integer)
        << VectorTy << Ty << R;
  } else
    return Diag(R.getBegin(),
                diag::err_invalid_conversion_between_vector_and_scalar)
      << VectorTy << Ty << R;

  Kind = CK_BitCast;
  return false;
}

ExprResult Sema::prepareVectorSplat(QualType VectorTy, Expr *SplattedExpr) {
  QualType DestElemTy = VectorTy->castAs<VectorType>()->getElementType();

  if (DestElemTy == SplattedExpr->getType())
    return SplattedExpr;

  assert(DestElemTy->isFloatingType() ||
         DestElemTy->isIntegralOrEnumerationType());

  CastKind CK;
  if (VectorTy->isExtVectorType() && SplattedExpr->getType()->isBooleanType()) {
    // OpenCL requires that we convert `true` boolean expressions to -1, but
    // only when splatting vectors.
    if (DestElemTy->isFloatingType()) {
      // To avoid having to have a CK_BooleanToSignedFloating cast kind, we cast
      // in two steps: boolean to signed integral, then to floating.
      ExprResult CastExprRes = ImpCastExprToType(SplattedExpr, Context.IntTy,
                                                 CK_BooleanToSignedIntegral);
      SplattedExpr = CastExprRes.get();
      CK = CK_IntegralToFloating;
    } else {
      CK = CK_BooleanToSignedIntegral;
    }
  } else {
    ExprResult CastExprRes = SplattedExpr;
    CK = PrepareScalarCast(CastExprRes, DestElemTy);
    if (CastExprRes.isInvalid())
      return ExprError();
    SplattedExpr = CastExprRes.get();
  }
  return ImpCastExprToType(SplattedExpr, DestElemTy, CK);
}

ExprResult Sema::CheckExtVectorCast(SourceRange R, QualType DestTy,
                                    Expr *CastExpr, CastKind &Kind) {
  assert(DestTy->isExtVectorType() && "Not an extended vector type!");

  QualType SrcTy = CastExpr->getType();

  // If SrcTy is a VectorType, the total size must match to explicitly cast to
  // an ExtVectorType.
  // In OpenCL, casts between vectors of different types are not allowed.
  // (See OpenCL 6.2).
  if (SrcTy->isVectorType()) {
    if (!areLaxCompatibleVectorTypes(SrcTy, DestTy) ||
        (getLangOpts().OpenCL &&
         !Context.hasSameUnqualifiedType(DestTy, SrcTy))) {
      Diag(R.getBegin(),diag::err_invalid_conversion_between_ext_vectors)
        << DestTy << SrcTy << R;
      return ExprError();
    }
    Kind = CK_BitCast;
    return CastExpr;
  }

  // All non-pointer scalars can be cast to ExtVector type.  The appropriate
  // conversion will take place first from scalar to elt type, and then
  // splat from elt type to vector.
  if (SrcTy->isPointerType())
    return Diag(R.getBegin(),
                diag::err_invalid_conversion_between_vector_and_scalar)
      << DestTy << SrcTy << R;

  Kind = CK_VectorSplat;
  return prepareVectorSplat(DestTy, CastExpr);
}

ExprResult
Sema::ActOnCastExpr(Scope *S, SourceLocation LParenLoc,
                    Declarator &D, ParsedType &Ty,
                    SourceLocation RParenLoc, Expr *CastExpr) {
  assert(!D.isInvalidType() && (CastExpr != nullptr) &&
         "ActOnCastExpr(): missing type or expr");

  TypeSourceInfo *castTInfo = GetTypeForDeclaratorCast(D, CastExpr->getType());
  if (D.isInvalidType())
    return ExprError();

  if (getLangOpts().CPlusPlus) {
    // Check that there are no default arguments (C++ only).
    CheckExtraCXXDefaultArguments(D);
  } else {
    // Make sure any TypoExprs have been dealt with.
    ExprResult Res = CorrectDelayedTyposInExpr(CastExpr);
    if (!Res.isUsable())
      return ExprError();
    CastExpr = Res.get();
  }

  checkUnusedDeclAttributes(D);

  QualType castType = castTInfo->getType();
  Ty = CreateParsedType(castType, castTInfo);

  bool isVectorLiteral = false;

  // Check for an altivec or OpenCL literal,
  // i.e. all the elements are integer constants.
  ParenExpr *PE = dyn_cast<ParenExpr>(CastExpr);
  ParenListExpr *PLE = dyn_cast<ParenListExpr>(CastExpr);
  if ((getLangOpts().AltiVec || getLangOpts().ZVector || getLangOpts().OpenCL)
       && castType->isVectorType() && (PE || PLE)) {
    if (PLE && PLE->getNumExprs() == 0) {
      Diag(PLE->getExprLoc(), diag::err_altivec_empty_initializer);
      return ExprError();
    }
    if (PE || PLE->getNumExprs() == 1) {
      Expr *E = (PE ? PE->getSubExpr() : PLE->getExpr(0));
      if (!E->isTypeDependent() && !E->getType()->isVectorType())
        isVectorLiteral = true;
    }
    else
      isVectorLiteral = true;
  }

  // If this is a vector initializer, '(' type ')' '(' init, ..., init ')'
  // then handle it as such.
  if (isVectorLiteral)
    return BuildVectorLiteral(LParenLoc, RParenLoc, CastExpr, castTInfo);

  // If the Expr being casted is a ParenListExpr, handle it specially.
  // This is not an AltiVec-style cast, so turn the ParenListExpr into a
  // sequence of BinOp comma operators.
  if (isa<ParenListExpr>(CastExpr)) {
    ExprResult Result = MaybeConvertParenListExprToParenExpr(S, CastExpr);
    if (Result.isInvalid()) return ExprError();
    CastExpr = Result.get();
  }

  if (getLangOpts().CPlusPlus && !castType->isVoidType())
    Diag(LParenLoc, diag::warn_old_style_cast) << CastExpr->getSourceRange();

  CheckTollFreeBridgeCast(castType, CastExpr);

  CheckObjCBridgeRelatedCast(castType, CastExpr);

  DiscardMisalignedMemberAddress(castType.getTypePtr(), CastExpr);

  return BuildCStyleCastExpr(LParenLoc, castTInfo, RParenLoc, CastExpr);
}

ExprResult Sema::BuildVectorLiteral(SourceLocation LParenLoc,
                                    SourceLocation RParenLoc, Expr *E,
                                    TypeSourceInfo *TInfo) {
  assert((isa<ParenListExpr>(E) || isa<ParenExpr>(E)) &&
         "Expected paren or paren list expression");

  Expr **exprs;
  unsigned numExprs;
  Expr *subExpr;
  SourceLocation LiteralLParenLoc, LiteralRParenLoc;
  if (ParenListExpr *PE = dyn_cast<ParenListExpr>(E)) {
    LiteralLParenLoc = PE->getLParenLoc();
    LiteralRParenLoc = PE->getRParenLoc();
    exprs = PE->getExprs();
    numExprs = PE->getNumExprs();
  } else { // isa<ParenExpr> by assertion at function entrance
    LiteralLParenLoc = cast<ParenExpr>(E)->getLParen();
    LiteralRParenLoc = cast<ParenExpr>(E)->getRParen();
    subExpr = cast<ParenExpr>(E)->getSubExpr();
    exprs = &subExpr;
    numExprs = 1;
  }

  QualType Ty = TInfo->getType();
  assert(Ty->isVectorType() && "Expected vector type");

  SmallVector<Expr *, 8> initExprs;
  const VectorType *VTy = Ty->castAs<VectorType>();
  unsigned numElems = VTy->getNumElements();

  // '(...)' form of vector initialization in AltiVec: the number of
  // initializers must be one or must match the size of the vector.
  // If a single value is specified in the initializer then it will be
  // replicated to all the components of the vector
  if (CheckAltivecInitFromScalar(E->getSourceRange(), Ty,
                                 VTy->getElementType()))
    return ExprError();
  if (ShouldSplatAltivecScalarInCast(VTy)) {
    // The number of initializers must be one or must match the size of the
    // vector. If a single value is specified in the initializer then it will
    // be replicated to all the components of the vector
    if (numExprs == 1) {
      QualType ElemTy = VTy->getElementType();
      ExprResult Literal = DefaultLvalueConversion(exprs[0]);
      if (Literal.isInvalid())
        return ExprError();
      Literal = ImpCastExprToType(Literal.get(), ElemTy,
                                  PrepareScalarCast(Literal, ElemTy));
      return BuildCStyleCastExpr(LParenLoc, TInfo, RParenLoc, Literal.get());
    }
    else if (numExprs < numElems) {
      Diag(E->getExprLoc(),
           diag::err_incorrect_number_of_vector_initializers);
      return ExprError();
    }
    else
      initExprs.append(exprs, exprs + numExprs);
  }
  else {
    // For OpenCL, when the number of initializers is a single value,
    // it will be replicated to all components of the vector.
    if (getLangOpts().OpenCL &&
        VTy->getVectorKind() == VectorType::GenericVector &&
        numExprs == 1) {
        QualType ElemTy = VTy->getElementType();
        ExprResult Literal = DefaultLvalueConversion(exprs[0]);
        if (Literal.isInvalid())
          return ExprError();
        Literal = ImpCastExprToType(Literal.get(), ElemTy,
                                    PrepareScalarCast(Literal, ElemTy));
        return BuildCStyleCastExpr(LParenLoc, TInfo, RParenLoc, Literal.get());
    }

    initExprs.append(exprs, exprs + numExprs);
  }
  // FIXME: This means that pretty-printing the final AST will produce curly
  // braces instead of the original commas.
  InitListExpr *initE = new (Context) InitListExpr(Context, LiteralLParenLoc,
                                                   initExprs, LiteralRParenLoc);
  initE->setType(Ty);
  return BuildCompoundLiteralExpr(LParenLoc, TInfo, RParenLoc, initE);
}

/// This is not an AltiVec-style cast or or C++ direct-initialization, so turn
/// the ParenListExpr into a sequence of comma binary operators.
ExprResult
Sema::MaybeConvertParenListExprToParenExpr(Scope *S, Expr *OrigExpr) {
  ParenListExpr *E = dyn_cast<ParenListExpr>(OrigExpr);
  if (!E)
    return OrigExpr;

  ExprResult Result(E->getExpr(0));

  for (unsigned i = 1, e = E->getNumExprs(); i != e && !Result.isInvalid(); ++i)
    Result = ActOnBinOp(S, E->getExprLoc(), tok::comma, Result.get(),
                        E->getExpr(i));

  if (Result.isInvalid()) return ExprError();

  return ActOnParenExpr(E->getLParenLoc(), E->getRParenLoc(), Result.get());
}

ExprResult Sema::ActOnParenListExpr(SourceLocation L,
                                    SourceLocation R,
                                    MultiExprArg Val) {
  return ParenListExpr::Create(Context, L, Val, R);
}

/// Emit a specialized diagnostic when one expression is a null pointer
/// constant and the other is not a pointer.  Returns true if a diagnostic is
/// emitted.
bool Sema::DiagnoseConditionalForNull(Expr *LHSExpr, Expr *RHSExpr,
                                      SourceLocation QuestionLoc) {
  Expr *NullExpr = LHSExpr;
  Expr *NonPointerExpr = RHSExpr;
  Expr::NullPointerConstantKind NullKind =
      NullExpr->isNullPointerConstant(Context,
                                      Expr::NPC_ValueDependentIsNotNull);

  if (NullKind == Expr::NPCK_NotNull) {
    NullExpr = RHSExpr;
    NonPointerExpr = LHSExpr;
    NullKind =
        NullExpr->isNullPointerConstant(Context,
                                        Expr::NPC_ValueDependentIsNotNull);
  }

  if (NullKind == Expr::NPCK_NotNull)
    return false;

  if (NullKind == Expr::NPCK_ZeroExpression)
    return false;

  if (NullKind == Expr::NPCK_ZeroLiteral) {
    // In this case, check to make sure that we got here from a "NULL"
    // string in the source code.
    NullExpr = NullExpr->IgnoreParenImpCasts();
    SourceLocation loc = NullExpr->getExprLoc();
    if (!findMacroSpelling(loc, "NULL"))
      return false;
  }

  int DiagType = (NullKind == Expr::NPCK_CXX11_nullptr);
  Diag(QuestionLoc, diag::err_typecheck_cond_incompatible_operands_null)
      << NonPointerExpr->getType() << DiagType
      << NonPointerExpr->getSourceRange();
  return true;
}

/// Return false if the condition expression is valid, true otherwise.
static bool checkCondition(Sema &S, Expr *Cond, SourceLocation QuestionLoc) {
  QualType CondTy = Cond->getType();

  // OpenCL v1.1 s6.3.i says the condition cannot be a floating point type.
  if (S.getLangOpts().OpenCL && CondTy->isFloatingType()) {
    S.Diag(QuestionLoc, diag::err_typecheck_cond_expect_nonfloat)
      << CondTy << Cond->getSourceRange();
    return true;
  }

  // C99 6.5.15p2
  if (CondTy->isScalarType()) return false;

  S.Diag(QuestionLoc, diag::err_typecheck_cond_expect_scalar)
    << CondTy << Cond->getSourceRange();
  return true;
}

/// Return false if the NullExpr can be promoted to PointerTy,
/// true otherwise.
static bool checkConditionalNullPointer(Sema &S, ExprResult &NullExpr,
                                        QualType PointerTy) {
  if ((!PointerTy->isAnyPointerType() && !PointerTy->isBlockPointerType()) ||
      !NullExpr.get()->isNullPointerConstant(S.Context,
                                            Expr::NPC_ValueDependentIsNull))
    return true;

  NullExpr = S.ImpCastExprToType(NullExpr.get(), PointerTy, CK_NullToPointer);
  return false;
}

/// Checks compatibility between two pointers and return the resulting
/// type.
static QualType checkConditionalPointerCompatibility(Sema &S, ExprResult &LHS,
                                                     ExprResult &RHS,
                                                     SourceLocation Loc) {
  QualType LHSTy = LHS.get()->getType();
  QualType RHSTy = RHS.get()->getType();

  if (S.Context.hasSameType(LHSTy, RHSTy)) {
    // Two identical pointers types are always compatible.
    return S.Context.getCommonSugaredType(LHSTy, RHSTy);
  }

  QualType lhptee, rhptee;

  // Get the pointee types.
  bool IsBlockPointer = false;
  if (const BlockPointerType *LHSBTy = LHSTy->getAs<BlockPointerType>()) {
    lhptee = LHSBTy->getPointeeType();
    rhptee = RHSTy->castAs<BlockPointerType>()->getPointeeType();
    IsBlockPointer = true;
  } else {
    lhptee = LHSTy->castAs<PointerType>()->getPointeeType();
    rhptee = RHSTy->castAs<PointerType>()->getPointeeType();
  }

  // C99 6.5.15p6: If both operands are pointers to compatible types or to
  // differently qualified versions of compatible types, the result type is
  // a pointer to an appropriately qualified version of the composite
  // type.

  // Only CVR-qualifiers exist in the standard, and the differently-qualified
  // clause doesn't make sense for our extensions. E.g. address space 2 should
  // be incompatible with address space 3: they may live on different devices or
  // anything.
  Qualifiers lhQual = lhptee.getQualifiers();
  Qualifiers rhQual = rhptee.getQualifiers();

  LangAS ResultAddrSpace = LangAS::Default;
  LangAS LAddrSpace = lhQual.getAddressSpace();
  LangAS RAddrSpace = rhQual.getAddressSpace();

  // OpenCL v1.1 s6.5 - Conversion between pointers to distinct address
  // spaces is disallowed.
  if (lhQual.isAddressSpaceSupersetOf(rhQual))
    ResultAddrSpace = LAddrSpace;
  else if (rhQual.isAddressSpaceSupersetOf(lhQual))
    ResultAddrSpace = RAddrSpace;
  else {
    S.Diag(Loc, diag::err_typecheck_op_on_nonoverlapping_address_space_pointers)
        << LHSTy << RHSTy << 2 << LHS.get()->getSourceRange()
        << RHS.get()->getSourceRange();
    return QualType();
  }

  unsigned MergedCVRQual = lhQual.getCVRQualifiers() | rhQual.getCVRQualifiers();
  auto LHSCastKind = CK_BitCast, RHSCastKind = CK_BitCast;
  lhQual.removeCVRQualifiers();
  rhQual.removeCVRQualifiers();

  // OpenCL v2.0 specification doesn't extend compatibility of type qualifiers
  // (C99 6.7.3) for address spaces. We assume that the check should behave in
  // the same manner as it's defined for CVR qualifiers, so for OpenCL two
  // qual types are compatible iff
  //  * corresponded types are compatible
  //  * CVR qualifiers are equal
  //  * address spaces are equal
  // Thus for conditional operator we merge CVR and address space unqualified
  // pointees and if there is a composite type we return a pointer to it with
  // merged qualifiers.
  LHSCastKind =
      LAddrSpace == ResultAddrSpace ? CK_BitCast : CK_AddressSpaceConversion;
  RHSCastKind =
      RAddrSpace == ResultAddrSpace ? CK_BitCast : CK_AddressSpaceConversion;
  lhQual.removeAddressSpace();
  rhQual.removeAddressSpace();

  lhptee = S.Context.getQualifiedType(lhptee.getUnqualifiedType(), lhQual);
  rhptee = S.Context.getQualifiedType(rhptee.getUnqualifiedType(), rhQual);

  QualType CompositeTy = S.Context.mergeTypes(
      lhptee, rhptee, /*OfBlockPointer=*/false, /*Unqualified=*/false,
      /*BlockReturnType=*/false, /*IsConditionalOperator=*/true);

  if (CompositeTy.isNull()) {
    // In this situation, we assume void* type. No especially good
    // reason, but this is what gcc does, and we do have to pick
    // to get a consistent AST.
    QualType incompatTy;
    incompatTy = S.Context.getPointerType(
        S.Context.getAddrSpaceQualType(S.Context.VoidTy, ResultAddrSpace));
    LHS = S.ImpCastExprToType(LHS.get(), incompatTy, LHSCastKind);
    RHS = S.ImpCastExprToType(RHS.get(), incompatTy, RHSCastKind);

    // FIXME: For OpenCL the warning emission and cast to void* leaves a room
    // for casts between types with incompatible address space qualifiers.
    // For the following code the compiler produces casts between global and
    // local address spaces of the corresponded innermost pointees:
    // local int *global *a;
    // global int *global *b;
    // a = (0 ? a : b); // see C99 6.5.16.1.p1.
    S.Diag(Loc, diag::ext_typecheck_cond_incompatible_pointers)
        << LHSTy << RHSTy << LHS.get()->getSourceRange()
        << RHS.get()->getSourceRange();

    return incompatTy;
  }

  // The pointer types are compatible.
  // In case of OpenCL ResultTy should have the address space qualifier
  // which is a superset of address spaces of both the 2nd and the 3rd
  // operands of the conditional operator.
  QualType ResultTy = [&, ResultAddrSpace]() {
    if (S.getLangOpts().OpenCL) {
      Qualifiers CompositeQuals = CompositeTy.getQualifiers();
      CompositeQuals.setAddressSpace(ResultAddrSpace);
      return S.Context
          .getQualifiedType(CompositeTy.getUnqualifiedType(), CompositeQuals)
          .withCVRQualifiers(MergedCVRQual);
    }
    return CompositeTy.withCVRQualifiers(MergedCVRQual);
  }();
  if (IsBlockPointer)
    ResultTy = S.Context.getBlockPointerType(ResultTy);
  else
    ResultTy = S.Context.getPointerType(ResultTy);

  LHS = S.ImpCastExprToType(LHS.get(), ResultTy, LHSCastKind);
  RHS = S.ImpCastExprToType(RHS.get(), ResultTy, RHSCastKind);
  return ResultTy;
}

/// Return the resulting type when the operands are both block pointers.
static QualType checkConditionalBlockPointerCompatibility(Sema &S,
                                                          ExprResult &LHS,
                                                          ExprResult &RHS,
                                                          SourceLocation Loc) {
  QualType LHSTy = LHS.get()->getType();
  QualType RHSTy = RHS.get()->getType();

  if (!LHSTy->isBlockPointerType() || !RHSTy->isBlockPointerType()) {
    if (LHSTy->isVoidPointerType() || RHSTy->isVoidPointerType()) {
      QualType destType = S.Context.getPointerType(S.Context.VoidTy);
      LHS = S.ImpCastExprToType(LHS.get(), destType, CK_BitCast);
      RHS = S.ImpCastExprToType(RHS.get(), destType, CK_BitCast);
      return destType;
    }
    S.Diag(Loc, diag::err_typecheck_cond_incompatible_operands)
      << LHSTy << RHSTy << LHS.get()->getSourceRange()
      << RHS.get()->getSourceRange();
    return QualType();
  }

  // We have 2 block pointer types.
  return checkConditionalPointerCompatibility(S, LHS, RHS, Loc);
}

/// Return the resulting type when the operands are both pointers.
static QualType
checkConditionalObjectPointersCompatibility(Sema &S, ExprResult &LHS,
                                            ExprResult &RHS,
                                            SourceLocation Loc) {
  // get the pointer types
  QualType LHSTy = LHS.get()->getType();
  QualType RHSTy = RHS.get()->getType();

  // get the "pointed to" types
  QualType lhptee = LHSTy->castAs<PointerType>()->getPointeeType();
  QualType rhptee = RHSTy->castAs<PointerType>()->getPointeeType();

  // ignore qualifiers on void (C99 6.5.15p3, clause 6)
  if (lhptee->isVoidType() && rhptee->isIncompleteOrObjectType()) {
    // Figure out necessary qualifiers (C99 6.5.15p6)
    QualType destPointee
      = S.Context.getQualifiedType(lhptee, rhptee.getQualifiers());
    QualType destType = S.Context.getPointerType(destPointee);
    // Add qualifiers if necessary.
    LHS = S.ImpCastExprToType(LHS.get(), destType, CK_NoOp);
    // Promote to void*.
    RHS = S.ImpCastExprToType(RHS.get(), destType, CK_BitCast);
    return destType;
  }
  if (rhptee->isVoidType() && lhptee->isIncompleteOrObjectType()) {
    QualType destPointee
      = S.Context.getQualifiedType(rhptee, lhptee.getQualifiers());
    QualType destType = S.Context.getPointerType(destPointee);
    // Add qualifiers if necessary.
    RHS = S.ImpCastExprToType(RHS.get(), destType, CK_NoOp);
    // Promote to void*.
    LHS = S.ImpCastExprToType(LHS.get(), destType, CK_BitCast);
    return destType;
  }

  return checkConditionalPointerCompatibility(S, LHS, RHS, Loc);
}

/// Return false if the first expression is not an integer and the second
/// expression is not a pointer, true otherwise.
static bool checkPointerIntegerMismatch(Sema &S, ExprResult &Int,
                                        Expr* PointerExpr, SourceLocation Loc,
                                        bool IsIntFirstExpr) {
  if (!PointerExpr->getType()->isPointerType() ||
      !Int.get()->getType()->isIntegerType())
    return false;

  Expr *Expr1 = IsIntFirstExpr ? Int.get() : PointerExpr;
  Expr *Expr2 = IsIntFirstExpr ? PointerExpr : Int.get();

  S.Diag(Loc, diag::ext_typecheck_cond_pointer_integer_mismatch)
    << Expr1->getType() << Expr2->getType()
    << Expr1->getSourceRange() << Expr2->getSourceRange();
  Int = S.ImpCastExprToType(Int.get(), PointerExpr->getType(),
                            CK_IntegralToPointer);
  return true;
}

/// Simple conversion between integer and floating point types.
///
/// Used when handling the OpenCL conditional operator where the
/// condition is a vector while the other operands are scalar.
///
/// OpenCL v1.1 s6.3.i and s6.11.6 together require that the scalar
/// types are either integer or floating type. Between the two
/// operands, the type with the higher rank is defined as the "result
/// type". The other operand needs to be promoted to the same type. No
/// other type promotion is allowed. We cannot use
/// UsualArithmeticConversions() for this purpose, since it always
/// promotes promotable types.
static QualType OpenCLArithmeticConversions(Sema &S, ExprResult &LHS,
                                            ExprResult &RHS,
                                            SourceLocation QuestionLoc) {
  LHS = S.DefaultFunctionArrayLvalueConversion(LHS.get());
  if (LHS.isInvalid())
    return QualType();
  RHS = S.DefaultFunctionArrayLvalueConversion(RHS.get());
  if (RHS.isInvalid())
    return QualType();

  // For conversion purposes, we ignore any qualifiers.
  // For example, "const float" and "float" are equivalent.
  QualType LHSType =
    S.Context.getCanonicalType(LHS.get()->getType()).getUnqualifiedType();
  QualType RHSType =
    S.Context.getCanonicalType(RHS.get()->getType()).getUnqualifiedType();

  if (!LHSType->isIntegerType() && !LHSType->isRealFloatingType()) {
    S.Diag(QuestionLoc, diag::err_typecheck_cond_expect_int_float)
      << LHSType << LHS.get()->getSourceRange();
    return QualType();
  }

  if (!RHSType->isIntegerType() && !RHSType->isRealFloatingType()) {
    S.Diag(QuestionLoc, diag::err_typecheck_cond_expect_int_float)
      << RHSType << RHS.get()->getSourceRange();
    return QualType();
  }

  // If both types are identical, no conversion is needed.
  if (LHSType == RHSType)
    return LHSType;

  // Now handle "real" floating types (i.e. float, double, long double).
  if (LHSType->isRealFloatingType() || RHSType->isRealFloatingType())
    return handleFloatConversion(S, LHS, RHS, LHSType, RHSType,
                                 /*IsCompAssign = */ false);

  // Finally, we have two differing integer types.
  return handleIntegerConversion<doIntegralCast, doIntegralCast>
  (S, LHS, RHS, LHSType, RHSType, /*IsCompAssign = */ false);
}

/// Convert scalar operands to a vector that matches the
///        condition in length.
///
/// Used when handling the OpenCL conditional operator where the
/// condition is a vector while the other operands are scalar.
///
/// We first compute the "result type" for the scalar operands
/// according to OpenCL v1.1 s6.3.i. Both operands are then converted
/// into a vector of that type where the length matches the condition
/// vector type. s6.11.6 requires that the element types of the result
/// and the condition must have the same number of bits.
static QualType
OpenCLConvertScalarsToVectors(Sema &S, ExprResult &LHS, ExprResult &RHS,
                              QualType CondTy, SourceLocation QuestionLoc) {
  QualType ResTy = OpenCLArithmeticConversions(S, LHS, RHS, QuestionLoc);
  if (ResTy.isNull()) return QualType();

  const VectorType *CV = CondTy->getAs<VectorType>();
  assert(CV);

  // Determine the vector result type
  unsigned NumElements = CV->getNumElements();
  QualType VectorTy = S.Context.getExtVectorType(ResTy, NumElements);

  // Ensure that all types have the same number of bits
  if (S.Context.getTypeSize(CV->getElementType())
      != S.Context.getTypeSize(ResTy)) {
    // Since VectorTy is created internally, it does not pretty print
    // with an OpenCL name. Instead, we just print a description.
    std::string EleTyName = ResTy.getUnqualifiedType().getAsString();
    SmallString<64> Str;
    llvm::raw_svector_ostream OS(Str);
    OS << "(vector of " << NumElements << " '" << EleTyName << "' values)";
    S.Diag(QuestionLoc, diag::err_conditional_vector_element_size)
      << CondTy << OS.str();
    return QualType();
  }

  // Convert operands to the vector result type
  LHS = S.ImpCastExprToType(LHS.get(), VectorTy, CK_VectorSplat);
  RHS = S.ImpCastExprToType(RHS.get(), VectorTy, CK_VectorSplat);

  return VectorTy;
}

/// Return false if this is a valid OpenCL condition vector
static bool checkOpenCLConditionVector(Sema &S, Expr *Cond,
                                       SourceLocation QuestionLoc) {
  // OpenCL v1.1 s6.11.6 says the elements of the vector must be of
  // integral type.
  const VectorType *CondTy = Cond->getType()->getAs<VectorType>();
  assert(CondTy);
  QualType EleTy = CondTy->getElementType();
  if (EleTy->isIntegerType()) return false;

  S.Diag(QuestionLoc, diag::err_typecheck_cond_expect_nonfloat)
    << Cond->getType() << Cond->getSourceRange();
  return true;
}

/// Return false if the vector condition type and the vector
///        result type are compatible.
///
/// OpenCL v1.1 s6.11.6 requires that both vector types have the same
/// number of elements, and their element types have the same number
/// of bits.
static bool checkVectorResult(Sema &S, QualType CondTy, QualType VecResTy,
                              SourceLocation QuestionLoc) {
  const VectorType *CV = CondTy->getAs<VectorType>();
  const VectorType *RV = VecResTy->getAs<VectorType>();
  assert(CV && RV);

  if (CV->getNumElements() != RV->getNumElements()) {
    S.Diag(QuestionLoc, diag::err_conditional_vector_size)
      << CondTy << VecResTy;
    return true;
  }

  QualType CVE = CV->getElementType();
  QualType RVE = RV->getElementType();

  if (S.Context.getTypeSize(CVE) != S.Context.getTypeSize(RVE)) {
    S.Diag(QuestionLoc, diag::err_conditional_vector_element_size)
      << CondTy << VecResTy;
    return true;
  }

  return false;
}

/// Return the resulting type for the conditional operator in
///        OpenCL (aka "ternary selection operator", OpenCL v1.1
///        s6.3.i) when the condition is a vector type.
static QualType
OpenCLCheckVectorConditional(Sema &S, ExprResult &Cond,
                             ExprResult &LHS, ExprResult &RHS,
                             SourceLocation QuestionLoc) {
  Cond = S.DefaultFunctionArrayLvalueConversion(Cond.get());
  if (Cond.isInvalid())
    return QualType();
  QualType CondTy = Cond.get()->getType();

  if (checkOpenCLConditionVector(S, Cond.get(), QuestionLoc))
    return QualType();

  // If either operand is a vector then find the vector type of the
  // result as specified in OpenCL v1.1 s6.3.i.
  if (LHS.get()->getType()->isVectorType() ||
      RHS.get()->getType()->isVectorType()) {
    bool IsBoolVecLang =
        !S.getLangOpts().OpenCL && !S.getLangOpts().OpenCLCPlusPlus;
    QualType VecResTy =
        S.CheckVectorOperands(LHS, RHS, QuestionLoc,
                              /*isCompAssign*/ false,
                              /*AllowBothBool*/ true,
                              /*AllowBoolConversions*/ false,
                              /*AllowBooleanOperation*/ IsBoolVecLang,
                              /*ReportInvalid*/ true);
    if (VecResTy.isNull())
      return QualType();
    // The result type must match the condition type as specified in
    // OpenCL v1.1 s6.11.6.
    if (checkVectorResult(S, CondTy, VecResTy, QuestionLoc))
      return QualType();
    return VecResTy;
  }

  // Both operands are scalar.
  return OpenCLConvertScalarsToVectors(S, LHS, RHS, CondTy, QuestionLoc);
}

/// Return true if the Expr is block type
static bool checkBlockType(Sema &S, const Expr *E) {
  if (const CallExpr *CE = dyn_cast<CallExpr>(E)) {
    QualType Ty = CE->getCallee()->getType();
    if (Ty->isBlockPointerType()) {
      S.Diag(E->getExprLoc(), diag::err_opencl_ternary_with_block);
      return true;
    }
  }
  return false;
}

/// Note that LHS is not null here, even if this is the gnu "x ?: y" extension.
/// In that case, LHS = cond.
/// C99 6.5.15
QualType Sema::CheckConditionalOperands(ExprResult &Cond, ExprResult &LHS,
                                        ExprResult &RHS, ExprValueKind &VK,
                                        ExprObjectKind &OK,
                                        SourceLocation QuestionLoc) {

  ExprResult LHSResult = CheckPlaceholderExpr(LHS.get());
  if (!LHSResult.isUsable()) return QualType();
  LHS = LHSResult;

  ExprResult RHSResult = CheckPlaceholderExpr(RHS.get());
  if (!RHSResult.isUsable()) return QualType();
  RHS = RHSResult;

  // C++ is sufficiently different to merit its own checker.
  if (getLangOpts().CPlusPlus)
    return CXXCheckConditionalOperands(Cond, LHS, RHS, VK, OK, QuestionLoc);

  VK = VK_PRValue;
  OK = OK_Ordinary;

  if (Context.isDependenceAllowed() &&
      (Cond.get()->isTypeDependent() || LHS.get()->isTypeDependent() ||
       RHS.get()->isTypeDependent())) {
    assert(!getLangOpts().CPlusPlus);
    assert((Cond.get()->containsErrors() || LHS.get()->containsErrors() ||
            RHS.get()->containsErrors()) &&
           "should only occur in error-recovery path.");
    return Context.DependentTy;
  }

  // The OpenCL operator with a vector condition is sufficiently
  // different to merit its own checker.
  if ((getLangOpts().OpenCL && Cond.get()->getType()->isVectorType()) ||
      Cond.get()->getType()->isExtVectorType())
    return OpenCLCheckVectorConditional(*this, Cond, LHS, RHS, QuestionLoc);

  // First, check the condition.
  Cond = UsualUnaryConversions(Cond.get());
  if (Cond.isInvalid())
    return QualType();
  if (checkCondition(*this, Cond.get(), QuestionLoc))
    return QualType();

  // Now check the two expressions.
  if (LHS.get()->getType()->isVectorType() ||
      RHS.get()->getType()->isVectorType())
    return CheckVectorOperands(LHS, RHS, QuestionLoc, /*isCompAssign*/ false,
                               /*AllowBothBool*/ true,
                               /*AllowBoolConversions*/ false,
                               /*AllowBooleanOperation*/ false,
                               /*ReportInvalid*/ true);

  QualType ResTy =
      UsualArithmeticConversions(LHS, RHS, QuestionLoc, ACK_Conditional);
  if (LHS.isInvalid() || RHS.isInvalid())
    return QualType();

  // WebAssembly tables are not allowed as conditional LHS or RHS.
  QualType LHSTy = LHS.get()->getType();
  QualType RHSTy = RHS.get()->getType();
  if (LHSTy->isWebAssemblyTableType() || RHSTy->isWebAssemblyTableType()) {
    Diag(QuestionLoc, diag::err_wasm_table_conditional_expression)
        << LHS.get()->getSourceRange() << RHS.get()->getSourceRange();
    return QualType();
  }

  // Diagnose attempts to convert between __ibm128, __float128 and long double
  // where such conversions currently can't be handled.
  if (unsupportedTypeConversion(*this, LHSTy, RHSTy)) {
    Diag(QuestionLoc,
         diag::err_typecheck_cond_incompatible_operands) << LHSTy << RHSTy
      << LHS.get()->getSourceRange() << RHS.get()->getSourceRange();
    return QualType();
  }

  // OpenCL v2.0 s6.12.5 - Blocks cannot be used as expressions of the ternary
  // selection operator (?:).
  if (getLangOpts().OpenCL &&
      ((int)checkBlockType(*this, LHS.get()) | (int)checkBlockType(*this, RHS.get()))) {
    return QualType();
  }

  // If both operands have arithmetic type, do the usual arithmetic conversions
  // to find a common type: C99 6.5.15p3,5.
  if (LHSTy->isArithmeticType() && RHSTy->isArithmeticType()) {
    // Disallow invalid arithmetic conversions, such as those between bit-
    // precise integers types of different sizes, or between a bit-precise
    // integer and another type.
    if (ResTy.isNull() && (LHSTy->isBitIntType() || RHSTy->isBitIntType())) {
      Diag(QuestionLoc, diag::err_typecheck_cond_incompatible_operands)
          << LHSTy << RHSTy << LHS.get()->getSourceRange()
          << RHS.get()->getSourceRange();
      return QualType();
    }

    LHS = ImpCastExprToType(LHS.get(), ResTy, PrepareScalarCast(LHS, ResTy));
    RHS = ImpCastExprToType(RHS.get(), ResTy, PrepareScalarCast(RHS, ResTy));

    return ResTy;
  }

  // And if they're both bfloat (which isn't arithmetic), that's fine too.
  if (LHSTy->isBFloat16Type() && RHSTy->isBFloat16Type()) {
    return Context.getCommonSugaredType(LHSTy, RHSTy);
  }

  // If both operands are the same structure or union type, the result is that
  // type.
  if (const RecordType *LHSRT = LHSTy->getAs<RecordType>()) {    // C99 6.5.15p3
    if (const RecordType *RHSRT = RHSTy->getAs<RecordType>())
      if (LHSRT->getDecl() == RHSRT->getDecl())
        // "If both the operands have structure or union type, the result has
        // that type."  This implies that CV qualifiers are dropped.
        return Context.getCommonSugaredType(LHSTy.getUnqualifiedType(),
                                            RHSTy.getUnqualifiedType());
    // FIXME: Type of conditional expression must be complete in C mode.
  }

  // C99 6.5.15p5: "If both operands have void type, the result has void type."
  // The following || allows only one side to be void (a GCC-ism).
  if (LHSTy->isVoidType() || RHSTy->isVoidType()) {
    QualType ResTy;
    if (LHSTy->isVoidType() && RHSTy->isVoidType()) {
      ResTy = Context.getCommonSugaredType(LHSTy, RHSTy);
    } else if (RHSTy->isVoidType()) {
      ResTy = RHSTy;
      Diag(RHS.get()->getBeginLoc(), diag::ext_typecheck_cond_one_void)
          << RHS.get()->getSourceRange();
    } else {
      ResTy = LHSTy;
      Diag(LHS.get()->getBeginLoc(), diag::ext_typecheck_cond_one_void)
          << LHS.get()->getSourceRange();
    }
    LHS = ImpCastExprToType(LHS.get(), ResTy, CK_ToVoid);
    RHS = ImpCastExprToType(RHS.get(), ResTy, CK_ToVoid);
    return ResTy;
  }

  // C23 6.5.15p7:
  //   ... if both the second and third operands have nullptr_t type, the
  //   result also has that type.
  if (LHSTy->isNullPtrType() && Context.hasSameType(LHSTy, RHSTy))
    return ResTy;

  // C99 6.5.15p6 - "if one operand is a null pointer constant, the result has
  // the type of the other operand."
  if (!checkConditionalNullPointer(*this, RHS, LHSTy)) return LHSTy;
  if (!checkConditionalNullPointer(*this, LHS, RHSTy)) return RHSTy;

  // All objective-c pointer type analysis is done here.
  QualType compositeType = FindCompositeObjCPointerType(LHS, RHS,
                                                        QuestionLoc);
  if (LHS.isInvalid() || RHS.isInvalid())
    return QualType();
  if (!compositeType.isNull())
    return compositeType;


  // Handle block pointer types.
  if (LHSTy->isBlockPointerType() || RHSTy->isBlockPointerType())
    return checkConditionalBlockPointerCompatibility(*this, LHS, RHS,
                                                     QuestionLoc);

  // Check constraints for C object pointers types (C99 6.5.15p3,6).
  if (LHSTy->isPointerType() && RHSTy->isPointerType())
    return checkConditionalObjectPointersCompatibility(*this, LHS, RHS,
                                                       QuestionLoc);

  // GCC compatibility: soften pointer/integer mismatch.  Note that
  // null pointers have been filtered out by this point.
  if (checkPointerIntegerMismatch(*this, LHS, RHS.get(), QuestionLoc,
      /*IsIntFirstExpr=*/true))
    return RHSTy;
  if (checkPointerIntegerMismatch(*this, RHS, LHS.get(), QuestionLoc,
      /*IsIntFirstExpr=*/false))
    return LHSTy;

  // Allow ?: operations in which both operands have the same
  // built-in sizeless type.
  if (LHSTy->isSizelessBuiltinType() && Context.hasSameType(LHSTy, RHSTy))
    return Context.getCommonSugaredType(LHSTy, RHSTy);

  // Emit a better diagnostic if one of the expressions is a null pointer
  // constant and the other is not a pointer type. In this case, the user most
  // likely forgot to take the address of the other expression.
  if (DiagnoseConditionalForNull(LHS.get(), RHS.get(), QuestionLoc))
    return QualType();

  // Otherwise, the operands are not compatible.
  Diag(QuestionLoc, diag::err_typecheck_cond_incompatible_operands)
    << LHSTy << RHSTy << LHS.get()->getSourceRange()
    << RHS.get()->getSourceRange();
  return QualType();
}

/// FindCompositeObjCPointerType - Helper method to find composite type of
/// two objective-c pointer types of the two input expressions.
QualType Sema::FindCompositeObjCPointerType(ExprResult &LHS, ExprResult &RHS,
                                            SourceLocation QuestionLoc) {
  QualType LHSTy = LHS.get()->getType();
  QualType RHSTy = RHS.get()->getType();

  // Handle things like Class and struct objc_class*.  Here we case the result
  // to the pseudo-builtin, because that will be implicitly cast back to the
  // redefinition type if an attempt is made to access its fields.
  if (LHSTy->isObjCClassType() &&
      (Context.hasSameType(RHSTy, Context.getObjCClassRedefinitionType()))) {
    RHS = ImpCastExprToType(RHS.get(), LHSTy, CK_CPointerToObjCPointerCast);
    return LHSTy;
  }
  if (RHSTy->isObjCClassType() &&
      (Context.hasSameType(LHSTy, Context.getObjCClassRedefinitionType()))) {
    LHS = ImpCastExprToType(LHS.get(), RHSTy, CK_CPointerToObjCPointerCast);
    return RHSTy;
  }
  // And the same for struct objc_object* / id
  if (LHSTy->isObjCIdType() &&
      (Context.hasSameType(RHSTy, Context.getObjCIdRedefinitionType()))) {
    RHS = ImpCastExprToType(RHS.get(), LHSTy, CK_CPointerToObjCPointerCast);
    return LHSTy;
  }
  if (RHSTy->isObjCIdType() &&
      (Context.hasSameType(LHSTy, Context.getObjCIdRedefinitionType()))) {
    LHS = ImpCastExprToType(LHS.get(), RHSTy, CK_CPointerToObjCPointerCast);
    return RHSTy;
  }
  // And the same for struct objc_selector* / SEL
  if (Context.isObjCSelType(LHSTy) &&
      (Context.hasSameType(RHSTy, Context.getObjCSelRedefinitionType()))) {
    RHS = ImpCastExprToType(RHS.get(), LHSTy, CK_BitCast);
    return LHSTy;
  }
  if (Context.isObjCSelType(RHSTy) &&
      (Context.hasSameType(LHSTy, Context.getObjCSelRedefinitionType()))) {
    LHS = ImpCastExprToType(LHS.get(), RHSTy, CK_BitCast);
    return RHSTy;
  }
  // Check constraints for Objective-C object pointers types.
  if (LHSTy->isObjCObjectPointerType() && RHSTy->isObjCObjectPointerType()) {

    if (Context.getCanonicalType(LHSTy) == Context.getCanonicalType(RHSTy)) {
      // Two identical object pointer types are always compatible.
      return LHSTy;
    }
    const ObjCObjectPointerType *LHSOPT = LHSTy->castAs<ObjCObjectPointerType>();
    const ObjCObjectPointerType *RHSOPT = RHSTy->castAs<ObjCObjectPointerType>();
    QualType compositeType = LHSTy;

    // If both operands are interfaces and either operand can be
    // assigned to the other, use that type as the composite
    // type. This allows
    //   xxx ? (A*) a : (B*) b
    // where B is a subclass of A.
    //
    // Additionally, as for assignment, if either type is 'id'
    // allow silent coercion. Finally, if the types are
    // incompatible then make sure to use 'id' as the composite
    // type so the result is acceptable for sending messages to.

    // FIXME: Consider unifying with 'areComparableObjCPointerTypes'.
    // It could return the composite type.
    if (!(compositeType =
          Context.areCommonBaseCompatible(LHSOPT, RHSOPT)).isNull()) {
      // Nothing more to do.
    } else if (Context.canAssignObjCInterfaces(LHSOPT, RHSOPT)) {
      compositeType = RHSOPT->isObjCBuiltinType() ? RHSTy : LHSTy;
    } else if (Context.canAssignObjCInterfaces(RHSOPT, LHSOPT)) {
      compositeType = LHSOPT->isObjCBuiltinType() ? LHSTy : RHSTy;
    } else if ((LHSOPT->isObjCQualifiedIdType() ||
                RHSOPT->isObjCQualifiedIdType()) &&
               Context.ObjCQualifiedIdTypesAreCompatible(LHSOPT, RHSOPT,
                                                         true)) {
      // Need to handle "id<xx>" explicitly.
      // GCC allows qualified id and any Objective-C type to devolve to
      // id. Currently localizing to here until clear this should be
      // part of ObjCQualifiedIdTypesAreCompatible.
      compositeType = Context.getObjCIdType();
    } else if (LHSTy->isObjCIdType() || RHSTy->isObjCIdType()) {
      compositeType = Context.getObjCIdType();
    } else {
      Diag(QuestionLoc, diag::ext_typecheck_cond_incompatible_operands)
      << LHSTy << RHSTy
      << LHS.get()->getSourceRange() << RHS.get()->getSourceRange();
      QualType incompatTy = Context.getObjCIdType();
      LHS = ImpCastExprToType(LHS.get(), incompatTy, CK_BitCast);
      RHS = ImpCastExprToType(RHS.get(), incompatTy, CK_BitCast);
      return incompatTy;
    }
    // The object pointer types are compatible.
    LHS = ImpCastExprToType(LHS.get(), compositeType, CK_BitCast);
    RHS = ImpCastExprToType(RHS.get(), compositeType, CK_BitCast);
    return compositeType;
  }
  // Check Objective-C object pointer types and 'void *'
  if (LHSTy->isVoidPointerType() && RHSTy->isObjCObjectPointerType()) {
    if (getLangOpts().ObjCAutoRefCount) {
      // ARC forbids the implicit conversion of object pointers to 'void *',
      // so these types are not compatible.
      Diag(QuestionLoc, diag::err_cond_voidptr_arc) << LHSTy << RHSTy
          << LHS.get()->getSourceRange() << RHS.get()->getSourceRange();
      LHS = RHS = true;
      return QualType();
    }
    QualType lhptee = LHSTy->castAs<PointerType>()->getPointeeType();
    QualType rhptee = RHSTy->castAs<ObjCObjectPointerType>()->getPointeeType();
    QualType destPointee
    = Context.getQualifiedType(lhptee, rhptee.getQualifiers());
    QualType destType = Context.getPointerType(destPointee);
    // Add qualifiers if necessary.
    LHS = ImpCastExprToType(LHS.get(), destType, CK_NoOp);
    // Promote to void*.
    RHS = ImpCastExprToType(RHS.get(), destType, CK_BitCast);
    return destType;
  }
  if (LHSTy->isObjCObjectPointerType() && RHSTy->isVoidPointerType()) {
    if (getLangOpts().ObjCAutoRefCount) {
      // ARC forbids the implicit conversion of object pointers to 'void *',
      // so these types are not compatible.
      Diag(QuestionLoc, diag::err_cond_voidptr_arc) << LHSTy << RHSTy
          << LHS.get()->getSourceRange() << RHS.get()->getSourceRange();
      LHS = RHS = true;
      return QualType();
    }
    QualType lhptee = LHSTy->castAs<ObjCObjectPointerType>()->getPointeeType();
    QualType rhptee = RHSTy->castAs<PointerType>()->getPointeeType();
    QualType destPointee
    = Context.getQualifiedType(rhptee, lhptee.getQualifiers());
    QualType destType = Context.getPointerType(destPointee);
    // Add qualifiers if necessary.
    RHS = ImpCastExprToType(RHS.get(), destType, CK_NoOp);
    // Promote to void*.
    LHS = ImpCastExprToType(LHS.get(), destType, CK_BitCast);
    return destType;
  }
  return QualType();
}

/// SuggestParentheses - Emit a note with a fixit hint that wraps
/// ParenRange in parentheses.
static void SuggestParentheses(Sema &Self, SourceLocation Loc,
                               const PartialDiagnostic &Note,
                               SourceRange ParenRange) {
  SourceLocation EndLoc = Self.getLocForEndOfToken(ParenRange.getEnd());
  if (ParenRange.getBegin().isFileID() && ParenRange.getEnd().isFileID() &&
      EndLoc.isValid()) {
    Self.Diag(Loc, Note)
      << FixItHint::CreateInsertion(ParenRange.getBegin(), "(")
      << FixItHint::CreateInsertion(EndLoc, ")");
  } else {
    // We can't display the parentheses, so just show the bare note.
    Self.Diag(Loc, Note) << ParenRange;
  }
}

static bool IsArithmeticOp(BinaryOperatorKind Opc) {
  return BinaryOperator::isAdditiveOp(Opc) ||
         BinaryOperator::isMultiplicativeOp(Opc) ||
         BinaryOperator::isShiftOp(Opc) || Opc == BO_And || Opc == BO_Or;
  // This only checks for bitwise-or and bitwise-and, but not bitwise-xor and
  // not any of the logical operators.  Bitwise-xor is commonly used as a
  // logical-xor because there is no logical-xor operator.  The logical
  // operators, including uses of xor, have a high false positive rate for
  // precedence warnings.
}

/// IsArithmeticBinaryExpr - Returns true if E is an arithmetic binary
/// expression, either using a built-in or overloaded operator,
/// and sets *OpCode to the opcode and *RHSExprs to the right-hand side
/// expression.
static bool IsArithmeticBinaryExpr(Expr *E, BinaryOperatorKind *Opcode,
                                   Expr **RHSExprs) {
  // Don't strip parenthesis: we should not warn if E is in parenthesis.
  E = E->IgnoreImpCasts();
  E = E->IgnoreConversionOperatorSingleStep();
  E = E->IgnoreImpCasts();
  if (auto *MTE = dyn_cast<MaterializeTemporaryExpr>(E)) {
    E = MTE->getSubExpr();
    E = E->IgnoreImpCasts();
  }

  // Built-in binary operator.
  if (BinaryOperator *OP = dyn_cast<BinaryOperator>(E)) {
    if (IsArithmeticOp(OP->getOpcode())) {
      *Opcode = OP->getOpcode();
      *RHSExprs = OP->getRHS();
      return true;
    }
  }

  // Overloaded operator.
  if (CXXOperatorCallExpr *Call = dyn_cast<CXXOperatorCallExpr>(E)) {
    if (Call->getNumArgs() != 2)
      return false;

    // Make sure this is really a binary operator that is safe to pass into
    // BinaryOperator::getOverloadedOpcode(), e.g. it's not a subscript op.
    OverloadedOperatorKind OO = Call->getOperator();
    if (OO < OO_Plus || OO > OO_Arrow ||
        OO == OO_PlusPlus || OO == OO_MinusMinus)
      return false;

    BinaryOperatorKind OpKind = BinaryOperator::getOverloadedOpcode(OO);
    if (IsArithmeticOp(OpKind)) {
      *Opcode = OpKind;
      *RHSExprs = Call->getArg(1);
      return true;
    }
  }

  return false;
}

/// ExprLooksBoolean - Returns true if E looks boolean, i.e. it has boolean type
/// or is a logical expression such as (x==y) which has int type, but is
/// commonly interpreted as boolean.
static bool ExprLooksBoolean(Expr *E) {
  E = E->IgnoreParenImpCasts();

  if (E->getType()->isBooleanType())
    return true;
  if (BinaryOperator *OP = dyn_cast<BinaryOperator>(E))
    return OP->isComparisonOp() || OP->isLogicalOp();
  if (UnaryOperator *OP = dyn_cast<UnaryOperator>(E))
    return OP->getOpcode() == UO_LNot;
  if (E->getType()->isPointerType())
    return true;
  // FIXME: What about overloaded operator calls returning "unspecified boolean
  // type"s (commonly pointer-to-members)?

  return false;
}

/// DiagnoseConditionalPrecedence - Emit a warning when a conditional operator
/// and binary operator are mixed in a way that suggests the programmer assumed
/// the conditional operator has higher precedence, for example:
/// "int x = a + someBinaryCondition ? 1 : 2".
static void DiagnoseConditionalPrecedence(Sema &Self,
                                          SourceLocation OpLoc,
                                          Expr *Condition,
                                          Expr *LHSExpr,
                                          Expr *RHSExpr) {
  BinaryOperatorKind CondOpcode;
  Expr *CondRHS;

  if (!IsArithmeticBinaryExpr(Condition, &CondOpcode, &CondRHS))
    return;
  if (!ExprLooksBoolean(CondRHS))
    return;

  // The condition is an arithmetic binary expression, with a right-
  // hand side that looks boolean, so warn.

  unsigned DiagID = BinaryOperator::isBitwiseOp(CondOpcode)
                        ? diag::warn_precedence_bitwise_conditional
                        : diag::warn_precedence_conditional;

  Self.Diag(OpLoc, DiagID)
      << Condition->getSourceRange()
      << BinaryOperator::getOpcodeStr(CondOpcode);

  SuggestParentheses(
      Self, OpLoc,
      Self.PDiag(diag::note_precedence_silence)
          << BinaryOperator::getOpcodeStr(CondOpcode),
      SourceRange(Condition->getBeginLoc(), Condition->getEndLoc()));

  SuggestParentheses(Self, OpLoc,
                     Self.PDiag(diag::note_precedence_conditional_first),
                     SourceRange(CondRHS->getBeginLoc(), RHSExpr->getEndLoc()));
}

/// Compute the nullability of a conditional expression.
static QualType computeConditionalNullability(QualType ResTy, bool IsBin,
                                              QualType LHSTy, QualType RHSTy,
                                              ASTContext &Ctx) {
  if (!ResTy->isAnyPointerType())
    return ResTy;

  auto GetNullability = [](QualType Ty) {
    std::optional<NullabilityKind> Kind = Ty->getNullability();
    if (Kind) {
      // For our purposes, treat _Nullable_result as _Nullable.
      if (*Kind == NullabilityKind::NullableResult)
        return NullabilityKind::Nullable;
      return *Kind;
    }
    return NullabilityKind::Unspecified;
  };

  auto LHSKind = GetNullability(LHSTy), RHSKind = GetNullability(RHSTy);
  NullabilityKind MergedKind;

  // Compute nullability of a binary conditional expression.
  if (IsBin) {
    if (LHSKind == NullabilityKind::NonNull)
      MergedKind = NullabilityKind::NonNull;
    else
      MergedKind = RHSKind;
  // Compute nullability of a normal conditional expression.
  } else {
    if (LHSKind == NullabilityKind::Nullable ||
        RHSKind == NullabilityKind::Nullable)
      MergedKind = NullabilityKind::Nullable;
    else if (LHSKind == NullabilityKind::NonNull)
      MergedKind = RHSKind;
    else if (RHSKind == NullabilityKind::NonNull)
      MergedKind = LHSKind;
    else
      MergedKind = NullabilityKind::Unspecified;
  }

  // Return if ResTy already has the correct nullability.
  if (GetNullability(ResTy) == MergedKind)
    return ResTy;

  // Strip all nullability from ResTy.
  while (ResTy->getNullability())
    ResTy = ResTy.getSingleStepDesugaredType(Ctx);

  // Create a new AttributedType with the new nullability kind.
  auto NewAttr = AttributedType::getNullabilityAttrKind(MergedKind);
  return Ctx.getAttributedType(NewAttr, ResTy, ResTy);
}

/// ActOnConditionalOp - Parse a ?: operation.  Note that 'LHS' may be null
/// in the case of a the GNU conditional expr extension.
ExprResult Sema::ActOnConditionalOp(SourceLocation QuestionLoc,
                                    SourceLocation ColonLoc,
                                    Expr *CondExpr, Expr *LHSExpr,
                                    Expr *RHSExpr) {
  if (!Context.isDependenceAllowed()) {
    // C cannot handle TypoExpr nodes in the condition because it
    // doesn't handle dependent types properly, so make sure any TypoExprs have
    // been dealt with before checking the operands.
    ExprResult CondResult = CorrectDelayedTyposInExpr(CondExpr);
    ExprResult LHSResult = CorrectDelayedTyposInExpr(LHSExpr);
    ExprResult RHSResult = CorrectDelayedTyposInExpr(RHSExpr);

    if (!CondResult.isUsable())
      return ExprError();

    if (LHSExpr) {
      if (!LHSResult.isUsable())
        return ExprError();
    }

    if (!RHSResult.isUsable())
      return ExprError();

    CondExpr = CondResult.get();
    LHSExpr = LHSResult.get();
    RHSExpr = RHSResult.get();
  }

  // If this is the gnu "x ?: y" extension, analyze the types as though the LHS
  // was the condition.
  OpaqueValueExpr *opaqueValue = nullptr;
  Expr *commonExpr = nullptr;
  if (!LHSExpr) {
    commonExpr = CondExpr;
    // Lower out placeholder types first.  This is important so that we don't
    // try to capture a placeholder. This happens in few cases in C++; such
    // as Objective-C++'s dictionary subscripting syntax.
    if (commonExpr->hasPlaceholderType()) {
      ExprResult result = CheckPlaceholderExpr(commonExpr);
      if (!result.isUsable()) return ExprError();
      commonExpr = result.get();
    }
    // We usually want to apply unary conversions *before* saving, except
    // in the special case of a C++ l-value conditional.
    if (!(getLangOpts().CPlusPlus
          && !commonExpr->isTypeDependent()
          && commonExpr->getValueKind() == RHSExpr->getValueKind()
          && commonExpr->isGLValue()
          && commonExpr->isOrdinaryOrBitFieldObject()
          && RHSExpr->isOrdinaryOrBitFieldObject()
          && Context.hasSameType(commonExpr->getType(), RHSExpr->getType()))) {
      ExprResult commonRes = UsualUnaryConversions(commonExpr);
      if (commonRes.isInvalid())
        return ExprError();
      commonExpr = commonRes.get();
    }

    // If the common expression is a class or array prvalue, materialize it
    // so that we can safely refer to it multiple times.
    if (commonExpr->isPRValue() && (commonExpr->getType()->isRecordType() ||
                                    commonExpr->getType()->isArrayType())) {
      ExprResult MatExpr = TemporaryMaterializationConversion(commonExpr);
      if (MatExpr.isInvalid())
        return ExprError();
      commonExpr = MatExpr.get();
    }

    opaqueValue = new (Context) OpaqueValueExpr(commonExpr->getExprLoc(),
                                                commonExpr->getType(),
                                                commonExpr->getValueKind(),
                                                commonExpr->getObjectKind(),
                                                commonExpr);
    LHSExpr = CondExpr = opaqueValue;
  }

  QualType LHSTy = LHSExpr->getType(), RHSTy = RHSExpr->getType();
  ExprValueKind VK = VK_PRValue;
  ExprObjectKind OK = OK_Ordinary;
  ExprResult Cond = CondExpr, LHS = LHSExpr, RHS = RHSExpr;
  QualType result = CheckConditionalOperands(Cond, LHS, RHS,
                                             VK, OK, QuestionLoc);
  if (result.isNull() || Cond.isInvalid() || LHS.isInvalid() ||
      RHS.isInvalid())
    return ExprError();

  DiagnoseConditionalPrecedence(*this, QuestionLoc, Cond.get(), LHS.get(),
                                RHS.get());

  CheckBoolLikeConversion(Cond.get(), QuestionLoc);

  result = computeConditionalNullability(result, commonExpr, LHSTy, RHSTy,
                                         Context);

  if (!commonExpr)
    return new (Context)
        ConditionalOperator(Cond.get(), QuestionLoc, LHS.get(), ColonLoc,
                            RHS.get(), result, VK, OK);

  return new (Context) BinaryConditionalOperator(
      commonExpr, opaqueValue, Cond.get(), LHS.get(), RHS.get(), QuestionLoc,
      ColonLoc, result, VK, OK);
}

// Check that the SME attributes for PSTATE.ZA and PSTATE.SM are compatible.
bool Sema::IsInvalidSMECallConversion(QualType FromType, QualType ToType,
                                      AArch64SMECallConversionKind C) {
  unsigned FromAttributes = 0, ToAttributes = 0;
  if (const auto *FromFn =
          dyn_cast<FunctionProtoType>(Context.getCanonicalType(FromType)))
    FromAttributes =
        FromFn->getAArch64SMEAttributes() & FunctionType::SME_AttributeMask;
  if (const auto *ToFn =
          dyn_cast<FunctionProtoType>(Context.getCanonicalType(ToType)))
    ToAttributes =
        ToFn->getAArch64SMEAttributes() & FunctionType::SME_AttributeMask;

  if (FromAttributes == ToAttributes)
    return false;

  // If the '__arm_preserves_za' is the only difference between the types,
  // check whether we're allowed to add or remove it.
  if ((FromAttributes ^ ToAttributes) ==
      FunctionType::SME_PStateZAPreservedMask) {
    switch (C) {
    case AArch64SMECallConversionKind::MatchExactly:
      return true;
    case AArch64SMECallConversionKind::MayAddPreservesZA:
      return !(ToAttributes & FunctionType::SME_PStateZAPreservedMask);
    case AArch64SMECallConversionKind::MayDropPreservesZA:
      return !(FromAttributes & FunctionType::SME_PStateZAPreservedMask);
    }
  }

  // There has been a mismatch of attributes
  return true;
}

// Check if we have a conversion between incompatible cmse function pointer
// types, that is, a conversion between a function pointer with the
// cmse_nonsecure_call attribute and one without.
static bool IsInvalidCmseNSCallConversion(Sema &S, QualType FromType,
                                          QualType ToType) {
  if (const auto *ToFn =
          dyn_cast<FunctionType>(S.Context.getCanonicalType(ToType))) {
    if (const auto *FromFn =
            dyn_cast<FunctionType>(S.Context.getCanonicalType(FromType))) {
      FunctionType::ExtInfo ToEInfo = ToFn->getExtInfo();
      FunctionType::ExtInfo FromEInfo = FromFn->getExtInfo();

      return ToEInfo.getCmseNSCall() != FromEInfo.getCmseNSCall();
    }
  }
  return false;
}

// checkPointerTypesForAssignment - This is a very tricky routine (despite
// being closely modeled after the C99 spec:-). The odd characteristic of this
// routine is it effectively iqnores the qualifiers on the top level pointee.
// This circumvents the usual type rules specified in 6.2.7p1 & 6.7.5.[1-3].
// FIXME: add a couple examples in this comment.
static Sema::AssignConvertType
checkPointerTypesForAssignment(Sema &S, QualType LHSType, QualType RHSType,
                               SourceLocation Loc) {
  assert(LHSType.isCanonical() && "LHS not canonicalized!");
  assert(RHSType.isCanonical() && "RHS not canonicalized!");

  // get the "pointed to" type (ignoring qualifiers at the top level)
  const Type *lhptee, *rhptee;
  Qualifiers lhq, rhq;
  std::tie(lhptee, lhq) =
      cast<PointerType>(LHSType)->getPointeeType().split().asPair();
  std::tie(rhptee, rhq) =
      cast<PointerType>(RHSType)->getPointeeType().split().asPair();

  Sema::AssignConvertType ConvTy = Sema::Compatible;

  // C99 6.5.16.1p1: This following citation is common to constraints
  // 3 & 4 (below). ...and the type *pointed to* by the left has all the
  // qualifiers of the type *pointed to* by the right;

  // As a special case, 'non-__weak A *' -> 'non-__weak const *' is okay.
  if (lhq.getObjCLifetime() != rhq.getObjCLifetime() &&
      lhq.compatiblyIncludesObjCLifetime(rhq)) {
    // Ignore lifetime for further calculation.
    lhq.removeObjCLifetime();
    rhq.removeObjCLifetime();
  }

  if (!lhq.compatiblyIncludes(rhq)) {
    // Treat address-space mismatches as fatal.
    if (!lhq.isAddressSpaceSupersetOf(rhq))
      return Sema::IncompatiblePointerDiscardsQualifiers;

    // It's okay to add or remove GC or lifetime qualifiers when converting to
    // and from void*.
    else if (lhq.withoutObjCGCAttr().withoutObjCLifetime()
                        .compatiblyIncludes(
                                rhq.withoutObjCGCAttr().withoutObjCLifetime())
             && (lhptee->isVoidType() || rhptee->isVoidType()))
      ; // keep old

    // Treat lifetime mismatches as fatal.
    else if (lhq.getObjCLifetime() != rhq.getObjCLifetime())
      ConvTy = Sema::IncompatiblePointerDiscardsQualifiers;

    // For GCC/MS compatibility, other qualifier mismatches are treated
    // as still compatible in C.
    else ConvTy = Sema::CompatiblePointerDiscardsQualifiers;
  }

  // C99 6.5.16.1p1 (constraint 4): If one operand is a pointer to an object or
  // incomplete type and the other is a pointer to a qualified or unqualified
  // version of void...
  if (lhptee->isVoidType()) {
    if (rhptee->isIncompleteOrObjectType())
      return ConvTy;

    // As an extension, we allow cast to/from void* to function pointer.
    assert(rhptee->isFunctionType());
    return Sema::FunctionVoidPointer;
  }

  if (rhptee->isVoidType()) {
    if (lhptee->isIncompleteOrObjectType())
      return ConvTy;

    // As an extension, we allow cast to/from void* to function pointer.
    assert(lhptee->isFunctionType());
    return Sema::FunctionVoidPointer;
  }

  if (!S.Diags.isIgnored(
          diag::warn_typecheck_convert_incompatible_function_pointer_strict,
          Loc) &&
      RHSType->isFunctionPointerType() && LHSType->isFunctionPointerType() &&
      !S.IsFunctionConversion(RHSType, LHSType, RHSType))
    return Sema::IncompatibleFunctionPointerStrict;

  // C99 6.5.16.1p1 (constraint 3): both operands are pointers to qualified or
  // unqualified versions of compatible types, ...
  QualType ltrans = QualType(lhptee, 0), rtrans = QualType(rhptee, 0);
  if (!S.Context.typesAreCompatible(ltrans, rtrans)) {
    // Check if the pointee types are compatible ignoring the sign.
    // We explicitly check for char so that we catch "char" vs
    // "unsigned char" on systems where "char" is unsigned.
    if (lhptee->isCharType())
      ltrans = S.Context.UnsignedCharTy;
    else if (lhptee->hasSignedIntegerRepresentation())
      ltrans = S.Context.getCorrespondingUnsignedType(ltrans);

    if (rhptee->isCharType())
      rtrans = S.Context.UnsignedCharTy;
    else if (rhptee->hasSignedIntegerRepresentation())
      rtrans = S.Context.getCorrespondingUnsignedType(rtrans);

    if (ltrans == rtrans) {
      // Types are compatible ignoring the sign. Qualifier incompatibility
      // takes priority over sign incompatibility because the sign
      // warning can be disabled.
      if (ConvTy != Sema::Compatible)
        return ConvTy;

      return Sema::IncompatiblePointerSign;
    }

    // If we are a multi-level pointer, it's possible that our issue is simply
    // one of qualification - e.g. char ** -> const char ** is not allowed. If
    // the eventual target type is the same and the pointers have the same
    // level of indirection, this must be the issue.
    if (isa<PointerType>(lhptee) && isa<PointerType>(rhptee)) {
      do {
        std::tie(lhptee, lhq) =
          cast<PointerType>(lhptee)->getPointeeType().split().asPair();
        std::tie(rhptee, rhq) =
          cast<PointerType>(rhptee)->getPointeeType().split().asPair();

        // Inconsistent address spaces at this point is invalid, even if the
        // address spaces would be compatible.
        // FIXME: This doesn't catch address space mismatches for pointers of
        // different nesting levels, like:
        //   __local int *** a;
        //   int ** b = a;
        // It's not clear how to actually determine when such pointers are
        // invalidly incompatible.
        if (lhq.getAddressSpace() != rhq.getAddressSpace())
          return Sema::IncompatibleNestedPointerAddressSpaceMismatch;

      } while (isa<PointerType>(lhptee) && isa<PointerType>(rhptee));

      if (lhptee == rhptee)
        return Sema::IncompatibleNestedPointerQualifiers;
    }

    // General pointer incompatibility takes priority over qualifiers.
    if (RHSType->isFunctionPointerType() && LHSType->isFunctionPointerType())
      return Sema::IncompatibleFunctionPointer;
    return Sema::IncompatiblePointer;
  }
  if (!S.getLangOpts().CPlusPlus &&
      S.IsFunctionConversion(ltrans, rtrans, ltrans))
    return Sema::IncompatibleFunctionPointer;
  if (IsInvalidCmseNSCallConversion(S, ltrans, rtrans))
    return Sema::IncompatibleFunctionPointer;
  if (S.IsInvalidSMECallConversion(
          rtrans, ltrans,
          Sema::AArch64SMECallConversionKind::MayDropPreservesZA))
    return Sema::IncompatibleFunctionPointer;
  return ConvTy;
}

/// checkBlockPointerTypesForAssignment - This routine determines whether two
/// block pointer types are compatible or whether a block and normal pointer
/// are compatible. It is more restrict than comparing two function pointer
// types.
static Sema::AssignConvertType
checkBlockPointerTypesForAssignment(Sema &S, QualType LHSType,
                                    QualType RHSType) {
  assert(LHSType.isCanonical() && "LHS not canonicalized!");
  assert(RHSType.isCanonical() && "RHS not canonicalized!");

  QualType lhptee, rhptee;

  // get the "pointed to" type (ignoring qualifiers at the top level)
  lhptee = cast<BlockPointerType>(LHSType)->getPointeeType();
  rhptee = cast<BlockPointerType>(RHSType)->getPointeeType();

  // In C++, the types have to match exactly.
  if (S.getLangOpts().CPlusPlus)
    return Sema::IncompatibleBlockPointer;

  Sema::AssignConvertType ConvTy = Sema::Compatible;

  // For blocks we enforce that qualifiers are identical.
  Qualifiers LQuals = lhptee.getLocalQualifiers();
  Qualifiers RQuals = rhptee.getLocalQualifiers();
  if (S.getLangOpts().OpenCL) {
    LQuals.removeAddressSpace();
    RQuals.removeAddressSpace();
  }
  if (LQuals != RQuals)
    ConvTy = Sema::CompatiblePointerDiscardsQualifiers;

  // FIXME: OpenCL doesn't define the exact compile time semantics for a block
  // assignment.
  // The current behavior is similar to C++ lambdas. A block might be
  // assigned to a variable iff its return type and parameters are compatible
  // (C99 6.2.7) with the corresponding return type and parameters of the LHS of
  // an assignment. Presumably it should behave in way that a function pointer
  // assignment does in C, so for each parameter and return type:
  //  * CVR and address space of LHS should be a superset of CVR and address
  //  space of RHS.
  //  * unqualified types should be compatible.
  if (S.getLangOpts().OpenCL) {
    if (!S.Context.typesAreBlockPointerCompatible(
            S.Context.getQualifiedType(LHSType.getUnqualifiedType(), LQuals),
            S.Context.getQualifiedType(RHSType.getUnqualifiedType(), RQuals)))
      return Sema::IncompatibleBlockPointer;
  } else if (!S.Context.typesAreBlockPointerCompatible(LHSType, RHSType))
    return Sema::IncompatibleBlockPointer;

  return ConvTy;
}

/// checkObjCPointerTypesForAssignment - Compares two objective-c pointer types
/// for assignment compatibility.
static Sema::AssignConvertType
checkObjCPointerTypesForAssignment(Sema &S, QualType LHSType,
                                   QualType RHSType) {
  assert(LHSType.isCanonical() && "LHS was not canonicalized!");
  assert(RHSType.isCanonical() && "RHS was not canonicalized!");

  if (LHSType->isObjCBuiltinType()) {
    // Class is not compatible with ObjC object pointers.
    if (LHSType->isObjCClassType() && !RHSType->isObjCBuiltinType() &&
        !RHSType->isObjCQualifiedClassType())
      return Sema::IncompatiblePointer;
    return Sema::Compatible;
  }
  if (RHSType->isObjCBuiltinType()) {
    if (RHSType->isObjCClassType() && !LHSType->isObjCBuiltinType() &&
        !LHSType->isObjCQualifiedClassType())
      return Sema::IncompatiblePointer;
    return Sema::Compatible;
  }
  QualType lhptee = LHSType->castAs<ObjCObjectPointerType>()->getPointeeType();
  QualType rhptee = RHSType->castAs<ObjCObjectPointerType>()->getPointeeType();

  if (!lhptee.isAtLeastAsQualifiedAs(rhptee) &&
      // make an exception for id<P>
      !LHSType->isObjCQualifiedIdType())
    return Sema::CompatiblePointerDiscardsQualifiers;

  if (S.Context.typesAreCompatible(LHSType, RHSType))
    return Sema::Compatible;
  if (LHSType->isObjCQualifiedIdType() || RHSType->isObjCQualifiedIdType())
    return Sema::IncompatibleObjCQualifiedId;
  return Sema::IncompatiblePointer;
}

Sema::AssignConvertType
Sema::CheckAssignmentConstraints(SourceLocation Loc,
                                 QualType LHSType, QualType RHSType) {
  // Fake up an opaque expression.  We don't actually care about what
  // cast operations are required, so if CheckAssignmentConstraints
  // adds casts to this they'll be wasted, but fortunately that doesn't
  // usually happen on valid code.
  OpaqueValueExpr RHSExpr(Loc, RHSType, VK_PRValue);
  ExprResult RHSPtr = &RHSExpr;
  CastKind K;

  return CheckAssignmentConstraints(LHSType, RHSPtr, K, /*ConvertRHS=*/false);
}

/// This helper function returns true if QT is a vector type that has element
/// type ElementType.
static bool isVector(QualType QT, QualType ElementType) {
  if (const VectorType *VT = QT->getAs<VectorType>())
    return VT->getElementType().getCanonicalType() == ElementType;
  return false;
}

/// CheckAssignmentConstraints (C99 6.5.16) - This routine currently
/// has code to accommodate several GCC extensions when type checking
/// pointers. Here are some objectionable examples that GCC considers warnings:
///
///  int a, *pint;
///  short *pshort;
///  struct foo *pfoo;
///
///  pint = pshort; // warning: assignment from incompatible pointer type
///  a = pint; // warning: assignment makes integer from pointer without a cast
///  pint = a; // warning: assignment makes pointer from integer without a cast
///  pint = pfoo; // warning: assignment from incompatible pointer type
///
/// As a result, the code for dealing with pointers is more complex than the
/// C99 spec dictates.
///
/// Sets 'Kind' for any result kind except Incompatible.
Sema::AssignConvertType
Sema::CheckAssignmentConstraints(QualType LHSType, ExprResult &RHS,
                                 CastKind &Kind, bool ConvertRHS) {
  QualType RHSType = RHS.get()->getType();
  QualType OrigLHSType = LHSType;

  // Get canonical types.  We're not formatting these types, just comparing
  // them.
  LHSType = Context.getCanonicalType(LHSType).getUnqualifiedType();
  RHSType = Context.getCanonicalType(RHSType).getUnqualifiedType();

  // Common case: no conversion required.
  if (LHSType == RHSType) {
    Kind = CK_NoOp;
    return Compatible;
  }

  // If the LHS has an __auto_type, there are no additional type constraints
  // to be worried about.
  if (const auto *AT = dyn_cast<AutoType>(LHSType)) {
    if (AT->isGNUAutoType()) {
      Kind = CK_NoOp;
      return Compatible;
    }
  }

  // If we have an atomic type, try a non-atomic assignment, then just add an
  // atomic qualification step.
  if (const AtomicType *AtomicTy = dyn_cast<AtomicType>(LHSType)) {
    Sema::AssignConvertType result =
      CheckAssignmentConstraints(AtomicTy->getValueType(), RHS, Kind);
    if (result != Compatible)
      return result;
    if (Kind != CK_NoOp && ConvertRHS)
      RHS = ImpCastExprToType(RHS.get(), AtomicTy->getValueType(), Kind);
    Kind = CK_NonAtomicToAtomic;
    return Compatible;
  }

  // If the left-hand side is a reference type, then we are in a
  // (rare!) case where we've allowed the use of references in C,
  // e.g., as a parameter type in a built-in function. In this case,
  // just make sure that the type referenced is compatible with the
  // right-hand side type. The caller is responsible for adjusting
  // LHSType so that the resulting expression does not have reference
  // type.
  if (const ReferenceType *LHSTypeRef = LHSType->getAs<ReferenceType>()) {
    if (Context.typesAreCompatible(LHSTypeRef->getPointeeType(), RHSType)) {
      Kind = CK_LValueBitCast;
      return Compatible;
    }
    return Incompatible;
  }

  // Allow scalar to ExtVector assignments, and assignments of an ExtVector type
  // to the same ExtVector type.
  if (LHSType->isExtVectorType()) {
    if (RHSType->isExtVectorType())
      return Incompatible;
    if (RHSType->isArithmeticType()) {
      // CK_VectorSplat does T -> vector T, so first cast to the element type.
      if (ConvertRHS)
        RHS = prepareVectorSplat(LHSType, RHS.get());
      Kind = CK_VectorSplat;
      return Compatible;
    }
  }

  // Conversions to or from vector type.
  if (LHSType->isVectorType() || RHSType->isVectorType()) {
    if (LHSType->isVectorType() && RHSType->isVectorType()) {
      // Allow assignments of an AltiVec vector type to an equivalent GCC
      // vector type and vice versa
      if (Context.areCompatibleVectorTypes(LHSType, RHSType)) {
        Kind = CK_BitCast;
        return Compatible;
      }

      // If we are allowing lax vector conversions, and LHS and RHS are both
      // vectors, the total size only needs to be the same. This is a bitcast;
      // no bits are changed but the result type is different.
      if (isLaxVectorConversion(RHSType, LHSType)) {
        // The default for lax vector conversions with Altivec vectors will
        // change, so if we are converting between vector types where
        // at least one is an Altivec vector, emit a warning.
        if (Context.getTargetInfo().getTriple().isPPC() &&
            anyAltivecTypes(RHSType, LHSType) &&
            !Context.areCompatibleVectorTypes(RHSType, LHSType))
          Diag(RHS.get()->getExprLoc(), diag::warn_deprecated_lax_vec_conv_all)
              << RHSType << LHSType;
        Kind = CK_BitCast;
        return IncompatibleVectors;
      }
    }

    // When the RHS comes from another lax conversion (e.g. binops between
    // scalars and vectors) the result is canonicalized as a vector. When the
    // LHS is also a vector, the lax is allowed by the condition above. Handle
    // the case where LHS is a scalar.
    if (LHSType->isScalarType()) {
      const VectorType *VecType = RHSType->getAs<VectorType>();
      if (VecType && VecType->getNumElements() == 1 &&
          isLaxVectorConversion(RHSType, LHSType)) {
        if (Context.getTargetInfo().getTriple().isPPC() &&
            (VecType->getVectorKind() == VectorType::AltiVecVector ||
             VecType->getVectorKind() == VectorType::AltiVecBool ||
             VecType->getVectorKind() == VectorType::AltiVecPixel))
          Diag(RHS.get()->getExprLoc(), diag::warn_deprecated_lax_vec_conv_all)
              << RHSType << LHSType;
        ExprResult *VecExpr = &RHS;
        *VecExpr = ImpCastExprToType(VecExpr->get(), LHSType, CK_BitCast);
        Kind = CK_BitCast;
        return Compatible;
      }
    }

    // Allow assignments between fixed-length and sizeless SVE vectors.
    if ((LHSType->isSVESizelessBuiltinType() && RHSType->isVectorType()) ||
        (LHSType->isVectorType() && RHSType->isSVESizelessBuiltinType()))
      if (Context.areCompatibleSveTypes(LHSType, RHSType) ||
          Context.areLaxCompatibleSveTypes(LHSType, RHSType)) {
        Kind = CK_BitCast;
        return Compatible;
      }

    // Allow assignments between fixed-length and sizeless RVV vectors.
    if ((LHSType->isRVVSizelessBuiltinType() && RHSType->isVectorType()) ||
        (LHSType->isVectorType() && RHSType->isRVVSizelessBuiltinType())) {
      if (Context.areCompatibleRVVTypes(LHSType, RHSType) ||
          Context.areLaxCompatibleRVVTypes(LHSType, RHSType)) {
        Kind = CK_BitCast;
        return Compatible;
      }
    }

    return Incompatible;
  }

  // Diagnose attempts to convert between __ibm128, __float128 and long double
  // where such conversions currently can't be handled.
  if (unsupportedTypeConversion(*this, LHSType, RHSType))
    return Incompatible;

  // Disallow assigning a _Complex to a real type in C++ mode since it simply
  // discards the imaginary part.
  if (getLangOpts().CPlusPlus && RHSType->getAs<ComplexType>() &&
      !LHSType->getAs<ComplexType>())
    return Incompatible;

  // Arithmetic conversions.
  if (LHSType->isArithmeticType() && RHSType->isArithmeticType() &&
      !(getLangOpts().CPlusPlus && LHSType->isEnumeralType())) {
    if (ConvertRHS)
      Kind = PrepareScalarCast(RHS, LHSType);
    return Compatible;
  }

  // Conversions to normal pointers.
  if (const PointerType *LHSPointer = dyn_cast<PointerType>(LHSType)) {
    // U* -> T*
    if (isa<PointerType>(RHSType)) {
      LangAS AddrSpaceL = LHSPointer->getPointeeType().getAddressSpace();
      LangAS AddrSpaceR = RHSType->getPointeeType().getAddressSpace();
      if (AddrSpaceL != AddrSpaceR)
        Kind = CK_AddressSpaceConversion;
      else if (Context.hasCvrSimilarType(RHSType, LHSType))
        Kind = CK_NoOp;
      else
        Kind = CK_BitCast;
      return checkPointerTypesForAssignment(*this, LHSType, RHSType,
                                            RHS.get()->getBeginLoc());
    }

    // int -> T*
    if (RHSType->isIntegerType()) {
      Kind = CK_IntegralToPointer; // FIXME: null?
      return IntToPointer;
    }

    // C pointers are not compatible with ObjC object pointers,
    // with two exceptions:
    if (isa<ObjCObjectPointerType>(RHSType)) {
      //  - conversions to void*
      if (LHSPointer->getPointeeType()->isVoidType()) {
        Kind = CK_BitCast;
        return Compatible;
      }

      //  - conversions from 'Class' to the redefinition type
      if (RHSType->isObjCClassType() &&
          Context.hasSameType(LHSType,
                              Context.getObjCClassRedefinitionType())) {
        Kind = CK_BitCast;
        return Compatible;
      }

      Kind = CK_BitCast;
      return IncompatiblePointer;
    }

    // U^ -> void*
    if (RHSType->getAs<BlockPointerType>()) {
      if (LHSPointer->getPointeeType()->isVoidType()) {
        LangAS AddrSpaceL = LHSPointer->getPointeeType().getAddressSpace();
        LangAS AddrSpaceR = RHSType->getAs<BlockPointerType>()
                                ->getPointeeType()
                                .getAddressSpace();
        Kind =
            AddrSpaceL != AddrSpaceR ? CK_AddressSpaceConversion : CK_BitCast;
        return Compatible;
      }
    }

    return Incompatible;
  }

  // Conversions to block pointers.
  if (isa<BlockPointerType>(LHSType)) {
    // U^ -> T^
    if (RHSType->isBlockPointerType()) {
      LangAS AddrSpaceL = LHSType->getAs<BlockPointerType>()
                              ->getPointeeType()
                              .getAddressSpace();
      LangAS AddrSpaceR = RHSType->getAs<BlockPointerType>()
                              ->getPointeeType()
                              .getAddressSpace();
      Kind = AddrSpaceL != AddrSpaceR ? CK_AddressSpaceConversion : CK_BitCast;
      return checkBlockPointerTypesForAssignment(*this, LHSType, RHSType);
    }

    // int or null -> T^
    if (RHSType->isIntegerType()) {
      Kind = CK_IntegralToPointer; // FIXME: null
      return IntToBlockPointer;
    }

    // id -> T^
    if (getLangOpts().ObjC && RHSType->isObjCIdType()) {
      Kind = CK_AnyPointerToBlockPointerCast;
      return Compatible;
    }

    // void* -> T^
    if (const PointerType *RHSPT = RHSType->getAs<PointerType>())
      if (RHSPT->getPointeeType()->isVoidType()) {
        Kind = CK_AnyPointerToBlockPointerCast;
        return Compatible;
      }

    return Incompatible;
  }

  // Conversions to Objective-C pointers.
  if (isa<ObjCObjectPointerType>(LHSType)) {
    // A* -> B*
    if (RHSType->isObjCObjectPointerType()) {
      Kind = CK_BitCast;
      Sema::AssignConvertType result =
        checkObjCPointerTypesForAssignment(*this, LHSType, RHSType);
      if (getLangOpts().allowsNonTrivialObjCLifetimeQualifiers() &&
          result == Compatible &&
          !CheckObjCARCUnavailableWeakConversion(OrigLHSType, RHSType))
        result = IncompatibleObjCWeakRef;
      return result;
    }

    // int or null -> A*
    if (RHSType->isIntegerType()) {
      Kind = CK_IntegralToPointer; // FIXME: null
      return IntToPointer;
    }

    // In general, C pointers are not compatible with ObjC object pointers,
    // with two exceptions:
    if (isa<PointerType>(RHSType)) {
      Kind = CK_CPointerToObjCPointerCast;

      //  - conversions from 'void*'
      if (RHSType->isVoidPointerType()) {
        return Compatible;
      }

      //  - conversions to 'Class' from its redefinition type
      if (LHSType->isObjCClassType() &&
          Context.hasSameType(RHSType,
                              Context.getObjCClassRedefinitionType())) {
        return Compatible;
      }

      return IncompatiblePointer;
    }

    // Only under strict condition T^ is compatible with an Objective-C pointer.
    if (RHSType->isBlockPointerType() &&
        LHSType->isBlockCompatibleObjCPointerType(Context)) {
      if (ConvertRHS)
        maybeExtendBlockObject(RHS);
      Kind = CK_BlockPointerToObjCPointerCast;
      return Compatible;
    }

    return Incompatible;
  }

  // Conversion to nullptr_t (C23 only)
  if (getLangOpts().C23 && LHSType->isNullPtrType() &&
      RHS.get()->isNullPointerConstant(Context,
                                       Expr::NPC_ValueDependentIsNull)) {
    // null -> nullptr_t
    Kind = CK_NullToPointer;
    return Compatible;
  }

  // Conversions from pointers that are not covered by the above.
  if (isa<PointerType>(RHSType)) {
    // T* -> _Bool
    if (LHSType == Context.BoolTy) {
      Kind = CK_PointerToBoolean;
      return Compatible;
    }

    // T* -> int
    if (LHSType->isIntegerType()) {
      Kind = CK_PointerToIntegral;
      return PointerToInt;
    }

    return Incompatible;
  }

  // Conversions from Objective-C pointers that are not covered by the above.
  if (isa<ObjCObjectPointerType>(RHSType)) {
    // T* -> _Bool
    if (LHSType == Context.BoolTy) {
      Kind = CK_PointerToBoolean;
      return Compatible;
    }

    // T* -> int
    if (LHSType->isIntegerType()) {
      Kind = CK_PointerToIntegral;
      return PointerToInt;
    }

    return Incompatible;
  }

  // struct A -> struct B
  if (isa<TagType>(LHSType) && isa<TagType>(RHSType)) {
    if (Context.typesAreCompatible(LHSType, RHSType)) {
      Kind = CK_NoOp;
      return Compatible;
    }
  }

  if (LHSType->isSamplerT() && RHSType->isIntegerType()) {
    Kind = CK_IntToOCLSampler;
    return Compatible;
  }

  return Incompatible;
}

/// Constructs a transparent union from an expression that is
/// used to initialize the transparent union.
static void ConstructTransparentUnion(Sema &S, ASTContext &C,
                                      ExprResult &EResult, QualType UnionType,
                                      FieldDecl *Field) {
  // Build an initializer list that designates the appropriate member
  // of the transparent union.
  Expr *E = EResult.get();
  InitListExpr *Initializer = new (C) InitListExpr(C, SourceLocation(),
                                                   E, SourceLocation());
  Initializer->setType(UnionType);
  Initializer->setInitializedFieldInUnion(Field);

  // Build a compound literal constructing a value of the transparent
  // union type from this initializer list.
  TypeSourceInfo *unionTInfo = C.getTrivialTypeSourceInfo(UnionType);
  EResult = new (C) CompoundLiteralExpr(SourceLocation(), unionTInfo, UnionType,
                                        VK_PRValue, Initializer, false);
}

Sema::AssignConvertType
Sema::CheckTransparentUnionArgumentConstraints(QualType ArgType,
                                               ExprResult &RHS) {
  QualType RHSType = RHS.get()->getType();

  // If the ArgType is a Union type, we want to handle a potential
  // transparent_union GCC extension.
  const RecordType *UT = ArgType->getAsUnionType();
  if (!UT || !UT->getDecl()->hasAttr<TransparentUnionAttr>())
    return Incompatible;

  // The field to initialize within the transparent union.
  RecordDecl *UD = UT->getDecl();
  FieldDecl *InitField = nullptr;
  // It's compatible if the expression matches any of the fields.
  for (auto *it : UD->fields()) {
    if (it->getType()->isPointerType()) {
      // If the transparent union contains a pointer type, we allow:
      // 1) void pointer
      // 2) null pointer constant
      if (RHSType->isPointerType())
        if (RHSType->castAs<PointerType>()->getPointeeType()->isVoidType()) {
          RHS = ImpCastExprToType(RHS.get(), it->getType(), CK_BitCast);
          InitField = it;
          break;
        }

      if (RHS.get()->isNullPointerConstant(Context,
                                           Expr::NPC_ValueDependentIsNull)) {
        RHS = ImpCastExprToType(RHS.get(), it->getType(),
                                CK_NullToPointer);
        InitField = it;
        break;
      }
    }

    CastKind Kind;
    if (CheckAssignmentConstraints(it->getType(), RHS, Kind)
          == Compatible) {
      RHS = ImpCastExprToType(RHS.get(), it->getType(), Kind);
      InitField = it;
      break;
    }
  }

  if (!InitField)
    return Incompatible;

  ConstructTransparentUnion(*this, Context, RHS, ArgType, InitField);
  return Compatible;
}

Sema::AssignConvertType
Sema::CheckSingleAssignmentConstraints(QualType LHSType, ExprResult &CallerRHS,
                                       bool Diagnose,
                                       bool DiagnoseCFAudited,
                                       bool ConvertRHS) {
  // We need to be able to tell the caller whether we diagnosed a problem, if
  // they ask us to issue diagnostics.
  assert((ConvertRHS || !Diagnose) && "can't indicate whether we diagnosed");

  // If ConvertRHS is false, we want to leave the caller's RHS untouched. Sadly,
  // we can't avoid *all* modifications at the moment, so we need some somewhere
  // to put the updated value.
  ExprResult LocalRHS = CallerRHS;
  ExprResult &RHS = ConvertRHS ? CallerRHS : LocalRHS;

  if (const auto *LHSPtrType = LHSType->getAs<PointerType>()) {
    if (const auto *RHSPtrType = RHS.get()->getType()->getAs<PointerType>()) {
      if (RHSPtrType->getPointeeType()->hasAttr(attr::NoDeref) &&
          !LHSPtrType->getPointeeType()->hasAttr(attr::NoDeref)) {
        Diag(RHS.get()->getExprLoc(),
             diag::warn_noderef_to_dereferenceable_pointer)
            << RHS.get()->getSourceRange();
      }
    }
  }

  if (getLangOpts().CPlusPlus) {
    if (!LHSType->isRecordType() && !LHSType->isAtomicType()) {
      // C++ 5.17p3: If the left operand is not of class type, the
      // expression is implicitly converted (C++ 4) to the
      // cv-unqualified type of the left operand.
      QualType RHSType = RHS.get()->getType();
      if (Diagnose) {
        RHS = PerformImplicitConversion(RHS.get(), LHSType.getUnqualifiedType(),
                                        AA_Assigning);
      } else {
        ImplicitConversionSequence ICS =
            TryImplicitConversion(RHS.get(), LHSType.getUnqualifiedType(),
                                  /*SuppressUserConversions=*/false,
                                  AllowedExplicit::None,
                                  /*InOverloadResolution=*/false,
                                  /*CStyle=*/false,
                                  /*AllowObjCWritebackConversion=*/false);
        if (ICS.isFailure())
          return Incompatible;
        RHS = PerformImplicitConversion(RHS.get(), LHSType.getUnqualifiedType(),
                                        ICS, AA_Assigning);
      }
      if (RHS.isInvalid())
        return Incompatible;
      Sema::AssignConvertType result = Compatible;
      if (getLangOpts().allowsNonTrivialObjCLifetimeQualifiers() &&
          !CheckObjCARCUnavailableWeakConversion(LHSType, RHSType))
        result = IncompatibleObjCWeakRef;
      return result;
    }

    // FIXME: Currently, we fall through and treat C++ classes like C
    // structures.
    // FIXME: We also fall through for atomics; not sure what should
    // happen there, though.
  } else if (RHS.get()->getType() == Context.OverloadTy) {
    // As a set of extensions to C, we support overloading on functions. These
    // functions need to be resolved here.
    DeclAccessPair DAP;
    if (FunctionDecl *FD = ResolveAddressOfOverloadedFunction(
            RHS.get(), LHSType, /*Complain=*/false, DAP))
      RHS = FixOverloadedFunctionReference(RHS.get(), DAP, FD);
    else
      return Incompatible;
  }

  // This check seems unnatural, however it is necessary to ensure the proper
  // conversion of functions/arrays. If the conversion were done for all
  // DeclExpr's (created by ActOnIdExpression), it would mess up the unary
  // expressions that suppress this implicit conversion (&, sizeof). This needs
  // to happen before we check for null pointer conversions because C does not
  // undergo the same implicit conversions as C++ does above (by the calls to
  // TryImplicitConversion() and PerformImplicitConversion()) which insert the
  // lvalue to rvalue cast before checking for null pointer constraints. This
  // addresses code like: nullptr_t val; int *ptr; ptr = val;
  //
  // Suppress this for references: C++ 8.5.3p5.
  if (!LHSType->isReferenceType()) {
    // FIXME: We potentially allocate here even if ConvertRHS is false.
    RHS = DefaultFunctionArrayLvalueConversion(RHS.get(), Diagnose);
    if (RHS.isInvalid())
      return Incompatible;
  }

  // The constraints are expressed in terms of the atomic, qualified, or
  // unqualified type of the LHS.
  QualType LHSTypeAfterConversion = LHSType.getAtomicUnqualifiedType();

  // C99 6.5.16.1p1: the left operand is a pointer and the right is
  // a null pointer constant <C23>or its type is nullptr_t;</C23>.
  if ((LHSTypeAfterConversion->isPointerType() ||
       LHSTypeAfterConversion->isObjCObjectPointerType() ||
       LHSTypeAfterConversion->isBlockPointerType()) &&
      ((getLangOpts().C23 && RHS.get()->getType()->isNullPtrType()) ||
       RHS.get()->isNullPointerConstant(Context,
                                        Expr::NPC_ValueDependentIsNull))) {
    if (Diagnose || ConvertRHS) {
      CastKind Kind;
      CXXCastPath Path;
      CheckPointerConversion(RHS.get(), LHSType, Kind, Path,
                             /*IgnoreBaseAccess=*/false, Diagnose);
      if (ConvertRHS)
        RHS = ImpCastExprToType(RHS.get(), LHSType, Kind, VK_PRValue, &Path);
    }
    return Compatible;
  }
  // C23 6.5.16.1p1: the left operand has type atomic, qualified, or
  // unqualified bool, and the right operand is a pointer or its type is
  // nullptr_t.
  if (getLangOpts().C23 && LHSType->isBooleanType() &&
      RHS.get()->getType()->isNullPtrType()) {
    // NB: T* -> _Bool is handled in CheckAssignmentConstraints, this only
    // only handles nullptr -> _Bool due to needing an extra conversion
    // step.
    // We model this by converting from nullptr -> void * and then let the
    // conversion from void * -> _Bool happen naturally.
    if (Diagnose || ConvertRHS) {
      CastKind Kind;
      CXXCastPath Path;
      CheckPointerConversion(RHS.get(), Context.VoidPtrTy, Kind, Path,
                             /*IgnoreBaseAccess=*/false, Diagnose);
      if (ConvertRHS)
        RHS = ImpCastExprToType(RHS.get(), Context.VoidPtrTy, Kind, VK_PRValue,
                                &Path);
    }
  }

  // OpenCL queue_t type assignment.
  if (LHSType->isQueueT() && RHS.get()->isNullPointerConstant(
                                 Context, Expr::NPC_ValueDependentIsNull)) {
    RHS = ImpCastExprToType(RHS.get(), LHSType, CK_NullToPointer);
    return Compatible;
  }

  CastKind Kind;
  Sema::AssignConvertType result =
    CheckAssignmentConstraints(LHSType, RHS, Kind, ConvertRHS);

  // C99 6.5.16.1p2: The value of the right operand is converted to the
  // type of the assignment expression.
  // CheckAssignmentConstraints allows the left-hand side to be a reference,
  // so that we can use references in built-in functions even in C.
  // The getNonReferenceType() call makes sure that the resulting expression
  // does not have reference type.
  if (result != Incompatible && RHS.get()->getType() != LHSType) {
    QualType Ty = LHSType.getNonLValueExprType(Context);
    Expr *E = RHS.get();

    // Check for various Objective-C errors. If we are not reporting
    // diagnostics and just checking for errors, e.g., during overload
    // resolution, return Incompatible to indicate the failure.
    if (getLangOpts().allowsNonTrivialObjCLifetimeQualifiers() &&
        CheckObjCConversion(SourceRange(), Ty, E, CCK_ImplicitConversion,
                            Diagnose, DiagnoseCFAudited) != ACR_okay) {
      if (!Diagnose)
        return Incompatible;
    }
    if (getLangOpts().ObjC &&
        (CheckObjCBridgeRelatedConversions(E->getBeginLoc(), LHSType,
                                           E->getType(), E, Diagnose) ||
         CheckConversionToObjCLiteral(LHSType, E, Diagnose))) {
      if (!Diagnose)
        return Incompatible;
      // Replace the expression with a corrected version and continue so we
      // can find further errors.
      RHS = E;
      return Compatible;
    }

    if (ConvertRHS)
      RHS = ImpCastExprToType(E, Ty, Kind);
  }

  return result;
}

namespace {
/// The original operand to an operator, prior to the application of the usual
/// arithmetic conversions and converting the arguments of a builtin operator
/// candidate.
struct OriginalOperand {
  explicit OriginalOperand(Expr *Op) : Orig(Op), Conversion(nullptr) {
    if (auto *MTE = dyn_cast<MaterializeTemporaryExpr>(Op))
      Op = MTE->getSubExpr();
    if (auto *BTE = dyn_cast<CXXBindTemporaryExpr>(Op))
      Op = BTE->getSubExpr();
    if (auto *ICE = dyn_cast<ImplicitCastExpr>(Op)) {
      Orig = ICE->getSubExprAsWritten();
      Conversion = ICE->getConversionFunction();
    }
  }

  QualType getType() const { return Orig->getType(); }

  Expr *Orig;
  NamedDecl *Conversion;
};
}

QualType Sema::InvalidOperands(SourceLocation Loc, ExprResult &LHS,
                               ExprResult &RHS) {
  OriginalOperand OrigLHS(LHS.get()), OrigRHS(RHS.get());

  Diag(Loc, diag::err_typecheck_invalid_operands)
    << OrigLHS.getType() << OrigRHS.getType()
    << LHS.get()->getSourceRange() << RHS.get()->getSourceRange();

  // If a user-defined conversion was applied to either of the operands prior
  // to applying the built-in operator rules, tell the user about it.
  if (OrigLHS.Conversion) {
    Diag(OrigLHS.Conversion->getLocation(),
         diag::note_typecheck_invalid_operands_converted)
      << 0 << LHS.get()->getType();
  }
  if (OrigRHS.Conversion) {
    Diag(OrigRHS.Conversion->getLocation(),
         diag::note_typecheck_invalid_operands_converted)
      << 1 << RHS.get()->getType();
  }

  return QualType();
}

// Diagnose cases where a scalar was implicitly converted to a vector and
// diagnose the underlying types. Otherwise, diagnose the error
// as invalid vector logical operands for non-C++ cases.
QualType Sema::InvalidLogicalVectorOperands(SourceLocation Loc, ExprResult &LHS,
                                            ExprResult &RHS) {
  QualType LHSType = LHS.get()->IgnoreImpCasts()->getType();
  QualType RHSType = RHS.get()->IgnoreImpCasts()->getType();

  bool LHSNatVec = LHSType->isVectorType();
  bool RHSNatVec = RHSType->isVectorType();

  if (!(LHSNatVec && RHSNatVec)) {
    Expr *Vector = LHSNatVec ? LHS.get() : RHS.get();
    Expr *NonVector = !LHSNatVec ? LHS.get() : RHS.get();
    Diag(Loc, diag::err_typecheck_logical_vector_expr_gnu_cpp_restrict)
        << 0 << Vector->getType() << NonVector->IgnoreImpCasts()->getType()
        << Vector->getSourceRange();
    return QualType();
  }

  Diag(Loc, diag::err_typecheck_logical_vector_expr_gnu_cpp_restrict)
      << 1 << LHSType << RHSType << LHS.get()->getSourceRange()
      << RHS.get()->getSourceRange();

  return QualType();
}

/// Try to convert a value of non-vector type to a vector type by converting
/// the type to the element type of the vector and then performing a splat.
/// If the language is OpenCL, we only use conversions that promote scalar
/// rank; for C, Obj-C, and C++ we allow any real scalar conversion except
/// for float->int.
///
/// OpenCL V2.0 6.2.6.p2:
/// An error shall occur if any scalar operand type has greater rank
/// than the type of the vector element.
///
/// \param scalar - if non-null, actually perform the conversions
/// \return true if the operation fails (but without diagnosing the failure)
static bool tryVectorConvertAndSplat(Sema &S, ExprResult *scalar,
                                     QualType scalarTy,
                                     QualType vectorEltTy,
                                     QualType vectorTy,
                                     unsigned &DiagID) {
  // The conversion to apply to the scalar before splatting it,
  // if necessary.
  CastKind scalarCast = CK_NoOp;

  if (vectorEltTy->isIntegralType(S.Context)) {
    if (S.getLangOpts().OpenCL && (scalarTy->isRealFloatingType() ||
        (scalarTy->isIntegerType() &&
         S.Context.getIntegerTypeOrder(vectorEltTy, scalarTy) < 0))) {
      DiagID = diag::err_opencl_scalar_type_rank_greater_than_vector_type;
      return true;
    }
    if (!scalarTy->isIntegralType(S.Context))
      return true;
    scalarCast = CK_IntegralCast;
  } else if (vectorEltTy->isRealFloatingType()) {
    if (scalarTy->isRealFloatingType()) {
      if (S.getLangOpts().OpenCL &&
          S.Context.getFloatingTypeOrder(vectorEltTy, scalarTy) < 0) {
        DiagID = diag::err_opencl_scalar_type_rank_greater_than_vector_type;
        return true;
      }
      scalarCast = CK_FloatingCast;
    }
    else if (scalarTy->isIntegralType(S.Context))
      scalarCast = CK_IntegralToFloating;
    else
      return true;
  } else {
    return true;
  }

  // Adjust scalar if desired.
  if (scalar) {
    if (scalarCast != CK_NoOp)
      *scalar = S.ImpCastExprToType(scalar->get(), vectorEltTy, scalarCast);
    *scalar = S.ImpCastExprToType(scalar->get(), vectorTy, CK_VectorSplat);
  }
  return false;
}

/// Convert vector E to a vector with the same number of elements but different
/// element type.
static ExprResult convertVector(Expr *E, QualType ElementType, Sema &S) {
  const auto *VecTy = E->getType()->getAs<VectorType>();
  assert(VecTy && "Expression E must be a vector");
  QualType NewVecTy =
      VecTy->isExtVectorType()
          ? S.Context.getExtVectorType(ElementType, VecTy->getNumElements())
          : S.Context.getVectorType(ElementType, VecTy->getNumElements(),
                                    VecTy->getVectorKind());

  // Look through the implicit cast. Return the subexpression if its type is
  // NewVecTy.
  if (auto *ICE = dyn_cast<ImplicitCastExpr>(E))
    if (ICE->getSubExpr()->getType() == NewVecTy)
      return ICE->getSubExpr();

  auto Cast = ElementType->isIntegerType() ? CK_IntegralCast : CK_FloatingCast;
  return S.ImpCastExprToType(E, NewVecTy, Cast);
}

/// Test if a (constant) integer Int can be casted to another integer type
/// IntTy without losing precision.
static bool canConvertIntToOtherIntTy(Sema &S, ExprResult *Int,
                                      QualType OtherIntTy) {
  QualType IntTy = Int->get()->getType().getUnqualifiedType();

  // Reject cases where the value of the Int is unknown as that would
  // possibly cause truncation, but accept cases where the scalar can be
  // demoted without loss of precision.
  Expr::EvalResult EVResult;
  bool CstInt = Int->get()->EvaluateAsInt(EVResult, S.Context);
  int Order = S.Context.getIntegerTypeOrder(OtherIntTy, IntTy);
  bool IntSigned = IntTy->hasSignedIntegerRepresentation();
  bool OtherIntSigned = OtherIntTy->hasSignedIntegerRepresentation();

  if (CstInt) {
    // If the scalar is constant and is of a higher order and has more active
    // bits that the vector element type, reject it.
    llvm::APSInt Result = EVResult.Val.getInt();
    unsigned NumBits = IntSigned
                           ? (Result.isNegative() ? Result.getSignificantBits()
                                                  : Result.getActiveBits())
                           : Result.getActiveBits();
    if (Order < 0 && S.Context.getIntWidth(OtherIntTy) < NumBits)
      return true;

    // If the signedness of the scalar type and the vector element type
    // differs and the number of bits is greater than that of the vector
    // element reject it.
    return (IntSigned != OtherIntSigned &&
            NumBits > S.Context.getIntWidth(OtherIntTy));
  }

  // Reject cases where the value of the scalar is not constant and it's
  // order is greater than that of the vector element type.
  return (Order < 0);
}

/// Test if a (constant) integer Int can be casted to floating point type
/// FloatTy without losing precision.
static bool canConvertIntTyToFloatTy(Sema &S, ExprResult *Int,
                                     QualType FloatTy) {
  QualType IntTy = Int->get()->getType().getUnqualifiedType();

  // Determine if the integer constant can be expressed as a floating point
  // number of the appropriate type.
  Expr::EvalResult EVResult;
  bool CstInt = Int->get()->EvaluateAsInt(EVResult, S.Context);

  uint64_t Bits = 0;
  if (CstInt) {
    // Reject constants that would be truncated if they were converted to
    // the floating point type. Test by simple to/from conversion.
    // FIXME: Ideally the conversion to an APFloat and from an APFloat
    //        could be avoided if there was a convertFromAPInt method
    //        which could signal back if implicit truncation occurred.
    llvm::APSInt Result = EVResult.Val.getInt();
    llvm::APFloat Float(S.Context.getFloatTypeSemantics(FloatTy));
    Float.convertFromAPInt(Result, IntTy->hasSignedIntegerRepresentation(),
                           llvm::APFloat::rmTowardZero);
    llvm::APSInt ConvertBack(S.Context.getIntWidth(IntTy),
                             !IntTy->hasSignedIntegerRepresentation());
    bool Ignored = false;
    Float.convertToInteger(ConvertBack, llvm::APFloat::rmNearestTiesToEven,
                           &Ignored);
    if (Result != ConvertBack)
      return true;
  } else {
    // Reject types that cannot be fully encoded into the mantissa of
    // the float.
    Bits = S.Context.getTypeSize(IntTy);
    unsigned FloatPrec = llvm::APFloat::semanticsPrecision(
        S.Context.getFloatTypeSemantics(FloatTy));
    if (Bits > FloatPrec)
      return true;
  }

  return false;
}

/// Attempt to convert and splat Scalar into a vector whose types matches
/// Vector following GCC conversion rules. The rule is that implicit
/// conversion can occur when Scalar can be casted to match Vector's element
/// type without causing truncation of Scalar.
static bool tryGCCVectorConvertAndSplat(Sema &S, ExprResult *Scalar,
                                        ExprResult *Vector) {
  QualType ScalarTy = Scalar->get()->getType().getUnqualifiedType();
  QualType VectorTy = Vector->get()->getType().getUnqualifiedType();
  QualType VectorEltTy;

  if (const auto *VT = VectorTy->getAs<VectorType>()) {
    assert(!isa<ExtVectorType>(VT) &&
           "ExtVectorTypes should not be handled here!");
    VectorEltTy = VT->getElementType();
  } else if (VectorTy->isSveVLSBuiltinType()) {
    VectorEltTy =
        VectorTy->castAs<BuiltinType>()->getSveEltType(S.getASTContext());
  } else {
    llvm_unreachable("Only Fixed-Length and SVE Vector types are handled here");
  }

  // Reject cases where the vector element type or the scalar element type are
  // not integral or floating point types.
  if (!VectorEltTy->isArithmeticType() || !ScalarTy->isArithmeticType())
    return true;

  // The conversion to apply to the scalar before splatting it,
  // if necessary.
  CastKind ScalarCast = CK_NoOp;

  // Accept cases where the vector elements are integers and the scalar is
  // an integer.
  // FIXME: Notionally if the scalar was a floating point value with a precise
  //        integral representation, we could cast it to an appropriate integer
  //        type and then perform the rest of the checks here. GCC will perform
  //        this conversion in some cases as determined by the input language.
  //        We should accept it on a language independent basis.
  if (VectorEltTy->isIntegralType(S.Context) &&
      ScalarTy->isIntegralType(S.Context) &&
      S.Context.getIntegerTypeOrder(VectorEltTy, ScalarTy)) {

    if (canConvertIntToOtherIntTy(S, Scalar, VectorEltTy))
      return true;

    ScalarCast = CK_IntegralCast;
  } else if (VectorEltTy->isIntegralType(S.Context) &&
             ScalarTy->isRealFloatingType()) {
    if (S.Context.getTypeSize(VectorEltTy) == S.Context.getTypeSize(ScalarTy))
      ScalarCast = CK_FloatingToIntegral;
    else
      return true;
  } else if (VectorEltTy->isRealFloatingType()) {
    if (ScalarTy->isRealFloatingType()) {

      // Reject cases where the scalar type is not a constant and has a higher
      // Order than the vector element type.
      llvm::APFloat Result(0.0);

      // Determine whether this is a constant scalar. In the event that the
      // value is dependent (and thus cannot be evaluated by the constant
      // evaluator), skip the evaluation. This will then diagnose once the
      // expression is instantiated.
      bool CstScalar = Scalar->get()->isValueDependent() ||
                       Scalar->get()->EvaluateAsFloat(Result, S.Context);
      int Order = S.Context.getFloatingTypeOrder(VectorEltTy, ScalarTy);
      if (!CstScalar && Order < 0)
        return true;

      // If the scalar cannot be safely casted to the vector element type,
      // reject it.
      if (CstScalar) {
        bool Truncated = false;
        Result.convert(S.Context.getFloatTypeSemantics(VectorEltTy),
                       llvm::APFloat::rmNearestTiesToEven, &Truncated);
        if (Truncated)
          return true;
      }

      ScalarCast = CK_FloatingCast;
    } else if (ScalarTy->isIntegralType(S.Context)) {
      if (canConvertIntTyToFloatTy(S, Scalar, VectorEltTy))
        return true;

      ScalarCast = CK_IntegralToFloating;
    } else
      return true;
  } else if (ScalarTy->isEnumeralType())
    return true;

  // Adjust scalar if desired.
  if (ScalarCast != CK_NoOp)
    *Scalar = S.ImpCastExprToType(Scalar->get(), VectorEltTy, ScalarCast);
  *Scalar = S.ImpCastExprToType(Scalar->get(), VectorTy, CK_VectorSplat);
  return false;
}

QualType Sema::CheckVectorOperands(ExprResult &LHS, ExprResult &RHS,
                                   SourceLocation Loc, bool IsCompAssign,
                                   bool AllowBothBool,
                                   bool AllowBoolConversions,
                                   bool AllowBoolOperation,
                                   bool ReportInvalid) {
  if (!IsCompAssign) {
    LHS = DefaultFunctionArrayLvalueConversion(LHS.get());
    if (LHS.isInvalid())
      return QualType();
  }
  RHS = DefaultFunctionArrayLvalueConversion(RHS.get());
  if (RHS.isInvalid())
    return QualType();

  // For conversion purposes, we ignore any qualifiers.
  // For example, "const float" and "float" are equivalent.
  QualType LHSType = LHS.get()->getType().getUnqualifiedType();
  QualType RHSType = RHS.get()->getType().getUnqualifiedType();

  const VectorType *LHSVecType = LHSType->getAs<VectorType>();
  const VectorType *RHSVecType = RHSType->getAs<VectorType>();
  assert(LHSVecType || RHSVecType);

  // AltiVec-style "vector bool op vector bool" combinations are allowed
  // for some operators but not others.
  if (!AllowBothBool &&
      LHSVecType && LHSVecType->getVectorKind() == VectorType::AltiVecBool &&
      RHSVecType && RHSVecType->getVectorKind() == VectorType::AltiVecBool)
    return ReportInvalid ? InvalidOperands(Loc, LHS, RHS) : QualType();

  // This operation may not be performed on boolean vectors.
  if (!AllowBoolOperation &&
      (LHSType->isExtVectorBoolType() || RHSType->isExtVectorBoolType()))
    return ReportInvalid ? InvalidOperands(Loc, LHS, RHS) : QualType();

  // If the vector types are identical, return.
  if (Context.hasSameType(LHSType, RHSType))
    return Context.getCommonSugaredType(LHSType, RHSType);

  // If we have compatible AltiVec and GCC vector types, use the AltiVec type.
  if (LHSVecType && RHSVecType &&
      Context.areCompatibleVectorTypes(LHSType, RHSType)) {
    if (isa<ExtVectorType>(LHSVecType)) {
      RHS = ImpCastExprToType(RHS.get(), LHSType, CK_BitCast);
      return LHSType;
    }

    if (!IsCompAssign)
      LHS = ImpCastExprToType(LHS.get(), RHSType, CK_BitCast);
    return RHSType;
  }

  // AllowBoolConversions says that bool and non-bool AltiVec vectors
  // can be mixed, with the result being the non-bool type.  The non-bool
  // operand must have integer element type.
  if (AllowBoolConversions && LHSVecType && RHSVecType &&
      LHSVecType->getNumElements() == RHSVecType->getNumElements() &&
      (Context.getTypeSize(LHSVecType->getElementType()) ==
       Context.getTypeSize(RHSVecType->getElementType()))) {
    if (LHSVecType->getVectorKind() == VectorType::AltiVecVector &&
        LHSVecType->getElementType()->isIntegerType() &&
        RHSVecType->getVectorKind() == VectorType::AltiVecBool) {
      RHS = ImpCastExprToType(RHS.get(), LHSType, CK_BitCast);
      return LHSType;
    }
    if (!IsCompAssign &&
        LHSVecType->getVectorKind() == VectorType::AltiVecBool &&
        RHSVecType->getVectorKind() == VectorType::AltiVecVector &&
        RHSVecType->getElementType()->isIntegerType()) {
      LHS = ImpCastExprToType(LHS.get(), RHSType, CK_BitCast);
      return RHSType;
    }
  }

  // Expressions containing fixed-length and sizeless SVE/RVV vectors are
  // invalid since the ambiguity can affect the ABI.
  auto IsSveRVVConversion = [](QualType FirstType, QualType SecondType,
                               unsigned &SVEorRVV) {
    const VectorType *VecType = SecondType->getAs<VectorType>();
    SVEorRVV = 0;
    if (FirstType->isSizelessBuiltinType() && VecType) {
      if (VecType->getVectorKind() == VectorType::SveFixedLengthDataVector ||
          VecType->getVectorKind() == VectorType::SveFixedLengthPredicateVector)
        return true;
      if (VecType->getVectorKind() == VectorType::RVVFixedLengthDataVector) {
        SVEorRVV = 1;
        return true;
      }
    }

    return false;
  };

  unsigned SVEorRVV;
  if (IsSveRVVConversion(LHSType, RHSType, SVEorRVV) ||
      IsSveRVVConversion(RHSType, LHSType, SVEorRVV)) {
    Diag(Loc, diag::err_typecheck_sve_rvv_ambiguous)
        << SVEorRVV << LHSType << RHSType;
    return QualType();
  }

  // Expressions containing GNU and SVE or RVV (fixed or sizeless) vectors are
  // invalid since the ambiguity can affect the ABI.
  auto IsSveRVVGnuConversion = [](QualType FirstType, QualType SecondType,
                                  unsigned &SVEorRVV) {
    const VectorType *FirstVecType = FirstType->getAs<VectorType>();
    const VectorType *SecondVecType = SecondType->getAs<VectorType>();

    SVEorRVV = 0;
    if (FirstVecType && SecondVecType) {
      if (FirstVecType->getVectorKind() == VectorType::GenericVector) {
        if (SecondVecType->getVectorKind() ==
                VectorType::SveFixedLengthDataVector ||
            SecondVecType->getVectorKind() ==
                VectorType::SveFixedLengthPredicateVector)
          return true;
        if (SecondVecType->getVectorKind() ==
            VectorType::RVVFixedLengthDataVector) {
          SVEorRVV = 1;
          return true;
        }
      }
      return false;
    }

    if (SecondVecType &&
        SecondVecType->getVectorKind() == VectorType::GenericVector) {
      if (FirstType->isSVESizelessBuiltinType())
        return true;
      if (FirstType->isRVVSizelessBuiltinType()) {
        SVEorRVV = 1;
        return true;
      }
    }

    return false;
  };

  if (IsSveRVVGnuConversion(LHSType, RHSType, SVEorRVV) ||
      IsSveRVVGnuConversion(RHSType, LHSType, SVEorRVV)) {
    Diag(Loc, diag::err_typecheck_sve_rvv_gnu_ambiguous)
        << SVEorRVV << LHSType << RHSType;
    return QualType();
  }

  // If there's a vector type and a scalar, try to convert the scalar to
  // the vector element type and splat.
  unsigned DiagID = diag::err_typecheck_vector_not_convertable;
  if (!RHSVecType) {
    if (isa<ExtVectorType>(LHSVecType)) {
      if (!tryVectorConvertAndSplat(*this, &RHS, RHSType,
                                    LHSVecType->getElementType(), LHSType,
                                    DiagID))
        return LHSType;
    } else {
      if (!tryGCCVectorConvertAndSplat(*this, &RHS, &LHS))
        return LHSType;
    }
  }
  if (!LHSVecType) {
    if (isa<ExtVectorType>(RHSVecType)) {
      if (!tryVectorConvertAndSplat(*this, (IsCompAssign ? nullptr : &LHS),
                                    LHSType, RHSVecType->getElementType(),
                                    RHSType, DiagID))
        return RHSType;
    } else {
      if (LHS.get()->isLValue() ||
          !tryGCCVectorConvertAndSplat(*this, &LHS, &RHS))
        return RHSType;
    }
  }

  // FIXME: The code below also handles conversion between vectors and
  // non-scalars, we should break this down into fine grained specific checks
  // and emit proper diagnostics.
  QualType VecType = LHSVecType ? LHSType : RHSType;
  const VectorType *VT = LHSVecType ? LHSVecType : RHSVecType;
  QualType OtherType = LHSVecType ? RHSType : LHSType;
  ExprResult *OtherExpr = LHSVecType ? &RHS : &LHS;
  if (isLaxVectorConversion(OtherType, VecType)) {
    if (Context.getTargetInfo().getTriple().isPPC() &&
        anyAltivecTypes(RHSType, LHSType) &&
        !Context.areCompatibleVectorTypes(RHSType, LHSType))
      Diag(Loc, diag::warn_deprecated_lax_vec_conv_all) << RHSType << LHSType;
    // If we're allowing lax vector conversions, only the total (data) size
    // needs to be the same. For non compound assignment, if one of the types is
    // scalar, the result is always the vector type.
    if (!IsCompAssign) {
      *OtherExpr = ImpCastExprToType(OtherExpr->get(), VecType, CK_BitCast);
      return VecType;
    // In a compound assignment, lhs += rhs, 'lhs' is a lvalue src, forbidding
    // any implicit cast. Here, the 'rhs' should be implicit casted to 'lhs'
    // type. Note that this is already done by non-compound assignments in
    // CheckAssignmentConstraints. If it's a scalar type, only bitcast for
    // <1 x T> -> T. The result is also a vector type.
    } else if (OtherType->isExtVectorType() || OtherType->isVectorType() ||
               (OtherType->isScalarType() && VT->getNumElements() == 1)) {
      ExprResult *RHSExpr = &RHS;
      *RHSExpr = ImpCastExprToType(RHSExpr->get(), LHSType, CK_BitCast);
      return VecType;
    }
  }

  // Okay, the expression is invalid.

  // If there's a non-vector, non-real operand, diagnose that.
  if ((!RHSVecType && !RHSType->isRealType()) ||
      (!LHSVecType && !LHSType->isRealType())) {
    Diag(Loc, diag::err_typecheck_vector_not_convertable_non_scalar)
      << LHSType << RHSType
      << LHS.get()->getSourceRange() << RHS.get()->getSourceRange();
    return QualType();
  }

  // OpenCL V1.1 6.2.6.p1:
  // If the operands are of more than one vector type, then an error shall
  // occur. Implicit conversions between vector types are not permitted, per
  // section 6.2.1.
  if (getLangOpts().OpenCL &&
      RHSVecType && isa<ExtVectorType>(RHSVecType) &&
      LHSVecType && isa<ExtVectorType>(LHSVecType)) {
    Diag(Loc, diag::err_opencl_implicit_vector_conversion) << LHSType
                                                           << RHSType;
    return QualType();
  }


  // If there is a vector type that is not a ExtVector and a scalar, we reach
  // this point if scalar could not be converted to the vector's element type
  // without truncation.
  if ((RHSVecType && !isa<ExtVectorType>(RHSVecType)) ||
      (LHSVecType && !isa<ExtVectorType>(LHSVecType))) {
    QualType Scalar = LHSVecType ? RHSType : LHSType;
    QualType Vector = LHSVecType ? LHSType : RHSType;
    unsigned ScalarOrVector = LHSVecType && RHSVecType ? 1 : 0;
    Diag(Loc,
         diag::err_typecheck_vector_not_convertable_implict_truncation)
        << ScalarOrVector << Scalar << Vector;

    return QualType();
  }

  // Otherwise, use the generic diagnostic.
  Diag(Loc, DiagID)
    << LHSType << RHSType
    << LHS.get()->getSourceRange() << RHS.get()->getSourceRange();
  return QualType();
}

QualType Sema::CheckSizelessVectorOperands(ExprResult &LHS, ExprResult &RHS,
                                           SourceLocation Loc,
                                           bool IsCompAssign,
                                           ArithConvKind OperationKind) {
  if (!IsCompAssign) {
    LHS = DefaultFunctionArrayLvalueConversion(LHS.get());
    if (LHS.isInvalid())
      return QualType();
  }
  RHS = DefaultFunctionArrayLvalueConversion(RHS.get());
  if (RHS.isInvalid())
    return QualType();

  QualType LHSType = LHS.get()->getType().getUnqualifiedType();
  QualType RHSType = RHS.get()->getType().getUnqualifiedType();

  const BuiltinType *LHSBuiltinTy = LHSType->getAs<BuiltinType>();
  const BuiltinType *RHSBuiltinTy = RHSType->getAs<BuiltinType>();

  unsigned DiagID = diag::err_typecheck_invalid_operands;
  if ((OperationKind == ACK_Arithmetic) &&
      ((LHSBuiltinTy && LHSBuiltinTy->isSVEBool()) ||
       (RHSBuiltinTy && RHSBuiltinTy->isSVEBool()))) {
    Diag(Loc, DiagID) << LHSType << RHSType << LHS.get()->getSourceRange()
                      << RHS.get()->getSourceRange();
    return QualType();
  }

  if (Context.hasSameType(LHSType, RHSType))
    return LHSType;

  if (LHSType->isSveVLSBuiltinType() && !RHSType->isSveVLSBuiltinType()) {
    if (!tryGCCVectorConvertAndSplat(*this, &RHS, &LHS))
      return LHSType;
  }
  if (RHSType->isSveVLSBuiltinType() && !LHSType->isSveVLSBuiltinType()) {
    if (LHS.get()->isLValue() ||
        !tryGCCVectorConvertAndSplat(*this, &LHS, &RHS))
      return RHSType;
  }

  if ((!LHSType->isSveVLSBuiltinType() && !LHSType->isRealType()) ||
      (!RHSType->isSveVLSBuiltinType() && !RHSType->isRealType())) {
    Diag(Loc, diag::err_typecheck_vector_not_convertable_non_scalar)
        << LHSType << RHSType << LHS.get()->getSourceRange()
        << RHS.get()->getSourceRange();
    return QualType();
  }

  if (LHSType->isSveVLSBuiltinType() && RHSType->isSveVLSBuiltinType() &&
      Context.getBuiltinVectorTypeInfo(LHSBuiltinTy).EC !=
          Context.getBuiltinVectorTypeInfo(RHSBuiltinTy).EC) {
    Diag(Loc, diag::err_typecheck_vector_lengths_not_equal)
        << LHSType << RHSType << LHS.get()->getSourceRange()
        << RHS.get()->getSourceRange();
    return QualType();
  }

  if (LHSType->isSveVLSBuiltinType() || RHSType->isSveVLSBuiltinType()) {
    QualType Scalar = LHSType->isSveVLSBuiltinType() ? RHSType : LHSType;
    QualType Vector = LHSType->isSveVLSBuiltinType() ? LHSType : RHSType;
    bool ScalarOrVector =
        LHSType->isSveVLSBuiltinType() && RHSType->isSveVLSBuiltinType();

    Diag(Loc, diag::err_typecheck_vector_not_convertable_implict_truncation)
        << ScalarOrVector << Scalar << Vector;

    return QualType();
  }

  Diag(Loc, DiagID) << LHSType << RHSType << LHS.get()->getSourceRange()
                    << RHS.get()->getSourceRange();
  return QualType();
}

// checkArithmeticNull - Detect when a NULL constant is used improperly in an
// expression.  These are mainly cases where the null pointer is used as an
// integer instead of a pointer.
static void checkArithmeticNull(Sema &S, ExprResult &LHS, ExprResult &RHS,
                                SourceLocation Loc, bool IsCompare) {
  // The canonical way to check for a GNU null is with isNullPointerConstant,
  // but we use a bit of a hack here for speed; this is a relatively
  // hot path, and isNullPointerConstant is slow.
  bool LHSNull = isa<GNUNullExpr>(LHS.get()->IgnoreParenImpCasts());
  bool RHSNull = isa<GNUNullExpr>(RHS.get()->IgnoreParenImpCasts());

  QualType NonNullType = LHSNull ? RHS.get()->getType() : LHS.get()->getType();

  // Avoid analyzing cases where the result will either be invalid (and
  // diagnosed as such) or entirely valid and not something to warn about.
  if ((!LHSNull && !RHSNull) || NonNullType->isBlockPointerType() ||
      NonNullType->isMemberPointerType() || NonNullType->isFunctionType())
    return;

  // Comparison operations would not make sense with a null pointer no matter
  // what the other expression is.
  if (!IsCompare) {
    S.Diag(Loc, diag::warn_null_in_arithmetic_operation)
        << (LHSNull ? LHS.get()->getSourceRange() : SourceRange())
        << (RHSNull ? RHS.get()->getSourceRange() : SourceRange());
    return;
  }

  // The rest of the operations only make sense with a null pointer
  // if the other expression is a pointer.
  if (LHSNull == RHSNull || NonNullType->isAnyPointerType() ||
      NonNullType->canDecayToPointerType())
    return;

  S.Diag(Loc, diag::warn_null_in_comparison_operation)
      << LHSNull /* LHS is NULL */ << NonNullType
      << LHS.get()->getSourceRange() << RHS.get()->getSourceRange();
}

static void DiagnoseDivisionSizeofPointerOrArray(Sema &S, Expr *LHS, Expr *RHS,
                                          SourceLocation Loc) {
  const auto *LUE = dyn_cast<UnaryExprOrTypeTraitExpr>(LHS);
  const auto *RUE = dyn_cast<UnaryExprOrTypeTraitExpr>(RHS);
  if (!LUE || !RUE)
    return;
  if (LUE->getKind() != UETT_SizeOf || LUE->isArgumentType() ||
      RUE->getKind() != UETT_SizeOf)
    return;

  const Expr *LHSArg = LUE->getArgumentExpr()->IgnoreParens();
  QualType LHSTy = LHSArg->getType();
  QualType RHSTy;

  if (RUE->isArgumentType())
    RHSTy = RUE->getArgumentType().getNonReferenceType();
  else
    RHSTy = RUE->getArgumentExpr()->IgnoreParens()->getType();

  if (LHSTy->isPointerType() && !RHSTy->isPointerType()) {
    if (!S.Context.hasSameUnqualifiedType(LHSTy->getPointeeType(), RHSTy))
      return;

    S.Diag(Loc, diag::warn_division_sizeof_ptr) << LHS << LHS->getSourceRange();
    if (const auto *DRE = dyn_cast<DeclRefExpr>(LHSArg)) {
      if (const ValueDecl *LHSArgDecl = DRE->getDecl())
        S.Diag(LHSArgDecl->getLocation(), diag::note_pointer_declared_here)
            << LHSArgDecl;
    }
  } else if (const auto *ArrayTy = S.Context.getAsArrayType(LHSTy)) {
    QualType ArrayElemTy = ArrayTy->getElementType();
    if (ArrayElemTy != S.Context.getBaseElementType(ArrayTy) ||
        ArrayElemTy->isDependentType() || RHSTy->isDependentType() ||
        RHSTy->isReferenceType() || ArrayElemTy->isCharType() ||
        S.Context.getTypeSize(ArrayElemTy) == S.Context.getTypeSize(RHSTy))
      return;
    S.Diag(Loc, diag::warn_division_sizeof_array)
        << LHSArg->getSourceRange() << ArrayElemTy << RHSTy;
    if (const auto *DRE = dyn_cast<DeclRefExpr>(LHSArg)) {
      if (const ValueDecl *LHSArgDecl = DRE->getDecl())
        S.Diag(LHSArgDecl->getLocation(), diag::note_array_declared_here)
            << LHSArgDecl;
    }

    S.Diag(Loc, diag::note_precedence_silence) << RHS;
  }
}

static void DiagnoseBadDivideOrRemainderValues(Sema& S, ExprResult &LHS,
                                               ExprResult &RHS,
                                               SourceLocation Loc, bool IsDiv) {
  // Check for division/remainder by zero.
  Expr::EvalResult RHSValue;
  if (!RHS.get()->isValueDependent() &&
      RHS.get()->EvaluateAsInt(RHSValue, S.Context) &&
      RHSValue.Val.getInt() == 0)
    S.DiagRuntimeBehavior(Loc, RHS.get(),
                          S.PDiag(diag::warn_remainder_division_by_zero)
                            << IsDiv << RHS.get()->getSourceRange());
}

QualType Sema::CheckMultiplyDivideOperands(ExprResult &LHS, ExprResult &RHS,
                                           SourceLocation Loc,
                                           bool IsCompAssign, bool IsDiv) {
  checkArithmeticNull(*this, LHS, RHS, Loc, /*IsCompare=*/false);

  QualType LHSTy = LHS.get()->getType();
  QualType RHSTy = RHS.get()->getType();
  if (LHSTy->isVectorType() || RHSTy->isVectorType())
    return CheckVectorOperands(LHS, RHS, Loc, IsCompAssign,
                               /*AllowBothBool*/ getLangOpts().AltiVec,
                               /*AllowBoolConversions*/ false,
                               /*AllowBooleanOperation*/ false,
                               /*ReportInvalid*/ true);
  if (LHSTy->isSveVLSBuiltinType() || RHSTy->isSveVLSBuiltinType())
    return CheckSizelessVectorOperands(LHS, RHS, Loc, IsCompAssign,
                                       ACK_Arithmetic);
  if (!IsDiv &&
      (LHSTy->isConstantMatrixType() || RHSTy->isConstantMatrixType()))
    return CheckMatrixMultiplyOperands(LHS, RHS, Loc, IsCompAssign);
  // For division, only matrix-by-scalar is supported. Other combinations with
  // matrix types are invalid.
  if (IsDiv && LHSTy->isConstantMatrixType() && RHSTy->isArithmeticType())
    return CheckMatrixElementwiseOperands(LHS, RHS, Loc, IsCompAssign);

  QualType compType = UsualArithmeticConversions(
      LHS, RHS, Loc, IsCompAssign ? ACK_CompAssign : ACK_Arithmetic);
  if (LHS.isInvalid() || RHS.isInvalid())
    return QualType();


  if (compType.isNull() || !compType->isArithmeticType())
    return InvalidOperands(Loc, LHS, RHS);
  if (IsDiv) {
    DiagnoseBadDivideOrRemainderValues(*this, LHS, RHS, Loc, IsDiv);
    DiagnoseDivisionSizeofPointerOrArray(*this, LHS.get(), RHS.get(), Loc);
  }
  return compType;
}

QualType Sema::CheckRemainderOperands(
  ExprResult &LHS, ExprResult &RHS, SourceLocation Loc, bool IsCompAssign) {
  checkArithmeticNull(*this, LHS, RHS, Loc, /*IsCompare=*/false);

  if (LHS.get()->getType()->isVectorType() ||
      RHS.get()->getType()->isVectorType()) {
    if (LHS.get()->getType()->hasIntegerRepresentation() &&
        RHS.get()->getType()->hasIntegerRepresentation())
      return CheckVectorOperands(LHS, RHS, Loc, IsCompAssign,
                                 /*AllowBothBool*/ getLangOpts().AltiVec,
                                 /*AllowBoolConversions*/ false,
                                 /*AllowBooleanOperation*/ false,
                                 /*ReportInvalid*/ true);
    return InvalidOperands(Loc, LHS, RHS);
  }

  if (LHS.get()->getType()->isSveVLSBuiltinType() ||
      RHS.get()->getType()->isSveVLSBuiltinType()) {
    if (LHS.get()->getType()->hasIntegerRepresentation() &&
        RHS.get()->getType()->hasIntegerRepresentation())
      return CheckSizelessVectorOperands(LHS, RHS, Loc, IsCompAssign,
                                         ACK_Arithmetic);

    return InvalidOperands(Loc, LHS, RHS);
  }

  QualType compType = UsualArithmeticConversions(
      LHS, RHS, Loc, IsCompAssign ? ACK_CompAssign : ACK_Arithmetic);
  if (LHS.isInvalid() || RHS.isInvalid())
    return QualType();

  if (compType.isNull() || !compType->isIntegerType())
    return InvalidOperands(Loc, LHS, RHS);
  DiagnoseBadDivideOrRemainderValues(*this, LHS, RHS, Loc, false /* IsDiv */);
  return compType;
}

/// Diagnose invalid arithmetic on two void pointers.
static void diagnoseArithmeticOnTwoVoidPointers(Sema &S, SourceLocation Loc,
                                                Expr *LHSExpr, Expr *RHSExpr) {
  S.Diag(Loc, S.getLangOpts().CPlusPlus
                ? diag::err_typecheck_pointer_arith_void_type
                : diag::ext_gnu_void_ptr)
    << 1 /* two pointers */ << LHSExpr->getSourceRange()
                            << RHSExpr->getSourceRange();
}

/// Diagnose invalid arithmetic on a void pointer.
static void diagnoseArithmeticOnVoidPointer(Sema &S, SourceLocation Loc,
                                            Expr *Pointer) {
  S.Diag(Loc, S.getLangOpts().CPlusPlus
                ? diag::err_typecheck_pointer_arith_void_type
                : diag::ext_gnu_void_ptr)
    << 0 /* one pointer */ << Pointer->getSourceRange();
}

/// Diagnose invalid arithmetic on a null pointer.
///
/// If \p IsGNUIdiom is true, the operation is using the 'p = (i8*)nullptr + n'
/// idiom, which we recognize as a GNU extension.
///
static void diagnoseArithmeticOnNullPointer(Sema &S, SourceLocation Loc,
                                            Expr *Pointer, bool IsGNUIdiom) {
  if (IsGNUIdiom)
    S.Diag(Loc, diag::warn_gnu_null_ptr_arith)
      << Pointer->getSourceRange();
  else
    S.Diag(Loc, diag::warn_pointer_arith_null_ptr)
      << S.getLangOpts().CPlusPlus << Pointer->getSourceRange();
}

/// Diagnose invalid subraction on a null pointer.
///
static void diagnoseSubtractionOnNullPointer(Sema &S, SourceLocation Loc,
                                             Expr *Pointer, bool BothNull) {
  // Null - null is valid in C++ [expr.add]p7
  if (BothNull && S.getLangOpts().CPlusPlus)
    return;

  // Is this s a macro from a system header?
  if (S.Diags.getSuppressSystemWarnings() && S.SourceMgr.isInSystemMacro(Loc))
    return;

  S.DiagRuntimeBehavior(Loc, Pointer,
                        S.PDiag(diag::warn_pointer_sub_null_ptr)
                            << S.getLangOpts().CPlusPlus
                            << Pointer->getSourceRange());
}

/// Diagnose invalid arithmetic on two function pointers.
static void diagnoseArithmeticOnTwoFunctionPointers(Sema &S, SourceLocation Loc,
                                                    Expr *LHS, Expr *RHS) {
  assert(LHS->getType()->isAnyPointerType());
  assert(RHS->getType()->isAnyPointerType());
  S.Diag(Loc, S.getLangOpts().CPlusPlus
                ? diag::err_typecheck_pointer_arith_function_type
                : diag::ext_gnu_ptr_func_arith)
    << 1 /* two pointers */ << LHS->getType()->getPointeeType()
    // We only show the second type if it differs from the first.
    << (unsigned)!S.Context.hasSameUnqualifiedType(LHS->getType(),
                                                   RHS->getType())
    << RHS->getType()->getPointeeType()
    << LHS->getSourceRange() << RHS->getSourceRange();
}

/// Diagnose invalid arithmetic on a function pointer.
static void diagnoseArithmeticOnFunctionPointer(Sema &S, SourceLocation Loc,
                                                Expr *Pointer) {
  assert(Pointer->getType()->isAnyPointerType());
  S.Diag(Loc, S.getLangOpts().CPlusPlus
                ? diag::err_typecheck_pointer_arith_function_type
                : diag::ext_gnu_ptr_func_arith)
    << 0 /* one pointer */ << Pointer->getType()->getPointeeType()
    << 0 /* one pointer, so only one type */
    << Pointer->getSourceRange();
}

/// Emit error if Operand is incomplete pointer type
///
/// \returns True if pointer has incomplete type
static bool checkArithmeticIncompletePointerType(Sema &S, SourceLocation Loc,
                                                 Expr *Operand) {
  QualType ResType = Operand->getType();
  if (const AtomicType *ResAtomicType = ResType->getAs<AtomicType>())
    ResType = ResAtomicType->getValueType();

  assert(ResType->isAnyPointerType() && !ResType->isDependentType());
  QualType PointeeTy = ResType->getPointeeType();
  return S.RequireCompleteSizedType(
      Loc, PointeeTy,
      diag::err_typecheck_arithmetic_incomplete_or_sizeless_type,
      Operand->getSourceRange());
}

/// Check the validity of an arithmetic pointer operand.
///
/// If the operand has pointer type, this code will check for pointer types
/// which are invalid in arithmetic operations. These will be diagnosed
/// appropriately, including whether or not the use is supported as an
/// extension.
///
/// \returns True when the operand is valid to use (even if as an extension).
static bool checkArithmeticOpPointerOperand(Sema &S, SourceLocation Loc,
                                            Expr *Operand) {
  QualType ResType = Operand->getType();
  if (const AtomicType *ResAtomicType = ResType->getAs<AtomicType>())
    ResType = ResAtomicType->getValueType();

  if (!ResType->isAnyPointerType()) return true;

  QualType PointeeTy = ResType->getPointeeType();
  if (PointeeTy->isVoidType()) {
    diagnoseArithmeticOnVoidPointer(S, Loc, Operand);
    return !S.getLangOpts().CPlusPlus;
  }
  if (PointeeTy->isFunctionType()) {
    diagnoseArithmeticOnFunctionPointer(S, Loc, Operand);
    return !S.getLangOpts().CPlusPlus;
  }

  if (checkArithmeticIncompletePointerType(S, Loc, Operand)) return false;

  return true;
}

/// Check the validity of a binary arithmetic operation w.r.t. pointer
/// operands.
///
/// This routine will diagnose any invalid arithmetic on pointer operands much
/// like \see checkArithmeticOpPointerOperand. However, it has special logic
/// for emitting a single diagnostic even for operations where both LHS and RHS
/// are (potentially problematic) pointers.
///
/// \returns True when the operand is valid to use (even if as an extension).
static bool checkArithmeticBinOpPointerOperands(Sema &S, SourceLocation Loc,
                                                Expr *LHSExpr, Expr *RHSExpr) {
  bool isLHSPointer = LHSExpr->getType()->isAnyPointerType();
  bool isRHSPointer = RHSExpr->getType()->isAnyPointerType();
  if (!isLHSPointer && !isRHSPointer) return true;

  QualType LHSPointeeTy, RHSPointeeTy;
  if (isLHSPointer) LHSPointeeTy = LHSExpr->getType()->getPointeeType();
  if (isRHSPointer) RHSPointeeTy = RHSExpr->getType()->getPointeeType();

  // if both are pointers check if operation is valid wrt address spaces
  if (isLHSPointer && isRHSPointer) {
    if (!LHSPointeeTy.isAddressSpaceOverlapping(RHSPointeeTy)) {
      S.Diag(Loc,
             diag::err_typecheck_op_on_nonoverlapping_address_space_pointers)
          << LHSExpr->getType() << RHSExpr->getType() << 1 /*arithmetic op*/
          << LHSExpr->getSourceRange() << RHSExpr->getSourceRange();
      return false;
    }
  }

  // Check for arithmetic on pointers to incomplete types.
  bool isLHSVoidPtr = isLHSPointer && LHSPointeeTy->isVoidType();
  bool isRHSVoidPtr = isRHSPointer && RHSPointeeTy->isVoidType();
  if (isLHSVoidPtr || isRHSVoidPtr) {
    if (!isRHSVoidPtr) diagnoseArithmeticOnVoidPointer(S, Loc, LHSExpr);
    else if (!isLHSVoidPtr) diagnoseArithmeticOnVoidPointer(S, Loc, RHSExpr);
    else diagnoseArithmeticOnTwoVoidPointers(S, Loc, LHSExpr, RHSExpr);

    return !S.getLangOpts().CPlusPlus;
  }

  bool isLHSFuncPtr = isLHSPointer && LHSPointeeTy->isFunctionType();
  bool isRHSFuncPtr = isRHSPointer && RHSPointeeTy->isFunctionType();
  if (isLHSFuncPtr || isRHSFuncPtr) {
    if (!isRHSFuncPtr) diagnoseArithmeticOnFunctionPointer(S, Loc, LHSExpr);
    else if (!isLHSFuncPtr) diagnoseArithmeticOnFunctionPointer(S, Loc,
                                                                RHSExpr);
    else diagnoseArithmeticOnTwoFunctionPointers(S, Loc, LHSExpr, RHSExpr);

    return !S.getLangOpts().CPlusPlus;
  }

  if (isLHSPointer && checkArithmeticIncompletePointerType(S, Loc, LHSExpr))
    return false;
  if (isRHSPointer && checkArithmeticIncompletePointerType(S, Loc, RHSExpr))
    return false;

  return true;
}

/// diagnoseStringPlusInt - Emit a warning when adding an integer to a string
/// literal.
static void diagnoseStringPlusInt(Sema &Self, SourceLocation OpLoc,
                                  Expr *LHSExpr, Expr *RHSExpr) {
  StringLiteral* StrExpr = dyn_cast<StringLiteral>(LHSExpr->IgnoreImpCasts());
  Expr* IndexExpr = RHSExpr;
  if (!StrExpr) {
    StrExpr = dyn_cast<StringLiteral>(RHSExpr->IgnoreImpCasts());
    IndexExpr = LHSExpr;
  }

  bool IsStringPlusInt = StrExpr &&
      IndexExpr->getType()->isIntegralOrUnscopedEnumerationType();
  if (!IsStringPlusInt || IndexExpr->isValueDependent())
    return;

  SourceRange DiagRange(LHSExpr->getBeginLoc(), RHSExpr->getEndLoc());
  Self.Diag(OpLoc, diag::warn_string_plus_int)
      << DiagRange << IndexExpr->IgnoreImpCasts()->getType();

  // Only print a fixit for "str" + int, not for int + "str".
  if (IndexExpr == RHSExpr) {
    SourceLocation EndLoc = Self.getLocForEndOfToken(RHSExpr->getEndLoc());
    Self.Diag(OpLoc, diag::note_string_plus_scalar_silence)
        << FixItHint::CreateInsertion(LHSExpr->getBeginLoc(), "&")
        << FixItHint::CreateReplacement(SourceRange(OpLoc), "[")
        << FixItHint::CreateInsertion(EndLoc, "]");
  } else
    Self.Diag(OpLoc, diag::note_string_plus_scalar_silence);
}

/// Emit a warning when adding a char literal to a string.
static void diagnoseStringPlusChar(Sema &Self, SourceLocation OpLoc,
                                   Expr *LHSExpr, Expr *RHSExpr) {
  const Expr *StringRefExpr = LHSExpr;
  const CharacterLiteral *CharExpr =
      dyn_cast<CharacterLiteral>(RHSExpr->IgnoreImpCasts());

  if (!CharExpr) {
    CharExpr = dyn_cast<CharacterLiteral>(LHSExpr->IgnoreImpCasts());
    StringRefExpr = RHSExpr;
  }

  if (!CharExpr || !StringRefExpr)
    return;

  const QualType StringType = StringRefExpr->getType();

  // Return if not a PointerType.
  if (!StringType->isAnyPointerType())
    return;

  // Return if not a CharacterType.
  if (!StringType->getPointeeType()->isAnyCharacterType())
    return;

  ASTContext &Ctx = Self.getASTContext();
  SourceRange DiagRange(LHSExpr->getBeginLoc(), RHSExpr->getEndLoc());

  const QualType CharType = CharExpr->getType();
  if (!CharType->isAnyCharacterType() &&
      CharType->isIntegerType() &&
      llvm::isUIntN(Ctx.getCharWidth(), CharExpr->getValue())) {
    Self.Diag(OpLoc, diag::warn_string_plus_char)
        << DiagRange << Ctx.CharTy;
  } else {
    Self.Diag(OpLoc, diag::warn_string_plus_char)
        << DiagRange << CharExpr->getType();
  }

  // Only print a fixit for str + char, not for char + str.
  if (isa<CharacterLiteral>(RHSExpr->IgnoreImpCasts())) {
    SourceLocation EndLoc = Self.getLocForEndOfToken(RHSExpr->getEndLoc());
    Self.Diag(OpLoc, diag::note_string_plus_scalar_silence)
        << FixItHint::CreateInsertion(LHSExpr->getBeginLoc(), "&")
        << FixItHint::CreateReplacement(SourceRange(OpLoc), "[")
        << FixItHint::CreateInsertion(EndLoc, "]");
  } else {
    Self.Diag(OpLoc, diag::note_string_plus_scalar_silence);
  }
}

/// Emit error when two pointers are incompatible.
static void diagnosePointerIncompatibility(Sema &S, SourceLocation Loc,
                                           Expr *LHSExpr, Expr *RHSExpr) {
  assert(LHSExpr->getType()->isAnyPointerType());
  assert(RHSExpr->getType()->isAnyPointerType());
  S.Diag(Loc, diag::err_typecheck_sub_ptr_compatible)
    << LHSExpr->getType() << RHSExpr->getType() << LHSExpr->getSourceRange()
    << RHSExpr->getSourceRange();
}

// C99 6.5.6
QualType Sema::CheckAdditionOperands(ExprResult &LHS, ExprResult &RHS,
                                     SourceLocation Loc, BinaryOperatorKind Opc,
                                     QualType* CompLHSTy) {
  checkArithmeticNull(*this, LHS, RHS, Loc, /*IsCompare=*/false);

  if (LHS.get()->getType()->isVectorType() ||
      RHS.get()->getType()->isVectorType()) {
    QualType compType =
        CheckVectorOperands(LHS, RHS, Loc, CompLHSTy,
                            /*AllowBothBool*/ getLangOpts().AltiVec,
                            /*AllowBoolConversions*/ getLangOpts().ZVector,
                            /*AllowBooleanOperation*/ false,
                            /*ReportInvalid*/ true);
    if (CompLHSTy) *CompLHSTy = compType;
    return compType;
  }

  if (LHS.get()->getType()->isSveVLSBuiltinType() ||
      RHS.get()->getType()->isSveVLSBuiltinType()) {
    QualType compType =
        CheckSizelessVectorOperands(LHS, RHS, Loc, CompLHSTy, ACK_Arithmetic);
    if (CompLHSTy)
      *CompLHSTy = compType;
    return compType;
  }

  if (LHS.get()->getType()->isConstantMatrixType() ||
      RHS.get()->getType()->isConstantMatrixType()) {
    QualType compType =
        CheckMatrixElementwiseOperands(LHS, RHS, Loc, CompLHSTy);
    if (CompLHSTy)
      *CompLHSTy = compType;
    return compType;
  }

  QualType compType = UsualArithmeticConversions(
      LHS, RHS, Loc, CompLHSTy ? ACK_CompAssign : ACK_Arithmetic);
  if (LHS.isInvalid() || RHS.isInvalid())
    return QualType();

  // Diagnose "string literal" '+' int and string '+' "char literal".
  if (Opc == BO_Add) {
    diagnoseStringPlusInt(*this, Loc, LHS.get(), RHS.get());
    diagnoseStringPlusChar(*this, Loc, LHS.get(), RHS.get());
  }

  // handle the common case first (both operands are arithmetic).
  if (!compType.isNull() && compType->isArithmeticType()) {
    if (CompLHSTy) *CompLHSTy = compType;
    return compType;
  }

  // Type-checking.  Ultimately the pointer's going to be in PExp;
  // note that we bias towards the LHS being the pointer.
  Expr *PExp = LHS.get(), *IExp = RHS.get();

  bool isObjCPointer;
  if (PExp->getType()->isPointerType()) {
    isObjCPointer = false;
  } else if (PExp->getType()->isObjCObjectPointerType()) {
    isObjCPointer = true;
  } else {
    std::swap(PExp, IExp);
    if (PExp->getType()->isPointerType()) {
      isObjCPointer = false;
    } else if (PExp->getType()->isObjCObjectPointerType()) {
      isObjCPointer = true;
    } else {
      return InvalidOperands(Loc, LHS, RHS);
    }
  }
  assert(PExp->getType()->isAnyPointerType());

  if (!IExp->getType()->isIntegerType())
    return InvalidOperands(Loc, LHS, RHS);

  // Adding to a null pointer results in undefined behavior.
  if (PExp->IgnoreParenCasts()->isNullPointerConstant(
          Context, Expr::NPC_ValueDependentIsNotNull)) {
    // In C++ adding zero to a null pointer is defined.
    Expr::EvalResult KnownVal;
    if (!getLangOpts().CPlusPlus ||
        (!IExp->isValueDependent() &&
         (!IExp->EvaluateAsInt(KnownVal, Context) ||
          KnownVal.Val.getInt() != 0))) {
      // Check the conditions to see if this is the 'p = nullptr + n' idiom.
      bool IsGNUIdiom = BinaryOperator::isNullPointerArithmeticExtension(
          Context, BO_Add, PExp, IExp);
      diagnoseArithmeticOnNullPointer(*this, Loc, PExp, IsGNUIdiom);
    }
  }

  if (!checkArithmeticOpPointerOperand(*this, Loc, PExp))
    return QualType();

  if (isObjCPointer && checkArithmeticOnObjCPointer(*this, Loc, PExp))
    return QualType();

  // Check array bounds for pointer arithemtic
  CheckArrayAccess(PExp, IExp);

  if (CompLHSTy) {
    QualType LHSTy = Context.isPromotableBitField(LHS.get());
    if (LHSTy.isNull()) {
      LHSTy = LHS.get()->getType();
      if (Context.isPromotableIntegerType(LHSTy))
        LHSTy = Context.getPromotedIntegerType(LHSTy);
    }
    *CompLHSTy = LHSTy;
  }

  return PExp->getType();
}

// C99 6.5.6
QualType Sema::CheckSubtractionOperands(ExprResult &LHS, ExprResult &RHS,
                                        SourceLocation Loc,
                                        QualType* CompLHSTy) {
  checkArithmeticNull(*this, LHS, RHS, Loc, /*IsCompare=*/false);

  if (LHS.get()->getType()->isVectorType() ||
      RHS.get()->getType()->isVectorType()) {
    QualType compType =
        CheckVectorOperands(LHS, RHS, Loc, CompLHSTy,
                            /*AllowBothBool*/ getLangOpts().AltiVec,
                            /*AllowBoolConversions*/ getLangOpts().ZVector,
                            /*AllowBooleanOperation*/ false,
                            /*ReportInvalid*/ true);
    if (CompLHSTy) *CompLHSTy = compType;
    return compType;
  }

  if (LHS.get()->getType()->isSveVLSBuiltinType() ||
      RHS.get()->getType()->isSveVLSBuiltinType()) {
    QualType compType =
        CheckSizelessVectorOperands(LHS, RHS, Loc, CompLHSTy, ACK_Arithmetic);
    if (CompLHSTy)
      *CompLHSTy = compType;
    return compType;
  }

  if (LHS.get()->getType()->isConstantMatrixType() ||
      RHS.get()->getType()->isConstantMatrixType()) {
    QualType compType =
        CheckMatrixElementwiseOperands(LHS, RHS, Loc, CompLHSTy);
    if (CompLHSTy)
      *CompLHSTy = compType;
    return compType;
  }

  QualType compType = UsualArithmeticConversions(
      LHS, RHS, Loc, CompLHSTy ? ACK_CompAssign : ACK_Arithmetic);
  if (LHS.isInvalid() || RHS.isInvalid())
    return QualType();

  // Enforce type constraints: C99 6.5.6p3.

  // Handle the common case first (both operands are arithmetic).
  if (!compType.isNull() && compType->isArithmeticType()) {
    if (CompLHSTy) *CompLHSTy = compType;
    return compType;
  }

  // Either ptr - int   or   ptr - ptr.
  if (LHS.get()->getType()->isAnyPointerType()) {
    QualType lpointee = LHS.get()->getType()->getPointeeType();

    // Diagnose bad cases where we step over interface counts.
    if (LHS.get()->getType()->isObjCObjectPointerType() &&
        checkArithmeticOnObjCPointer(*this, Loc, LHS.get()))
      return QualType();

    // The result type of a pointer-int computation is the pointer type.
    if (RHS.get()->getType()->isIntegerType()) {
      // Subtracting from a null pointer should produce a warning.
      // The last argument to the diagnose call says this doesn't match the
      // GNU int-to-pointer idiom.
      if (LHS.get()->IgnoreParenCasts()->isNullPointerConstant(Context,
                                           Expr::NPC_ValueDependentIsNotNull)) {
        // In C++ adding zero to a null pointer is defined.
        Expr::EvalResult KnownVal;
        if (!getLangOpts().CPlusPlus ||
            (!RHS.get()->isValueDependent() &&
             (!RHS.get()->EvaluateAsInt(KnownVal, Context) ||
              KnownVal.Val.getInt() != 0))) {
          diagnoseArithmeticOnNullPointer(*this, Loc, LHS.get(), false);
        }
      }

      if (!checkArithmeticOpPointerOperand(*this, Loc, LHS.get()))
        return QualType();

      // Check array bounds for pointer arithemtic
      CheckArrayAccess(LHS.get(), RHS.get(), /*ArraySubscriptExpr*/nullptr,
                       /*AllowOnePastEnd*/true, /*IndexNegated*/true);

      if (CompLHSTy) *CompLHSTy = LHS.get()->getType();
      return LHS.get()->getType();
    }

    // Handle pointer-pointer subtractions.
    if (const PointerType *RHSPTy
          = RHS.get()->getType()->getAs<PointerType>()) {
      QualType rpointee = RHSPTy->getPointeeType();

      if (getLangOpts().CPlusPlus) {
        // Pointee types must be the same: C++ [expr.add]
        if (!Context.hasSameUnqualifiedType(lpointee, rpointee)) {
          diagnosePointerIncompatibility(*this, Loc, LHS.get(), RHS.get());
        }
      } else {
        // Pointee types must be compatible C99 6.5.6p3
        if (!Context.typesAreCompatible(
                Context.getCanonicalType(lpointee).getUnqualifiedType(),
                Context.getCanonicalType(rpointee).getUnqualifiedType())) {
          diagnosePointerIncompatibility(*this, Loc, LHS.get(), RHS.get());
          return QualType();
        }
      }

      if (!checkArithmeticBinOpPointerOperands(*this, Loc,
                                               LHS.get(), RHS.get()))
        return QualType();

      bool LHSIsNullPtr = LHS.get()->IgnoreParenCasts()->isNullPointerConstant(
          Context, Expr::NPC_ValueDependentIsNotNull);
      bool RHSIsNullPtr = RHS.get()->IgnoreParenCasts()->isNullPointerConstant(
          Context, Expr::NPC_ValueDependentIsNotNull);

      // Subtracting nullptr or from nullptr is suspect
      if (LHSIsNullPtr)
        diagnoseSubtractionOnNullPointer(*this, Loc, LHS.get(), RHSIsNullPtr);
      if (RHSIsNullPtr)
        diagnoseSubtractionOnNullPointer(*this, Loc, RHS.get(), LHSIsNullPtr);

      // The pointee type may have zero size.  As an extension, a structure or
      // union may have zero size or an array may have zero length.  In this
      // case subtraction does not make sense.
      if (!rpointee->isVoidType() && !rpointee->isFunctionType()) {
        CharUnits ElementSize = Context.getTypeSizeInChars(rpointee);
        if (ElementSize.isZero()) {
          Diag(Loc,diag::warn_sub_ptr_zero_size_types)
            << rpointee.getUnqualifiedType()
            << LHS.get()->getSourceRange() << RHS.get()->getSourceRange();
        }
      }

      if (CompLHSTy) *CompLHSTy = LHS.get()->getType();
      return Context.getPointerDiffType();
    }
  }

  return InvalidOperands(Loc, LHS, RHS);
}

static bool isScopedEnumerationType(QualType T) {
  if (const EnumType *ET = T->getAs<EnumType>())
    return ET->getDecl()->isScoped();
  return false;
}

static void DiagnoseBadShiftValues(Sema& S, ExprResult &LHS, ExprResult &RHS,
                                   SourceLocation Loc, BinaryOperatorKind Opc,
                                   QualType LHSType) {
  // OpenCL 6.3j: shift values are effectively % word size of LHS (more defined),
  // so skip remaining warnings as we don't want to modify values within Sema.
  if (S.getLangOpts().OpenCL)
    return;

  // Check right/shifter operand
  Expr::EvalResult RHSResult;
  if (RHS.get()->isValueDependent() ||
      !RHS.get()->EvaluateAsInt(RHSResult, S.Context))
    return;
  llvm::APSInt Right = RHSResult.Val.getInt();

  if (Right.isNegative()) {
    S.DiagRuntimeBehavior(Loc, RHS.get(),
                          S.PDiag(diag::warn_shift_negative)
                            << RHS.get()->getSourceRange());
    return;
  }

  QualType LHSExprType = LHS.get()->getType();
  uint64_t LeftSize = S.Context.getTypeSize(LHSExprType);
  if (LHSExprType->isBitIntType())
    LeftSize = S.Context.getIntWidth(LHSExprType);
  else if (LHSExprType->isFixedPointType()) {
    auto FXSema = S.Context.getFixedPointSemantics(LHSExprType);
    LeftSize = FXSema.getWidth() - (unsigned)FXSema.hasUnsignedPadding();
  }
  llvm::APInt LeftBits(Right.getBitWidth(), LeftSize);
  if (Right.uge(LeftBits)) {
    S.DiagRuntimeBehavior(Loc, RHS.get(),
                          S.PDiag(diag::warn_shift_gt_typewidth)
                            << RHS.get()->getSourceRange());
    return;
  }

  // FIXME: We probably need to handle fixed point types specially here.
  if (Opc != BO_Shl || LHSExprType->isFixedPointType())
    return;

  // When left shifting an ICE which is signed, we can check for overflow which
  // according to C++ standards prior to C++2a has undefined behavior
  // ([expr.shift] 5.8/2). Unsigned integers have defined behavior modulo one
  // more than the maximum value representable in the result type, so never
  // warn for those. (FIXME: Unsigned left-shift overflow in a constant
  // expression is still probably a bug.)
  Expr::EvalResult LHSResult;
  if (LHS.get()->isValueDependent() ||
      LHSType->hasUnsignedIntegerRepresentation() ||
      !LHS.get()->EvaluateAsInt(LHSResult, S.Context))
    return;
  llvm::APSInt Left = LHSResult.Val.getInt();

  // Don't warn if signed overflow is defined, then all the rest of the
  // diagnostics will not be triggered because the behavior is defined.
  // Also don't warn in C++20 mode (and newer), as signed left shifts
  // always wrap and never overflow.
  if (S.getLangOpts().isSignedOverflowDefined() || S.getLangOpts().CPlusPlus20)
    return;

  // If LHS does not have a non-negative value then, the
  // behavior is undefined before C++2a. Warn about it.
  if (Left.isNegative()) {
    S.DiagRuntimeBehavior(Loc, LHS.get(),
                          S.PDiag(diag::warn_shift_lhs_negative)
                            << LHS.get()->getSourceRange());
    return;
  }

  llvm::APInt ResultBits =
      static_cast<llvm::APInt &>(Right) + Left.getSignificantBits();
  if (LeftBits.uge(ResultBits))
    return;
  llvm::APSInt Result = Left.extend(ResultBits.getLimitedValue());
  Result = Result.shl(Right);

  // Print the bit representation of the signed integer as an unsigned
  // hexadecimal number.
  SmallString<40> HexResult;
  Result.toString(HexResult, 16, /*Signed =*/false, /*Literal =*/true);

  // If we are only missing a sign bit, this is less likely to result in actual
  // bugs -- if the result is cast back to an unsigned type, it will have the
  // expected value. Thus we place this behind a different warning that can be
  // turned off separately if needed.
  if (LeftBits == ResultBits - 1) {
    S.Diag(Loc, diag::warn_shift_result_sets_sign_bit)
        << HexResult << LHSType
        << LHS.get()->getSourceRange() << RHS.get()->getSourceRange();
    return;
  }

  S.Diag(Loc, diag::warn_shift_result_gt_typewidth)
      << HexResult.str() << Result.getSignificantBits() << LHSType
      << Left.getBitWidth() << LHS.get()->getSourceRange()
      << RHS.get()->getSourceRange();
}

/// Return the resulting type when a vector is shifted
///        by a scalar or vector shift amount.
static QualType checkVectorShift(Sema &S, ExprResult &LHS, ExprResult &RHS,
                                 SourceLocation Loc, bool IsCompAssign) {
  // OpenCL v1.1 s6.3.j says RHS can be a vector only if LHS is a vector.
  if ((S.LangOpts.OpenCL || S.LangOpts.ZVector) &&
      !LHS.get()->getType()->isVectorType()) {
    S.Diag(Loc, diag::err_shift_rhs_only_vector)
      << RHS.get()->getType() << LHS.get()->getType()
      << LHS.get()->getSourceRange() << RHS.get()->getSourceRange();
    return QualType();
  }

  if (!IsCompAssign) {
    LHS = S.UsualUnaryConversions(LHS.get());
    if (LHS.isInvalid()) return QualType();
  }

  RHS = S.UsualUnaryConversions(RHS.get());
  if (RHS.isInvalid()) return QualType();

  QualType LHSType = LHS.get()->getType();
  // Note that LHS might be a scalar because the routine calls not only in
  // OpenCL case.
  const VectorType *LHSVecTy = LHSType->getAs<VectorType>();
  QualType LHSEleType = LHSVecTy ? LHSVecTy->getElementType() : LHSType;

  // Note that RHS might not be a vector.
  QualType RHSType = RHS.get()->getType();
  const VectorType *RHSVecTy = RHSType->getAs<VectorType>();
  QualType RHSEleType = RHSVecTy ? RHSVecTy->getElementType() : RHSType;

  // Do not allow shifts for boolean vectors.
  if ((LHSVecTy && LHSVecTy->isExtVectorBoolType()) ||
      (RHSVecTy && RHSVecTy->isExtVectorBoolType())) {
    S.Diag(Loc, diag::err_typecheck_invalid_operands)
        << LHS.get()->getType() << RHS.get()->getType()
        << LHS.get()->getSourceRange();
    return QualType();
  }

  // The operands need to be integers.
  if (!LHSEleType->isIntegerType()) {
    S.Diag(Loc, diag::err_typecheck_expect_int)
      << LHS.get()->getType() << LHS.get()->getSourceRange();
    return QualType();
  }

  if (!RHSEleType->isIntegerType()) {
    S.Diag(Loc, diag::err_typecheck_expect_int)
      << RHS.get()->getType() << RHS.get()->getSourceRange();
    return QualType();
  }

  if (!LHSVecTy) {
    assert(RHSVecTy);
    if (IsCompAssign)
      return RHSType;
    if (LHSEleType != RHSEleType) {
      LHS = S.ImpCastExprToType(LHS.get(),RHSEleType, CK_IntegralCast);
      LHSEleType = RHSEleType;
    }
    QualType VecTy =
        S.Context.getExtVectorType(LHSEleType, RHSVecTy->getNumElements());
    LHS = S.ImpCastExprToType(LHS.get(), VecTy, CK_VectorSplat);
    LHSType = VecTy;
  } else if (RHSVecTy) {
    // OpenCL v1.1 s6.3.j says that for vector types, the operators
    // are applied component-wise. So if RHS is a vector, then ensure
    // that the number of elements is the same as LHS...
    if (RHSVecTy->getNumElements() != LHSVecTy->getNumElements()) {
      S.Diag(Loc, diag::err_typecheck_vector_lengths_not_equal)
        << LHS.get()->getType() << RHS.get()->getType()
        << LHS.get()->getSourceRange() << RHS.get()->getSourceRange();
      return QualType();
    }
    if (!S.LangOpts.OpenCL && !S.LangOpts.ZVector) {
      const BuiltinType *LHSBT = LHSEleType->getAs<clang::BuiltinType>();
      const BuiltinType *RHSBT = RHSEleType->getAs<clang::BuiltinType>();
      if (LHSBT != RHSBT &&
          S.Context.getTypeSize(LHSBT) != S.Context.getTypeSize(RHSBT)) {
        S.Diag(Loc, diag::warn_typecheck_vector_element_sizes_not_equal)
            << LHS.get()->getType() << RHS.get()->getType()
            << LHS.get()->getSourceRange() << RHS.get()->getSourceRange();
      }
    }
  } else {
    // ...else expand RHS to match the number of elements in LHS.
    QualType VecTy =
      S.Context.getExtVectorType(RHSEleType, LHSVecTy->getNumElements());
    RHS = S.ImpCastExprToType(RHS.get(), VecTy, CK_VectorSplat);
  }

  return LHSType;
}

static QualType checkSizelessVectorShift(Sema &S, ExprResult &LHS,
                                         ExprResult &RHS, SourceLocation Loc,
                                         bool IsCompAssign) {
  if (!IsCompAssign) {
    LHS = S.UsualUnaryConversions(LHS.get());
    if (LHS.isInvalid())
      return QualType();
  }

  RHS = S.UsualUnaryConversions(RHS.get());
  if (RHS.isInvalid())
    return QualType();

  QualType LHSType = LHS.get()->getType();
  const BuiltinType *LHSBuiltinTy = LHSType->castAs<BuiltinType>();
  QualType LHSEleType = LHSType->isSveVLSBuiltinType()
                            ? LHSBuiltinTy->getSveEltType(S.getASTContext())
                            : LHSType;

  // Note that RHS might not be a vector
  QualType RHSType = RHS.get()->getType();
  const BuiltinType *RHSBuiltinTy = RHSType->castAs<BuiltinType>();
  QualType RHSEleType = RHSType->isSveVLSBuiltinType()
                            ? RHSBuiltinTy->getSveEltType(S.getASTContext())
                            : RHSType;

  if ((LHSBuiltinTy && LHSBuiltinTy->isSVEBool()) ||
      (RHSBuiltinTy && RHSBuiltinTy->isSVEBool())) {
    S.Diag(Loc, diag::err_typecheck_invalid_operands)
        << LHSType << RHSType << LHS.get()->getSourceRange();
    return QualType();
  }

  if (!LHSEleType->isIntegerType()) {
    S.Diag(Loc, diag::err_typecheck_expect_int)
        << LHS.get()->getType() << LHS.get()->getSourceRange();
    return QualType();
  }

  if (!RHSEleType->isIntegerType()) {
    S.Diag(Loc, diag::err_typecheck_expect_int)
        << RHS.get()->getType() << RHS.get()->getSourceRange();
    return QualType();
  }

  if (LHSType->isSveVLSBuiltinType() && RHSType->isSveVLSBuiltinType() &&
      (S.Context.getBuiltinVectorTypeInfo(LHSBuiltinTy).EC !=
       S.Context.getBuiltinVectorTypeInfo(RHSBuiltinTy).EC)) {
    S.Diag(Loc, diag::err_typecheck_invalid_operands)
        << LHSType << RHSType << LHS.get()->getSourceRange()
        << RHS.get()->getSourceRange();
    return QualType();
  }

  if (!LHSType->isSveVLSBuiltinType()) {
    assert(RHSType->isSveVLSBuiltinType());
    if (IsCompAssign)
      return RHSType;
    if (LHSEleType != RHSEleType) {
      LHS = S.ImpCastExprToType(LHS.get(), RHSEleType, clang::CK_IntegralCast);
      LHSEleType = RHSEleType;
    }
    const llvm::ElementCount VecSize =
        S.Context.getBuiltinVectorTypeInfo(RHSBuiltinTy).EC;
    QualType VecTy =
        S.Context.getScalableVectorType(LHSEleType, VecSize.getKnownMinValue());
    LHS = S.ImpCastExprToType(LHS.get(), VecTy, clang::CK_VectorSplat);
    LHSType = VecTy;
  } else if (RHSBuiltinTy && RHSBuiltinTy->isSveVLSBuiltinType()) {
    if (S.Context.getTypeSize(RHSBuiltinTy) !=
        S.Context.getTypeSize(LHSBuiltinTy)) {
      S.Diag(Loc, diag::err_typecheck_vector_lengths_not_equal)
          << LHSType << RHSType << LHS.get()->getSourceRange()
          << RHS.get()->getSourceRange();
      return QualType();
    }
  } else {
    const llvm::ElementCount VecSize =
        S.Context.getBuiltinVectorTypeInfo(LHSBuiltinTy).EC;
    if (LHSEleType != RHSEleType) {
      RHS = S.ImpCastExprToType(RHS.get(), LHSEleType, clang::CK_IntegralCast);
      RHSEleType = LHSEleType;
    }
    QualType VecTy =
        S.Context.getScalableVectorType(RHSEleType, VecSize.getKnownMinValue());
    RHS = S.ImpCastExprToType(RHS.get(), VecTy, CK_VectorSplat);
  }

  return LHSType;
}

// C99 6.5.7
QualType Sema::CheckShiftOperands(ExprResult &LHS, ExprResult &RHS,
                                  SourceLocation Loc, BinaryOperatorKind Opc,
                                  bool IsCompAssign) {
  checkArithmeticNull(*this, LHS, RHS, Loc, /*IsCompare=*/false);

  // Vector shifts promote their scalar inputs to vector type.
  if (LHS.get()->getType()->isVectorType() ||
      RHS.get()->getType()->isVectorType()) {
    if (LangOpts.ZVector) {
      // The shift operators for the z vector extensions work basically
      // like general shifts, except that neither the LHS nor the RHS is
      // allowed to be a "vector bool".
      if (auto LHSVecType = LHS.get()->getType()->getAs<VectorType>())
        if (LHSVecType->getVectorKind() == VectorType::AltiVecBool)
          return InvalidOperands(Loc, LHS, RHS);
      if (auto RHSVecType = RHS.get()->getType()->getAs<VectorType>())
        if (RHSVecType->getVectorKind() == VectorType::AltiVecBool)
          return InvalidOperands(Loc, LHS, RHS);
    }
    return checkVectorShift(*this, LHS, RHS, Loc, IsCompAssign);
  }

  if (LHS.get()->getType()->isSveVLSBuiltinType() ||
      RHS.get()->getType()->isSveVLSBuiltinType())
    return checkSizelessVectorShift(*this, LHS, RHS, Loc, IsCompAssign);

  // Shifts don't perform usual arithmetic conversions, they just do integer
  // promotions on each operand. C99 6.5.7p3

  // For the LHS, do usual unary conversions, but then reset them away
  // if this is a compound assignment.
  ExprResult OldLHS = LHS;
  LHS = UsualUnaryConversions(LHS.get());
  if (LHS.isInvalid())
    return QualType();
  QualType LHSType = LHS.get()->getType();
  if (IsCompAssign) LHS = OldLHS;

  // The RHS is simpler.
  RHS = UsualUnaryConversions(RHS.get());
  if (RHS.isInvalid())
    return QualType();
  QualType RHSType = RHS.get()->getType();

  // C99 6.5.7p2: Each of the operands shall have integer type.
  // Embedded-C 4.1.6.2.2: The LHS may also be fixed-point.
  if ((!LHSType->isFixedPointOrIntegerType() &&
       !LHSType->hasIntegerRepresentation()) ||
      !RHSType->hasIntegerRepresentation())
    return InvalidOperands(Loc, LHS, RHS);

  // C++0x: Don't allow scoped enums. FIXME: Use something better than
  // hasIntegerRepresentation() above instead of this.
  if (isScopedEnumerationType(LHSType) ||
      isScopedEnumerationType(RHSType)) {
    return InvalidOperands(Loc, LHS, RHS);
  }
  DiagnoseBadShiftValues(*this, LHS, RHS, Loc, Opc, LHSType);

  // "The type of the result is that of the promoted left operand."
  return LHSType;
}

/// Diagnose bad pointer comparisons.
static void diagnoseDistinctPointerComparison(Sema &S, SourceLocation Loc,
                                              ExprResult &LHS, ExprResult &RHS,
                                              bool IsError) {
  S.Diag(Loc, IsError ? diag::err_typecheck_comparison_of_distinct_pointers
                      : diag::ext_typecheck_comparison_of_distinct_pointers)
    << LHS.get()->getType() << RHS.get()->getType()
    << LHS.get()->getSourceRange() << RHS.get()->getSourceRange();
}

/// Returns false if the pointers are converted to a composite type,
/// true otherwise.
static bool convertPointersToCompositeType(Sema &S, SourceLocation Loc,
                                           ExprResult &LHS, ExprResult &RHS) {
  // C++ [expr.rel]p2:
  //   [...] Pointer conversions (4.10) and qualification
  //   conversions (4.4) are performed on pointer operands (or on
  //   a pointer operand and a null pointer constant) to bring
  //   them to their composite pointer type. [...]
  //
  // C++ [expr.eq]p1 uses the same notion for (in)equality
  // comparisons of pointers.

  QualType LHSType = LHS.get()->getType();
  QualType RHSType = RHS.get()->getType();
  assert(LHSType->isPointerType() || RHSType->isPointerType() ||
         LHSType->isMemberPointerType() || RHSType->isMemberPointerType());

  QualType T = S.FindCompositePointerType(Loc, LHS, RHS);
  if (T.isNull()) {
    if ((LHSType->isAnyPointerType() || LHSType->isMemberPointerType()) &&
        (RHSType->isAnyPointerType() || RHSType->isMemberPointerType()))
      diagnoseDistinctPointerComparison(S, Loc, LHS, RHS, /*isError*/true);
    else
      S.InvalidOperands(Loc, LHS, RHS);
    return true;
  }

  return false;
}

static void diagnoseFunctionPointerToVoidComparison(Sema &S, SourceLocation Loc,
                                                    ExprResult &LHS,
                                                    ExprResult &RHS,
                                                    bool IsError) {
  S.Diag(Loc, IsError ? diag::err_typecheck_comparison_of_fptr_to_void
                      : diag::ext_typecheck_comparison_of_fptr_to_void)
    << LHS.get()->getType() << RHS.get()->getType()
    << LHS.get()->getSourceRange() << RHS.get()->getSourceRange();
}

static bool isObjCObjectLiteral(ExprResult &E) {
  switch (E.get()->IgnoreParenImpCasts()->getStmtClass()) {
  case Stmt::ObjCArrayLiteralClass:
  case Stmt::ObjCDictionaryLiteralClass:
  case Stmt::ObjCStringLiteralClass:
  case Stmt::ObjCBoxedExprClass:
    return true;
  default:
    // Note that ObjCBoolLiteral is NOT an object literal!
    return false;
  }
}

static bool hasIsEqualMethod(Sema &S, const Expr *LHS, const Expr *RHS) {
  const ObjCObjectPointerType *Type =
    LHS->getType()->getAs<ObjCObjectPointerType>();

  // If this is not actually an Objective-C object, bail out.
  if (!Type)
    return false;

  // Get the LHS object's interface type.
  QualType InterfaceType = Type->getPointeeType();

  // If the RHS isn't an Objective-C object, bail out.
  if (!RHS->getType()->isObjCObjectPointerType())
    return false;

  // Try to find the -isEqual: method.
  Selector IsEqualSel = S.NSAPIObj->getIsEqualSelector();
  ObjCMethodDecl *Method = S.LookupMethodInObjectType(IsEqualSel,
                                                      InterfaceType,
                                                      /*IsInstance=*/true);
  if (!Method) {
    if (Type->isObjCIdType()) {
      // For 'id', just check the global pool.
      Method = S.LookupInstanceMethodInGlobalPool(IsEqualSel, SourceRange(),
                                                  /*receiverId=*/true);
    } else {
      // Check protocols.
      Method = S.LookupMethodInQualifiedType(IsEqualSel, Type,
                                             /*IsInstance=*/true);
    }
  }

  if (!Method)
    return false;

  QualType T = Method->parameters()[0]->getType();
  if (!T->isObjCObjectPointerType())
    return false;

  QualType R = Method->getReturnType();
  if (!R->isScalarType())
    return false;

  return true;
}

Sema::ObjCLiteralKind Sema::CheckLiteralKind(Expr *FromE) {
  FromE = FromE->IgnoreParenImpCasts();
  switch (FromE->getStmtClass()) {
    default:
      break;
    case Stmt::ObjCStringLiteralClass:
      // "string literal"
      return LK_String;
    case Stmt::ObjCArrayLiteralClass:
      // "array literal"
      return LK_Array;
    case Stmt::ObjCDictionaryLiteralClass:
      // "dictionary literal"
      return LK_Dictionary;
    case Stmt::BlockExprClass:
      return LK_Block;
    case Stmt::ObjCBoxedExprClass: {
      Expr *Inner = cast<ObjCBoxedExpr>(FromE)->getSubExpr()->IgnoreParens();
      switch (Inner->getStmtClass()) {
        case Stmt::IntegerLiteralClass:
        case Stmt::FloatingLiteralClass:
        case Stmt::CharacterLiteralClass:
        case Stmt::ObjCBoolLiteralExprClass:
        case Stmt::CXXBoolLiteralExprClass:
          // "numeric literal"
          return LK_Numeric;
        case Stmt::ImplicitCastExprClass: {
          CastKind CK = cast<CastExpr>(Inner)->getCastKind();
          // Boolean literals can be represented by implicit casts.
          if (CK == CK_IntegralToBoolean || CK == CK_IntegralCast)
            return LK_Numeric;
          break;
        }
        default:
          break;
      }
      return LK_Boxed;
    }
  }
  return LK_None;
}

static void diagnoseObjCLiteralComparison(Sema &S, SourceLocation Loc,
                                          ExprResult &LHS, ExprResult &RHS,
                                          BinaryOperator::Opcode Opc){
  Expr *Literal;
  Expr *Other;
  if (isObjCObjectLiteral(LHS)) {
    Literal = LHS.get();
    Other = RHS.get();
  } else {
    Literal = RHS.get();
    Other = LHS.get();
  }

  // Don't warn on comparisons against nil.
  Other = Other->IgnoreParenCasts();
  if (Other->isNullPointerConstant(S.getASTContext(),
                                   Expr::NPC_ValueDependentIsNotNull))
    return;

  // This should be kept in sync with warn_objc_literal_comparison.
  // LK_String should always be after the other literals, since it has its own
  // warning flag.
  Sema::ObjCLiteralKind LiteralKind = S.CheckLiteralKind(Literal);
  assert(LiteralKind != Sema::LK_Block);
  if (LiteralKind == Sema::LK_None) {
    llvm_unreachable("Unknown Objective-C object literal kind");
  }

  if (LiteralKind == Sema::LK_String)
    S.Diag(Loc, diag::warn_objc_string_literal_comparison)
      << Literal->getSourceRange();
  else
    S.Diag(Loc, diag::warn_objc_literal_comparison)
      << LiteralKind << Literal->getSourceRange();

  if (BinaryOperator::isEqualityOp(Opc) &&
      hasIsEqualMethod(S, LHS.get(), RHS.get())) {
    SourceLocation Start = LHS.get()->getBeginLoc();
    SourceLocation End = S.getLocForEndOfToken(RHS.get()->getEndLoc());
    CharSourceRange OpRange =
      CharSourceRange::getCharRange(Loc, S.getLocForEndOfToken(Loc));

    S.Diag(Loc, diag::note_objc_literal_comparison_isequal)
      << FixItHint::CreateInsertion(Start, Opc == BO_EQ ? "[" : "![")
      << FixItHint::CreateReplacement(OpRange, " isEqual:")
      << FixItHint::CreateInsertion(End, "]");
  }
}

/// Warns on !x < y, !x & y where !(x < y), !(x & y) was probably intended.
static void diagnoseLogicalNotOnLHSofCheck(Sema &S, ExprResult &LHS,
                                           ExprResult &RHS, SourceLocation Loc,
                                           BinaryOperatorKind Opc) {
  // Check that left hand side is !something.
  UnaryOperator *UO = dyn_cast<UnaryOperator>(LHS.get()->IgnoreImpCasts());
  if (!UO || UO->getOpcode() != UO_LNot) return;

  // Only check if the right hand side is non-bool arithmetic type.
  if (RHS.get()->isKnownToHaveBooleanValue()) return;

  // Make sure that the something in !something is not bool.
  Expr *SubExpr = UO->getSubExpr()->IgnoreImpCasts();
  if (SubExpr->isKnownToHaveBooleanValue()) return;

  // Emit warning.
  bool IsBitwiseOp = Opc == BO_And || Opc == BO_Or || Opc == BO_Xor;
  S.Diag(UO->getOperatorLoc(), diag::warn_logical_not_on_lhs_of_check)
      << Loc << IsBitwiseOp;

  // First note suggest !(x < y)
  SourceLocation FirstOpen = SubExpr->getBeginLoc();
  SourceLocation FirstClose = RHS.get()->getEndLoc();
  FirstClose = S.getLocForEndOfToken(FirstClose);
  if (FirstClose.isInvalid())
    FirstOpen = SourceLocation();
  S.Diag(UO->getOperatorLoc(), diag::note_logical_not_fix)
      << IsBitwiseOp
      << FixItHint::CreateInsertion(FirstOpen, "(")
      << FixItHint::CreateInsertion(FirstClose, ")");

  // Second note suggests (!x) < y
  SourceLocation SecondOpen = LHS.get()->getBeginLoc();
  SourceLocation SecondClose = LHS.get()->getEndLoc();
  SecondClose = S.getLocForEndOfToken(SecondClose);
  if (SecondClose.isInvalid())
    SecondOpen = SourceLocation();
  S.Diag(UO->getOperatorLoc(), diag::note_logical_not_silence_with_parens)
      << FixItHint::CreateInsertion(SecondOpen, "(")
      << FixItHint::CreateInsertion(SecondClose, ")");
}

// Returns true if E refers to a non-weak array.
static bool checkForArray(const Expr *E) {
  const ValueDecl *D = nullptr;
  if (const DeclRefExpr *DR = dyn_cast<DeclRefExpr>(E)) {
    D = DR->getDecl();
  } else if (const MemberExpr *Mem = dyn_cast<MemberExpr>(E)) {
    if (Mem->isImplicitAccess())
      D = Mem->getMemberDecl();
  }
  if (!D)
    return false;
  return D->getType()->isArrayType() && !D->isWeak();
}

/// Diagnose some forms of syntactically-obvious tautological comparison.
static void diagnoseTautologicalComparison(Sema &S, SourceLocation Loc,
                                           Expr *LHS, Expr *RHS,
                                           BinaryOperatorKind Opc) {
  Expr *LHSStripped = LHS->IgnoreParenImpCasts();
  Expr *RHSStripped = RHS->IgnoreParenImpCasts();

  QualType LHSType = LHS->getType();
  QualType RHSType = RHS->getType();
  if (LHSType->hasFloatingRepresentation() ||
      (LHSType->isBlockPointerType() && !BinaryOperator::isEqualityOp(Opc)) ||
      S.inTemplateInstantiation())
    return;

  // WebAssembly Tables cannot be compared, therefore shouldn't emit
  // Tautological diagnostics.
  if (LHSType->isWebAssemblyTableType() || RHSType->isWebAssemblyTableType())
    return;

  // Comparisons between two array types are ill-formed for operator<=>, so
  // we shouldn't emit any additional warnings about it.
  if (Opc == BO_Cmp && LHSType->isArrayType() && RHSType->isArrayType())
    return;

  // For non-floating point types, check for self-comparisons of the form
  // x == x, x != x, x < x, etc.  These always evaluate to a constant, and
  // often indicate logic errors in the program.
  //
  // NOTE: Don't warn about comparison expressions resulting from macro
  // expansion. Also don't warn about comparisons which are only self
  // comparisons within a template instantiation. The warnings should catch
  // obvious cases in the definition of the template anyways. The idea is to
  // warn when the typed comparison operator will always evaluate to the same
  // result.

  // Used for indexing into %select in warn_comparison_always
  enum {
    AlwaysConstant,
    AlwaysTrue,
    AlwaysFalse,
    AlwaysEqual, // std::strong_ordering::equal from operator<=>
  };

  // C++2a [depr.array.comp]:
  //   Equality and relational comparisons ([expr.eq], [expr.rel]) between two
  //   operands of array type are deprecated.
  if (S.getLangOpts().CPlusPlus20 && LHSStripped->getType()->isArrayType() &&
      RHSStripped->getType()->isArrayType()) {
    S.Diag(Loc, diag::warn_depr_array_comparison)
        << LHS->getSourceRange() << RHS->getSourceRange()
        << LHSStripped->getType() << RHSStripped->getType();
    // Carry on to produce the tautological comparison warning, if this
    // expression is potentially-evaluated, we can resolve the array to a
    // non-weak declaration, and so on.
  }

  if (!LHS->getBeginLoc().isMacroID() && !RHS->getBeginLoc().isMacroID()) {
    if (Expr::isSameComparisonOperand(LHS, RHS)) {
      unsigned Result;
      switch (Opc) {
      case BO_EQ:
      case BO_LE:
      case BO_GE:
        Result = AlwaysTrue;
        break;
      case BO_NE:
      case BO_LT:
      case BO_GT:
        Result = AlwaysFalse;
        break;
      case BO_Cmp:
        Result = AlwaysEqual;
        break;
      default:
        Result = AlwaysConstant;
        break;
      }
      S.DiagRuntimeBehavior(Loc, nullptr,
                            S.PDiag(diag::warn_comparison_always)
                                << 0 /*self-comparison*/
                                << Result);
    } else if (checkForArray(LHSStripped) && checkForArray(RHSStripped)) {
      // What is it always going to evaluate to?
      unsigned Result;
      switch (Opc) {
      case BO_EQ: // e.g. array1 == array2
        Result = AlwaysFalse;
        break;
      case BO_NE: // e.g. array1 != array2
        Result = AlwaysTrue;
        break;
      default: // e.g. array1 <= array2
        // The best we can say is 'a constant'
        Result = AlwaysConstant;
        break;
      }
      S.DiagRuntimeBehavior(Loc, nullptr,
                            S.PDiag(diag::warn_comparison_always)
                                << 1 /*array comparison*/
                                << Result);
    }
  }

  if (isa<CastExpr>(LHSStripped))
    LHSStripped = LHSStripped->IgnoreParenCasts();
  if (isa<CastExpr>(RHSStripped))
    RHSStripped = RHSStripped->IgnoreParenCasts();

  // Warn about comparisons against a string constant (unless the other
  // operand is null); the user probably wants string comparison function.
  Expr *LiteralString = nullptr;
  Expr *LiteralStringStripped = nullptr;
  if ((isa<StringLiteral>(LHSStripped) || isa<ObjCEncodeExpr>(LHSStripped)) &&
      !RHSStripped->isNullPointerConstant(S.Context,
                                          Expr::NPC_ValueDependentIsNull)) {
    LiteralString = LHS;
    LiteralStringStripped = LHSStripped;
  } else if ((isa<StringLiteral>(RHSStripped) ||
              isa<ObjCEncodeExpr>(RHSStripped)) &&
             !LHSStripped->isNullPointerConstant(S.Context,
                                          Expr::NPC_ValueDependentIsNull)) {
    LiteralString = RHS;
    LiteralStringStripped = RHSStripped;
  }

  if (LiteralString) {
    S.DiagRuntimeBehavior(Loc, nullptr,
                          S.PDiag(diag::warn_stringcompare)
                              << isa<ObjCEncodeExpr>(LiteralStringStripped)
                              << LiteralString->getSourceRange());
  }
}

static ImplicitConversionKind castKindToImplicitConversionKind(CastKind CK) {
  switch (CK) {
  default: {
#ifndef NDEBUG
    llvm::errs() << "unhandled cast kind: " << CastExpr::getCastKindName(CK)
                 << "\n";
#endif
    llvm_unreachable("unhandled cast kind");
  }
  case CK_UserDefinedConversion:
    return ICK_Identity;
  case CK_LValueToRValue:
    return ICK_Lvalue_To_Rvalue;
  case CK_ArrayToPointerDecay:
    return ICK_Array_To_Pointer;
  case CK_FunctionToPointerDecay:
    return ICK_Function_To_Pointer;
  case CK_IntegralCast:
    return ICK_Integral_Conversion;
  case CK_FloatingCast:
    return ICK_Floating_Conversion;
  case CK_IntegralToFloating:
  case CK_FloatingToIntegral:
    return ICK_Floating_Integral;
  case CK_IntegralComplexCast:
  case CK_FloatingComplexCast:
  case CK_FloatingComplexToIntegralComplex:
  case CK_IntegralComplexToFloatingComplex:
    return ICK_Complex_Conversion;
  case CK_FloatingComplexToReal:
  case CK_FloatingRealToComplex:
  case CK_IntegralComplexToReal:
  case CK_IntegralRealToComplex:
    return ICK_Complex_Real;
  }
}

static bool checkThreeWayNarrowingConversion(Sema &S, QualType ToType, Expr *E,
                                             QualType FromType,
                                             SourceLocation Loc) {
  // Check for a narrowing implicit conversion.
  StandardConversionSequence SCS;
  SCS.setAsIdentityConversion();
  SCS.setToType(0, FromType);
  SCS.setToType(1, ToType);
  if (const auto *ICE = dyn_cast<ImplicitCastExpr>(E))
    SCS.Second = castKindToImplicitConversionKind(ICE->getCastKind());

  APValue PreNarrowingValue;
  QualType PreNarrowingType;
  switch (SCS.getNarrowingKind(S.Context, E, PreNarrowingValue,
                               PreNarrowingType,
                               /*IgnoreFloatToIntegralConversion*/ true)) {
  case NK_Dependent_Narrowing:
    // Implicit conversion to a narrower type, but the expression is
    // value-dependent so we can't tell whether it's actually narrowing.
  case NK_Not_Narrowing:
    return false;

  case NK_Constant_Narrowing:
    // Implicit conversion to a narrower type, and the value is not a constant
    // expression.
    S.Diag(E->getBeginLoc(), diag::err_spaceship_argument_narrowing)
        << /*Constant*/ 1
        << PreNarrowingValue.getAsString(S.Context, PreNarrowingType) << ToType;
    return true;

  case NK_Variable_Narrowing:
    // Implicit conversion to a narrower type, and the value is not a constant
    // expression.
  case NK_Type_Narrowing:
    S.Diag(E->getBeginLoc(), diag::err_spaceship_argument_narrowing)
        << /*Constant*/ 0 << FromType << ToType;
    // TODO: It's not a constant expression, but what if the user intended it
    // to be? Can we produce notes to help them figure out why it isn't?
    return true;
  }
  llvm_unreachable("unhandled case in switch");
}

static QualType checkArithmeticOrEnumeralThreeWayCompare(Sema &S,
                                                         ExprResult &LHS,
                                                         ExprResult &RHS,
                                                         SourceLocation Loc) {
  QualType LHSType = LHS.get()->getType();
  QualType RHSType = RHS.get()->getType();
  // Dig out the original argument type and expression before implicit casts
  // were applied. These are the types/expressions we need to check the
  // [expr.spaceship] requirements against.
  ExprResult LHSStripped = LHS.get()->IgnoreParenImpCasts();
  ExprResult RHSStripped = RHS.get()->IgnoreParenImpCasts();
  QualType LHSStrippedType = LHSStripped.get()->getType();
  QualType RHSStrippedType = RHSStripped.get()->getType();

  // C++2a [expr.spaceship]p3: If one of the operands is of type bool and the
  // other is not, the program is ill-formed.
  if (LHSStrippedType->isBooleanType() != RHSStrippedType->isBooleanType()) {
    S.InvalidOperands(Loc, LHSStripped, RHSStripped);
    return QualType();
  }

  // FIXME: Consider combining this with checkEnumArithmeticConversions.
  int NumEnumArgs = (int)LHSStrippedType->isEnumeralType() +
                    RHSStrippedType->isEnumeralType();
  if (NumEnumArgs == 1) {
    bool LHSIsEnum = LHSStrippedType->isEnumeralType();
    QualType OtherTy = LHSIsEnum ? RHSStrippedType : LHSStrippedType;
    if (OtherTy->hasFloatingRepresentation()) {
      S.InvalidOperands(Loc, LHSStripped, RHSStripped);
      return QualType();
    }
  }
  if (NumEnumArgs == 2) {
    // C++2a [expr.spaceship]p5: If both operands have the same enumeration
    // type E, the operator yields the result of converting the operands
    // to the underlying type of E and applying <=> to the converted operands.
    if (!S.Context.hasSameUnqualifiedType(LHSStrippedType, RHSStrippedType)) {
      S.InvalidOperands(Loc, LHS, RHS);
      return QualType();
    }
    QualType IntType =
        LHSStrippedType->castAs<EnumType>()->getDecl()->getIntegerType();
    assert(IntType->isArithmeticType());

    // We can't use `CK_IntegralCast` when the underlying type is 'bool', so we
    // promote the boolean type, and all other promotable integer types, to
    // avoid this.
    if (S.Context.isPromotableIntegerType(IntType))
      IntType = S.Context.getPromotedIntegerType(IntType);

    LHS = S.ImpCastExprToType(LHS.get(), IntType, CK_IntegralCast);
    RHS = S.ImpCastExprToType(RHS.get(), IntType, CK_IntegralCast);
    LHSType = RHSType = IntType;
  }

  // C++2a [expr.spaceship]p4: If both operands have arithmetic types, the
  // usual arithmetic conversions are applied to the operands.
  QualType Type =
      S.UsualArithmeticConversions(LHS, RHS, Loc, Sema::ACK_Comparison);
  if (LHS.isInvalid() || RHS.isInvalid())
    return QualType();
  if (Type.isNull())
    return S.InvalidOperands(Loc, LHS, RHS);

  std::optional<ComparisonCategoryType> CCT =
      getComparisonCategoryForBuiltinCmp(Type);
  if (!CCT)
    return S.InvalidOperands(Loc, LHS, RHS);

  bool HasNarrowing = checkThreeWayNarrowingConversion(
      S, Type, LHS.get(), LHSType, LHS.get()->getBeginLoc());
  HasNarrowing |= checkThreeWayNarrowingConversion(S, Type, RHS.get(), RHSType,
                                                   RHS.get()->getBeginLoc());
  if (HasNarrowing)
    return QualType();

  assert(!Type.isNull() && "composite type for <=> has not been set");

  return S.CheckComparisonCategoryType(
      *CCT, Loc, Sema::ComparisonCategoryUsage::OperatorInExpression);
}

static QualType checkArithmeticOrEnumeralCompare(Sema &S, ExprResult &LHS,
                                                 ExprResult &RHS,
                                                 SourceLocation Loc,
                                                 BinaryOperatorKind Opc) {
  if (Opc == BO_Cmp)
    return checkArithmeticOrEnumeralThreeWayCompare(S, LHS, RHS, Loc);

  // C99 6.5.8p3 / C99 6.5.9p4
  QualType Type =
      S.UsualArithmeticConversions(LHS, RHS, Loc, Sema::ACK_Comparison);
  if (LHS.isInvalid() || RHS.isInvalid())
    return QualType();
  if (Type.isNull())
    return S.InvalidOperands(Loc, LHS, RHS);
  assert(Type->isArithmeticType() || Type->isEnumeralType());

  if (Type->isAnyComplexType() && BinaryOperator::isRelationalOp(Opc))
    return S.InvalidOperands(Loc, LHS, RHS);

  // Check for comparisons of floating point operands using != and ==.
  if (Type->hasFloatingRepresentation())
    S.CheckFloatComparison(Loc, LHS.get(), RHS.get(), Opc);

  // The result of comparisons is 'bool' in C++, 'int' in C.
  return S.Context.getLogicalOperationType();
}

void Sema::CheckPtrComparisonWithNullChar(ExprResult &E, ExprResult &NullE) {
  if (!NullE.get()->getType()->isAnyPointerType())
    return;
  int NullValue = PP.isMacroDefined("NULL") ? 0 : 1;
  if (!E.get()->getType()->isAnyPointerType() &&
      E.get()->isNullPointerConstant(Context,
                                     Expr::NPC_ValueDependentIsNotNull) ==
        Expr::NPCK_ZeroExpression) {
    if (const auto *CL = dyn_cast<CharacterLiteral>(E.get())) {
      if (CL->getValue() == 0)
        Diag(E.get()->getExprLoc(), diag::warn_pointer_compare)
            << NullValue
            << FixItHint::CreateReplacement(E.get()->getExprLoc(),
                                            NullValue ? "NULL" : "(void *)0");
    } else if (const auto *CE = dyn_cast<CStyleCastExpr>(E.get())) {
        TypeSourceInfo *TI = CE->getTypeInfoAsWritten();
        QualType T = Context.getCanonicalType(TI->getType()).getUnqualifiedType();
        if (T == Context.CharTy)
          Diag(E.get()->getExprLoc(), diag::warn_pointer_compare)
              << NullValue
              << FixItHint::CreateReplacement(E.get()->getExprLoc(),
                                              NullValue ? "NULL" : "(void *)0");
      }
  }
}

// C99 6.5.8, C++ [expr.rel]
QualType Sema::CheckCompareOperands(ExprResult &LHS, ExprResult &RHS,
                                    SourceLocation Loc,
                                    BinaryOperatorKind Opc) {
  bool IsRelational = BinaryOperator::isRelationalOp(Opc);
  bool IsThreeWay = Opc == BO_Cmp;
  bool IsOrdered = IsRelational || IsThreeWay;
  auto IsAnyPointerType = [](ExprResult E) {
    QualType Ty = E.get()->getType();
    return Ty->isPointerType() || Ty->isMemberPointerType();
  };

  // C++2a [expr.spaceship]p6: If at least one of the operands is of pointer
  // type, array-to-pointer, ..., conversions are performed on both operands to
  // bring them to their composite type.
  // Otherwise, all comparisons expect an rvalue, so convert to rvalue before
  // any type-related checks.
  if (!IsThreeWay || IsAnyPointerType(LHS) || IsAnyPointerType(RHS)) {
    LHS = DefaultFunctionArrayLvalueConversion(LHS.get());
    if (LHS.isInvalid())
      return QualType();
    RHS = DefaultFunctionArrayLvalueConversion(RHS.get());
    if (RHS.isInvalid())
      return QualType();
  } else {
    LHS = DefaultLvalueConversion(LHS.get());
    if (LHS.isInvalid())
      return QualType();
    RHS = DefaultLvalueConversion(RHS.get());
    if (RHS.isInvalid())
      return QualType();
  }

  checkArithmeticNull(*this, LHS, RHS, Loc, /*IsCompare=*/true);
  if (!getLangOpts().CPlusPlus && BinaryOperator::isEqualityOp(Opc)) {
    CheckPtrComparisonWithNullChar(LHS, RHS);
    CheckPtrComparisonWithNu