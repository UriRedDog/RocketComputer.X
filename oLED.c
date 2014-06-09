/*
* oLED.h
*
* create global oLED structures using runtime constructors which
* configure the TRIS and ADPCFG.
*/
#include "p30fxxxx.h"
#include <stdlib.h>
#include "oLED.h"

struct OLED * YellowLed;
struct OLED * RedOneLed;
struct OLED * RedTwoLed;
struct OLED * RedHiiLed;
struct OLED * GreenLed;
struct OLED * WhiteLed;
struct OLED * BlinkLed;
struct OLED * HeartBeat;

static LEDSTATE Get(struct OLED *This)
{
  if (*This->mpLat & This->mMask)
    return LED_ON;
  return LED_OFF;
}

static void Set(struct OLED *This, LEDSTATE newstate)
{
  if(newstate == LED_ON)
    *This->mpLat |= This->mMask;
  if(newstate == LED_OFF)
    *This->mpLat &= ~This->mMask;
  // we could assert if the newstate is not ON or OFF, but we will do nothing
}

static void Toggle(struct OLED *This)
{
  *This->mpLat ^= This->mMask;
}

// the LAT must be passed by reference (i.e. pointer)
struct OLED * NewLED(volatile unsigned int *lat, WORD mask)
{
  struct OLED * This;

  This = malloc(sizeof(*This));
  if(This != NULL)
  {
    This->mMask = mask;
    This->mpLat = lat;
    This->Set = Set;
    This->Get = Get;
    This->Toggle = Toggle;
    if (lat == &LATB)  // only port B has analog pins
      ADPCFG |= mask; // make digital
    *(lat -2) &= ~mask; // set the TRIS to configure pin as an output
  }
  return This;
}

