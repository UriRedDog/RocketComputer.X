 

/*
 Use one #defines per the device speed selection.
 The internal Base XTAL on the dsPIC 
   is 7.3728 MHz. Depending on PLL selection final 
   device speed will be:
   	7.3728 MIPs (for FRCx4PLL) or
   	14.745600 MIPs (for FRCTx8PLL) or
   	29.491200 MIPs (for FRCx16PLL) or
   	
   This file is included in the following source code files and is used to
   adjust timing specific parameters based on the instruction cycle rate:
 
*/

/* Set ONLY ONE of the following defines to TRUE */

#define FRCx4PLL    1   //	7.3728 MIPs (for FRCx4PLL)
#define FRCx8PLL    0   //   14.745600 MIPs (for FRCx8PLL)
#define FRCx16PLL   0   //   29.491200 MIPs (for FRCx16PLL)

