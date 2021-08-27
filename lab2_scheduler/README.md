# Lab 2 : Scheduler

### About
This is simulation for scheduling policies of processes/threads executing on a system 
and the system is implemented using Discrete Event Simulation(DES).

### How to run
1. put all the samples(input and output files - output files are in refout directory) and codes in the same directory.
2. mkdir [your-outdir]
3. make sched
4. ./runit.sh [your-outdir] ./sched
5. ./gradeit.sh ./refout [your-outdir]

### Description
#### Algorithms
* FCFS(F)
* LCFS(L)
* SRTF(S)
* RR(R)
* PRIO(P)
* PREPRIO(E)

#### File description
* Constants.h
* Event.h : definition fo class Event
* Event.cpp : implementation of class Event
* Process.h : definition of class Process
* Process.cpp : implementation of class Process
* Scheduler.h : definition of class Scheduler (FCFS, LCFS, SRTF, RR, PRIO, Preemptive PRIO)
* Scheduler.cpp : implementation of class Scheduler
* sched.cpp : simulation logic
* Makefile : builds the executable for sched