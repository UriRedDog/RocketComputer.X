/*
* oLED.h
* defines the LED object structure and member methods
*/
#ifndef __OLED_OBJECT_H_
#define __OLED_OBJECT_H_
#include "GenericTypeDefs.h"

typedef enum _LEDSTATE { LED_OFF = 0, LED_ON } LEDSTATE;
struct OLED;  // forward reference
typedef void (*OL_Set)(struct OLED * This, LEDSTATE newstate);
typedef LEDSTATE (*OL_Get)(struct OLED *This);
typedef void (*OL_Toggle)(struct OLED *This);

struct OLED {
  WORD mMask;
  volatile unsigned int * mpLat;
  OL_Set Set;
  OL_Get Get;
  OL_Toggle Toggle;
};

extern struct OLED * YellowLed;
extern struct OLED * RedOneLed;
extern struct OLED * RedTwoLed;
extern struct OLED * RedHiiLed;
extern struct OLED * GreenLed;
extern struct OLED * WhiteLed;
extern struct OLED * BlinkLed;
extern struct OLED * HeartBeat;

extern struct OLED * NewLED(volatile unsigned int *lat, WORD mask);
#endif

