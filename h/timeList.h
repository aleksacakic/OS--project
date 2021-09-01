#ifndef SEMTIMELIST_
#define SEMTIMELIST_

#include "kerSem.h"
#include "pcb.h"

class SemTimeList{

private:
	struct Elem{

		PCB* pcb;
		int value;
		Elem* next;

		Elem(PCB* p, int val, Elem* n = 0):pcb(p),value(val), next(n){}

	};

	friend KernelSem;
	friend PCB;

	Elem* head,* tail;

public:

	SemTimeList();

	~SemTimeList();

	void clear();

	void add(PCB*, int);

	int getTimeLeftForFirst();

	PCB* removeFirst();

	void remove(PCB*);

	void tick();
};


#endif /* SEMTIMELIST_ */
