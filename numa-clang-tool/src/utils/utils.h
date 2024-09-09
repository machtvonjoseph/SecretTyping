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
{ class DeclStmtVisitor : public RecursiveASTVisitor<DeclStmtVisitor>{
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
            return true;
        }
        std::vector<CXXNewExpr* > getCompoundStmts() { return CXXNewExprs; }
        void clearCXXNewExprs() { CXXNewExprs.clear(); }
    private:
        std::vector<CXXNewExpr*> CXXNewExprs;
        ASTContext *Context;
    };


    class VarDeclVisitor : public RecursiveASTVisitor<VarDeclVisitor>
    {
    public:
        explicit VarDeclVisitor(clang::ASTContext *Context) : Context(Context) {}
        bool VarDeclExpr(VarDecl *VD) {
            VarDecls.push_back(VD);
            return true;
        }
        std::vector<VarDecl* > getVarDecls() { return VarDecls; }
    private:
        std::vector<VarDecl*> VarDecls;
        ASTContext *Context;
    };


    std::string getMemberInitString(std::map<std::string, std::string>& initMemberlist); 
    std::string getDelegatingInitString(CXXConstructorDecl* Ctor);


    



    // std::vector<std::string> getSyntaxOnlyToolArgs(const std::vector<std::string> &ExtraArgs, llvm::StringRef FileName);

    // bool customRunToolOnCodeWithArgs(std::unique_ptr<clang::FrontendAction> frontendAction, const llvm::Twine &Code,
    //                                  const std::vector<std::string> &Args, const llvm::Twine &FileName,
    //                                  const clang::tooling::FileContentMappings &VirtualMappedFiles = clang::tooling::FileContentMappings());


    
    // std::vector<std::string> getCompileArgs(const std::vector<clang::tooling::CompileCommand> &compileCommands);
    // std::string getSourceCode(const std::string &sourceFile);

    // std::string getClangBuiltInIncludePath(const std::string &fullCallPath);
}

#endif
