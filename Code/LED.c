#include <REG932.H>
#include "definitions.h"
#define TBL_LNGTH 37


unsigned char display[4];
unsigned char seg=1;
unsigned char dig=0;
code unsigned char rld_TH1 = 0xfc; //re-loads for timer	1
code unsigned char rld_TL1 = 0x18; //leaves 1000 counts left to go
code unsigned char sev_seg [TBL_LNGTH] = {0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7C,0x07,0x7F,0x67,
								          0x77,0x7C,0x39,0x5E,0x79,0x71,0x6F,0x74,0x30,0x1E,
							              0xF6,0x38,0xD4,0x54,0x3F,0x73,0x6F,0x50,0x6D,0x78,
							              0x1C,0xBE,0xFE,0xD2,0x6E,0xDB,0x00};
										/* This is a table of port outputs to display on 7 segment for the following:
										0,1,2,3,4,5,6,7,8,9,
										a,b,c,d,e,f,g,h,i,j,
										k,l,m,n,o,p,q,r,s,t,
										u,v,w,x,y,z,'space' */
							              
void dig_select(unsigned char pos)
{  // --- function that sets port pins to select a particular LED

	switch (pos) {
		case 0: TxD = 0;RxD = 1;OCB = 1;OCC = 1; break;
		// digit zero selected 
		case 1: TxD = 1;RxD = 0;OCB = 1;OCC = 1; break;
		// digit one selected 
		case 2: TxD = 1;RxD = 1;OCB = 0;OCC = 1; break;
		// digit two selected 
		case 3: TxD = 1;RxD = 1;OCB = 1;OCC = 0; break;
		// digit three selected 
		default: TxD = 1;RxD = 1;OCB = 1;OCC = 1; 
		// if faulty dig value set all digits off
		  }
}

void settim1 ()
{ // --- function to initialise timer 1 ---

  // set timer 1 mode to mode 1 without disturbing timer0's settings
  TMOD &= 0x0f; // bitwise and to maintain upper 4 bits values as-is & force lower 4 to zero
  TMOD |= 0x10;	// bitwise or to maintain upper 4 bits values as-is & force lower 4 to 0001
  //select mode one both times
  TH1 =  rld_TH1; //initialise timer re-load
  TL1 =  rld_TL1;
  ET1 = 1;  //enable timer 0 interrupts
  EA = 1;  //switch on all enabled interrupts
  TR1 = 1;  //start timer 0 running
}


void timer1 () interrupt 3 using 3
{  // --- timer 0 interrupt handling routine ---

//interrupt only occurs when timer1 overflows
  
  TR1 = 0;  //stop timer
  TH1 = rld_TH1;  //reload once again
  TL1 = rld_TL1;
  TR1 = 1;  //re-start timer

// timer related stuff here

    P2 = display[dig] & seg;
	dig_select(dig);
	seg = seg << 1;
	if (seg==0x80)
	{
		dig = (dig+1) & 0x03;
		seg = 1;
	}
}



unsigned char ascii_to_led(unsigned char ch )
{  // --- function to turn an ascii character into the appropriate ---
   //     seven segment pattern

   // selects appropriate table offset
	if(ch==PASS_NULL||ch==32)
	ch=36; //this is a space or blank character
	else
	   if(ch>=0 && ch<10)
	   ch=ch; //do nothing
	   else
		   if (ch >= '0' && ch <= '9')
			 ch = ch - '0';  // numerals are at start of table [0] - [9]
		   else
			 if (ch >= 'A' && ch <= 'Z')
			   ch = 10 + ch - 'A';	// alphbetic chars come after numerals [10] - [35]
			 else
			   if (ch >= 'a' && ch <= 'z')
				 ch = 10 + ch - 'a'; // lower-case coverted to same as upper case
			   else
				 ch = TBL_LNGTH; // for an unsupported character use an out of range value
		
	// now access the table to get the port 2 segement pattern needed
	if (ch < TBL_LNGTH)	
	  return ( sev_seg[ch]);  // get seven segment pattern for P2 from table
	else
	  return (0); // blank 
}

void Write (unsigned char *m)
{	// --- function to put one character on the desired digit of the display ---
	//     ascii_ch must contain the ASCII character to display and 
	//     digit should specify which digit it should appear on (0 left most, 3 right most)
	unsigned char i;

	for (i=0; i<4; i++) 
	{
		display[i] = ascii_to_led(m[i]);  // convert the ascii and store to display
	}
}

void DisplayInit ()
{
    P2 = 0;  // blank LED
    P2M1 = 0;  
    P2M2 = 0xFF; //set port 2 to be all push-pull outputs
   
    P1 = 0xFF;	// write all port 1 to all ones
    TxD = 0;		// write p1.0 to zero
    P1M1 = 0xFF; // set port 1 only bits 0,1,6 & 7 as open-drain outputs
    P1M2 = 0xC3; // all the rest as input-only 

	dig = 0;
	seg = 1;
	display[0] = 0;
	display[1] = 0;
	display[2] = 0;
	display[3] = 0;
	settim1();
}							              
							              


    
							              
