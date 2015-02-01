// threadtest.cc 
//      Simple test case for the threads assignment.
//
//      Create two threads, and have them context switch
//      back and forth between themselves by calling Thread::Yield, 
//      to illustratethe inner workings of the thread system.
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "system.h"

//----------------------------------------------------------------------
// SimpleThread
//      Loop 5 times, yielding the CPU to another ready thread 
//      each iteration.
//
//      "which" is simply a number identifying the thread, for debugging
//      purposes.
//----------------------------------------------------------------------

void
SimpleThread (int which)
{
    int num;

    for (num = 0; num < 5; num++)
      {
	  printf ("*** thread %d looped %d times\n", which, num);
	  currentThread->Yield ();
      }
}

//author malek : le which corrspond au No du thread
void SimpleThread2 (int which) {
   int i; 
    for(i=0; i<5; i++) { 
      printf ("*** thread %d looped %d times\n", which, i);
      printf ("--\n");
	  currentThread->Yield ();
    }
}
//----------------------------------------------------------------------
// ThreadTest
//      Set up a ping-pong between two threads, by forking a thread 
//      to call SimpleThread, and then calling SimpleThread ourselves.
//----------------------------------------------------------------------

void
ThreadTest ()
{
    DEBUG ('t', "Entering SimpleTest\n");

    Thread *t = new Thread ("forked thread");
    Thread *t2 = new Thread ("forked2 thread");

    t->Fork (SimpleThread, 0); //En argument la fonction et le numero de thread
    t2->Fork (SimpleThread2, 1);
}
