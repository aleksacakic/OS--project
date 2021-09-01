#include "semList.h"

#include "kerSem.h"

SemList::SemList(){
	head = tail = 0;
}

SemList::~SemList(){

	while(head != 0){

		Elem* erase = head;
		head = head->next;

		delete erase;
	}
	tail = 0;

}


void SemList::clear(){

	while(head != 0){

		Elem* erase = head;
		head = head->next;

		delete erase;
	}
	head = tail = 0;

}

void SemList::addLast(KernelSem* sem){

	Elem* newElem = new Elem(sem, 0);

	if(tail == 0){
		head = newElem;
	}else{
		tail->next = newElem;
	}
	tail = newElem;

}


void SemList::addFirst(KernelSem* sem){

	Elem* newElem = new Elem(sem, head);

	if(tail == 0){
		tail = newElem;
	}
	head = newElem;

}

void SemList::remove(KernelSem * sem){

	Elem* prev = 0,* curr = head;
	while(curr && curr->sem != sem){
		prev = curr;
		curr = curr->next;
	}
	if(curr == 0)return; // there is no such elemenet
	Elem* del = curr;
	if(prev == 0)
		head = curr->next;
	else
		prev->next = curr->next;

	if(curr->next == 0)
		tail = prev;

	delete del;
}

void SemList::revalidate(){

	for(Elem * curr = head; curr != 0; curr = curr->next)
		curr->sem->tick();

}
