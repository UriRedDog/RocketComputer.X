/*
*  This is not a fully functional OC object.  We are only interested in PWM
*/
#include "p30fxxxx.h"
#include <assert.h>
#include "BitDefines.h"
#include "oCOC.h"

static void Set(struct OCOC const * This, OCSET ocset, WORD val)
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
void InitNewOC(struct OCOC const * This)
{
  if(This->mOC == &OC1CON)
  {
    _TRISB8 = 0;
    _PCFG8 = 1;
    *This->mOC &= ~BIT3; // OCTSEL
  }
  if(This->mOC == &OC2CON)
  {
    _TRISB8 = 0;
    _PCFG8 = 1;
    *This->mOC |= BIT3; // OCTSEL
  }
}

struct OCOC const OutputComp1 = {&OC1CON, &Timer2, Set};
struct OCOC const OutputComp2 = {&OC1CON, &Timer3, Set};

