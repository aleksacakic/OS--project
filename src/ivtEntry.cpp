#include "ivtEntry.h"
#include "util.h"
#include <dos.h>

IVTEntry* IVTEntry::IVT[256] = {0};

IVTEntry::IVTEntry(IVTNo num, pInterrupt newRoutine){
	lock;

	this->number = num;
	this->event = 0;

#ifndef BCC_BLOCK_IGNORE
	this->oldRoutine = getvect(num);
	setvect(num, newRoutine);
#endif

	IVT[num] = this;

	unlock;
}

IVTEntry::~IVTEntry(){
	lock;

#ifndef BCC_BLOCK_IGNORE
	setvect(number, oldRoutine);
#endif

	IVT[number] = 0;

	unlock;
}

void IVTEntry::setEventToEntry(IVTNo num, KernelEv* kerEv){

	if(IVT[num] == 0) return;
	IVT[num]->event = kerEv;

}

void IVTEntry::clearEventFromEntry(IVTNo num){

	if(IVT[num] != 0)
		IVT[num]->event = 0;

}


void IVTEntry::signal(){

	if(event != 0)
		event->signal();

}
