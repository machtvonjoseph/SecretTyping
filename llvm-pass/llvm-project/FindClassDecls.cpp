#include "clang/AST/ASTConsumer.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/FrontendAction.h"
#include "clang/Tooling/Tooling.h"

using namespace clang;


//By using the RecursiveASTVisitor we can extract relevant information from the AST.
//It provides hooks of the form `bool VisitNODETYPE(NODETYPE *)` for most AST nodes except 
//TypeLoc nodes which are passed by value. Here NODETYPE = CXXRecordDecl
class myFindClassNameVisitor : public RecursiveASTVisitor<myFindClassNameVisitor> {
	public:
		explicit myFindClassNameVisitor(ASTContext *Context) : Context(Context) {}

		bool VisitCXXRecordDecl(CXXRecordDecl *Declaration){
			//For debugging will dump the AST nodes that have already been visited
			Declaration ->dump();
			if(Declaration->getQualifiedNameAsString() == "n::m::C"){
				//getFullLoc uses the ASTContext's SourceManager to resolve the source location and
				//break it up into its line and column parts.
				FullSourceLoc FullLocation = Context->getFullLoc(Declaration->getBeginLoc());
				if(FullLocation.isValid()){
					llvm::outs() << "Found declaration at "
						<< FullLocation.getSpellingLineNumber() << ":"
						<< FullLocation.getSpellingColumnNumber() << "\n"<<Declaration->hasDefinition()<<"\n";
				}
			}
			//The return value indicates whether we want the visitation to proceed 
			//Return false to stop the traversal of the AST.
			return true;
			}
	private:
		ASTContext *Context;
};



//An ASTConsumer is an inteface used to write generic actions on an AST, regardless of how
// the AST is produced It provides multiple entrypoints into the AST but here we use the HandleTranslationUnit, which is called with
//the ASTContext for the translation unit.
class myFindClassNameConsumer : public clang::ASTConsumer {
	public:
		explicit myFindClassNameConsumer(ASTContext *Context) : Visitor(Context) {}
		virtual void HandleTranslationUnit(clang::ASTContext &Context) {
			//Traversing the translation unit decl via a RecorsiveASTVisitor will visit all nodes
			//in the AST
			Visitor.TraverseDecl(Context.getTranslationUnitDecl());
		}
	private:
		//A RecursiveASTVisistor implementation.
		myFindClassNameVisitor Visitor;
};


//First thing's first, write a frontend action. It is the common entry point in any standalone tool based on
//LibTooling

class myFindClassNameAction : public clang::ASTFrontendAction {
	public:
		virtual std::unique_ptr<clang::ASTConsumer> CreateASTConsumer(
				clang::CompilerInstance &Compiler, llvm::StringRef InFile){
			return std::make_unique<myFindClassNameConsumer>(&Compiler.getASTContext());
		}
};



int main(int argc, char **argv) {
	if (argc > 1){
		clang::tooling::runToolOnCode(std::make_unique<myFindClassNameAction>(), argv[1]);
	}
}








