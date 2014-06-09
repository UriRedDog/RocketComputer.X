#include "p30fxxxx.h"
#include <stdlib.h>
#include "BitDefines.h"
#include "ocButton.h"

static BUTTONSTATE ButtonOneState = BUTTON_OFF;
static BUTTONSTATE ButtonTwoState = BUTTON_OFF;
static BUTTONSTATE ButtonThreeState = BUTTON_OFF;
static WORD ButtonOneBounce = 0;
static WORD ButtonTwoBounce = 0;
static WORD ButtonThreeBounce = 0;

static BUTTONSTATE Get( struct OBUTTON const * This)
{
  switch(*This->mBounceState)
  {
    case 0xFFFF :
      *This->mButtonState = BUTTON_ON;
      break;
    case 0 :
      *This->mButtonState = BUTTON_OFF;
      break;
    default :
      break;
  }
  return *This->mButtonState;
}

/*
Credit:
A Guide to Debouncing
August 2004
Rev 1: April, 2006
Rev 2: April, 2007
The Ganssle Group
*/
void DeBounce(void const * instance)
{
  struct OBUTTON const * This = (struct OBUTTON const *)instance;
  *This->mBounceState = (*This->mBounceState << 1) | !(*This->mPort & This->mMask);
}
    

void InitNewButton(struct OBUTTON const * This)
{
   *(This->mPort - 1) |= This->mMask; // set tris for input on pin
   *This->mBounceState = 0;
  // if a button is on an shared analog port, then add code to configure ADPCFG
}

struct OBUTTON const ButtonOne = {&ButtonOneBounce, &ButtonOneState, BIT6, &PORTF, Get, DeBounce};
struct OBUTTON const ButtonTwo = {&ButtonTwoBounce, &ButtonTwoState, BIT8, &PORTD, Get, DeBounce};
struct OBUTTON const ButtonThree = {&ButtonThreeBounce, &ButtonThreeState, BIT9, &PORTD, Get, DeBounce};
