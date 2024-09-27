#ifndef RECURSIVENUMATYPER_HPP
#define RECURSIVENUMATYPER_HPP

#include "transformer.h"

// #include "RecursiveNumaTyper.h"
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
class RecursiveNumaTyper : public Transformer
{
    private:
        std::map<clang::VarDecl*, const clang::CXXNewExpr*> numaDeclTable;
        std::map<clang::QualType, int64_t> specializedClasses;
        clang::SourceLocation rewriteLocation;
        std::vector<clang::FileID> fileIDs;
        
    public:
        explicit RecursiveNumaTyper(clang::ASTContext &context, clang::Rewriter &rewriter);

        virtual void start() override;
        virtual void print(clang::raw_ostream &stream) override;
        virtual void run(const clang::ast_matchers::MatchFinder::MatchResult &result);

        bool NumaDeclExists(clang::ASTContext *Context, QualType FirstTemplateArg, int64_t SecondTemplateArg);
        void addAllSpecializations(clang::ASTContext *Context);
        void extractNumaDecls(clang::Stmt *stmt, ASTContext *Context);
        bool NumaSpeclExists(QualType FirstTemplateArg, int64_t SecondTemplateArg);
        void makeVirtual(const clang::CXXRecordDecl *classDecl);

        void specializeClass(clang::ASTContext* Context, clang::QualType FirstTemplateArg, int64_t SecondTemplateArg);
        void constructSpecialization(clang::ASTContext* Context,clang::CXXRecordDecl* classDecl, int64_t nodeID);

        void numaPublicMembers(clang::ASTContext* Context, clang::SourceLocation& rewriteLocation, std::vector<clang::FieldDecl*> publicFields,std::vector<clang::CXXMethodDecl*> publicMethods, int64_t nodeID);
        void numaPrivateMembers(clang::ASTContext* Context, clang::SourceLocation& rewriteLocation,std::vector<clang::FieldDecl*> privateFields, std::vector<clang::CXXMethodDecl*> privateMethods, int64_t nodeID);

        void numaConstructors(clang::CXXConstructorDecl* Ctor, clang::SourceLocation& rewriteLocation, int64_t nodeID);
        std::string getNumaConstructorSignature(clang::CXXConstructorDecl* Ctor);
       
        void numaDestructors(clang::CXXDestructorDecl* Dtor, clang::SourceLocation& rewriteLocation, int64_t nodeID);
        //TODO: Refactor numaDestructors according to numaConstructors
        void numaMethods(clang::CXXMethodDecl* method, clang::SourceLocation& rewriteLocation, int64_t nodeID);
        std::string getNumaMethodSignature(clang::CXXMethodDecl* method);
      
        

        
};





#endif
