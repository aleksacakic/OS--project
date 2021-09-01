#ifndef KERNELEV_H_
#define KERNELEV_H_

#include "event.h"
#include "pcb.h"

class KernelEv{
public:
	KernelEv(IVTNo ivtNo);
	~KernelEv();

	void wait();
	void signal();

private:
	PCB* creator;

	int value;// value can be -1, 0 or 1

	IVTNo ivtNo;

};


#endif /* KERNELEV_H_ */
