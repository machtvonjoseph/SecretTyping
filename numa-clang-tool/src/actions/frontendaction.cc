#include "frontendaction.h"

#include <clang/AST/ASTContext.h>
#include <clang/AST/ASTConsumer.h>
#include <clang/Frontend/CompilerInstance.h>
#include "clang/Lex/Preprocessor.h"
#include "../consumer/consumer.h"
#include "../inclusiondirective/inclusiondirective.h"
#include "../inclusiondirective/Excludeheader.h"

std::unique_ptr<clang::ASTConsumer> PPFrontendAction::CreateASTConsumer(clang::CompilerInstance &compiler, llvm::StringRef inFile)
{

    // llvm::errs() << "** Creating AST consumer for: " << inFile << "\n";
    TheRewriter.setSourceMgr(compiler.getSourceManager(), compiler.getLangOpts());
    clang::Preprocessor &PP = compiler.getPreprocessor();
    clang::SourceManager &SM = compiler.getSourceManager();
    TheRewriter.setSourceMgr(SM, compiler.getLangOpts());

    PP.addPPCallbacks(std::make_unique<IncludeChangePPCallbacks>(SM, TheRewriter));

    return std::make_unique<PPConsumer>(TheRewriter, &compiler.getASTContext());

}


std::unique_ptr<clang::ASTConsumer> NumaFrontendAction::CreateASTConsumer(clang::CompilerInstance &compiler, llvm::StringRef inFile)
{
    TheRewriter.setSourceMgr(compiler.getSourceManager(), compiler.getLangOpts());
    compiler.getPreprocessor().addPPCallbacks(std::make_unique<ExcludeHeaderCallback>(compiler.getPreprocessor()));
    return std::make_unique<NumaConsumer>(TheRewriter, &compiler.getASTContext());
}

// void NumaFrontendAction::EndSourceFileAction() {
//     clang::SourceManager &SM = TheRewriter.getSourceMgr();
//     llvm::errs() << "** EndSourceFileAction for: "
//                  << SM.getFileEntryForID(SM.getMainFileID())->getName() << "\n";

//     // Now emit the rewritten buffer.
//     TheRewriter.getEditBuffer(SM.getMainFileID()).write(llvm::outs()); //--> this will output to screen as what you got.
//     std::error_code error_code;
//     llvm::raw_fd_ostream outFile("../output/output.cpp", error_code);
//     const clang::RewriteBuffer *RewriteBuf = TheRewriter.getRewriteBufferFor(SM.getMainFileID());
//       if (RewriteBuf) {
//         RewriteBuf->write(llvm::outs());
//         //outFile << std::string(RewriteBuf->begin(), RewriteBuf->end());
//       } else {
//         llvm::errs() << "No rewrite buffer found!\n";
//       }

//     //TheRewriter.getEditBuffer(SM.getMainFileID()).write(outFile); // --> this will write the result to outFile
//     outFile.close();
//   }