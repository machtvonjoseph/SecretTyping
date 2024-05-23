#ifndef TEMPLATEARG_TRANSFORMER_HPP
#define TEMPLATEARG_TRANSFORMER_HPP

#include "transformer.h"

#include <string>
#include <set>
#include <map>

namespace clang
{
    class ASTContext;
    class raw_ostream;
    class Rewriter;
}

class TemplateArgTransformer : public Transformer
{
    private:
        
        std::set<std::string> functions;
        
    public:
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
        void myTemplateTransformer(clang::VarDecl *varDecl);
        void recursive_introspective_typer(clang::FieldDecl *field, std::string N, clang::SourceLocation endLoc,clang::ClassTemplateSpecializationDecl* tsd);
        void constructTemplate_fieldPtr(clang::FieldDecl *field, std::string N, clang::SourceLocation endLoc);
        void fundamental_introspective_typer(std::string className, std::string N, clang::SourceLocation endLoc, clang::ClassTemplateSpecializationDecl* tsd);
        std::string replaceNewType(const std::string& str, std::string N);
        std::string replaceConstructWithInits(const std::string& input); 
};





#endif
