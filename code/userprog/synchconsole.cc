/*
 * synchconsole.cc
 *
 *  Created on: 1 févr. 2015
 *      Author: malek
 */
#ifdef CHANGED

#include "copyright.h"
#include "system.h"
#include "synchconsole.h"
#include "synch.h"

static Semaphore *readAvail;
static Semaphore *writeDone;

static void ReadAvail(int arg) { readAvail->V(); }
static void WriteDone(int arg) { writeDone->V(); }

SynchConsole::SynchConsole(char *readFile, char *writeFile)
{
	readAvail = new Semaphore("read avail", 0);
	writeDone = new Semaphore("write done", 0);
	console = new Console (readFile, writeFile, ReadAvail, WriteDone, 0);
}

SynchConsole::~SynchConsole()
{
	delete console;
	delete writeDone;
	delete readAvail;
}
void SynchConsole::SynchPutChar(const char ch)
{
	console->PutChar(ch);
	writeDone->P (); // wait for write to finish
}

char SynchConsole::SynchGetChar()
{
	readAvail->P ();
	return console->GetChar();
}

void SynchConsole::SynchPutString(const char s[])
{
	int length = strlen(s);
	for(int i=0; i<length; i++) {
		SynchPutChar(s[i]);
	}
}

void SynchConsole::SynchGetString(char *s, int n)
{
	for(int i=0; i<n; i++) {
		s[i] = SynchGetChar ();
	}
}
#endif // CHANGED

