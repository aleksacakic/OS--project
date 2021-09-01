#ifndef USERTHR_H_
#define USERTHR_H_

#include "thread.h"
#include "util.h"


class UserThread:public Thread{
public:
	UserThread(usrFunc func, int argc, char* argv[],StackSize stackSize = defaultStackSize, Time timeSlice = defaultTimeSlice)
:Thread(defaultStackSize,defaultTimeSlice),func(func),argc(argc),argv(argv),ret(0){
		size = stackSize;
		time = timeSlice;
	}

	~UserThread(){waitToComplete();}

	virtual void run(){
		ret = func(argc,argv);
	}

	int funcReturn(){
		return ret;
	}

	virtual Thread* clone()const {
		return new UserThread(func, argc, argv, size, time);
	}

private:
	usrFunc func;
	 int argc;
	 char** argv;
	 int ret;

	 StackSize size;
	 Time time;
};



#endif /* USERTHR_H_ */
