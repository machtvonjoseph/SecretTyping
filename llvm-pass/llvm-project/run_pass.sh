ninja -C build/ opt

clang -S -emit-llvm -O0 -Xclang -disable-llvm-passes -Xclang -disable-O0-optnone  -fno-discard-value-names ./input/$1.cpp -o ./output/temp.ll

build/bin/opt -S ./output/temp.ll  -passes=instnamer > ./output/$1.ll
#build/bin/opt -S ./output/temp.ll  -passes=mem2reg > ./output/$1.ll


build/bin/opt -disable-output -passes=func-pointer-analysis ./output/$1.ll
#build/bin/opt -disable-output -passes=print-alias-sets ./output/$1.ll