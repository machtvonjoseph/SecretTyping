#ifndef TEMPLATEARG_TRANSFORMER_HPP
#define TEMPLATEARG_TRANSFORMER_HPP

#include "transformer.h"

#include "templateargtransformer.h"
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

// namespace clang
// {
//     class ASTContext;
//     class raw_ostream;
//     class Rewriter;
// }
using namespace clang;

class CompoundStmtVisitor : public RecursiveASTVisitor<CompoundStmtVisitor>
{
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

class DeclStmtVisitor : public RecursiveASTVisitor<DeclStmtVisitor>
{
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


class CXXNewExprVisitor : public RecursiveASTVisitor<CXXNewExprVisitor>
{
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

class TemplateArgTransformer : public Transformer
{
    private:
        std::set<std::string> functions;
        std::vector<const clang::FunctionDecl*> functionDecls;
        std::map<clang::VarDecl*, const clang::CXXNewExpr*> numaDeclTable;
        std::vector<const clang::CXXMethodDecl*> methodDecls;
    public:
        void setNumaDeclTable(clang::VarDecl* varDecl, const clang::CXXNewExpr* newExpr){
            numaDeclTable[varDecl] = newExpr;
        }
        std::map<clang::VarDecl*, const clang::CXXNewExpr*> getNumaDeclTable(){
            return numaDeclTable;
        }
        void clearNumaDeclTable(){
            numaDeclTable.clear();
        }
        void printNumaDeclTable(){
            llvm::outs()<<"------------------------------Printing NumaDeclTable----------------------------------\n";
            llvm::outs()<<"Size of NumaDeclTable: "<<numaDeclTable.size()<<"\n";
            for(auto it = numaDeclTable.begin(); it != numaDeclTable.end(); it++){
                llvm::outs()<<"VarDecl Name: " << it->first->getNameAsString() << "\n";
                llvm::outs()<<"VarType: " << it->first->getType().getAsString() << "\n";
                llvm::outs()<<"CXXNewExpr Return Type: " << it->second->getType().getAsString() << "\n";
            }
        }

        bool NumaDeclExists(clang::ASTContext *Context, QualType FirstTemplateArg, int64_t SecondTemplateArg);
        void makeVirtual(clang::CXXRecordDecl *classDecl);

        void startRecursiveTyping(std::map<clang::VarDecl*, const clang::CXXNewExpr*> numaDeclTable, clang::ASTContext *Context);

        std::map<std::string, std::map<std::string, bool>> field_info_table;
        std::vector<std::string> method_info_table;
        //std::map<int,std::map<llvm::MutableArrayRef<clang::ParmVarDecl *>,clang::Stmt*>> constructor_info_table;
        std::string className; 
        clang::CXXRecordDecl *numaed_class; 
        std::string integerarg;
        clang::SourceLocation endLoc;
        clang::SourceLocation rewriteLoc;
        
        explicit TemplateArgTransformer(clang::ASTContext &context, clang::Rewriter &rewriter);

        virtual void start() override;
        virtual void run(const clang::ast_matchers::MatchFinder::MatchResult &result);
        virtual void print(clang::raw_ostream &stream) override;
        void findNumaExpr(clang::CompoundStmt *compoundStmt, ASTContext *Context);
        void myTemplateTransformer(clang::VarDecl *varDecl);
        void extractNumaDecls(clang::Stmt *stmt, ASTContext *Context);
        void recursive_introspective_typer(clang::FieldDecl *field, std::string N, clang::SourceLocation endLoc,clang::ClassTemplateSpecializationDecl* tsd);
        //void extractFunctions(const FunctionDecl *func);
        void constructTemplate_fieldPtr(clang::FieldDecl *field, std::string N, clang::SourceLocation endLoc);
        void fundamental_introspective_typer(std::string className, std::string N, clang::SourceLocation endLoc, clang::ClassTemplateSpecializationDecl* tsd);
        std::string replaceNewType(const std::string& str, std::string N);
        std::string replaceConstructWithInits(const std::string& input); 
};





#endif
