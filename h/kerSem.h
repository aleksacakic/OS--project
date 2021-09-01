#ifndef KERSEM_H_
#define KERSEM_H_

#include "semaphor.h"
#include "pcbList.h"

class SemList;
class SemTimeList;

class KernelSem{
public:

	KernelSem(int value);
	~KernelSem();

	int wait(Time maxTimeToWait);

	void signal();

	int val()const;


	void tick();

	PCBList* allPCBsWaitingOnSem;
	SemTimeList* allPCBsWithTimeLimit;

	PCBList* allPCBsUnblockedWithSignal;

	static SemList* allKernelSems;//all kernel semaphores in system


	int value;

};




#endif /* KERSEM_H_ */
