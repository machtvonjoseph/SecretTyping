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
#include "llvm/TargetParser/Host.h"
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

std::vector<std::string> benchmarks = {"Dummy","Data-Structures"};


void registerBenchmarks(){
    for(auto &benchmark : benchmarks)
  {
      std::string directory = "../output/" + benchmark;
      if(utils::fileExists(directory) == true)
      {
          //remove everything in the directory
          std::string command = "rm -rf " + directory + "/*";
          system(command.c_str());
      }
      else
      {
          std::string command = "mkdir -p " + directory;
          system(command.c_str());
      }
  }

  //For each benchmark, copy the contents from the input directory to the output directory
  for(auto &benchmark : benchmarks)
  {
      std::string inputDirectory = "../input/" + benchmark;
      std::string outputDirectory = "../output/" + benchmark;
      std::string command = "cp -r " + inputDirectory + "/* " + outputDirectory;
      system(command.c_str());
  }
}

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
  const std::vector<std::string> &ExtraArgs = OptionsParser.getCompilations().getAllCompileCommands().front().CommandLine;
  std::vector<clang::tooling::CompileCommand> compileCommands = OptionsParser.getCompilations().getAllCompileCommands();
  for (auto &cmd: compileCommands)
  {
      for (auto &arg : cmd.CommandLine)
      {
          llvm::outs() << arg << "\n";
      }
  }
    
  registerBenchmarks();


  ClangTool Tool(OptionsParser.getCompilations(),
                OptionsParser.getSourcePathList());

  return Tool.run(newFrontendActionFactory<NumaFrontendAction>().get());
  
}
