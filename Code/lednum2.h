
#define BLANK 0   // used when character is to be blanked made available to other parts of program here


extern void put_led (unsigned char ascii_ch, unsigned char digit);
/*
  Purpose: Function to put one character on the desired digit of the display. 
  Param.s: unsigned char ascii_ch must contain the ASCII character to display.
           unsigned char  digit should specify which digit it should appear on (0 left most, 3 right most).
  Return:  None.
  Author:  PLJ 08-03-11
  Revisions: none.
*/

extern void setup_led (void);
/*
  Purpose: Function to initialise port pins ready to use for display.  Initialises port 2 for outputs
           as segment drives and bits 0, 1, 6 & 7 of port 1 as outputs for digit selection.
            
  Param.s: None.
  Return:  None.
  Author:  PLJ 08-03-11
  Revisions: none.
*/


