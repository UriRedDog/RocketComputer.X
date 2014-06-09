/*
* oOC
*
* the Output Compare object's constructor needs a previously constructed timer.
* We will use the Output Compare in PWM mode.
*
*/
#ifndef __OOUTPUT_COMPARE_OBJECT_H_
#define __OOUTPUT_COMPARE_OBJECT_H_

#include "oTimer.h"
struct OOC;
typedef enum _OCSET { OCxCON = 0, OCxPR, OCxR, OCxRS } OCSET;
typedef void (*OC_Set)(struct OOC * This, OCSET ocset, WORD val);

struct OOC {
  volatile unsigned int * mOC;
  struct OTIMER *mTimer;
  OC_Set Set;
};

extern struct OOC * NewOC(volatile unsigned int *OCx, struct OTIMER *timer);
extern struct OOC *OutputComp1, *OutputComp2;

#endif
