#include "consumer.h"
#include "../transformer/templateargtransformer.h"
#include <fstream>
#include <string>
#include "llvm/Support/WithColor.h"
NumaConsumer::NumaConsumer(clang::Rewriter& TheReWriter, clang::ASTContext* context)
{
    rewriter = TheReWriter;
}

void NumaConsumer::WriteOutput(clang::SourceManager &SM){
    for(auto it = SM.fileinfo_begin(); it != SM.fileinfo_end(); it++){
        const FileEntry *FE = it->first;

        if(FE){
            FileID FID= SM.getOrCreateFileID(it->first, SrcMgr::CharacteristicKind::C_User);
            auto buffer = rewriter.getRewriteBufferFor(FID);
            if(buffer){
                llvm::outs() << "File ID: " << FID.getHashValue() << " File Name: " << it->first.getName() << "\n";
                //change /input to /output in File Name
                std::string fileName = (std::string)it->first.getName();
                std::string outputFileName = fileName.replace(fileName.find("input"), 5, "output");
                llvm::outs() << "Output File Name: " << outputFileName << "\n";
                //if directory doesn't exist create it
            
                std::string directory = outputFileName.substr(0, outputFileName.find_last_of("/"));
                std::string command = "mkdir -p " + directory;  
                system(command.c_str());

                std::error_code EC;
                llvm::raw_fd_ostream OutFile((llvm::StringRef)outputFileName, EC, llvm::sys::fs::OF_Text);
                if(EC){
                    llvm::errs() << "Error opening output file: " << EC.message() << "\n";
                    return;
                }
                buffer->write(OutFile);
                //write buffer
                // buffer->write(llvm::outs());
            }
        }
    }
}


void NumaConsumer::HandleTranslationUnit(clang::ASTContext &context){
    llvm::outs() <<"Calling template arg transformer\n";
    TemplateArgTransformer tempArgTransformer(context, rewriter);
    // // //fntransformer.start();
    tempArgTransformer.start();
    //fntransformer.print(llvm::outs());   
    llvm::outs() << "Get all the file names in rewriter source manager\n";
    for(auto it = rewriter.getSourceMgr().fileinfo_begin(); it != rewriter.getSourceMgr().fileinfo_end(); it++){
        rewriterFileNames.push_back( it->first.getName());
    }
    WriteOutput(rewriter.getSourceMgr());
}
    




