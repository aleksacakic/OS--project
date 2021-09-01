#include "pcbList.h"
#include "pcb.h"

PCBList::PCBList(){
	head = tail = 0;
}

PCBList::~PCBList(){

	while(head != 0){

		Elem* erase = head;
		head = head->next;

		delete erase;
	}
	tail = 0;

}


void PCBList::clear(){

	while(head != 0){

		Elem* erase = head;
		head = head->next;

		delete erase;
	}
	head = tail = 0;

}

void PCBList::addLast(PCB* pcb){

	Elem* newElem = new Elem(pcb, 0);

	if(tail == 0){
		head = newElem;
	}else{
		tail->next = newElem;
	}
	tail = newElem;

}


void PCBList::addFirst(PCB* pcb){

	Elem* newElem = new Elem(pcb, head);

	if(tail == 0){
		tail = newElem;
	}
	head = newElem;

}

int PCBList::remove(PCB * pcb){

	Elem* prev = 0,* curr = head;
	while(curr && curr->pcb != pcb){
		prev = curr;
		curr = curr->next;
	}
	if(curr == 0)return 0; // there is no such elemenet
	Elem * del = curr;
	if(prev == 0)
		head = curr->next;
	else
		prev->next = curr->next;

	if(curr->next == 0)
		tail = prev;

	delete del;

	return 1;
}

PCB* PCBList::getAndRemoveFirst(){

	if(head == 0)return 0;

	PCB* retPCB = head->pcb;

	Elem * erase = head;

	head = head->next;
	if(head == 0)
		tail = 0;

	delete erase;

	return retPCB;

}


PCB* PCBList::findID(ID id){

	Elem* curr = head;
	for(; curr != 0; curr = curr->next)
		if(curr->pcb->threadID == id)
			return curr->pcb;
	return 0;

}
