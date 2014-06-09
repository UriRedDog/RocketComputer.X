/*
* oTimer
*
* The Timer object is allocated from the heap
*/
#ifndef __OTIMER_OBJECT_H_
#define __OTIMER_OBJECT_H_

#include "GenericTypeDefs.h"

struct OTIMER;

typedef enum _TIMERSET { TxCON = 0, TxPERIOD, TxREGISTER, TxISR, TxISRPRIORITY } TIMERSET;
typedef enum _TIMEREXE { TxSTART = 0, TxSTOP } TIMEREXE;
typedef BOOL (*OT_Set) (struct OTIMER *This, TIMERSET timerset, WORD val);
typedef WORD (*OT_Get) (struct OTIMER *This, TIMERSET timerset);
typedef void (*OT_Execute) (struct OTIMER *This, TIMEREXE timerexe);
typedef void (*OT_CallBack)(void * instance);
typedef BOOL (*OT_RegisterCallBack)(struct OTIMER *This, OT_CallBack callback, void * instance);

typedef struct _CallBackList
{
  OT_CallBack CallBack;
  void * instance;
  struct _CallBackList *next; 
} CallBackList;

struct OTIMER {
  volatile unsigned int * mTimer;
  CallBackList *mList;
  OT_Set Set;
  OT_Get Get;
  OT_Execute Execute;
  OT_RegisterCallBack RegisterCallBack;
};

extern struct OTIMER * NewTimer(volatile unsigned int *timer);
extern struct OTIMER *Timer1;
extern struct OTIMER *Timer2;
extern struct OTIMER *Timer3;

#endif
