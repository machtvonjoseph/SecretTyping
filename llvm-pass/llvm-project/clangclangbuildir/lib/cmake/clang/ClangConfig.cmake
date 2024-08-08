# This file allows users to call find_package(Clang) and pick up our targets.



set(LLVM_VERSION 18.0.0)
find_package(LLVM ${LLVM_VERSION} EXACT REQUIRED CONFIG
             HINTS "/usr/local/lib/cmake/llvm")

set(CLANG_EXPORTED_TARGETS "clangBasic;clangAPINotes;clangLex;clangParse;clangAST;clangASTMatchers;clangCrossTU;clangSema;clangCodeGen;clangAnalysis;clangEdit;clangExtractAPI;clangRewrite;clangARCMigrate;clangDriver;clangSerialization;clangFrontend;clangFrontendTool;clangTooling;clangDirectoryWatcher;clangIndex;clangIndexSerialization;clangFormat;clangInterpreter;clangSupport;clang-linker-wrapper;clang-offload-bundler;clang-refactor;clang-extdef-mapping")
set(CLANG_CMAKE_DIR "/home/kidus/NUMATyping/llvm-pass/llvm-project/clangclangbuildir/lib/cmake/clang")
set(CLANG_INCLUDE_DIRS "/home/kidus/NUMATyping/llvm-pass/llvm-project/clang/include;/home/kidus/NUMATyping/llvm-pass/llvm-project/clangclangbuildir/include")
set(CLANG_LINK_CLANG_DYLIB "")

# Provide all our library targets to users.
include("/home/kidus/NUMATyping/llvm-pass/llvm-project/clangclangbuildir/lib/cmake/clang/ClangTargets.cmake")

# By creating clang-tablegen-targets here, subprojects that depend on Clang's
# tablegen-generated headers can always depend on this target whether building
# in-tree with Clang or not.
if(NOT TARGET clang-tablegen-targets)
  add_custom_target(clang-tablegen-targets)
endif()
