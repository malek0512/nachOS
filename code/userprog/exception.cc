// exception.cc 
//      Entry point into the Nachos kernel from user programs.
//      There are two kinds of things that can cause control to
//      transfer back to here from user code:
//
//      syscall -- The user code explicitly requests to call a procedure
//      in the Nachos kernel.  Right now, the only function we support is
//      "Halt".
//
//      exceptions -- The user code does something that the CPU can't handle.
//      For instance, accessing memory that doesn't exist, arithmetic errors,
//      etc.  
//
//      Interrupts (which can also cause control to transfer from user
//      code into the Nachos kernel) are handled elsewhere.
//
// For now, this only handles the Halt() system call.
// Everything else core dumps.
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "system.h"
#include "syscall.h"

//----------------------------------------------------------------------
// UpdatePC : Increments the Program Counter register in order to resume
// the user program immediately after the "syscall" instruction.
//----------------------------------------------------------------------
static void
UpdatePC ()
{
    int pc = machine->ReadRegister (PCReg);
    machine->WriteRegister (PrevPCReg, pc);
    pc = machine->ReadRegister (NextPCReg);
    machine->WriteRegister (PCReg, pc);
    pc += 4;
    machine->WriteRegister (NextPCReg, pc);
}

//char buffer[MAX_STRING_SIZE];
//void copyStringFromMachine(int from, char *to, unsigned size) {
////		(char*) (machine->ReadRegister (4))
//		int _from = from;
//		unsigned _size = size - 1;
//		//char* to = calloc(size); //allocation du buffer effetué a l'appel
//		int* dataByte;
//		int cpt = 0;
//		while (cpt<_size && machine->ReadMem(_from, 1, dataByte)) { //Le && en C est equivalent a un ET PUIS donc quand cpt==_size on ne fait l'appel de lecture
//			to[cpt] = (char) (*dataBte);
//			cpt++;
//		}
//		//On tronc si le string est plus grand
//		if (cpt == _size) {
//			cpt++;
//			to[cpt] = '\0';
//		}
//}

//----------------------------------------------------------------------
// ExceptionHandler
//      Entry point into the Nachos kernel.  Called when a user program
//      is executing, and either does a syscall, or generates an addressing
//      or arithmetic exception.
//
//      For system calls, the following is the calling convention:
//
//      system call code -- r2
//              arg1 -- r4
//              arg2 -- r5
//              arg3 -- r6
//              arg4 -- r7
//
//      The result of the system call, if any, must be put back into r2. 
//
// And don't forget to increment the pc before returning. (Or else you'll
// loop making the same system call forever!
//
//      "which" is the kind of exception.  The list of possible exceptions 
//      are in machine.h.
//----------------------------------------------------------------------

void
ExceptionHandler (ExceptionType which)
{
	//Lecture de l'exception (appel systeme ecrit dans r2 dans Start.S)
    int type = machine->ReadRegister (2);


#ifndef CHANGED //if*not*def
    if ((which == SyscallException) && (type == SC_Halt))
      {
	  DEBUG ('a', "Shutdown, initiated by user program.\n");
	  interrupt->Halt ();
	      UpdatePC ();
      }
    else
      {
	  printf ("Unexpected user mode exception %d %d\n", which, type);
	  ASSERT (FALSE);
      }


#else //CHANGED
    if (which == SyscallException )
    {
    	switch (type) {
    	case SC_Halt :
    		DEBUG ('a', "Shutdown, initiated by user program.\n");
    		interrupt->Halt ();
    		break;

    	case SC_PutChar :
    		DEBUG ('a', "PutCHar, initiated by user program.\n");
//    		interrupt->Halt ();
//    		char ccc =(char) (machine->ReadRegister (4));
    		synchconsole->SynchPutChar((char) (machine->ReadRegister (4))); //L'objet syncosole est déclaré dans main.cc donc initialiser au demarrage du systeme
    		break;

    	case SC_PutString :
			DEBUG ('a', "PutSTring, initiated by user program.\n");
			synchconsole->SynchPutString((char*) (machine->ReadRegister (4))); //L'objet syncosole est déclaré dans main.cc donc initialiser au demarrage du systeme
			break;

    	default:
    		printf ("Unexpected user mode exception %d %d\n", which, type);
    		ASSERT (FALSE);
    		break;
    	}

    	// LB: Do not forget to increment the pc before returning!
    	    UpdatePC ();
    // End of addition
    }

#endif
//    // LB: Do not forget to increment the pc before returning!
//    UpdatePC ();
//    // End of addition
}
