
#include "p30fxxxx.h"
#include <stdlib.h>
#include "device_fcy.h"
#include "BitDefines.h"
#include "oButton.h"
#include "oTimer.h"
#include "oLed.h"
#include "oOC.h"

typedef enum _POWERSTATE { OFF = 0, ON } POWERSTATE;

#define POWERBUS_CTL_LAT _LATC13
#define POWERBUS_CTL_TRIS _TRISC13
static void __attribute__((always_inline)) PowerManager(POWERSTATE state)
{
  POWERBUS_CTL_LAT = state; 
}

static void __attribute__((always_inline)) initPowerManager(void)
{
  POWERBUS_CTL_LAT = 1;
  PowerManager(ON);
}



/* Macros to load configuration fuse bits in MPLAB IDE for ICD 2
   Note: If using ICD 2 as a Debugger, ensure Comm Channel Select is 
         properly selected for this application.  */

/* Set up for Internal Fast RC */
#if  FRCx4PLL          
  _FOSC( CSW_FSCM_OFF & XT_PLL4 );
  #define TIMER1_PERIOD 0xFFF	 // ~2ms 7.37/4 = 1.84 Mhz -> ~ 500ns
#endif 
#if   FRCx8PLL         
  _FOSC( CSW_FSCM_OFF & XT_PLL8 );
  #define TIMER1_PERIOD 0x8FFF	 // ~2ms (4 * 7.37)/4 = 7.37 Mhz -> ~ 140ns
#endif
#if   FRCx16PLL       
  _FOSC( CSW_FSCM_OFF & XT_PLL16 );  // not recomended
  #define TIMER1_PERIOD 0xFFFF
#endif
#define ONE_HALF_SECOND 500

_FWDT(WDT_OFF);                 /* Turn off the Watch-Dog Timer.  */
_FGS(CODE_PROT_OFF);            /* Disable Code Protection */
_FBORPOR(MCLR_EN & BORV_45 & PWRT_OFF);   /* Enable MCLR reset pin and turn off the power-up timers. */

void Execute(void)
{
  Timer1->Set(Timer1,TxISR, 1);
  Timer1->Execute(Timer1, TxSTART);
  while(1)
  {
    if(Button1->Get(Button1))
      YellowLed->Toggle(YellowLed);
    if(Button2->Get(Button2))
      RedOneLed->Toggle(RedOneLed);
    if(Button3->Get(Button3))
      RedTwoLed->Toggle(RedTwoLed);    
  }
}

void HeartBeatCallBack(void * instance)
{
  static volatile unsigned int g_HeartBeatCounter = 0;
  struct OLED * ThisLed = (struct OLED *) instance;
  if (g_HeartBeatCounter > ONE_HALF_SECOND)
  {
    g_HeartBeatCounter = 0;
    ThisLed->Toggle(ThisLed);
  }
  g_HeartBeatCounter++;
}

/*
* main must set the power switch FET to maintain power after the power button is released.
* Set up the heap in the project setting so that there is enough space for all inital objects.
* Then there is no need to check for a null return from NewObjects at init time.  If the application
* creates and destroys objects during Execute(), then NewObject NULL checks are nessesary.
*/ 
int main()
{
  initPowerManager();
  
  // buttons defined in oButton.h
  Button1 = NewButton(&PORTF, BIT6);
  Button2 = NewButton(&PORTD, BIT8);
  Button3 = NewButton(&PORTD, BIT9);
  
  // Timers defined in oTimer.h 
  Timer1 = NewTimer(&T1CON);
  Timer1->RegisterCallBack(Timer1, (OT_CallBack)Button1->DeBounce, (void *)Button1);
  Timer1->RegisterCallBack(Timer1, (OT_CallBack)Button2->DeBounce, (void *)Button2);
  Timer1->RegisterCallBack(Timer1, (OT_CallBack)Button3->DeBounce, (void *)Button3);
  Timer1->Set(Timer1, TxPERIOD, TIMER1_PERIOD);
  
  YellowLed = NewLED(&LATB, BIT0);
  RedOneLed = NewLED(&LATB, BIT1);
  RedTwoLed = NewLED(&LATB, BIT2);
  RedHiiLed = NewLED(&LATB, BIT3);
  GreenLed  = NewLED(&LATB, BIT4);
  WhiteLed  = NewLED(&LATB, BIT5);
  BlinkLed  = NewLED(&LATB, BIT6);
  
  HeartBeat = NewLED(&LATC, BIT13);
  Timer1->RegisterCallBack(Timer1, (OT_CallBack)HeartBeatCallBack, (void *)HeartBeat);
 
  Timer2 = NewTimer(&T2CON);
  OutputComp1 = NewOC(&OC1CON, Timer2);
  
  Execute();

	PowerManager(OFF);
  return 0;
}



