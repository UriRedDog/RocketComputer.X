/*
* OButton
*
* debounces a button press.
* The debounce method is called periodicaly via a timer ISR.
*
* A button object cannot be completely located in FLASH since there needs
* to be a state variable that is modified by the debounce member function.
* We could make the state variable a global static variable within the the scope of 
* this object and that would allow it to be a const.
*/


/*
* oButton
*
* a oButton does not need to be allocated from the heap.  But the object cannot
* be a const object since the object needs a state variable for the debbounce member function.
* One could complicate things by defining static globals within the scope of this object, but it
* gets messy.
*/
#ifndef __OBUTTON_OBJECT_H_
#define __OBUTTON_OBJECT_H_


#include "GenericTypeDefs.h"
#define CHECK_MSEC 5 // Read hardware every 5 msec
#define PRESS_MSEC 10 // Stable time before registering pressed
#define RELEASE_MSEC 10 // Stable time before registering released

typedef enum _BUTTONSTATE { BUTTON_OFF = 0, BUTTON_ON, BUTTON_BOUNCING } BUTTONSTATE;
struct OBUTTON;
typedef BUTTONSTATE (*OB_GetMethod)(struct OBUTTON const * This);
typedef void (*OB_DeBounceMethod)(void const * instance);

extern void InitNewButton(struct OBUTTON const * This);
extern void DeBounce(void const * instance);

struct OBUTTON {
  WORD *mBounceState;
  BUTTONSTATE *mButtonState;
  WORD mMask;
  volatile unsigned int *mPort;
  OB_GetMethod Get;
  OB_DeBounceMethod DeBounce;
};

extern struct OBUTTON const ButtonOne;
extern struct OBUTTON const ButtonTwo;
extern struct OBUTTON const ButtonThree;



#endif

