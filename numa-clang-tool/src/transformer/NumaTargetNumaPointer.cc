
#include "NumaTargetNumaPointer.h"
#include "../utils/utils.h"
#include <clang/AST/Decl.h>
#include <clang/AST/Expr.h>
#include <clang/ASTMatchers/ASTMatchers.h>
#include <clang/Rewrite/Core/Rewriter.h>
#include <string>
#include "clang/AST/ASTConsumer.h"
#include "clang/AST/ASTContext.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Basic/SourceManager.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include "clang/Tooling/Tooling.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"
#include "llvm/Analysis/AliasAnalysis.h"
#include "llvm/Analysis/BasicAliasAnalysis.h"
#include "llvm/IRReader/IRReader.h"
#include <sstream>
#include <string>
#include <unordered_map>
#include <map>



NumaTargetNumaPointer::NumaTargetNumaPointer(clang::ASTContext &context, clang::Rewriter &rewriter)
    : Transformer(context, rewriter)
{}
using namespace clang;

void NumaTargetNumaPointer::start()
{
    using namespace clang::ast_matchers;
    MatchFinder functionFinder;
    auto functionDeclMatcher = functionDecl().bind("functionDecl");
    functionFinder.addMatcher(functionDeclMatcher, this);
    functionFinder.matchAST(context);
}



void NumaTargetNumaPointer::run(const clang::ast_matchers::MatchFinder::MatchResult &result){
    // llvm::outs() << "Running TransformerFunction\n";
    // if (const FunctionDecl *FD = result.Nodes.getNodeAs<FunctionDecl>("funcDecl")) {
    //     llvm::errs() << "Matched function: " << FD->getNameAsString() << "\n";

    //     // Now we retrieve the LLVM function corresponding to the AST FunctionDecl
    //     if (llvm::Function *LLVMFunc = CGM->getFunction(FD)) {
    //         llvm::errs() << "LLVM IR function found: " << LLVMFunc->getName() << "\n";
    //     } else {
    //         llvm::errs() << "No corresponding LLVM IR function found.\n";
    //     }
    // }
}

void NumaTargetNumaPointer::print(clang::raw_ostream &stream)
{

}