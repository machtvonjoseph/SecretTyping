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
#include <sstream>
#include <string>

using namespace std;
using namespace llvm;
using namespace clang;
using namespace clang::tooling;
using namespace llvm::cl;


// Apply a custom category to all command-line options so that they are the
// only ones displayed.
static cl::OptionCategory MyToolCategory("my-tool options");

// CommonOptionsParser declares HelpMessage with a description of the common
// command-line options related to the compilation database and input files.
// It's nice to have this help message in all tools.
static cl::extrahelp CommonHelp(CommonOptionsParser::HelpMessage);

// A help message for this specific tool can be added afterwards.
static cl::extrahelp MoreHelp("\nMore help text...\n");

int main(int argc, const char **argv) {
  auto ExpectedParser = CommonOptionsParser::create(argc, argv, MyToolCategory);
  if (!ExpectedParser) {
    llvm::errs() << ExpectedParser.takeError();
    return 1;
  }
  CommonOptionsParser& OptionsParser = ExpectedParser.get();


  //print source path list
  for(auto &sourcePath : OptionsParser.getSourcePathList())
  {
      cout<<"Source Path: "<<sourcePath<<"\n";
  }
  cout<<"Compile Commands: \n";
  //print compile commands
  // for(auto &cmd : OptionsParser.getCompilations().getAllCompileCommands() )
  // {
  //     for(auto &arg : cmd.CommandLine)
  //     {
  //         cout<<(string)arg<<"\n";
  //     }
  // }
    const std::vector<std::string> &ExtraArgs = OptionsParser.getCompilations().getAllCompileCommands().front().CommandLine;
    std::vector<clang::tooling::CompileCommand> compileCommands = OptionsParser.getCompilations().getAllCompileCommands();
    for (auto &cmd: compileCommands)
    {
        for (auto &arg : cmd.CommandLine)
        {
            llvm::outs() << arg << "\n";
        }
    }
    // Find the position of `--` in the arguments.
    // auto it = std::find(ExtraArgs.begin(), ExtraArgs.end(), "--");
    // if (it != ExtraArgs.end()) {
    //     llvm::outs() << "Arguments passed after `--`:\n";
    //     for (++it; it != ExtraArgs.end(); ++it) {
    //         llvm::outs() << *it << "\n";
    //     }
    // } else {
    //     llvm::outs() << "No arguments passed after `--`.\n";
    // }


  ClangTool Tool(OptionsParser.getCompilations(),
                OptionsParser.getSourcePathList());

  return Tool.run(newFrontendActionFactory<NumaFrontendAction>().get());
}



    // for(auto &sourceFile : OptionParser->getSourcePathList())
    // {
    //     llvm::outs() << "Processing source file: " << sourceFile << "\n";

    //     if(utils::fileExists(sourceFile) == false)
    //     {
    //         llvm::errs() << "File: " << sourceFile << " does not exist!\n";
    //         return -1;
    //     }

    //     auto sourcetxt = utils::getSourceCode(sourceFile);
    //     auto compileCommands = option->getCompilations().getCompileCommands(getAbsolutePath(sourceFile));

    //     std::vector<std::string> compileArgs = utils::getCompileArgs(compileCommands);
    //     compileArgs.push_back("-I" + utils::getClangBuiltInIncludePath(argv[0]));
    //     for (auto &cmd: compileCommands)
    //     {
    //         for (auto &arg : cmd.CommandLine)
    //         {
    //             llvm::outs() << "My CMDS"<< arg << "\n";
    //         }
    //     }

    //     cout<<"Processing source file: "<<sourceFile<<"\n";

    //     clang::tooling::ClangTool preProcessorTool(option->getCompilations(),option->getSourcePathList());
    //     //print source path list
    //     for(auto &sourcePath : option->getSourcePathList())
    //     {
    //         cout<<"Source Path: "<<sourcePath<<"\n";
    //     }
    //     //set compilerinstance


    //     //int PreProcessorRun = preProcessorTool.run(newFrontendActionFactory<PPFrontendAction>().get());
    //     //int XRun = preProcessorTool.run(newFrontendActionFactory<NumaFrontendAction>().get());
    //     //clang::tooling::runToolOnCode(std::make_unique<PPFrontendAction>(),sourcetxt,sourceFile);

    //    // clang::tooling::runToolOnCode(std::make_unique<NumaFrontendAction>(),sourcetxt,sourceFile);
    //     cout<<"\n";

    // }
    
//     return 0;
// }


    

