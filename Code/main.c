#include <REG932.H>
//#include "lednum2.h" //is this still needed?
#include "scan.h"
#include "eeprom.h"
#include "LED.h"
#include "definitions.h"
#include "open_door.h"
#include "interupts.h"


/***Global Variables and defines***/


code extern unsigned int clock_freq; 		//defined in 'interupts.c'. defines the frequency(Hz) of increases in global "clock". Used to calc time delays, value in "interrupts.c"
extern unsigned int clock;				//defined in 'interupts.c'. clock is used to time operations. should be increased by an interrupt.
bit key_flag;		 				//flag to show when key has been pressed.
unsigned char key_pressed;					//key pressed.


/***Functions***/

char check_arrays(char *str_1, char *str_2)
//input: 2 arrays
//output: outputs 1 if code correct, else 0
//sys reqs: define PASS_LENGTH (should be in definitions.h);
{
	char flag=0, x;
 
	for(x=0;x<PASS_LENGTH;x++)
	{						
		if(str_1[x]!=str_2[x])
		{flag=1;}
	}
 
	if(flag==0)
		return(1);
	else
		return(0);
}

char code_check(char *code_in)
//input: arrays containing code entered;
//output: outputs 1 if code correct, else 0
//sys reqs: define PASS_LENGTH (should be in definitions.h), define pass_read();

//This is written as a separate function so main doesn't see the stored pass code
{
	char pass[PASS_LENGTH];
 
	pass_read(pass); //read the passcode from EEPROM memory
 
	return(check_arrays(pass,code_in));
}

void display_code(char *str, char entered)
//takes in a passcode string and displays the last 4 digits entered
//input: pointer to char array, 'entered' counting how many numbers have been entered
//output: calls the write() function to send last 4 digits of code to screen.
//pre-reqs: write() function defined, PASS_NULL defined

//Test: PASSED (compiled in DEVC with input/output driver)
{
	char send[4]={PASS_NULL,PASS_NULL,PASS_NULL,PASS_NULL}; //initialises a temp array to all "null"
	
	
	if(entered>3) 						//if atleast 4 characters have been entered, it's not complex to display 
		Write(str+(entered-4)); 	//as the display function will just choose the first 4 chars, send the passcode, but with an appropriate offset 
	else
		{
           switch(entered)			//perhaps a tad complex. Essentially fills the 'send' array from the 4th position, depending on how many characters have been entered. This means the display writes in from left digit.
		    {
             case 3:
                  send[6-entered]=str[2];
             case 2:
                  send[5-entered]=str[1];
             case 1:
                  send[4-entered]=str[0];
                  break;     
             default:
                  Write("err2"); //shouldn't occur, but if so, writes "err2" to the screen, then loops forever.
				  while(1);
                  break;                          
            }
            Write(send);
        }
}

char passcode_entering(char *pass, int idle_time)
//input: address of array to store passcode, idle time in seconds
//returns: 0 if timer interrupt causes exit. KEY_ENTER if 'enter' causes exit. KEY_CHANGE if 'change' causes exit.
//reqs: display_code(), PASS_LENGTH, KEY_ENTER, KEY_CHANGE, clock, clock_freq, key_pressed, key_flag.
{
	int entered=0,loop;
	
	for(clock=0;clock<(idle_time*clock_freq);)
	{
		if(key_flag==1)
		{
			if(key_pressed!=KEY_ENTER && key_pressed!=KEY_CHANGE)  //key is not "enter" and is not "pass change"
			{
			  	entered++;                       
  				if(entered>PASS_LENGTH)
				{
				  	//shuffle all codes down the array
					for(loop=0;loop<PASS_LENGTH;++loop)
						{
							pass[loop]=pass[loop+1];
				  		}
				  	entered=PASS_LENGTH;
				}
				pass[entered-1]=key_pressed;	//add data to the pass string
				display_code(pass,entered);		//display the new string
				
				while(scan()==1); //wait whilst the key is held
				key_flag=0;
			}
		
			if(key_pressed==KEY_ENTER)	//enter has been pressed.
			{
				while(scan()==1); //wait whilst the key is held
				key_flag=0;
				return(KEY_ENTER);
			}
			if(key_pressed==KEY_CHANGE)	//enter has been pressed.
			{
				while(scan()==1); //wait whilst the key is held
				key_flag=0;
				return(KEY_CHANGE);
			}
		}
	}
	return(0);//timer caused exit, return 0.
}




//MAIN function
void main (void)
{
	/*Variable Declaration*/
	char pass[PASS_LENGTH], x; 	//pass is the pass entered. 'x' stores temp data
	int loop; 					//variable for loops
	char attempts, entered;		//amount of passcode attempts. amount of data entered (when entering passcode)
	
	/*Setup*/
	{
	DisplayInit();
	settim0 ();
	init_scan();
	init_door();
		/*This section may cause issues on DEV board as it's not configured to hold P1.4 low during normal operation*/
		P1M1|=0x10;
		P1M2&=0xEF; //sets P1.4 to '10'(input mode)
		if((P1&0x10)==0x10) //if P1.4 is held high (Hardware override to wipe eeprom)
		{
			eeprom_clear(0);
			Write("done");
			while(1); //infinite loop, Hardware must be reset again.
		}
		//*************************************************************************************************************/
		
	for(loop=0;loop<PASS_LENGTH;++loop)
	{
		pass[loop]=PASS_NULL; //sets the passcode to PASS_NULL
	}
	entered=0;
	attempts=0;
	Write("init");
	//Should I shut the door here? Spec doesn't Spec and I would assume so.
	}
	
	
	//infinite loop begins
	while(1)
	{
		/*--idling state.--*/
		
		x=passcode_entering(pass,30);	//store data to pass, 30seconds till idle
		
		if(x==KEY_ENTER)	//enter has been pressed.
		{
			while(scan()==1)
			key_flag=0;
			//code check portion
			if (code_check(pass)==1)
			{
				
				//passcode is correct
				//enter open routine
				door_routine();
				//should include passcode change section.
				
				
				//clear passcode, ready for idling
				for(loop=0;loop<PASS_LENGTH;++loop)
				{
					pass[loop]=PASS_NULL; 			//clears stored passcode
				}
				entered=0;
				attempts=0;
				Write("shut");
			}
			else 
			{
				//passcode incorrect
				++attempts;
				if(attempts>=3) //third fail will lock door down
				{
					Write("Bar ");
					
					for(clock=0;clock<(120*clock_freq);) 
						{/*enter three minute loop*/}
					
					attempts=0;	//reset attempts to 0
				}
				for(loop=0;loop<PASS_LENGTH;++loop)
				{
					pass[loop]=PASS_NULL; //clears stored passcode
				}
				Write("shut");
			}
		}//end of 'if(x==KEY_ENTER)'
		
		//clears stored passcode
		for(loop=0;loop<PASS_LENGTH;++loop)
		{
			pass[loop]=PASS_NULL; 
		}
		Write("shut");
		
		if(x==0) //display
			{
				attempts=0; //clears attempts if passcode entering times out
			}
		
	}
	//end of while(1) loop

	/*Error catching Area*/
	Write("Err1");
	while(1);
	/*********************/
}
