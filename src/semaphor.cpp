#include "semaphor.h"

#include "kerSem.h"
#include "util.h"


Semaphore::Semaphore(int init){
	lock;

	myImpl = new KernelSem(init);

	unlock;
}

Semaphore::~Semaphore(){
	lock;

	delete myImpl;

	unlock;
}

int Semaphore::wait (Time maxTimeToWait){

	int ret = myImpl -> wait(maxTimeToWait);

	return ret;
}

void Semaphore::signal(){
	lock;

	myImpl->signal();

	unlock;
}

int Semaphore::val () const{

	return myImpl->value;

}
