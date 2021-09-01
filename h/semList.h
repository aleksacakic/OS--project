#ifndef SEMLIST_H_
#define SEMLIST_H_

#include "semaphor.h"

class SemList{
private:
	struct Elem{

		KernelSem* sem;
		Elem* next;

		Elem(KernelSem* s, Elem* n):sem(s), next(n){}

	};

	friend KernelSem;

	Elem* head,* tail;

public:

	SemList();
	~SemList();

	void clear();

	void addLast(KernelSem*);
	void addFirst(KernelSem*);

	void remove(KernelSem* pcb);

	void revalidate();

};

#endif /* SEMLIST_H_ */
