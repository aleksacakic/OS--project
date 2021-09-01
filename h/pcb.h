#ifndef PCB_H_
#define PCB_H_

#include "thread.h"

class PCBList;

typedef void (*Func)();

class PCB {
public:

	PCB(Thread* thr, StackSize ss, Time ts);

	~PCB();

	unsigned ss;//stack segment
	unsigned sp;//stack offset
	unsigned bp;//base pointer

	unsigned* stack;//pointer to dynamic allocated stack
	int stackSize;

	Time timeSlice;
	Thread* myThread;

	ID threadID;//uniqe identification number that every thread has


	PCB* parentPCB;
	PCBList* children;

	static void fork();

	void waitForForkChildren();


	enum Status{INITIALIZED, READY, BLOCKED, FINISHED};
	Status status;


	PCBList* threadsWaitForMe;


	static volatile PCB* running;
	static PCB* idle;
	static PCBList* allPCBs;


	static PCB* makeIdlePCB();
	static void idleRun();
	static void wrapper();

	void unblockThreadWaitsForMe();

private:

	static ID staticID;

};

#endif /* PCB_H_ */
