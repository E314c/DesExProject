//Header for EEPROM.c

char eeprom_read(unsigned char address);
//input: address to be read
//return: data (byte)


void eeprom_write(unsigned char wr_data, unsigned char address);
//input: data (byte) to be written and address to write to
//return: none
	
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