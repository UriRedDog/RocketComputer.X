#include <stdlib.h>
#include "oButton.h"

struct OBUTTON * Button1;
struct OBUTTON * Button2;
struct OBUTTON * Button3;

static BOOL Get( struct OBUTTON *This)
{
  if (This->mState == 0xF000)
    return TRUE;
  else
    return FALSE;
}

/*
Credit:
A Guide to Debouncing
August 2004
Rev 1: April, 2006
Rev 2: April, 2007
The Ganssle Group

One bit of cleverness lurks in the algorithm. As long as the switch isn’t closed ones shift
through State. When the user pushes on the button, the stream changes to a bouncy
pattern of ones and zeroes, but at some point there’s the last bounce (a one) followed by a
stream of zeroes. We OR in 0xe000 to create a “don’t care” condition in the upper bits.
But as the button remains depressed State continues to propagate zeroes. There’s just the
one time, when the last bouncy “one” was in the upper bit position, that the code returns a
TRUE. That bit of wizardry eliminates bounces and detects the edge, the transition from
open to closed.
*/
static void DeBounce(struct OBUTTON *This)
{
  This->mState = (This->mState << 1) | !(*This->mPort & This->mMask) | 0xE000;
}
    
// mask must only have one bit set, a debug build could check this
extern struct OBUTTON * NewButton(volatile unsigned int *port, WORD mask)
{
  struct OBUTTON * This;
     
  This = malloc(sizeof(*This));
  if(This != NULL)
  {
    *(port - 1) |= mask;   // set tris for input on pin
    // if a button is on an shared analog pin, then add code to configure ADPCFG
    This->mMask = mask;
    This->mPort = port;
    This->mState = 0;
    This->Get = Get;
    This->DeBounce = DeBounce;
  }
  return This;
}
