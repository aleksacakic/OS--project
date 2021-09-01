#ifndef IVTENTRY_H_
#define IVTENTRY_H_

#include "event.h"
#include "kernelEv.h"

//##numEntry is changed with parameter of macro numEntry
//so every time when we call this macro we get new set of functions
//example: void interrupt interruptRoutineForEntry1(...) if numEntry=1

#define PREPAREENTRY(numEntry, callOld)\
void interrupt interruptRoutineForEntry##numEntry(...); \
\
IVTEntry newIVTEntry##numEntry(numEntry, interruptRoutineForEntry##numEntry); \
\
void interrupt interruptRoutineForEntry##numEntry(...) {\
	IVTEntry::IVT[numEntry]->signal();\
	if (callOld == 1)\
		(IVTEntry::IVT[numEntry]->oldRoutine)();\
	dispatch();\
}


typedef void interrupt (*pInterrupt)(...);

class IVTEntry{
public:
	IVTEntry(IVTNo, pInterrupt);
	~IVTEntry();

	void signal();

	static void setEventToEntry(IVTNo, KernelEv*);
	static void clearEventFromEntry(IVTNo);

	static IVTEntry* IVT[256];
	pInterrupt oldRoutine;

private:
	IVTNo number;
	KernelEv* event;

};


#endif /* IVTENTRY_H_ */
