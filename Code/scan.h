/* Header file to use when also using functions from scan.c file 
   allows use of keypad connected to port 0.
   Uses: Port 0 bits 0 - 6.
   Author: PLJ 08-03-11
   Revisions: none.
*/

#define NO_KEY 0xFF	   // a #define used in scan.c also available here to other parts of program

extern void init_scan();
/*
  Purpose: Function to set up port pins to allow for a scan of a keypad.   
           Use once before first use of scan() function.
  Param.s:  None.
  Return:  None.
  Author:  PLJ 08-03-11
  Revisions: none.
*/

extern char scan();
/*
  Purpose: Scans all keys by setting one row low, rest high then reading state of
           columns and then changing to next row low and so on. Returning a 
		   binary number equivalent to the key value of one of the keys found pressed.
  Param.s:  None.
  Return:  Value of key found pressed as a type char binary number or 0xFF if no key 
           found pressed. * key returns 10, # key returns 13.
  Author:  PLJ 08-03-11
  Revisions: none.
*/

 