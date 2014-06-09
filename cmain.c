/*
* Construct the objects at compile time which will place them in 
* FLASH rather than RAM.  This will save valuable RAM space at the expense
* of a little more code space.

*/

#include "p30fxxxx.h"
#include "libpic30.h"
#include "device_fcy.h"
#include "BitDefines.h"
#include "oCButton.h"
#include "oCTimer.h"
#include "oCLed.h"
#include "oCOC.h"

/* Macros to load configuration fuse bits in MPLAB IDE for ICD 2
   Note: If using ICD 2 as a Debugger, ensure Channel Select is 
         properly selected for this application.  */

/* Set up for Internal Fast RC */

#if 0
#if FRCx4PLL
	_FOSC(CSW_FSCM_OFF & FRC_PLL4)
	#define TIMER1_PERIOD 0xFFF	 // ~2ms 7.37/4 = 1.84 Mhz -> ~ 500ns
#endif
#if FRCx8PLL
	_FOSC(CSW_FSCM_OFF & FRC_PLL8)
	#define TIMER1_PERIOD 0x8FFF	 // ~2ms (4 * 7.37)/4 = 7.37 Mhz -> ~ 140ns
#endif
#if FRCx16PLL
	_FOSC(CSW_FSCM_OFF & FRC_PLL16)  // not recomended
	#define TIMER1_PERIOD 0xFFFF
#endif
#endif
	#define TIMER1_PERIOD 0x8FF
_FOSC(CSW_FSCM_OFF & FRC)
_FWDT(WDT_OFF)                 /* Turn off the Watch-Dog Timer.  */
_FGS(CODE_PROT_OFF)            /* Disable Code Protection */
_FBORPOR(PBOR_OFF & BORV_20 & MCLR_EN & PWRT_OFF)   /* Enable MCLR reset pin and turn off the power-up timers. */
_FICD(0xFFFF)  // this is weird, cannot use a define since it messes up things

typedef enum _POWERSTATE { OFF = 0, ON } POWERSTATE;

volatile int Activity = 1;

void SetLeds(DWORD state);
void Cylon(void);

#define POWER_BUS_CTL _LATC14
#define POWER_BUS_CTL_ENABLE _TRISC14
#define POWER_BUS_LED _LATC13
#define POWER_BUS_LED_ENABLE _TRISC13
void PowerManager(POWERSTATE state)
{
  POWER_BUS_CTL = state;
  POWER_BUS_LED = state; 
}

static void __attribute__((always_inline)) initPowerManager(void)
{
  POWER_BUS_CTL_ENABLE = 0;
  POWER_BUS_LED_ENABLE = 0;
  PowerManager(ON);
}

DWORD GetButtonsState()
{
  BUTTONSTATE ButtonState;
  DWORD retval = 0;
  ButtonState = ButtonOne.Get(&ButtonOne);
  if(ButtonState == BUTTON_ON)
    retval |= BIT0;
  ButtonState = ButtonTwo.Get(&ButtonTwo);
  if(ButtonState == BUTTON_ON)
    retval |= BIT1;
  ButtonState = ButtonThree.Get(&ButtonThree);
  if(ButtonState == BUTTON_ON)
    retval |= BIT2;
  if(retval)
	Activity = 1;
  return retval;
}
/*
* the state machine will let the student count up to 7 buy pushing buttons
*
*/

void SetLeds(DWORD state)
{
  YellowLed.Set(&YellowLed, LED_OFF);
  RedOneLed.Set(&RedOneLed, LED_OFF);
  RedTwoLed.Set(&RedTwoLed, LED_OFF);
  RedHiiLed.Set(&RedHiiLed, LED_OFF);
  GreenLed.Set(&GreenLed, LED_OFF);
  WhiteLed.Set(&WhiteLed, LED_OFF);
  BlinkLed.Set(&BlinkLed, LED_OFF);
  switch(state) 
  {
    case 1 :
      YellowLed.Set(&YellowLed, LED_ON);
      break;
    case 2 :
      RedOneLed.Set(&RedOneLed, LED_ON);
      break;
    case 3:
      RedTwoLed.Set(&RedTwoLed, LED_ON);
      break;
    case 4:
      RedHiiLed.Set(&RedHiiLed, LED_ON);
      break;
    case 5:  
      GreenLed.Set(&GreenLed, LED_ON);
      break;
    case 6:
      WhiteLed.Set(&WhiteLed, LED_ON);
      break;
    case 7:
      BlinkLed.Set(&BlinkLed, LED_ON);
      break;
	case 8:
		Cylon();
		break;
    default:
      break;
	}
}

/* does they cylon eye thing with the leds */
void Cylon(void)
{
  int loops = 20;
  int i;
  unsigned long cycles = 50000L;
    
  while(loops--)
  {
    for(i=0;i<7;i++)
    {
	  SetLeds(i);
 	  __delay32(cycles);
	}
	for(i=7;i;i--)
	{
	  SetLeds(i);
 	  __delay32(cycles);
	}
  }
}


#define STATE_INIT  0
#define STATE_ONE   1
#define STATE_TWO   2
#define STATE_THREE 3
#define STATE_FOUR  4
#define STATE_FIVE  5
#define STATE_SIX   6
#define STATE_SEVEN 7
#define STATE_EIGHT 8



void Execute(void)
{
  DWORD CurrentState = STATE_INIT;
  DWORD NextState = STATE_ONE;
  DWORD ButtonBits;

  Timer2.Set(&Timer2, TxCON, 0x30);
  Timer2.Set(&Timer2, TxPERIOD, 0xffff);
  Timer2.Set(&Timer2, TxISR, 1);
  Timer2.Execute(&Timer2, TxSTART);

  Timer1.Set(&Timer1, TxPERIOD, TIMER1_PERIOD);
  Timer1.Set(&Timer1, TxISR, 1);
  Timer1.Execute(&Timer1, TxSTART);
  while(1)
  {
    ButtonBits = GetButtonsState();
    switch(CurrentState)
    {
      case STATE_INIT:
        SetLeds(STATE_INIT);
        if(ButtonBits == STATE_ONE)
        {
          CurrentState = NextState;
          NextState = STATE_TWO;
        }  
        break;
      case STATE_ONE:
        SetLeds(STATE_ONE);
        if(ButtonBits == STATE_TWO)
        {
          CurrentState = NextState;
          NextState = STATE_THREE;
        }
        break;
      case STATE_TWO:
        SetLeds(STATE_TWO);
        if(ButtonBits == STATE_THREE)
        {
          CurrentState = NextState;
          NextState = STATE_FOUR;
        }
        break;
      case STATE_THREE:
        SetLeds(STATE_THREE);
        if(ButtonBits == STATE_FOUR)
        {
          CurrentState = NextState;
          NextState = STATE_FIVE;
        }
        break;
      case STATE_FOUR:
        SetLeds(STATE_FOUR);
        if(ButtonBits == STATE_FIVE)
        {
          CurrentState = NextState;
          NextState = STATE_SIX;
        }
        break;        
      case STATE_FIVE:
        SetLeds(STATE_FIVE);
        if(ButtonBits == STATE_SIX)
        {
          CurrentState = NextState;
          NextState = STATE_SEVEN;
        }
        break;      
      case STATE_SIX:
        SetLeds(STATE_SIX);
        if(ButtonBits == STATE_SEVEN)
        {
          CurrentState = NextState;
          NextState = STATE_EIGHT;
        }
        break;      
      case STATE_SEVEN:
        SetLeds(STATE_SEVEN);
        if(ButtonBits == STATE_EIGHT)
        {
          CurrentState = NextState;
          NextState = STATE_INIT;
        }
	  case STATE_EIGHT:
		{
		    int loops = 90;
			unsigned long cycles = 100000L;
    
  			while(loops--)
  			  __delay32(cycles);

         SetLeds(STATE_EIGHT);
         CurrentState = STATE_INIT;
         NextState = STATE_ONE;
		}
        break;
    }    
  }
}

/*
* main must set the power switch FET to maintain power after the power button is released.
*/ 
int main()
{
  ADPCFG = 0xFFFF;                /*Ensure AN pins are digital for ICD 2 debugging*/
  initPowerManager();

  // buttons defined in ocButton.h
  InitNewButton(&ButtonOne);
  InitNewButton(&ButtonTwo);
  InitNewButton(&ButtonThree);

  // Timers defined in ocTimer.h
  Timer1.Set(&Timer1, TxCON, 0);
  Timer1.Set(&Timer1, TxPERIOD, TIMER1_PERIOD);

  // led defined in ocLED.h
  InitNewLED(&YellowLed);
  InitNewLED(&RedOneLed);
  InitNewLED(&RedTwoLed);
  InitNewLED(&RedHiiLed);
  InitNewLED(&GreenLed);
  InitNewLED(&WhiteLed);
  InitNewLED(&BlinkLed);
  InitNewLED(&HeartBeat);
   
//  InitNewOC(&OutputComp1);
  
  Execute();   

  PowerManager(OFF);
  return 0;
}



