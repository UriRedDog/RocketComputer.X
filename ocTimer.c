/*
* ocTimer.c
* Flash resedent oTimer objects.
* We still want the flexability of having dynamic callback registration.
* The callback list is defined as a static fixed size array
*
*/

#include "p30fxxxx.h"
#include <stdlib.h>
#include "BitDefines.h"
#include "ocTimer.h"
#include "ocButton.h"
#include "ocLED.h"

extern CallBackList mListTimer1[5];
extern CallBackList mListTimer2[2]; 
CallBackList mListTimer3[2] = {{NULL,NULL},{NULL,NULL}};


static BOOL Set(struct OTIMER const *This, TIMERSET timerset, WORD val)
{
  volatile unsigned int * pr;
  volatile unsigned int * tmr;
  WORD IEbit = BIT7; // default to timer 3
  
//  assert (This->mTimer == &T1CON || This->mTimer == &T2CON || This->mTimer == &T3CON)
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

static WORD Get(struct OTIMER const *This, TIMERSET timerset)
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

static void Execute(struct OTIMER const *This, TIMEREXE timerexe)
{
  if (timerexe == TxSTART)
    *This->mTimer |= BIT15;
  if (timerexe == TxSTOP)
    *This->mTimer &= ~BIT15;
}
  
void __attribute__((__interrupt__, no_auto_psv)) _T1Interrupt(void)
{
  CallBackList *list = mListTimer1;

  while(list->CallBack != NULL)
  {
    list->CallBack(list->instance);
    if(list)list++;
  }
  _T1IF = 0;
}

void __attribute__((__interrupt__, no_auto_psv)) _T2Interrupt(void)
{
  CallBackList *list = mListTimer2;

  while(list->CallBack != NULL)
  {
    list->CallBack(list->instance);
    list++;
  }  
  _T2IF = 0;
}

void __attribute__((__interrupt__, no_auto_psv)) _T3Interrupt(void)
{
  CallBackList *list = mListTimer3;

  while(list->CallBack != NULL)
  {
    list->CallBack(list->instance);
    list++;
  }
  _T3IF = 0;
}

static int ShutDownCount = 0;
void ShutDown(void const * instance)
{
	extern void PowerManager(int);
	extern volatile int Activity;
	if(Activity)
    {
		Activity = 0;
		ShutDownCount = 0;
	}
	if(ShutDownCount++ == 5)
		PowerManager(0);
}

struct OTIMER const Timer1 = {&T1CON, Set, Get, Execute};
struct OTIMER const Timer2 = {&T2CON, Set, Get, Execute};
struct OTIMER const Timer3 = {&T3CON, Set, Get, Execute};

CallBackList mListTimer1[5] = {
{DeBounce, &ButtonOne},
{DeBounce, &ButtonTwo},
{DeBounce, &ButtonThree},
{HeartBeatCallBack,&HeartBeat},
{NULL,NULL}
};

CallBackList mListTimer2[2] = {
{ShutDown,NULL},
{NULL,NULL}
};
