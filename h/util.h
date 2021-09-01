#ifndef UTIL_H_
#define UTIL_H_

extern volatile int switchOnDemand;
extern volatile int lockFlag;
extern void dispatch();


#define lock lockFlag++;

#define unlock lockFlag=(lockFlag>0)?lockFlag-1:0;if(lockFlag==0 && switchOnDemand==1) dispatch();

typedef int (*usrFunc)(int, char*[]);


#endif /* UTIL_H_ */
