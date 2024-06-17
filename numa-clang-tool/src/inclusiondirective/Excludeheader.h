#ifndef EXCLUDEHEADER_H
#define EXCLUDEHEADER_H

#include "clang/Frontend/FrontendPluginRegistry.h"
#include "clang/Lex/Preprocessor.h"
#include "clang/Tooling/Tooling.h"
#include "clang/AST/ASTConsumer.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/FrontendAction.h"
#include "clang/Tooling/CommonOptionsParser.h"

using namespace clang;

class ExcludeHeaderCallback : public PPCallbacks {
public:
    explicit ExcludeHeaderCallback(Preprocessor &PP) : PP(PP) {}

    void InclusionDirective(SourceLocation HashLoc,
                                  const Token &IncludeTok, StringRef FileName,
                                  bool IsAngled, CharSourceRange FilenameRange,
                                  OptionalFileEntryRef File,
                                  StringRef SearchPath, StringRef RelativePath,
                                  const Module *Imported,
                                  SrcMgr::CharacteristicKind FileType) override;
private:
    Preprocessor &PP;
};



#endif