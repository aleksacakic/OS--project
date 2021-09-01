#ifndef PCBLIST_H_
#define PCBLIST_H_

#include "pcb.h"


class PCBList{
private:
	struct Elem{

		PCB* pcb;
		Elem* next;

		Elem(PCB* p, Elem* n):pcb(p), next(n){}

	};

	friend PCB;

	Elem* head,* tail;

public:

	PCBList();
	~PCBList();

	void clear();

	void addLast(PCB*);
	void addFirst(PCB*);

	PCB* getAndRemoveFirst();

	int remove(PCB* pcb);

	PCB* findID(ID id);

};

#endif /* PCBLIST_H_ */
