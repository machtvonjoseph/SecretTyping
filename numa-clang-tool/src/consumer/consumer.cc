#include "consumer.h"
#include "../transformer/templateargtransformer.h"
#include <fstream>
#include <string>
NumaConsumer::NumaConsumer(clang::Rewriter& TheReWriter, clang::ASTContext* context)
{
    rewriter = TheReWriter;
    // context->getTranslationUnitDecl()->dump();

    // TemplateArgTransformer tempArgTransformer(*context, rewriter);

    // tempArgTransformer.start();

}

PPConsumer::PPConsumer(clang::Rewriter TheReWriter, clang::ASTContext* context)
{
//Does nothing.
}



void NumaConsumer::HandleTranslationUnit(clang::ASTContext &context)
{
    //print ast
    //context.getTranslationUnitDecl()->dump();

    //rewriter.setSourceMgr(context.getSourceManager(), context.getLangOpts());
    std::ofstream outputHeader("../output/output.h");
    std::ofstream outputImpl("../output/output.cpp");

    //FunctionCallTransformer fntransformer(context, rewriter);
    llvm::outs() <<"Calling template arg transformer\n";
    TemplateArgTransformer tempArgTransformer(context, rewriter);
    // // //fntransformer.start();
    tempArgTransformer.start();
    //fntransformer.print(llvm::outs());   

    llvm::outs() <<"Something\n";
    auto buffer = rewriter.getRewriteBufferFor(context.getSourceManager().getMainFileID());

    //iterate through buffer using loop
    // for(auto it = buffer->begin(); it != buffer->end(); it++){
    //     // llvm::outs()<< "ITERATION\n";
        
    //     llvm::outs() << *it;
    // }
    // rewriter.buffer_begin()->second.write(llvm::outs());
    // print the buffer
    // if (buffer) {
    //     llvm::outs() << std::string(buffer->begin(), buffer->end());
    // }
    //print rewriter
    //rewriter.getEditBuffer(rewriter.getSourceMgr().getMainFileID());//.write(llvm::outs());
    //for each source location rewrite on the two files
    //if (buffer) {
    //     //Rewrite into header if the location is a header file
        
    // outputImpl.close();
    // outputHeader.close();
    //cast outputImpl to raw_ostream
    //outputImpl.write(llvm::outs());
    // std::error_code error_code;
    // llvm::raw_fd_ostream outFile("../output/output.cpp", error_code);
    // const clang::RewriteBuffer *RewriteBuf = rewriter.getRewriteBufferFor(rewriter.getSourceMgr().getMainFileID());
    //   if (RewriteBuf) {
    //       outFile << std::string(RewriteBuf->begin(), RewriteBuf->end());
    //   } else {
    //       llvm::errs() << "No rewrite buffer found!\n";
    //   }
    // const char *output = "../output/output.cpp";
    // llvm::raw_fd_ostream outfile(*output, true);
    // std::error_code error_code;
    // llvm::raw_fd_ostream outFile("../output/output.cpp", error_code);
    // if(buffer != nullptr)
    //     buffer->write(llvm::outs());
}
    //writeToFiles(rewriter);
    




