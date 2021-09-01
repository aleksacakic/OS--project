#include "thread.h"
#include "pcb.h"
#include "timer.h"
#include "util.h"
#include "userThr.h"
#include <IOSTREAM.H>

extern int userMain(int argc, char* argv[]);

int main(int argc, char* argv[]){

	int ret;

	initializeTimer();

	lock;
	PCB* usrMain = new PCB(0,1,0);
	usrMain->status = PCB::READY;
	PCB::running = usrMain;

	UserThread* user = new UserThread(userMain, argc,argv);
	unlock;

	user->start();
	user->waitToComplete();
	ret = user->funcReturn();

	lock;
	delete user;
	delete usrMain;
	unlock;

	restoreOldTimer();

	return ret;

}

