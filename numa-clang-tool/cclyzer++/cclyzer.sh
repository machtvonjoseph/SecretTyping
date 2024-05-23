rm -rf output/*
rm -rf fact_gen/*
clang-14 -emit-llvm -O0 -c -fno-discard-value-names -fcomplete-member-pointers $1.cpp -o $1.bc
clang-14 -emit-llvm -O0 -S -fno-discard-value-names -fcomplete-member-pointers $1.cpp -o $1.ll
factgen-exe --context-sensitivity 3-callsite --out-dir ./fact_gen/ $1.bc
souffle --fact-dir ./fact_gen/ --output-dir ./output/  ../../cclyzerpp-0.7.0/datalog/subset.project
gunzip output/*