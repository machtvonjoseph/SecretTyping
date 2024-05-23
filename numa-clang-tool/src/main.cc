#include <llvm/Support/CommandLine.h>
#include <clang/Tooling/CommonOptionsParser.h>
#include <clang-c/Index.h>
#include <iostream>
#include "actions/frontendaction.h"
#include "utils/utils.h"
#include "clang/Tooling/Tooling.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/FrontendActions.h"
#include "clang/Basic/DiagnosticOptions.h"
#include "clang/Basic/FileManager.h"
#include "clang/Basic/SourceManager.h"
#include "clang/Basic/TargetOptions.h"
#include "clang/CodeGen/ModuleBuilder.h"
#include "clang/Parse/ParseAST.h"
#include "llvm/Support/Host.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Support/InitLLVM.h"
#include "llvm/Support/raw_ostream.h"
#include <string>
#include <iostream>
#include<sstream>
#include <string>

using namespace std;
using namespace llvm;
using namespace clang;
using namespace clang::tooling;
using namespace llvm::cl;






int main(int argc, const char **argv)
{
    llvm::cl::OptionCategory ctCategory("clang-tool options");
    llvm::Expected<clang::tooling::CommonOptionsParser> option = CommonOptionsParser::create(argc, argv, ctCategory);

    for(auto &sourceFile : option->getSourcePathList())
    {
        llvm::outs() << "Processing source file: " << sourceFile << "\n";

        if(utils::fileExists(sourceFile) == false)
        {
            llvm::errs() << "File: " << sourceFile << " does not exist!\n";
            return -1;
        }

        auto sourcetxt = utils::getSourceCode(sourceFile);
        auto compileCommands = option->getCompilations().getCompileCommands(getAbsolutePath(sourceFile));

        std::vector<std::string> compileArgs = utils::getCompileArgs(compileCommands);
        compileArgs.push_back("-I" + utils::getClangBuiltInIncludePath(argv[0]));
        for (auto &cmd: compileCommands)
        {
            for (auto &arg : cmd.CommandLine)
            {
                llvm::outs() << "My CMDS"<< arg << "\n";
            }
        }

        cout<<"Processing source file: "<<sourceFile<<"\n";

        clang::tooling::ClangTool preProcessorTool(option->getCompilations(),option->getSourcePathList());
        //print source path list
        for(auto &sourcePath : option->getSourcePathList())
        {
            cout<<"Source Path: "<<sourcePath<<"\n";
        }
        //set compilerinstance


        //int PreProcessorRun = preProcessorTool.run(newFrontendActionFactory<PPFrontendAction>().get());
        //int XRun = preProcessorTool.run(newFrontendActionFactory<NumaFrontendAction>().get());
        //clang::tooling::runToolOnCode(std::make_unique<PPFrontendAction>(),sourcetxt,sourceFile);

        clang::tooling::runToolOnCode(std::make_unique<NumaFrontendAction>(),sourcetxt,sourceFile);
        cout<<"\n";

    }
    
    return 0;
}


    

