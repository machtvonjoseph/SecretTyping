#include "frontendaction.h"
#include "cast_frontendaction.h"
#include <clang/AST/ASTContext.h>
#include <clang/AST/ASTConsumer.h>
#include <clang/Frontend/CompilerInstance.h>
#include "clang/Lex/Preprocessor.h"
#include "../consumer/cast_consumer.h"
#include "../inclusiondirective/inclusiondirective.h"
//#include "../inclusiondirective/Excludeheader.h"




PreservedAnalyses HelloWorldPass::run(Function &F,
                                      FunctionAnalysisManager &AM) {
  errs() << F.getName() << "\n";
  return PreservedAnalyses::all();
}

std::unique_ptr<clang::ASTConsumer> CastNumaFrontendAction::CreateASTConsumer(clang::CompilerInstance &compiler, llvm::StringRef inFile)
{
    
    llvm::errs() << "** Ceating Consumer from Cast FEA" << inFile << "\n";
    TheRewriter.setSourceMgr(compiler.getSourceManager(), compiler.getLangOpts());
    llvm::outs() << "file hash value: " << TheRewriter.getSourceMgr().getMainFileID().getHashValue() << "\n";

    return std::make_unique<CastConsumer>(TheRewriter, &compiler.getASTContext());
}

