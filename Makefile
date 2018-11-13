.PHONY: default

default: 
	@echo 'Use either "make  asm.a" (assembler) or "make ssem.a" (SSEM) to build the target'

asm.a:
	g++ -std=c++14 -o asm ./Assembler/main.cpp ./Assembler/assembler.cpp -Wall -Wextra -pedantic

ssem.a:
	g++ -std=c++14 -o manchesterBaby  ./SSEM/main.cpp ./SSEM/ssem.cpp ./SSEM/cpu.cpp ./SSEM/store.cpp -Wall -Wextra -pedantic
	
