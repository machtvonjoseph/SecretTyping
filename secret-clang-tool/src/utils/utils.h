#ifndef UTILS_HPP
#define UTILS_HPP

#include <fstream>
#include <vector>
#include <string>

#include <llvm/ADT/StringRef.h>
#include <llvm/ADT/Twine.h>

#include <clang/Tooling/Tooling.h>
#include <clang/Frontend/FrontendActions.h>
#include "clang/Tooling/CompilationDatabase.h"
#include <clang/AST/Decl.h>
#include <clang/AST/Expr.h>
#include <clang/ASTMatchers/ASTMatchers.h>
#include <clang/Rewrite/Core/Rewriter.h>
#include "clang/AST/ASTConsumer.h"
#include "clang/AST/ASTContext.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Basic/SourceManager.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include "clang/Tooling/Tooling.h"
#include <sstream>
#include <string>
#include "llvm/Support/raw_ostream.h"
#include <set>
#include <map>

using namespace clang;

namespace utils
{ 
   
    class NewExprInBinaryOperatorVisitor : public RecursiveASTVisitor<NewExprInBinaryOperatorVisitor>
    {
    public:
        explicit NewExprInBinaryOperatorVisitor(clang::ASTContext *Context) : Context(Context) {}
        bool VisitBinaryOperator(BinaryOperator *BO)
        {
            // if (const Expr *LHS = BO->getLHS()) {
            //     if (const CXXNewExpr *NewExpr = dyn_cast<CXXNewExpr>(LHS->IgnoreParenImpCasts())) {
            //         llvm::outs() << "Found CXXNewExpr in BinaryOperator LHS\n";
            //     }
            // }
           
            if (const Expr *RHS = BO->getRHS()) {
                llvm::outs() << "KaylordFound Binary Operator: " << BO->getOpcodeStr() << "\n";
                BO->getRHS()->dump();
            std::string original_type;
                if(const CXXReinterpretCastExpr *ReinterpretCastExpr = dyn_cast<CXXReinterpretCastExpr>(RHS->IgnoreParenImpCasts())){

                    if(const CXXNewExpr *NewExpr = dyn_cast<CXXNewExpr>(ReinterpretCastExpr->getSubExpr()->IgnoreParenImpCasts())){
                        llvm::outs() << "KOMAGAD found a new expr under a reinterpret cast\n";
                        llvm::outs() << "The type is " << NewExpr->getType().getAsString() << "\n";
                        llvm::outs() << "The allocated type is " << NewExpr->getAllocatedType().getAsString() << "\n";
                        if ((NewExpr->getType().getAsString().substr(0, 4).compare("numa") == 0)|| (original_type.substr(0,4).compare("numa")==0))
                        {
                            llvm::outs() << "The RHS expression is" << RHS->getStmtClassName() << "\n";
                            const Expr *LHS = BO->getLHS();
                            QualType LHSType = LHS->getType();
                            llvm::outs() << "The LHS type is " << LHSType.getAsString() << "\n";
                            CXXNewExprs.insert({NewExpr,LHSType});
                        }
                    }
                    // llvm::outs() << "FromRIKFound CXXReinterpretCastExpr in BinaryOperator RHS\n";
                    // llvm::outs() << "FromRIKThe RHS expression is" << RHS->getStmtClassName() << "\n";
                    // llvm::outs() << "FromRIKThe RHS type is " << ReinterpretCastExpr->getType().getAsString() << "\n";
                    // llvm::outs() << "FROMRIKThe RHS original type is " << ReinterpretCastExpr->getSubExpr()->getType().getAsString() << "\n";
                    // original_type = ReinterpretCastExpr->getSubExpr()->getType().getAsString();
                }


                if (const CXXNewExpr *NewExpr = dyn_cast<CXXNewExpr>(RHS->IgnoreParenImpCasts())) {

                    llvm::outs() << "KFound CXXNewExpr in BinaryOperator RHS\n";
                    llvm::outs() << "KThe RHS expression is" << RHS->getStmtClassName() << "\n";
                    llvm::outs() << "KThe RHS type is " << NewExpr->getType().getAsString() << "\n";
                    llvm::outs() << "KThe RHS original type is " << NewExpr->getAllocatedType().getAsString() << "\n";
                    QualType allocatedType = NewExpr->getAllocatedType();
                    llvm::outs() << "KThe allocated type is " << allocatedType.getAsString() << "\n";
                    allocatedType.dump();

                    // CXallocatedType->getAs<CXXRecordDecl>()
                    llvm::outs()<<"Koriginal type to be checked is either " << original_type << " or " << NewExpr->getType().getAsString() << "\n";
                    //if newexprs type  is numa, then push to CXXNewExprs
                    if ((NewExpr->getType().getAsString().substr(0, 4).compare("numa") == 0)|| (original_type.substr(0,4).compare("numa")==0))
                    {
                        llvm::outs() << "The RHS expression is" << RHS->getStmtClassName() << "\n";
                        const Expr *LHS = BO->getLHS();
                        QualType LHSType = LHS->getType();
                        llvm::outs() << "The LHS type is " << LHSType.getAsString() << "\n";
                        CXXNewExprs.insert({NewExpr,LHSType});
                    }

                }
            }
            //get LHS

            
            return true; // Continue traversal
            //BinaryOperators.push_back(BO);
            //return true;
        }
        
        std::map<const CXXNewExpr *, QualType> getBinaryOperators() { return CXXNewExprs; }
        void clearBinaryOperators() { CXXNewExprs.clear(); }
    private:
        std::map<const CXXNewExpr *, QualType> CXXNewExprs;
        // std::string original_type;
        ASTContext *Context;
    };

    
    class DeclStmtVisitor : public RecursiveASTVisitor<DeclStmtVisitor>{
    public:
        explicit DeclStmtVisitor(clang::ASTContext *Context) : Context(Context) {}
        bool VisitDeclStmt(DeclStmt *DS) {
            DeclStmts.push_back(DS);
            return true;
        }
        std::vector<DeclStmt* > getDeclStmts() { return DeclStmts; }
        void clearDeclStmts() { DeclStmts.clear(); }
    private:
        std::vector<DeclStmt*> DeclStmts;
        ASTContext *Context;
    };

    bool fileExists(const std::string &file);
   

    class CompoundStmtVisitor : public RecursiveASTVisitor<CompoundStmtVisitor>{
        public:
            explicit CompoundStmtVisitor(clang::ASTContext *Context) : Context(Context) {}
            bool VisitCompoundStmt(CompoundStmt *CD) {
                CompoundStmts.push_back(CD);
                return true;
            }
            std::vector<CompoundStmt* > getCompoundStmts() { return CompoundStmts; }
            void clearCompoundStmts() { CompoundStmts.clear(); }
        private:
            std::vector<CompoundStmt*> CompoundStmts;
            ASTContext *Context;
    };

    class CXXNewExprVisitor : public RecursiveASTVisitor<CXXNewExprVisitor>{
    public:
        explicit CXXNewExprVisitor(clang::ASTContext *Context) : Context(Context) {}
        bool VisitCXXNewExpr(CXXNewExpr *NE) {
            CXXNewExprs.push_back(NE);
            SourceLocation Loc = NE->getBeginLoc();
            CXXNewExprLocs.push_back(Loc);
            // std::string FileName = SM.getFilename(Loc).str();
            // unsigned Line = SM.getSpellingLineNumber(Loc);

            // llvm::outs() << "'new' expression found at " << FileName << ":" << Line << "\n";
            return true;
        }
        std::vector<CXXNewExpr* > getCXXNewExprs() { return CXXNewExprs; }
        std::vector<SourceLocation> getCXXNewExprLocs() { return CXXNewExprLocs; }
        void clearCXXNewExprs() { CXXNewExprs.clear(); CXXNewExprLocs.clear(); }
    private:
        std::vector<CXXNewExpr*> CXXNewExprs;
        std::vector<SourceLocation> CXXNewExprLocs;
        ASTContext *Context;
    };

    class AssignmentOperatorCallVisitor :public RecursiveASTVisitor<AssignmentOperatorCallVisitor>{
    public:
        explicit AssignmentOperatorCallVisitor(clang::ASTContext *Context) : Context(Context) {}
        bool VisitCXXOperatorCallExpr(CXXOperatorCallExpr *OCE){
            // if(OCE->isLValue()){
            AssignmentOperatorCallExprs.push_back(OCE);
            SourceLocation Loc = OCE->getBeginLoc();
            AssignmentOperatorCallExprLocs.push_back(Loc);
            return true;
            // }
            // return false;
        }
        std::vector<CXXOperatorCallExpr* > getAssignmentOperatorCallExprs() { return AssignmentOperatorCallExprs; }
        std::vector<SourceLocation> getAssignmentOperatorCallExprLocs() { return AssignmentOperatorCallExprLocs; }
        void clearAssignmentOperatorCallExprs() { AssignmentOperatorCallExprs.clear(); AssignmentOperatorCallExprLocs.clear(); }
    private:
        std::vector<CXXOperatorCallExpr*> AssignmentOperatorCallExprs;
        std::vector<SourceLocation> AssignmentOperatorCallExprLocs;
        ASTContext *Context;
    };

    class MemberExprVisitor : public RecursiveASTVisitor<MemberExprVisitor>{
    public:
        explicit MemberExprVisitor(clang::ASTContext *Context) : Context(Context) {}
        bool VisitMemberExpr(MemberExpr *ME) {
            MemberExprs.push_back(ME);
            return true;
        }
        std::vector<MemberExpr* > getMemberExprs() { return MemberExprs; }
        void clearMemberExprs() { MemberExprs.clear(); }
    private:
        std::vector<MemberExpr*> MemberExprs;
        ASTContext *Context;
    };


    class VarDeclVisitor : public RecursiveASTVisitor<VarDeclVisitor>
    {
    public:
        explicit VarDeclVisitor(clang::ASTContext *Context) : Context(Context) {}
        // bool VarDeclExpr(VarDecl *VD) {
        //     llvm::outs() << "VarDecl found from inside the visitor\n";
        //     llvm::outs() << "VarDecl Name: " << VD->getNameAsString() << "\n";
        //     VarDecls.push_back(VD);
        //     return true;
        // }
        bool VisitVarDecl(VarDecl *VD) {
            llvm::outs() << "VarDecl found from inside the visitor\n";
            llvm::outs() << "VarDecl Name: " << VD->getNameAsString() << "\n";
            VarDecls.push_back(VD);
            return true;
        }
        std::vector<VarDecl* > getVarDecls() { return VarDecls; }
        void clearVarDecls() { VarDecls.clear(); }
    private:
        std::vector<VarDecl*> VarDecls;
        ASTContext *Context;
    };


    std::string getMemberInitString(std::map<std::string, std::string>& initMemberlist); 
    std::string getDelegatingInitString(CXXConstructorDecl* Ctor);
    std::string getSecretAllocatorCode(std::string secretClassName);

}

#endif
