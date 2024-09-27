#include "utils.h"


namespace utils
{

bool fileExists(const std::string &file)
{
    return std::ifstream(file).good();
}

// std::vector<std::string> getCompileArgs(const std::vector<clang::tooling::CompileCommand> &compileCommands)
// {
//     std::vector<std::string> compileArgs;

//     for(auto &cmd : compileCommands)
//     {
//         for(auto &arg : cmd.CommandLine)
//             compileArgs.push_back(arg);
//     }

//     if(compileArgs.empty() == false)
//     {
//         compileArgs.erase(begin(compileArgs));
//         compileArgs.pop_back();
//     }

//     return compileArgs;
// }

// std::string getSourceCode(const std::string &sourceFile)
// {
//     std::string sourcetxt = "";
//     std::string temp = "";

//     std::ifstream file(sourceFile);

//     while(std::getline(file, temp))
//         sourcetxt += temp + "\n";

//     return sourcetxt;
// }

// std::string getClangBuiltInIncludePath(const std::string &fullCallPath)
// {
//     auto currentPath = fullCallPath;
//     currentPath.erase(currentPath.rfind("/"));

//     std::string line;
//     std::ifstream file(currentPath + "/builtInInclude.path");
//     std::getline(file, line);

//     return line;
// }

}
