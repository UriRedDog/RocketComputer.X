/*
* ocLED.h
* defines the const LED object structure and member methods
*/
#ifndef __OLED_OBJECT_H_
#define __OLED_OBJECT_H_
#include "GenericTypeDefs.h"

#define ONE_HALF_SECOND 500

typedef enum _LEDSTATE { LED_OFF = 0, LED_ON } LEDSTATE;
struct OLED;
typedef void (*OL_Set)(struct OLED const * This, LEDSTATE newstate);
typedef LEDSTATE (*OL_Get)(struct OLED const * This);
typedef void (*OL_Toggle)(struct OLED const * This);

struct OLED {
  WORD mMask;
  volatile unsigned int *mpLat;
  OL_Set Set;
  OL_Get Get;
  OL_Toggle Toggle;
};

extern void InitNewLED(struct OLED const * This);
extern void HeartBeatCallBack(void const * instance);

extern struct OLED const YellowLed;
extern struct OLED const RedOneLed;
extern struct OLED const RedTwoLed;
extern struct OLED const RedHiiLed;
extern struct OLED const GreenLed;
extern struct OLED const WhiteLed;
extern struct OLED const BlinkLed;
extern struct OLED const HeartBeat;

#endif

