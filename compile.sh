gcc -c bigInt.c -o bigInt.o
gcc -c bigInt_linux_aarch64.S -o bigInt_linux_aarch64.o
gcc bigInt.o bigInt_linux_aarch64.o -o bigInt
rm bigInt.o bigInt_linux_aarch64.o
