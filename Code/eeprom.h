//Header for EEPROM.c
#define PASS_FLAG 0 //address of "current password" flag
#define PASS_ADDR 1	//start address of passcode data (with regards to eeprom addresses)
#define PASS_1	0x62 	//	flag values to denote which stored
#define PASS_2	0xa9 	//	password to use.(Arbitrary Values)

void pass_read(char *pass);
//Reads the eeprom address  of the passcode and writes the data to given array
//input: pointer to array (RAM)
//output: writes passcode to given array location

void pass_write(char *pass);
//Writes EEPROM address of Passcode with the values passed in array
//input: pointer to array with new passcode
//output: n/a

void eeprom_clear(char val);
//This function is used to clear all of EEPROM to 0's (or 0xFF if you'd prefer)