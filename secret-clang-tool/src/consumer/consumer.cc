#include "consumer.h"
#include "../transformer/RecursiveSecretTyper.h"
#include <fstream>
#include <string>
#include "llvm/Support/WithColor.h"
#include <cstdlib>
RecursiveSecretConsumer::RecursiveSecretConsumer(clang::Rewriter& TheReWriter, clang::ASTContext* context)
{
    rewriter = TheReWriter;
}

void RecursiveSecretConsumer::WriteOutput(clang::SourceManager &SM){
    for(auto it = SM.fileinfo_begin(); it != SM.fileinfo_end(); it++){
        const FileEntry *FE = it->first;

        if(FE){
            FileID FID= SM.getOrCreateFileID(it->first, SrcMgr::CharacteristicKind::C_User);
            auto buffer = rewriter.getRewriteBufferFor(FID);
            if(buffer){
                // llvm::outs() << "File ID: " << FID.getHashValue() << " File Name: " << it->first.getName() << "\n";
                //change /input to /output in File Name
                SourceLocation Loc = SM.getLocForStartOfFile(FID);
                
                if(SM.isInSystemHeader(Loc)){
                    continue;
                }

                if(it->first.getName().find("secretLib/secrettype.hpp") != std::string::npos){
                    continue;
                }

                std::string fileName = (std::string)it->first.getName();
                std::string outputFileName = fileName.replace(fileName.find("input"), 5, "output");
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
            }
        }
    }
}
void RecursiveSecretConsumer::includeSecretHeader(clang::ASTContext &context){
    for(auto it = rewriter.getSourceMgr().fileinfo_begin(); it != rewriter.getSourceMgr().fileinfo_end(); it++){
        const FileEntry *FE = it->first;
        if(FE){
            FileID FID= context.getSourceManager().getOrCreateFileID(it->first, SrcMgr::CharacteristicKind::C_User);
            //print all the file names
            //llvm::outs() << "FROM INCLUDE NUMA HEADER File Name: " << it->first.getName() << "\n";
            //skip if file name is testsuite.hpp
            std::string home = std::getenv("HOME");
            if(it->first.getName().find("TestSuite.hpp") != std::string::npos){
                llvm::outs() << "Skipping :" << it->first.getName() << "\n";
                continue;
            }
            std::string includeheaders = R"(#ifdef UMF 
	                #include "numatype.hpp"
	                #include <umf/mempolicy.h>
	                #include <umf/memspace.h>
                    #include "utils_examples.h"
                    #include <stdio.h>
                    #include <string.h>
                #endif
                #include "numatype.hpp"
                )";

            // Skip invalid or built-in files
            if (FID.isInvalid() || FID == context.getSourceManager().getMainFileID())
            continue;

            // Insert the include directive at the beginning of each file
            rewriter.InsertTextBefore( context.getSourceManager().getLocForStartOfFile(FID),includeheaders);

        }
    }
}


void RecursiveSecretConsumer::HandleTranslationUnit(clang::ASTContext &context){
    llvm::outs() <<"Calling template arg transformer\n";
    RecursiveSecretTyper recursiveSecretTyper(context, rewriter);
    // // //fntransformer.start();
    recursiveSecretTyper.start();
    //fntransformer.print(llvm::outs());   
    llvm::outs() << "Get all the file names in rewriter source manager\n";
    for(auto it = rewriter.getSourceMgr().fileinfo_begin(); it != rewriter.getSourceMgr().fileinfo_end(); it++){
        rewriterFileNames.push_back( it->first.getName());
    }

    includeSecretHeader(context);       //turn this on to include numaheaders
    WriteOutput(rewriter.getSourceMgr());
}
    




