#include "llvm/Transforms/Utils/FuncPointerAnalysis.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Instructions.h"
#include "llvm/Analysis/AliasAnalysis.h"
#include "llvm/Analysis/BasicAliasAnalysis.h"
#include "llvm/IR/PassManager.h"
#include "llvm/IR/Module.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Transforms/Scalar.h"
#include "llvm/ADT/SetVector.h"
#include "llvm/IR/DataLayout.h"
#include "llvm/IR/InstIterator.h"
#include "llvm/InitializePasses.h"
#include "llvm/Pass.h"
#include "llvm/Support/CommandLine.h"
#include "unordered_map"
using namespace llvm;
using namespace std;

PreservedAnalyses FuncPointerAnalysisPass::run(Module &F,
                                      ModuleAnalysisManager &FAM) {

  runInternal(F, FAM.getResult<AAManager>(F));
  return PreservedAnalyses::all();
}




void printMyResult(const llvm::Value* val1,const llvm::Value* val2) {
   
}

Instruction myinst;
void FuncPointerAnalysisPass::runInternal(Module &M, AAResults &AA){
    for(auto &F :M){
    const DataLayout &DL = F.getParent()->getDataLayout();

    //get modref info
    


    ++FunctionCount;
    //std::unordered_map<bool, std::pair<const Value *, Type *>> Pointers;
    SetVector<std::pair<const Value *, Type *>> Pointers;
    SmallSetVector<CallBase *, 16> Calls;
    //SetVector<Value *> Loads;
    //std::unordered_map<llvm::StringRef, Value *> Loads;
    MapVector<llvm::StringRef, Value *> Loads;
    SetVector<Value *> Stores;
    for (Instruction &Inst : instructions(F)) {  
        Instruction* myInst = dyn_cast<Instruction>(&Inst);
        if (auto *LI = dyn_cast<LoadInst>(&Inst)) {
            
            // LI->getOperandList();
            // //print operand lists
            // for (unsigned i = 0; i < LI->getNumOperands(); i++) {
            //     errs() << "Operand " << i << ": " << LI->getOperand(i)->getName() << "\n";
            // }
            LoadInst *LIm = dyn_cast<LoadInst>(&Inst);
            Instruction* myInst = dyn_cast<Instruction>(LIm);
            Pointers.insert({LI->getPointerOperand(), LI->getType()});
            // errs() << "Load Instruction Name: " << LI->getName()<< "\n";
            // errs() << "Load Pointer: " << LI->getPointerOperand()->getName() << " : Value" << LI->getPointerOperand()->getValueName() << "\n";
            // errs() << "Load Value: " << LI->getValueName() << "\n";
            Loads.insert({LI->getName(),LI->getPointerOperand()});
        } else if (auto *SI = dyn_cast<StoreInst>(&Inst)) {
            Pointers.insert({SI->getPointerOperand(),SI->getValueOperand()->getType()});
            // errs() << "Store Pointer: " << SI->getPointerOperand()->getName() << " : Value" << SI->getPointerOperand()->getValueName() << "\n";
            // errs() << "Store Value: " << SI->getValueOperand()->getValueName() << "\n";
            Stores.insert(SI);
        } else if (auto *CB = dyn_cast<CallBase>(&Inst))
            Calls.insert(CB);
    }
    //print Pointers
    // for (auto I1 = Pointers.begin(), E = Pointers.end(); I1 != E; ++I1) {
    //     errs() << "Pointer: " << I1->first->getName() << " : Value" <<I1->first->get<<"\n";
    // }

    //print Loads
    errs() << "About to print loads\n" << "\n"; 
    for (auto I1 = Loads.begin(), E = Loads.end(); I1 != E; ++I1) {
        //errs() << "Load: " << I1->first << " : Value: " << I1->second->getName() << "\n";
    }

    errs() << "Function: " << F.getName() << ": " << Pointers.size()
           << " pointers, " << Calls.size() << " call sites\n";
    // iterate over the worklist, and run the full (n^2)/2 disambiguations
    for (auto I1 = Pointers.begin(), E = Pointers.end(); I1 != E; ++I1) {
        LocationSize Size1 = LocationSize::precise(DL.getTypeStoreSize(I1->second));
        for (auto I2 = Pointers.begin(); I2 != I1; ++I2) {
            LocationSize Size2 =
                LocationSize::precise(DL.getTypeStoreSize(I2->second));
            AliasResult AR = AA.alias(I1->first, Size1, I2->first, Size2);
            
            ModRefInfo MRI = AA.getModRefInfo(&myinst, I1->first, Size1);
            switch (AR) {
            case AliasResult::NoAlias:
            //PrintResults(AR, PrintNoAlias, *I1, *I2, F.getParent());
                if (Loads.find(I1->first->getName()) != Loads.end() && Loads.find(I2->first->getName()) != Loads.end()) {
                    errs() << "Pointer " << Loads[I1->first->getName()]->getName()  << " and Pointer " << Loads[I2->first->getName()]->getName()  << " do not alias\n";
                }
                else if (Loads.find(I1->first->getName()) != Loads.end() && Loads.find(I2->first->getName()) == Loads.end()){
                    errs() << "Pointer " << Loads[I1->first->getName()]->getName() << " and Pointer " << I2->first->getName() << " do not alias\n";
                }
                else if (Loads.find(I1->first->getName()) == Loads.end() && Loads.find(I2->first->getName()) != Loads.end()){
                    errs() << "Pointer " << I1->first->getName() << " and Pointer " << Loads[I2->first->getName()]->getName()  << " do not alias\n";
                }    
                else if (Loads.find(I1->first->getName()) == Loads.end() && Loads.find(I2->first->getName()) == Loads.end()){
                    errs() << "Pointer "<< I1->first->getName() << " and Pointer " << I2->first->getName() << " do not alias\n";   
                }  
            ++NoAliasCount;
            break;
            case AliasResult::MayAlias:
            //PrintResults(AR, PrintMayAlias, *I1, *I2, F.getParent());
            if(Loads.find(I1->first->getName()) != Loads.end() && Loads.find(I2->first->getName()) != Loads.end()) {
                errs() << "Pointer " << Loads[I1->first->getName()]->getName() << " and Pointer " << Loads[I2->first->getName()]->getName() << " may alias\n";
            }
            else if (Loads.find(I1->first->getName()) != Loads.end() && Loads.find(I2->first->getName()) == Loads.end()){
                errs() << "Pointer " << Loads[I1->first->getName()]->getName() << " and Pointer " << I2->first->getName() << " may alias\n";
            }
            else if (Loads.find(I1->first->getName()) == Loads.end() && Loads.find(I2->first->getName()) != Loads.end()){
                errs() << "Pointer " << I1->first->getName() << " and Pointer " << Loads[I2->first->getName()]->getName() << " may alias\n";
            }    
            else if (Loads.find(I1->first->getName()) == Loads.end() && Loads.find(I2->first->getName()) == Loads.end()){
                errs() << "Pointer "<< I1->first->getName() << " and Pointer " << I2->first->getName() << " may alias\n";   
            }
            ++MayAliasCount;
            break;
            case AliasResult::PartialAlias:
            //PrintResults(AR, PrintPartialAlias, *I1, *I2, F.getParent());
            errs()<< "Pointer "<< I1->first->getName() << " and Pointer " << I2->first->getName() << " partially alias\n";
            ++PartialAliasCount;
            break;
            case AliasResult::MustAlias:
            //PrintResults(AR, PrintMustAlias, *I1, *I2, F.getParent());
            errs()<< "Pointer "<< I1->first->getName() << " and Pointer " << I2->first->getName() << " must alias\n";
            ++MustAliasCount;
            break;
            }
        }
    }
}
