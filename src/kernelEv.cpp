#include "kernelEv.h"
#include "thread.h"
#include "SCHEDULE.H"
#include "ivtEntry.h"
#include "util.h"

KernelEv::KernelEv(IVTNo ivtNo){

	this->creator = (PCB*)PCB::running;

	this->value = 0;

	this->ivtNo = ivtNo;

	IVTEntry::setEventToEntry(ivtNo, this);

}

KernelEv::~KernelEv(){

	IVTEntry::clearEventFromEntry(ivtNo);

}


void KernelEv::wait(){

	lock;

	if(PCB::running !=creator){
		unlock;
		return;//only owner can wait on event
	}

	if(value == 0){//thread should be blocked

		value = -1;//creator is blocked
		creator->status = PCB::BLOCKED;

		unlock;
		dispatch();
		return;

	}else{
		//value was 1
		value = 0;

	}
	unlock;
}


void KernelEv::signal(){

	value++;

	if(value >=1){//creator was not blocked

		value = 1;

	}
	else{//value was -1 and creator was blocked and should be released

		creator->status = PCB::READY;
		Scheduler::put(creator);

	}

}
