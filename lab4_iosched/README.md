# Lab 4 : IO Scheduling

### About
This is simulation for scheduling and optimization of I/O operations. 
Applications submit their IO requests to the IO subsystem (potentially via the filesystem and page-cache),
where they are maintained in an IO-queue until the dist device is ready for servicing another request.
The IO-scheduler then selects a request from the IO-queue and submits it to the disk device. 
On completion, another request can be taken from the IO-queue and submitted to the disk.
The scheduling policies will allow for some optimization as to reduce disk head movement or overall wait time in the system.


### How to run
1. put all the samples(input and output files) and codes in the same directory.
2. mkdir [your-outdir]
3. make iosched
4. ./runit.sh [your-outdir] ./linker
5. ./gradeit.sh . [your-outdir]

### Description
#### Algorithms
* FIFO(i)
* SSTF(j)
* LOOK(s)
* CLOOK(c)
* FLOOK(f)

#### File description
* IOreq.h : definition of class IOreq
* IOreq.cpp : implementation of class IOreq
* IOscheduler.h : definition of class IOscheduler (FIFO, SSTF, LOOK, CLOOK, FLOOK algorithms)
* IOscheduler.cpp : implementation of class IOscheduler
* iosched.cpp: simulation logic
* Makefile : builds the executable for iosched