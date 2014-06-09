/*
* oOC
*
* the Output Compare object's constructor needs a previously constructed timer.
* We will use the Output Compare in PWM mode.
*
*/
#ifndef __OOUTPUT_COMPARE_OBJECT_H_
#define __OOUTPUT_COMPARE_OBJECT_H_

#include "oCTimer.h"
struct OCOC;
typedef enum _OCSET { OCxCON = 0, OCxPR, OCxR, OCxRS } OCSET;
typedef void (*OC_Set)(struct OCOC const * This, OCSET ocset, WORD val);

struct OCOC {
  volatile unsigned int * mOC;
  struct OTIMER const *mTimer;
  OC_Set Set;
};

extern struct OCOC const OutputComp1;
extern struct OCOC const OutputComp2;

extern void InitNewOC(struct OCOC const * This);

#endif
