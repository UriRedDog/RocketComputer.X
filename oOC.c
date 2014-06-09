/*
*  This is not a fully functional OC object.  We are only interested in PWM
*/
#include "p30fxxxx.h"
#include <stdlib.h>
#include "BitDefines.h"
#include "oOC.h"

struct OOC *OutputComp1, *OutputComp2;

static void Set(struct OOC * This, OCSET ocset, WORD val)
{
  switch (ocset)
  {
    case OCxCON:
      *This->mOC = val;
      break;
    case OCxPR:
      This->mTimer->Set(This->mTimer, TxPERIOD, val);
      break;
    case OCxR :
      *(This->mOC -1) = val;
      break;
    case OCxRS :
      *(This->mOC -2) = val;
      break;
  }  
}

/*
*  OC1 is on port BR8
*  OC2 is on port RB9
*/
struct OOC * NewOC(volatile unsigned int *ocx, struct OTIMER *timer)
{
  struct OOC * This;

  This = malloc(sizeof(*This));
  if(This != NULL)
  {
    This->mOC = ocx;
    if (ocx == &OC1CON)
    {
      _TRISB8 = 0;
      _PCFG8 = 1;
    }
    if (ocx == &OC2CON)
    {
      _TRISB9 = 0;
      _PCFG9 = 1;
    }    
    This->mTimer = timer;
    if (timer->mTimer == &T2CON)
      *ocx &= ~BIT3; // OCTSEL
    if (timer->mTimer == &T3CON)
      *ocx |= BIT3; // OCTSEL
    This->Set = Set;
  }
  return This;
}
