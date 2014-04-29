#include <REG932.H>
#include "definitions.h"

/* File containing functions to allow use of keypad connected to port 0.
   Uses: Port 0 bits 0 - 6.
   Author: PLJ 08-03-11
   Revisions: none.
*/

/* --- My Own #define --- */
#define NO_KEY 0xFF

/* --- File level variables - in this case declaring keypad port pins with meaningful names --- */

sbit KeyCol3 = P0^0; // column with keys 3, 6, 8 and #
sbit KeyCol2 = P0^1; // column with keys 2, 5, 8 and 0
sbit KeyCol1 = P0^2; //	column with keys 1, 4, 7 and *
sbit KeyRow4 = P0^3; // row with keys *, 0 and #
sbit KeyRow3 = P0^4; // row with keys 7, 8 and 9
sbit KeyRow2 = P0^5; // row with keys 4, 5 and 6
sbit KeyRow1 = P0^6; // row with keys 1, 2 and 3

/*Global Variables*/

extern bit key_flag;
extern unsigned char key_pressed;

/* --- Functions --- */

void init_scan()
{ 
/*
  Purpose: Function to set up port pins to allow for a scan of a keypad.   
           Use once before first use of scan() function.
  Param.s:  None.
  Return:  None.
  Author:  PLJ 08-03-11
  Revisions: none.
*/
  /* set port 0 bits 0 to 6 to quasi-bidirectional mode, leaves 
     bit 7 in state found.
  */
  P0M1 &= 0x80;		// forces all 7 least significant bits of register to '0' leaving 8th bit unchanged
  P0M2 &= 0x80;		// forces all 7 least significant bits of register to '0' leaving 8th bit unchanged

  /* write all 1s to bits 0 - 6 leaving 7 unchanged so bits 0 -6 
     are all weakly pulled high but can be overridden by a strong low
	 from outside.
  */
  P0 |= 0x7f;	  // forces all 7 least significant bits of register to '1' leaving 8th bit unchanged
  
}


extern char scan()
{ 
/*
  Purpose: Scans all keys by setting one row low, rest high then reading state of
           columns and then changing to next row low and so on. Returning a 
		   binary number equivalent to the key value of one of the keys found pressed.
  Param.s:  None.
  Return:  Value of key found pressed as a type char binary number or 0xFF if no key 
           found pressed. * key returns 10, # key returns 13.
  Author:  PLJ 08-03-11
  Revisions:	- editted to for use of key_flag and key_pressed global variable
*/

  
  
  //char key_pressed;


  /* initialise return value to NO KEY using my #define */
  key_pressed = NO_KEY;

  /* clear top row output (init_scan() has set all to high to begin) */
  KeyRow1 = 0;

  /*  read each column - if zero assume key pressed */
  if (KeyCol1 == 0) key_pressed = 1;
  if (KeyCol2 == 0) key_pressed = 2;
  if (KeyCol3 == 0) key_pressed = 3;

  /*  ditto for 2nd row */
  KeyRow1 = 1;
  KeyRow2 = 0;
  if (KeyCol1 == 0) key_pressed = 4;
  if (KeyCol2 == 0) key_pressed = 5;
  if (KeyCol3 == 0) key_pressed = 6;

  /*  ditto for 3rd row */
  KeyRow2 = 1;
  KeyRow3 = 0;
  if (KeyCol1 == 0) key_pressed = 7;
  if (KeyCol2 == 0) key_pressed = 8;
  if (KeyCol3 == 0) key_pressed = 9;

  /*  ditto for 4th row */
  KeyRow3 = 1;
  KeyRow4 = 0;
  if (KeyCol1 == 0) key_pressed = KEY_CHANGE; // '*' button
  if (KeyCol2 == 0) key_pressed = 0;
  if (KeyCol3 == 0) key_pressed = KEY_ENTER; // '#' button
   
  if( key_pressed != NO_KEY)
  {
	key_flag=1;
	return (1);
  }
  else 
  {
	key_flag=0;
	return(0);
  }
  /*  return the key pressed - if any */
  
}