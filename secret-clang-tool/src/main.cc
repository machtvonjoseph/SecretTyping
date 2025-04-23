#include <llvm/Support/CommandLine.h>
#include <clang/Tooling/CommonOptionsParser.h>
#include <clang-c/Index.h>
#include <iostream>
#include "actions/frontendaction.h"
#include "actions/cast_frontendaction.h"
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

#include "clang/AST/AST.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"


#include "clang/Frontend/CompilerInstance.h"
#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Basic/SourceManager.h"

#include <filesystem>
using namespace std;
using namespace llvm;
using namespace clang;
using namespace clang::tooling;
using namespace llvm::cl;

const char* HOME = std::getenv("HOME");
// Apply a custom category to all command-line options so that they are the
// only ones displayed.
static cl::OptionCategory MyToolCategory("my-tool options");

// CommonOptionsParser declares HelpMessage with a description of the common
// command-line options related to the compilation database and input files.
// It's nice to have this help message in all tools.
static cl::extrahelp CommonHelp(CommonOptionsParser::HelpMessage);

// A help message for this specific tool can be added afterwards.
static cl::extrahelp MoreHelp("\nMore help text...\n");

std::vector<std::string> benchmarks = {"SecretExprs"};
namespace fs = std::filesystem;

void registerBenchmarks(){

    std::string inputDirectory = "input/";
    std::string outputDirectory = "output/";
    std::string output2Directory = "output2/";

    for(auto &benchmark : benchmarks){
        //import the benchmarkst to the input directory
        std::string baseDir = (std::string)HOME + "/SecretTyping/secret-clang-tool/";
        if (fs::exists(baseDir+"input/"+benchmark) && fs::is_directory(baseDir+"input/"+benchmark)) {
            std::string command = "rm -rf " + baseDir+"input/"+benchmark;
            system(command.c_str());
            command = "cp -r " +  (std::string)HOME + "/SecretTyping/" + benchmark + " " + baseDir+"input/";
            system(command.c_str());
        } else {
            std::string command = "cp -r " +  (std::string)HOME + "/SecretTyping/" + benchmark + " " + baseDir+"input/";
            system(command.c_str());
        }

        if (fs::exists(baseDir+"output/"+benchmark) && fs::is_directory(baseDir+"output/"+benchmark)) {
            std::string command = "rm -rf " + baseDir +"output/"+ benchmark+"/*";
            system(command.c_str());
        }
        if (fs::exists(baseDir+"output2/"+benchmark) && fs::is_directory(baseDir+"output2/"+benchmark)) {
            std::string command = "rm -rf " + baseDir +"output2/"+ benchmark+"/*";
            system(command.c_str());
        }
    
    
        
        //remove everything from output
        if(utils::fileExists(outputDirectory) == true){   
            std::string command = "rm -rf " + outputDirectory + "/*";
            system(command.c_str());
        }
        else{
            std::string command = "mkdir -p " + outputDirectory;
            system(command.c_str());
        }

        std::string command = "cp -r " + inputDirectory + "/* " + outputDirectory;
        system(command.c_str());
    }
}

void copyOuputToOutput2(){
    for(auto &benchmark : benchmarks)
  {
      std::string directory = "output2/" + benchmark;
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
      std::string inputDirectory = "output/" + benchmark;
      std::string outputDirectory = "output2/" + benchmark;
      std::string command = "cp -r " + inputDirectory + "/* " + outputDirectory;
      system(command.c_str());
  }
}

void print(const std::vector<CompileCommand> &Commands) {
  if (Commands.empty()) {
    return;
  }
  for (auto opt : Commands[0].CommandLine) {
    llvm::errs() << "\t" << opt << "\n";
  }
}


int main(int argc, const char **argv) {

    cout<<"Starting the tool..."<<endl;

    static cl::OptionCategory ToolCategory("my-clang-tool options");

    static cl::opt<bool> SecretFrontendAction(
        "secret",
        cl::desc("Run secret frontend action"),
        cl::cat(ToolCategory)
    );

    static cl::opt<bool> CastFrontendAction(
        "cast",
        cl::desc("Run the cast frontend action"),
        cl::cat(ToolCategory)
    );


    auto ExpectedParser = CommonOptionsParser::create(argc, argv, ToolCategory);

    if (!ExpectedParser) {
        llvm::errs() << ExpectedParser.takeError();
        return 1;
    }
    CommonOptionsParser& OptionsParser = ExpectedParser.get();

    std::unique_ptr<FrontendActionFactory> Factory;

    if (SecretFrontendAction) {
        registerBenchmarks();
        Factory = newFrontendActionFactory<SecretRecursiveFrontendAction>();
    } else if (CastFrontendAction) {
        // copyOuputToOutput2();
        // Factory = newFrontendActionFactory<CastNumaFrontendAction>();
    } else {
        llvm::errs() << "Please specify either --count-functions or --count-classes.\n";
        return 1;
    }
    ClangTool Tool(OptionsParser.getCompilations(), OptionsParser.getSourcePathList());
    return Tool.run(Factory.get());
    //return 0;
}
