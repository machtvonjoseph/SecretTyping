cmake_minimum_required(VERSION 2.8)

find_package(LLVM REQUIRED)
find_package(Clang REQUIRED)
find_package(jsoncpp REQUIRED)

#NUMA_LLVM_COMPILE_FLAGS is just ${LLVM_COMPILE_FLAGS} without the -fno-exceptions flag
set(NUMA_LLVM_COMPILE_FLAGS ""-I/usr/local/include -std=c++20 -fexperimental-library -I/usr/local/lib/clang/20/include/-fno-exceptions -funwind-tables -fno-rtti -D_GNU_SOURCE -D__STDC_CONSTANT_MACROS -D__STDC_FORMAT_MACROS -D__STDC_LIMIT_MACROS -march=native -pthread"")
#
set(CMAKE_CXX_FLAGS "-Wall -lnuma -g3 -O0 -fno-rtti ${NUMA__LLVM_COMPILE_FLAGS}")

include_directories(${LLVM_INCLUDE_DIRS})
include_directories(${CLANG_INCLUDE_DIRS})
message(LLVM_COMPILE_FLAGS="${LLVM_COMPILE_FLAGS}")

set(CLANG_TOOL_SRC

        main.cc
        
        actions/frontendaction.cc
        actions/cast_frontendaction.cc
        consumer/consumer.cc
        consumer/cast_consumer.cc
        #inclusiondirective/inclusiondirective.cc
        utils/utils.cc
        
        transformer/transformer.cc
        # transformer/functioncalltransformer.cc
        transformer/RecursiveNumaTyper.cc
        transformer/NumaTargetNumaPointer.cc
        # finder/finder.cc
        # finder/integervariablefinder.cc
)

# remove trailing whitespaces
string(STRIP ${LLVM_LDFLAGS} LLVM_LDFLAGS)

add_executable(clang-tool ${CLANG_TOOL_SRC})

target_link_libraries(clang-tool ${CLANG_LIBS} ${LLVM_LIBS_CORE} ${LLVM_LDFLAGS} jsoncpp_lib)

