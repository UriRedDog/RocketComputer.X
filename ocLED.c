/*
* ocLED.c
*
* create global oLED structures using compile time constructors
* The InitNewLED is called to set TRIS and ADPCFG registers since
* this cannot be accomplished in the compile time constructors.
*/
#include "p30fxxxx.h"
#include <stdlib.h>
#include "BitDefines.h"
#include "oCLED.h"

static LEDSTATE Get(struct OLED const *This)
{
  if (*This->mpLat & This->mMask)
    return LED_ON;
  return LED_OFF;
}

static void Set(struct OLED const *This, LEDSTATE newstate)
{
  if(newstate == LED_ON)
    *This->mpLat |= This->mMask;
  if(newstate == LED_OFF)
    *This->mpLat &= ~This->mMask;
  // we could assert if the newstate is not ON or OFF, but we will do nothing
}

static void Toggle(struct OLED const *This)
{
  *This->mpLat ^= This->mMask;
}

void InitNewLED(struct OLED const * This)
{
  if (This->mpLat == &LATB)  // only port B has analog pins
    ADPCFG |= This->mMask; // make digital
  *(This->mpLat -2) &= ~This->mMask;// set the TRIS to configure pin as an output
}

void HeartBeatCallBack(void const * instance)
{
  static volatile unsigned int g_HeartBeatCounter = 0;
  struct OLED const * ThisLed = (struct OLED const *) instance;
  if (g_HeartBeatCounter > ONE_HALF_SECOND)
  {
    g_HeartBeatCounter = 0;
    ThisLed->Toggle(ThisLed);
  }
  g_HeartBeatCounter++;
}

// LATB pins 0 - 6
struct OLED const YellowLed = {BIT0, &LATB, Set, Get, Toggle};
struct OLED const RedOneLed = {BIT1, &LATB, Set, Get, Toggle};
struct OLED const RedTwoLed = {BIT2, &LATB, Set, Get, Toggle};
struct OLED const RedHiiLed = {BIT3, &LATB, Set, Get, Toggle};
struct OLED const GreenLed  = {BIT4, &LATB, Set, Get, Toggle};
struct OLED const WhiteLed  = {BIT5, &LATB, Set, Get, Toggle};
struct OLED const BlinkLed  = {BIT6, &LATB, Set, Get, Toggle};
struct OLED const HeartBeat = {BIT13, &LATC, Set, Get, Toggle};
