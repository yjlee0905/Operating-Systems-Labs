# Lab 1 : Linker

### About
This is two-pass linker.
Pass One parses the input and verifies the correct syntax and determines the base address for each module and the absolute address for each defined symbol, storing the latter in a symbol table.
Pass Two again parses the input and uses the base addresses and the symbol table entries created in pass one to generate the actual output by relocating relative addresses and resolving external references.


### How to run
1. put all the samples(input and output files) and codes in the same directory.
2. mkdir [your-outdir]
3. make linker
4. ./runit.sh [your-outdir] ./linker
5. ./gradeit.sh . [your-outdir]

### Description
* Header.h : definition of struct and errors
* Readers.h : functions for checking valid tokens
* linker.cpp : main algorithms for pass1 and pass2
* Makefile : builds the executable for linker