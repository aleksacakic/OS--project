#include "kerSem.h"
#include "pcbList.h"
#include "pcb.h"
#include "timeList.h"

SemTimeList::SemTimeList(){
	head = tail = 0;
}


SemTimeList::~SemTimeList(){

	while(head != 0){

		Elem* erase = head;
		head = head->next;

		delete erase;
	}
	tail = 0;

}


void SemTimeList::clear(){

	while(head != 0){

		Elem* erase = head;
		head = head->next;

		delete erase;
	}
	head = tail = 0;

}

void SemTimeList::add(PCB* pcb, int val){

	Elem* prev = 0;
	Elem* curr = head;
	int count = 0;

	while(curr != 0 && count + curr->value <= val){
		prev = curr;
		curr = curr->next;
		count += prev->value;
	}

	Elem* insert = new Elem(pcb, val - count);

	if(prev == 0){
		insert->next = head;
		head = insert;

	}else{
		prev->next = insert;
		insert->next = curr;
	}

	if(curr != 0){
		curr->value -= insert->value;
	}

	if(insert->next == 0)
		tail = insert;
}

int SemTimeList::getTimeLeftForFirst(){

	if(head == 0) return -1;//return -1 as sign that list is empty

	return head->value;

}

PCB* SemTimeList::removeFirst(){

	if(head == 0) return 0;

	PCB* retPCB = head->pcb;

	Elem * erase = head;

	head = head->next;
	if(head == 0)
		tail = 0;

	delete erase;

	return retPCB;

}


void SemTimeList::remove(PCB * pcb){

	Elem* prev = 0,* curr = head;
	while(curr && curr->pcb != pcb){
		prev = curr;
		curr = curr->next;
	}
	if(curr == 0)return; // there is no such elemenet

	Elem * del = curr;
	if(prev == 0)
		head = curr->next;
	else
		prev->next = curr->next;

	if(curr->next == 0)
		tail = prev;

	if(curr->next != 0){
		curr->next->value += curr->value;
	}

	delete del;

}

void SemTimeList::tick(){

	if(head == 0) return;

	if(head->value > 0)
		head->value--;

}
