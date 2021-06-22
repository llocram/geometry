g++ -std=c++20 -c bezier.cpp -o bezier_gcc.o -O2
clang++ -std=c++20 -c bezier.cpp -o bezier_clang.o -O2

objdump -d bezier_gcc.o > bezier_gcc_objdump
objdump -d bezier_clang.o > bezier_clang_objdump
