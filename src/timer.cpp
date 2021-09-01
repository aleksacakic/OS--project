#include <dos.h>
#include "timer.h"
#include "util.h"
#include "SCHEDULE.H"
#include "pcb.h"
#include "pcbList.h"
#include "kerSem.h"
#include "semList.h"


pInterrupt oldTimerInterruptRoutine;
int timeCounter;


volatile int lockFlag = 0;
volatile int switchOnDemand;
int tss, tsp , tbp;

extern void tick();

void initializeTimer(){
	lock

#ifndef BCC_BLOCK_IGNORE
	oldTimerInterruptRoutine = getvect(8);
	setvect(8, timer);
	setvect(0x60, oldTimerInterruptRoutine);
#endif

	PCB::allPCBs = new PCBList();

	PCB::idle = PCB::makeIdlePCB();

	KernelSem::allKernelSems = new SemList();

	unlock
}

void restoreOldTimer(){
	lock;

	delete KernelSem::allKernelSems;
	delete PCB::idle;
	delete PCB::allPCBs;

#ifndef BCC_BLOCK_IGNORE
	setvect(8, oldTimerInterruptRoutine);
#endif

	unlock;
}


void interrupt timer(...){

	if(!switchOnDemand){

			//(*oldTimerInterruptRoutine)();
			oldTimerInterruptRoutine();

			KernelSem::allKernelSems->revalidate();
			tick();//function tick from public test
	}


	if (!switchOnDemand) timeCounter--;

	if (timeCounter < 0)
		timeCounter = -1;

	if (timeCounter == 0 || switchOnDemand) {

		if(lockFlag == 0){

			switchOnDemand = 0;

#ifndef BCC_BLOCK_IGNORE
			asm {
				mov tsp, sp
				mov tss, ss
				mov tbp, bp
			}
#endif
			//pamcenje starih vrednosti
			PCB::running->sp = tsp;
			PCB::running->ss = tss;
			PCB::running->bp = tbp;

			//choosing new thread to execute
			if(PCB::running->status == PCB::READY && PCB::running != PCB::idle)
				Scheduler::put((PCB*)PCB::running);

			PCB::running = Scheduler::get();
			if(PCB::running == 0){
				//put idle thread as running
				PCB::running = PCB::idle;
			}


			//setting new values
			tsp = PCB::running->sp;
			tss = PCB::running->ss;
			tbp = PCB::running->bp;

			timeCounter = PCB::running->timeSlice;

#ifndef BCC_BLOCK_IGNORE
			asm {
				mov sp, tsp
				mov ss, tss
				mov bp, tbp
			}
#endif
		}
		else
				switchOnDemand = 1;

	}


}

