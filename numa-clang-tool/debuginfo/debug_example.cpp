#include "llvm/Pass.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/DebugInfoMetadata.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;

namespace {
    struct VariableNamesPass : public FunctionPass {
        static char ID;
        VariableNamesPass() : FunctionPass(ID) {}

        bool runOnFunction(Function &F) override {
            for (auto& BB : F) {
                for (auto& I : BB) {
                    if (auto* dbgInst = dyn_cast<DbgDeclareInst>(&I)) {
                        Value* var = dbgInst->getAddress();
                        if (auto* localVar = dyn_cast<AllocaInst>(var)) {
                            std::string varName = localVar->getName().str();
                            std::string varType = localVar->getAllocatedType()->getPointerElementType()->getStructName().str();
                            errs() << "Variable Name: " << varName << ", Type: " << varType << "\n";
                        }
                    }
                }
            }
            return false; // We haven't modified the function
        }
    };
}

char VariableNamesPass::ID = 0;
static RegisterPass<VariableNamesPass> X("variable-names", "Variable Names Pass", false, false);
