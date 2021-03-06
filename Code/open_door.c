//this file contains all functionality for when the door is opened.

#include <REG932.H>
#include "definitions.h"
#include "eeprom.h"
#include "main.h"
#include "LED.h"
#include "scan.h"
#include "interupts.h"


//Global Variables
extern bit key_flag;		 				//declared in main //flag to show when key has been pressed.
extern char key_pressed;					//declared in main //key pressed.
code extern unsigned int clock_freq; 		//defined in 'interupts.c'. defines the frequency(Hz) of increases in global "clock". Used to calc time delays, value in "interrupts.c"
extern unsigned int clock;				//defined in 'interupts.c'. clock is used to time operations. should be increased by an interrupt.

void init_door(void)
//initialises ports for the door
{
	P1M1&=0xF3;
	P1M2&=0xF3; //sets Port1 pins .2 and .3 to '00'(Quasi-bidirectional)
	P1&=0xF3;	//sets them to start low
}

void open_door(void)
//Purpose: Opens the door. For our build, this means setting P1.2 high for atleast 3ms (the specified latching time for the relay)
{
	P1|=0x04; 		//set P1.2 high
	delay_10ms(1); 	//wait 10ms
	P1&=0xFB; 		//set P1.2 low
}

void close_door(void)
//Purpose: Closes the door. For our build, this means setting P1.3 high for atleast 3ms (the specified latching time for the relay)
{
	P1|=0x08; 		//set P1.3 high
	delay_10ms(1); 	//wait 10ms
	P1&=0xF7;		//set P1.3 Low	
}

void door_routine(void)
//This function opens the door and exits when the door is locked
{
	//variables for this function
	char pass[PASS_LENGTH],pass_2[PASS_LENGTH];
	int x,y,exit=0,loop; //x,y,temp are used to check return values from passcode_func. exit is used to hold infinite loop till door closed
	
	//initialisation
	for(loop=0;loop<PASS_LENGTH;++loop)
	{
		pass[loop]=PASS_NULL;
		pass_2[loop]=PASS_NULL; 
	}
	
	//first, open the door
	open_door();
	Write("OPEN");

	while(exit!=1) //function will only exit when door closed
	{
		if(key_flag==1)
		{
			if(key_pressed==KEY_CHANGE)
			{
				while(scan()==1); //debounce
				key_flag=0;
				Write("Chan"); 		//indicate that they've entered pass change mode
				delay_10ms(150); 	//delay before blanking screen
				Write("    ");
				
				//passcode change mode
				x=passcode_entering(pass,30);
				if(x==KEY_ENTER) //only continue if passcode_func exited due to enter being pressed
				{
					Write("    ");
					y=passcode_entering(pass_2,30);
					if(y==KEY_ENTER)
					{
						if(check_arrays(pass,pass_2))
						{
							pass_write(pass);
							Write("done"); 		//say 'done to indicate password changed.
						}
						else
						{
							Write("Err ");
						}
						delay_10ms(150); //1.5 second delay   >>Could change this to "wait for enter pressed."
					}
				}
				
				//clear values stored
				for(loop=0;loop<PASS_LENGTH;++loop)
				{
					pass[loop]=PASS_NULL;
					pass_2[loop]=PASS_NULL;
				}
				Write("OPEN");
			
			}
			
			if(key_pressed==KEY_ENTER)
			{
				while(scan()==1); //debounce
				key_flag=0;
				
				//now close the door
				close_door();
				Write("Shut");
				exit=1; //door has been closed, function can now exit.
			}
			
			if(key_pressed!=KEY_ENTER&&key_pressed!=KEY_CHANGE) //just to handle/clear random numeric key presses whilst door is open
			{
				while(scan()==1); //debounce
				key_flag=0;
			}
		}
	}//end of while loop
}