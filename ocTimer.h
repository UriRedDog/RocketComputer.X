/*
* ocTimer.h
*
* The Timer object is a const (in flash) structure member rather than allocating
* the object from the heap.  One concern would be that the callback list would have
* to be static globals within the scope of this object rather than a member variable.
*/

#ifndef __OTIMER_OBJECT_H_
#define __OTIMER_OBJECT_H_

#include "GenericTypeDefs.h"

struct OTIMER;

typedef enum _TIMERSET { TxCON = 0, TxPERIOD, TxREGISTER, TxISR, TxISRPRIORITY } TIMERSET; 
typedef enum _TIMEREXE { TxSTART = 0, TxSTOP} TIMEREXE;
typedef BOOL (*OT_Set) (struct OTIMER const *This, TIMERSET timerset, WORD val);
typedef WORD (*OT_Get) (struct OTIMER const *This, TIMERSET timerset);
typedef void (*OT_Execute) (struct OTIMER const *This, TIMEREXE timerexe);
typedef void (*OT_CallBack)(void const * instance);

typedef struct _CallBackList
{
  OT_CallBack CallBack;
  void const * instance;
} CallBackList;

struct OTIMER {
  volatile unsigned int * mTimer;
  OT_Set Set;
  OT_Get Get;
  OT_Execute Execute;
 };

extern struct OTIMER const Timer1;
extern struct OTIMER const Timer2;
extern struct OTIMER const Timer3;

#endif

