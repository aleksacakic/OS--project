#ifndef TIMER_H_
#define TIMER_H_

typedef void interrupt (*pInterrupt)(...);

void initializeTimer();

void interrupt timer(...);

void restoreOldTimer();

#endif /* TIMER_H_ */
