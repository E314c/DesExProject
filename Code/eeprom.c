//This will have functions for reading/writing from EEprom for the password and display functions
//fully tested and satisfactory performance


/*Global Variable and Defines*/
#include <REG932.H>
#include "definitions.h" 


// need to ask if there's a way to set up eeprom when flashing chip

#define PASS_FLAG 0 //address of "current password" flag
#define PASS_ADDR 1	//start address of passcode data (with regards to eeprom addresses)
#define PASS_1	0x62 	//	flag values to denote which stored
#define PASS_2	0xa9 	//	password to use.(Arbitrary Values)

char eeprom_read(unsigned char address)
//input: address to be read
//return: data
{
	DEECON &= 0x00;					//set DEECON register to all 0's. LSB is EADR8, so if address >128, will need to be set to 1
	DEEADR=address;					// read from pass_flag address
	while((0x80&DEECON)!=0x80);		//waits until read has completed.
	return(DEEDAT);					// data from pass_flag does into flag variable
}

void eeprom_write(unsigned char wr_data, unsigned char address)
{
	DEECON &= 0x00;					//set DEECON register to all 0's. LSB is EADR8, so if address >128, will need to be set to 1
	DEEDAT=wr_data;					//writes in character for given passcode position
	DEEADR=address;					//Writes address of position in passcode_2
	while((0x80&DEECON)!=0x80);	 	//waits until write has completed.
}




void pass_read(char *pass)
//Reads the eeprom address  of the passcode and writes the data to given array
//input: pointer to array (RAM)
//output: writes passcode to given RAM location
{
    unsigned char loop, flag;

	flag=eeprom_read(PASS_FLAG);		// data from pass_flag does into flag variable
	
    
	if(flag==PASS_1)
	{
	    for(loop=0;loop<PASS_LENGTH;loop++) //loops through all values of passcode (8 characters);
	    {
			pass[loop]=eeprom_read(PASS_ADDR+loop);
	    }
	}
	if(flag==PASS_2)
	{
		for(loop=0;loop<PASS_LENGTH;loop++) //loops through all values of passcode (8 characters);
	    {
			pass[loop]=eeprom_read(PASS_ADDR+loop+PASS_LENGTH);	//sets DEEADR with passcode_2[loop] address. Passcode should then be present in DEEDAT SFR.
	    }
	}
    if((flag!=PASS_1)&&(flag!=PASS_2))		//if the flag value is not set to 1 or 2, use default '01234567' passcode
	{
		for(loop=0;loop<PASS_LENGTH;loop++) //loops through all values of passcode (8 characters);
	    {
			pass[loop]=loop;
	    }
	}
}


void pass_write(char *pass)
//Writes EEPROM address of Passcode with the values passed in array
//input: pointer to array with new passcode
//output: n/a
{
    unsigned char loop, flag;

	flag=eeprom_read(PASS_FLAG);		// data from pass_flag does into flag variable

	if(flag==PASS_1)
	{
		//write the password in
		for(loop=0;loop<PASS_LENGTH;loop++) //loops through all values of passcode (8 characters);
			{
				eeprom_write(pass[loop],PASS_ADDR+PASS_LENGTH+loop); //writes to positions of pass_2
			}
	
		//write flag to new appropriate value.
		eeprom_write(PASS_2, PASS_FLAG); //assign flag to flag 2 now.
	}
	else //this means that passcode will write into pos1 for cur_flag=PASS_2 and for initial startup when flag might be anything.
	{
		//write the password in
	    for(loop=0;loop<PASS_LENGTH;loop++) //loops through all values of passcode (8 characters);
			{
				eeprom_write(pass[loop],PASS_ADDR+loop); //writes to positions of pass_1

			}
	
		//write flag to new appropriate value.
	
		eeprom_write(PASS_1, PASS_FLAG); //assign flag to flag1 now. 

	}
}



void eeprom_clear(char val)
//This function is used to clear all of EEPROM to 'val' (recommend 0 or 0xFF )
{
	EA=0;							//disable interrupts
	DEECON &= 0x00;					//set DEECON register to all 0's.
	DEECON |= 0x31;					//sets DEECON to 00110001 for block fill
	DEEDAT = val;						// block fill with 'val' (best to clear to 0's or 'FF'
	DEEADR=0xFF;					//can be any address, doesn't matter. Chip ignores when in block fill, just needs it to start operation
	while((0x80&DEECON)!=0x80);	 	//waits until write has completed.
	EA=1;							//Enable interrupts
}

