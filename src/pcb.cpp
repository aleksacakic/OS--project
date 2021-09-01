#include "pcb.h"
#include "pcbList.h"
#include <dos.h>
#include "util.h"
#include "thread.h"
#include "SCHEDULE.H"
#include "timer.h"
#include<stdio.h>

#include<string.h>

ID PCB::staticID = 0;
volatile PCB* PCB::running = 0;
PCB* PCB::idle = 0;
PCBList* PCB::allPCBs = 0;


PCB::PCB(Thread* myThr, StackSize sS, Time tS){


	this->myThread = myThr;
	//this->timeSlice = (tS<0)? 0 : tS;//correcting timeSlice if it is less than 0
	this->timeSlice = tS;

	this->threadID = staticID++;

	this->threadsWaitForMe = new PCBList();



	//making and preparing stack
	this->stackSize = sS / sizeof(unsigned);
	this->stack = new unsigned[stackSize];

	stack[stackSize - 1] = 0x200; // PSW(program status word) = 0x200; interrupt flag  I = 1

#ifndef BCC_BLOCK_IGNORE

	stack[stackSize-2] = FP_SEG(wrapper); //CS - code segment
	stack[stackSize-3] = FP_OFF(wrapper); //IP - instruction pointer(offset)

	// ax, bx, cx, dx, es, ds, si, di, bp - 9 entries in stack

	stack[stackSize-12] = 0;//limiter in fork(bp=0)

	this->ss = FP_SEG(stack + stackSize - 12);
	this->sp= FP_OFF(stack + stackSize - 12);

#endif

	this->bp = this->sp;

	this->status = INITIALIZED;

	this->parentPCB = 0;
	this->children = new PCBList();

	PCB::allPCBs->addLast(this);// adding pcb to the list of all PCBs that is currently in use
}

PCB::~PCB(){
	lock;

	PCB::allPCBs->remove(this);//removing from list of all PCBs

	delete threadsWaitForMe;
	delete children;

	delete [] stack;

	unlock;
}

void PCB::unblockThreadWaitsForMe(){

	for(PCBList::Elem* curr = threadsWaitForMe->head; curr!= 0; curr = curr->next){
		curr->pcb->status = READY;
		Scheduler::put(curr->pcb);
	}
	threadsWaitForMe->clear();

}

void PCB::wrapper(){

	PCB::running->myThread->run();

	lock;
	PCB::running->status = PCB::FINISHED;
	((PCB*)PCB::running)->unblockThreadWaitsForMe();
	unlock;

	dispatch();

}

void PCB::idleRun(){
	while(1){}
}

PCB* PCB::makeIdlePCB(){

	PCB* ret =  new PCB(0, 512, 1);

#ifndef BCC_BLOCK_IGNORE
	ret->stack[ret->stackSize-2] = FP_SEG(idleRun); //CS - code segment
	ret->stack[ret->stackSize-3] = FP_OFF(idleRun); //IP - instruction pointer(offset)
#endif

	return ret;
}


static PCB* childPCB = 0;
static unsigned tbp;
static unsigned pom;
static unsigned childSeg;
unsigned flagForkFailed = 0;
ID chID = 0;
static Thread* thr;

void interrupt stack_copy(){

	memcpy(((PCB*)childPCB)->stack,((PCB*)PCB::running)->stack, sizeof(unsigned) * ((PCB*)PCB::running)->stackSize);

#ifndef BCC_BLOCK_IGNORE
	asm{
		mov tbp, bp
	}

	pom = tbp - FP_OFF(((PCB*)PCB::running)->stack) + FP_OFF(((PCB*)childPCB)->stack);
#endif


	((PCB*)childPCB)->bp = pom;
	((PCB*)childPCB)->sp = pom;

	childSeg = ((PCB*)childPCB)->ss;


#ifndef BCC_BLOCK_IGNORE
	asm{
		mov es, childSeg
		mov bx, bp

		mov di, pom
	}
	loop:asm{
		mov ax, word ptr ss:bx //value of old bp
		cmp ax, 0
		je end

		sub ax, bx // bp_old-bp_new

		mov dx, di
		add dx, ax

		mov word ptr es:di, dx //correct bp value

		add bx, ax
		mov di, dx

		jmp loop
	}
	end:asm{}

#endif

	((PCB*)childPCB)->status = PCB::READY;
	Scheduler::put((PCB*)childPCB);

}




void PCB::fork(){

	flagForkFailed = 0;
	thr = ((PCB*)PCB::running)->myThread->clone();

	if(thr == 0)
		flagForkFailed = 1;
	else if(thr->myPCB == 0)
		flagForkFailed = 1;
	else if(thr->myPCB->stack == 0)
		flagForkFailed = 1;

	if(flagForkFailed == 1)
		return;

	chID = childPCB->threadID;


	childPCB = thr->myPCB;

	((PCB*)childPCB)->parentPCB = (PCB*)PCB::running;

	((PCB*)PCB::running)->children->addLast((PCB*)childPCB);


	stack_copy();

}



void PCB::waitForForkChildren(){


	for(PCBList::Elem* curr = children->head; curr != 0; curr = curr->next){

		//curr->pcb->waitForForkChildren();

		if(curr->pcb->status != FINISHED){
			lock;
			curr->pcb->threadsWaitForMe->addLast((PCB*)PCB::running);
			PCB::running->status = PCB::BLOCKED;

			unlock;
			dispatch();

		}

	}

}
