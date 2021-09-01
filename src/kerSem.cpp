#include "kerSem.h"
#include "semaphor.h"
#include "pcb.h"
#include "SCHEDULE.H"
#include "timeList.h"
#include "semList.h"
#include "timer.h"
#include "util.h"

SemList* KernelSem::allKernelSems = 0;


KernelSem::KernelSem(int init){

	value = init;

	allPCBsWaitingOnSem = new PCBList();
	allPCBsWithTimeLimit = new SemTimeList();
	allPCBsUnblockedWithSignal = new PCBList();

	allKernelSems->addLast(this);

}

KernelSem::~KernelSem(){

	allKernelSems->remove(this);


	delete allPCBsUnblockedWithSignal;
	delete allPCBsWaitingOnSem;
	delete allPCBsWithTimeLimit;

}

int KernelSem::wait(Time waitingTime){

	lock;

	int ret = 0;

	if(--value < 0){
		allPCBsWaitingOnSem->addLast((PCB*)PCB::running);
		if(waitingTime > 0)
			allPCBsWithTimeLimit->add((PCB*)PCB::running, waitingTime);

		PCB::running->status= PCB::BLOCKED;

		unlock;
		dispatch();

		lock;
		ret = allPCBsUnblockedWithSignal->remove((PCB*)PCB::running);
		//this function returns 1 if is such element in list and remove it from list
		//and returns 0 if there is no such element in list

		unlock;
		return ret;
	}

	unlock;
	return ret;
}

void KernelSem::signal(){

	if(++value <= 0){
		PCB* releasePCB = allPCBsWaitingOnSem->getAndRemoveFirst();
		allPCBsWithTimeLimit->remove(releasePCB);//remove releasePCB from timeLimit list(if it was there)

		releasePCB->status = PCB::READY;
		allPCBsUnblockedWithSignal->addLast(releasePCB);
		Scheduler::put(releasePCB);
	}

}

void KernelSem::tick(){

	allPCBsWithTimeLimit->tick();

	while(allPCBsWithTimeLimit->getTimeLeftForFirst() == 0){
		PCB* releasePCB = allPCBsWithTimeLimit->removeFirst();
		allPCBsWaitingOnSem->remove(releasePCB);

		releasePCB->status = PCB::READY;
		Scheduler::put(releasePCB);
	}

}


int KernelSem::val()const{
	return value;
}
