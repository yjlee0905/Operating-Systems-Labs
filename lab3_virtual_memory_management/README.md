# Lab 3 : Virtual Memory Management

### About



### How to run
1. put all the samples(input and out files - out files are in refout directory) and codes in the same directory
2. mkdir [your-outdir]
3. make mmu
4. ./runit.sh . [your-outdir] ./mmu
5. ./gradeit.sh ./refout [your-outdir]

### Description
#### Algorithms
* FIFO
* Random
* Clock
* NRU
* Aging
* WorkingSet

#### File description
* Header.h : definition of struct and constants
* Pager.h : definition of class Pager (FIFO, Random, Clock, NRU, Aging, WorkingSet)
* Pager.cpp : implementation of class Pager
* Process.h : definition of class Process
* Process.cpp : implementation of class Process
* mmu.cpp : simulation logic
* Makefile : builds the executable for mmu