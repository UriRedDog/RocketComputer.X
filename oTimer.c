#include "p30fxxxx.h"
#include <stdlib.h>
//#include <assert.h>
#include "BitDefines.h"
#include "oTimer.h"


struct OTIMER *Timer1 = NULL;
struct OTIMER *Timer2 = NULL;
struct OTIMER *Timer3 = NULL;

static BOOL Set(struct OTIMER *This, TIMERSET timerset, WORD val)
{
  volatile unsigned int * pr;
  volatile unsigned int * tmr;
  unsigned int IEbit = BIT7; // default to timer 3
  
  // assert(This->mTimer == &T1CON || This->mTimer == &T2CON || This->mTimer == &T3CON)
    pr = This->mTimer - 1;
    tmr = pr -1;
    if (This->mTimer != &T1CON) 
      { pr--; tmr--; }
    if (This->mTimer == &T2CON)
      { tmr--; IEbit = BIT6; }
    if (This->mTimer == &T1CON)
      IEbit = BIT3;
    switch (timerset)
    {
      case TxCON: *This->mTimer = val; break;
      case TxPERIOD : *pr = val; break;
      case TxREGISTER : *tmr = val; break;
      case TxISR: if(val) IEC0 |= IEbit; else IEC0 &= ~IEbit; break;
      case TxISRPRIORITY : break;  // not used
    }  
  return TRUE;
}  

static WORD Get(struct OTIMER *This, TIMERSET timerset)
{
  volatile unsigned int * pr;
  volatile unsigned int * tmr;
  
  if (This->mTimer == &T1CON || This->mTimer == &T2CON || This->mTimer == &T3CON)
  {
    pr = This->mTimer - 1;
    tmr = pr -1;
    if (This->mTimer != &T1CON) 
      { pr--; tmr--; }
    if (This->mTimer == &T2CON)
      tmr--;
    if(timerset == TxCON)
      return *This->mTimer;
    if(timerset == TxPERIOD)
      return *pr;
    if(timerset == TxREGISTER)
      return *tmr;
   }
  return 0;
}

static void Execute(struct OTIMER *This, TIMEREXE timerexe)
{
  if (timerexe == TxSTART)
    *This->mTimer |= BIT15;
  if (timerexe == TxSTOP)
    *This->mTimer &= ~BIT15;
}

static BOOL RegisterCallBack(struct OTIMER *This, OT_CallBack CallBack, void * instance)
{
  CallBackList *newcb, *list;
  newcb = malloc(sizeof(*newcb));
  if (newcb == NULL)
    return FALSE;
  newcb->CallBack = CallBack;
  newcb->instance = instance;
  newcb->next = NULL;
  if (This->mList == NULL)
    This->mList = newcb;
  else
  {
    list = This->mList;
    while(list->next != NULL)
      list = list->next;
    list->next = newcb;
  }
  return TRUE;
}

struct OTIMER * NewTimer(volatile unsigned int *timerBase)
{
  struct OTIMER * This;

//  assert(timerBase != NULL);
  This = malloc(sizeof(*This));
  if(This != NULL)
  {
    This->mTimer = timerBase;
    This->Set = Set;
    This->Get = Get;
    This->Execute = Execute;
    This->mList = NULL;
    This->RegisterCallBack = RegisterCallBack;
  }
  return This;
}



void __attribute__((__interrupt__, no_auto_psv)) _T1Interrupt(void)
{
  CallBackList *list;
  if(Timer1 != NULL)
  {
    list = Timer1->mList;
    while(list != NULL)
    {
      list->CallBack(list->instance);
      list = list->next;
    }
  }
  
  _T1IF = 0;
}

void __attribute__((__interrupt__, no_auto_psv)) _T2Interrupt(void)
{
  CallBackList *list;
  if(Timer2 != NULL)
  {
    list = Timer2->mList;
    while(list != NULL)
    {
      list->CallBack(list->instance);
      list = list->next;
    }
  }
  
  _T2IF = 0;
}

void __attribute__((__interrupt__, no_auto_psv)) _T3Interrupt(void)
{
  CallBackList *list;
  if(Timer3 != NULL)
  {
    list = Timer3->mList;
    while(list != NULL)
    {
      list->CallBack(list->instance);
      list = list->next;
    }
  }
  
  _T3IF = 0;
}

