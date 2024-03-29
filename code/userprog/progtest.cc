// progtest.cc 
//      Test routines for demonstrating that Nachos can load
//      a user program and execute it.  
//
//      Also, routines for testing the Console hardware device.
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "system.h"
#include "console.h"
#include "addrspace.h"
#include "synch.h"

#include "synchconsole.h"

//----------------------------------------------------------------------
// StartProcess
//      Run a user program.  Open the executable, load it into
//      memory, and jump to it.
//----------------------------------------------------------------------

void
StartProcess (char *filename)
{
    OpenFile *executable = fileSystem->Open (filename);
    AddrSpace *space;

    if (executable == NULL)
      {
	  printf ("Unable to open file %s\n", filename);
	  return;
      }
    space = new AddrSpace (executable);
    currentThread->space = space;

    delete executable;		// close file

    space->InitRegisters ();	// set the initial register values
    space->RestoreState ();	// load page table register

    machine->Run ();		// jump to the user progam
    ASSERT (FALSE);		// machine->Run never returns;
    // the address space exits
    // by doing the syscall "exit"
}

// Data structures needed for the console test.  Threads making
// I/O requests wait on a Semaphore to delay until the I/O completes.

static Console *console;

static Semaphore *readAvail;
static Semaphore *writeDone;

//----------------------------------------------------------------------
// ConsoleInterruptHandlers
//      Wake up the thread that requested the I/O.
//----------------------------------------------------------------------

static void
ReadAvail (int arg) //La fonction est appelée lorsqu'un caractere est tapé sur clavier (par Console.c)
{
    readAvail->V ();
}
static void
WriteDone (int arg) //La fonction est appelée lorsqu'un caractere est bien affiché a dans la console (par Console.c)
{
    writeDone->V ();
}

//----------------------------------------------------------------------
// ConsoleTest
//      Test the console by echoing characters typed at the input onto
//      the output.  Stop when the user types a 'q'.
//----------------------------------------------------------------------

void
ConsoleTest (char *in, char *out)
{
    char ch;

    console = new Console (in, out, ReadAvail, WriteDone, 0);
    readAvail = new Semaphore ("read avail", 0); //Handler d'interruption lors d'une lecture availble
    writeDone = new Semaphore ("write done", 0); //Handler d'interruption d'une ecriture terminée

    for (;;)
      {
	  readAvail->P ();	// wait for character to arrive
	  ch = console->GetChar ();
	  if (ch != '\n') {
		  console->PutChar ('<');
		  writeDone->P ();	// wait for write to finish
	  }
	  console->PutChar (ch);	// echo it!
	  writeDone->P ();	// wait for write to finish
	  if (ch != '\n') {
	  		  console->PutChar ('>');
	  		  writeDone->P ();	// wait for write to finish
	  	  }
	  if (ch == EOF)
	      return;		// if q, quit
      }
}

#ifdef CHANGED

void
SynchConsoleTest (char *in, char *out)
{
	static SynchConsole *synchconsoleProgTest;
    char ch;

    synchconsoleProgTest = new SynchConsole (in, out);

    while((ch = synchconsole->SynchGetChar()) != EOF){
    	synchconsoleProgTest->SynchPutChar(ch);
    }
    fprintf(stderr, "Solaris:EOF detected in SynchConsole!\n);");

//    for (;;)
//      {
//	  ch = synchconsole->SynchGetChar ();
//	  if (ch != '\n') {
//		  synchconsole->SynchPutChar ('<');
//	  }
//	  synchconsole->SynchPutChar (ch);	// echo it!
//	  if (ch != '\n') {
//		  	  synchconsole->SynchPutChar ('>');
//	  	  }
//	  if (ch == EOF)
//	      return;		// if q, quit
//      }
}

#endif //CHANGED
