//This will house all interrupts used on the system
#include <REG932.h>
#include"scan.h"
#define PASS_LENGTH 8
code unsigned int clock_freq=100; 		//defines the frequency (Hz) of increases in global "clock". Used to calc time delays
unsigned int clock;			//clock is used to time operations. should be increased by an interrupt.
unsigned int del_time; 		//used to time the delay function
extern bit key_flag;


/*Timer Reload Values*/
/*
-CPU clock is at 7.373Mhz, timers clock at half this.
-for timer interrupt frequency of 100, interrupt needs to occur after 36865 clocks
-register must be: 65535-36865 == 28670
-28670 in hex is 0x6FFE
	on reload:
		-TH0 must be loaded with 6F
		-TL0 must be loaded with FE
*/
code unsigned char rld_TH0=0x6F;
code unsigned char rlg_TL0=0xFE;

/*TIMER 0 FUNCTIONS*/
void settim0 ()
{ // --- function to initialise timer 0 ---

  // set timer 0 mode to mode 1 without disturbing timer1's settings
  TMOD &= 0xF0; // bitwise and to maintain upper 4 bits values as-is & force lower 4 to zero
  TMOD |= 0x01;	// bitwise or to maintain upper 4 bits values as-is & force lower 4 to 0001

  //select mode one both times
  TH0 =  rld_TH0; //initialise timer re-load
  TL0 =  rlg_TL0;
  clock = 0;  //initialise my own variable to zero
  ET0 = 1;  //enable timer 0 interrupts
  EA = 1;  //switch on all enabled interrupts
  TR0 = 1;  //start timer 0 running
}

void timer0 () interrupt 1 using 2	  //interrupt 1 is 'Timer 0 Interrupt'(from UM). 'using 2' refers to the scratchpad/cache used by the processor during the processes 
//Interrupt functions of timer_0.
//Freq: 200 Hz  (also copy this to the variable)
//Uses: Used to time delays in the system
{  // --- timer 0 interrupt handling routine ---

	//interrupt only occurs when timer0 overflows
  TR0 = 0;  //stop timer
  TH0 = rld_TH0;  //reload the timer with preset values.
  TL0 = rlg_TL0;
  TR0 = 1;  //re-start timer


	clock ++;  //increment count of overflows
	if (clock > 65534) // in case clock tries to overflow. (this should catch it just before overflow)
   	{
		clock = 0;  //reset count
	}
	del_time ++;
	if (del_time > 65534) // in case del_time tries to overflow. (this should catch it just before overflow)
   	{
		del_time = 0;  //reset count
	}
	
  if(key_flag!=1)  //only scans when previous key press hasn't been accepted.
  	{
		scan();
	}
  
}

void delay_10ms(int time)
{
	char exit=0;
	for(del_time=0;(del_time<(time*(clock_freq/100)))&&exit==0;)	//would typecast equation to float for safety, but can't due to KEIL's Evaluation limitations
	{
		if(key_flag==1)
		{
			exit=1;
			key_flag=0;
		}
	}
}