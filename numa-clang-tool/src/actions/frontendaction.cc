#include "frontendaction.h"

#include <clang/AST/ASTContext.h>
#include <clang/AST/ASTConsumer.h>
#include <clang/Frontend/CompilerInstance.h>
#include "clang/Lex/Preprocessor.h"
#include "../consumer/consumer.h"
#include "../inclusiondirective/inclusiondirective.h"
//#include "../inclusiondirective/Excludeheader.h"




std::unique_ptr<clang::ASTConsumer> NumaFrontendAction::CreateASTConsumer(clang::CompilerInstance &compiler, llvm::StringRef inFile)
{
    llvm::errs() << "** Creating AST consumer for: " << inFile << "\n";
    TheRewriter.setSourceMgr(compiler.getSourceManager(), compiler.getLangOpts());
    llvm::outs() << "file hash value: " << TheRewriter.getSourceMgr().getMainFileID().getHashValue() << "\n";
    return std::make_unique<NumaConsumer>(TheRewriter, &compiler.getASTContext());
}

