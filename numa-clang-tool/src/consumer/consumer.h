#ifndef CONSUMER_HPP
#define CONSUMER_HPP

#include <clang/AST/ASTContext.h>
#include <clang/AST/ASTConsumer.h>
#include <clang/Rewrite/Core/Rewriter.h>

namespace clang
{
    class ASTContext;
}

class NumaConsumer : public clang::ASTConsumer 
{
    private:
    
        clang::Rewriter rewriter;

    public:
        explicit NumaConsumer(clang::Rewriter& TheReWriter, clang::ASTContext* context);
        
        virtual void HandleTranslationUnit( clang::ASTContext &context) override;
};

class PPConsumer : public clang::ASTConsumer 
{
    private:
    
        clang::Rewriter rewriter;

    public:
        explicit PPConsumer(clang::Rewriter TheReWriter, clang::ASTContext* context);
        //PPConsumer is a deadend. The PPFrontend action has already taken care of the include directives.
};



#endif
