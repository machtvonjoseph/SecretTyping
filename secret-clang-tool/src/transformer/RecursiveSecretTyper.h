#ifndef RECURSIVESECRETTYPER_HPP
#define RECURSIVESECRETTYPER_HPP

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
class RecursiveSecretTyper : public Transformer
{
    private:
        std::vector<const clang::CXXNewExpr*> numaDeclTable;
        std::vector<std::pair<const clang::CXXRecordDecl*, int64_t>> specializedClasses;
        std::vector<const clang::CXXRecordDecl*> specializedSecretClasses;
        clang::SourceLocation rewriteLocation;
        std::vector<clang::FileID> fileIDs;
        
    public:
        
        explicit RecursiveSecretTyper(clang::ASTContext &context, clang::Rewriter &rewriter);

        virtual void start() override;
        virtual void print(clang::raw_ostream &stream) override;
        virtual void run(const clang::ast_matchers::MatchFinder::MatchResult &result);

        bool NumaDeclExists(clang::ASTContext *Context, QualType FirstTemplateArg, int64_t SecondTemplateArg);
        void addAllSpecializations(clang::ASTContext *Context);
        std::vector<std::pair<const clang::CXXRecordDecl*, int64_t>> getSpecializedClasses(){
            return specializedClasses;
        }
        void extractNumaDecls(clang::Stmt *stmt, ASTContext *Context);
        bool NumaSpeclExists(const clang::CXXRecordDecl* FirstTemplateArg, int64_t SecondTemplateArg);
        void makeVirtual(const clang::CXXRecordDecl *classDecl);

        void specializeClass(clang::ASTContext* Context, const clang::CXXRecordDecl* secretClass);
        void constructSpecialization(clang::ASTContext* Context,const clang::CXXRecordDecl* secretClass);

        void secretPublicMembers(clang::ASTContext* Context, clang::SourceLocation& rewriteLocation, std::vector<clang::FieldDecl*> publicFields,std::vector<clang::CXXMethodDecl*> publicMethods);
        void secretPrivateMembers(clang::ASTContext* Context, clang::SourceLocation& rewriteLocation,std::vector<clang::FieldDecl*> privateFields, std::vector<clang::CXXMethodDecl*> privateMethods);

        void secretConstructors(clang::CXXConstructorDecl* Ctor, clang::SourceLocation& rewriteLocation);
        std::string getSecretConstructorSignature(clang::CXXConstructorDecl* Ctor);
       
        void secretDestructors(clang::CXXDestructorDecl* Dtor, clang::SourceLocation& rewriteLocation);
        //TODO: Refactor numaDestructors according to numaConstructors
        void secretMethods(clang::CXXMethodDecl* method, clang::SourceLocation& rewriteLocation);
        std::string getSecretMethodSignature(clang::CXXMethodDecl* method);
    
        
};





#endif
