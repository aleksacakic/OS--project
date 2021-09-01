#include "thread.h"
#include "timer.h"
#include "util.h"
#include "pcb.h"
#include "SCHEDULE.H"
#include "pcbList.h"
#include <STDIO.H>

Thread::Thread(StackSize stackSize, Time timeSlice){
	lock;

	myPCB = new PCB(this, stackSize, timeSlice);

	unlock;
}


void Thread::start(){
	lock;

	if(myPCB->status == PCB::INITIALIZED){
		myPCB->status = PCB::READY;
		Scheduler::put(myPCB);
	}

	unlock;
}


void Thread::waitToComplete(){
	lock;

	if(this->myPCB->status != PCB::FINISHED && this->myPCB->status != PCB::INITIALIZED && PCB::running != myPCB){

		myPCB->threadsWaitForMe->addLast((PCB*)PCB::running);
		PCB::running->status = PCB::BLOCKED;
		unlock;
		dispatch();
		return;

	}

	unlock;
}

Thread::~Thread(){

	waitToComplete();

	lock;
	delete myPCB;
	unlock;

}

ID Thread::getId(){
	return myPCB->threadID;
}


ID Thread::getRunningId(){
	return PCB::running->threadID;
}

Thread * Thread::getThreadById(ID id){
	lock;

	PCB *pcb =  PCB::allPCBs->findID(id);

	Thread* ret = pcb->myThread;

	unlock;

 	return ret;
}

void dispatch(){

#ifndef BCC_BLOCK_IGNORE
	asm{
		pushf
		cli
	}
#endif

	switchOnDemand = 1;
	timer();

#ifndef BCC_BLOCK_IGNORE
	asm popf
#endif

}

extern ID chID;
extern int flagForkFailed;

ID Thread::fork(){
	int fail = 0;
	ID childID = 0;

#ifndef BCC_BLOCK_IGNORE
	asm{
		pushf
		cli
	}
#endif

	PCB* parent = (PCB*)PCB::running;

	PCB::fork();
	fail = flagForkFailed;
	childID = chID;


#ifndef BCC_BLOCK_IGNORE
	asm popf
#endif

	if((PCB*)PCB::running != parent)
	{//child
		return 0;
	}
	else{//parent

		if(fail == 1){//if fork failed
			return -1;
		}

		return childID;
	}
}


void Thread::exit(){

	lock;

	PCB::running->status = PCB::FINISHED;
	((PCB*)PCB::running)->unblockThreadWaitsForMe();

	unlock;
	dispatch();

}

void Thread::waitForForkChildren(){

	((PCB*)PCB::running)->waitForForkChildren();

}

Thread* Thread::clone() const{
	Thread* ret = 0;
	lock;

	ret = new Thread(myPCB->stackSize,myPCB->timeSlice);

	unlock;
	return ret;
}

