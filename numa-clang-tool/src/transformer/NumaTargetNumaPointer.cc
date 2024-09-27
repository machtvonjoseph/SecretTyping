
#include "NumaTargetNumaPointer.h"
#include "../utils/utils.h"
#include <clang/AST/Decl.h>
#include <clang/AST/Expr.h>
#include <clang/ASTMatchers/ASTMatchers.h>
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include <clang/Rewrite/Core/Rewriter.h>
#include <string>
#include "clang/AST/ASTConsumer.h"
#include "clang/AST/ASTContext.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Basic/SourceManager.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include "clang/Tooling/Tooling.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"
#include "llvm/Analysis/AliasAnalysis.h"
#include "llvm/Analysis/BasicAliasAnalysis.h"
#include "llvm/IRReader/IRReader.h"
#include "RecursiveNumaTyper.h"
#include <sstream>
#include <string>
#include <unordered_map>
#include <map>

#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"



NumaTargetNumaPointer::NumaTargetNumaPointer(clang::ASTContext &context, clang::Rewriter &rewriter)
    : Transformer(context, rewriter)
{}
using namespace clang;

void NumaTargetNumaPointer::start(){       
    using namespace clang::ast_matchers;
    MatchFinder templateSpecializationFinder;
    auto templateSpecializationDeclMatcher = classTemplateSpecializationDecl().bind("templateSpecializationDecl");
    templateSpecializationFinder.addMatcher(templateSpecializationDeclMatcher, this);
    templateSpecializationFinder.matchAST(context);
    return;
}


void NumaTargetNumaPointer::run(const clang::ast_matchers::MatchFinder::MatchResult &result){
    RecursiveNumaTyper RecursiveNumaTyper(*result.Context, rewriter);
    if(result.SourceManager->isInSystemHeader(result.Nodes.getNodeAs<ClassTemplateSpecializationDecl>("templateSpecializationDecl")->getSourceRange().getBegin()))
        return;
    if(result.SourceManager->getFilename(result.Nodes.getNodeAs<ClassTemplateSpecializationDecl>("templateSpecializationDecl")->getLocation()).find("../numaLib/numatype.hpp") != std::string::npos)
        return;
    if(result.SourceManager->getFilename(result.Nodes.getNodeAs<ClassTemplateSpecializationDecl>("templateSpecializationDecl")->getLocation()).empty())
        return;
    if(result.Nodes.getNodeAs<ClassTemplateSpecializationDecl>("templateSpecializationDecl")->isImplicit())
        return;
    if(result.Nodes.getNodeAs<ClassTemplateSpecializationDecl>("templateSpecializationDecl")->getNameAsString().empty())
        return;
    
    
    const auto *TemplateDecl = result.Nodes.getNodeAs<ClassTemplateSpecializationDecl>("templateSpecializationDecl")->getSpecializedTemplate();
            //check if its numa
    clang::QualType FoundType;
    llvm::APSInt FoundInt;
    llvm::outs() << "The template class name is " << TemplateDecl->getNameAsString() << "\n";

    if(TemplateDecl->getNameAsString().compare("numa") == 0){

        for (const auto &Arg : result.Nodes.getNodeAs<ClassTemplateSpecializationDecl>("templateSpecializationDecl")->getTemplateArgs().asArray()) {
            if (Arg.getKind() == clang::TemplateArgument::ArgKind::Type) {
                FoundType = Arg.getAsType();
                llvm::outs() << "The type is " << FoundType.getAsString() << "\n";
            }
            if (Arg.getKind() == clang::TemplateArgument::ArgKind::Integral) {
                llvm::outs() << "The integral is " << Arg.getAsIntegral() << "\n";
                FoundInt = Arg.getAsIntegral();
            }
        }
    }

    //print all the methods in the specialization
    for(auto method : result.Nodes.getNodeAs<ClassTemplateSpecializationDecl>("templateSpecializationDecl")->methods()){
    if (method->hasBody()) { // Check if the method has a body
        //look for CXXnewExpr in the method body
        utils::CXXNewExprVisitor CXXNewExprVisitor(result.Context);
        utils::CompoundStmtVisitor CompoundStmtVisitor(result.Context);
        utils::DeclStmtVisitor DeclStmtVisitor(result.Context);
        utils::VarDeclVisitor VarDeclVisitor(result.Context);

        Stmt *MethodBody = method->getBody(); 
        //MethodBody->dump();
        
        if(DeclStmtVisitor.TraverseStmt(MethodBody)){
            for(auto declStmt : DeclStmtVisitor.getDeclStmts()){
                if(CXXNewExprVisitor.TraverseStmt(declStmt)){
                    for(auto CXXNewExpr : CXXNewExprVisitor.getCXXNewExprs()){
                        if(CXXNewExpr){

                            llvm::outs() << "CXXNewExpr found in method: " << method->getNameAsString() << "\n";
                            std::string NewType = CXXNewExpr->getAllocatedType().getAsString();
                            //check if dump is empty
                            rewriteLocation = CXXNewExprVisitor.getCXXNewExprLocs().back();
                            llvm::outs() << "Rewrite Location: ";
                            rewriteLocation.print(llvm::outs(),context.getSourceManager());
                            llvm::outs() << "\n";
                            llvm::outs() << "New Type length: " << NewType.length() << "\n";
                            llvm::outs() << "New Type: " << NewType << "\n";

                            SourceLocation TypeLoc = CXXNewExpr->getAllocatedTypeSourceInfo()->getTypeLoc().getBeginLoc();
                            rewriter.ReplaceText(TypeLoc, NewType.length(), "numa<"+NewType+","+std::to_string(FoundInt.getExtValue())+">");

                            rewriter.InsertTextBefore(rewriteLocation, "reinterpret_cast<"+NewType+"*>(");

                            //getFIle ID 
                            clang::SourceLocation NewEndLocaiton = CXXNewExpr->getEndLoc();
                            rewriter.InsertTextAfter(NewEndLocaiton, ")");
                            RecursiveNumaTyper.constructSpecialization(result.Context,CXXNewExpr->getAllocatedType()->getAsCXXRecordDecl(), FoundInt.getExtValue());
                            fileIDs.push_back(rewriter.getSourceMgr().getFileID(rewriteLocation)); 
                        }
                    }      
                    CXXNewExprVisitor.clearCXXNewExprs();
                }
            }
        }

// Get the body
        std::string BodyStr;
        llvm::raw_string_ostream OS(BodyStr);
        // Pretty print the body
        MethodBody->printPretty(OS, nullptr, method->getASTContext().getPrintingPolicy());
        llvm::outs() << "\nMethod Body:\n" << OS.str() << "\n";
        }
    }
    return;
}

void NumaTargetNumaPointer::print(clang::raw_ostream &stream){

}