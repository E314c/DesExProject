DesExProject
============

Design Exercise Codes (Door Security module)

This code is primarily for the operation of a door lock system using a Phillips P89LPC932 micro controller, under a specification given later in the read-me.


=========
CONTENTS
=========
1.Pin out of design

2.Specification
	a.Hardware spec
	b.software spec
	
3.List of functions

4.List of Definitions

5.List of Global Variables




====================
1.Pin Out of Design
====================
Port0: 
	.0-.2	Matrix keyboard columns
	.3-.6	Matrix keyboard rows
	.7		Door unlock port (held high whilst door is open)

Port1:
	.0 .1 .6 .7		7-Segment common cathodes (for display digit selection)
	.4				Enables EEPROM_CLEAR on system start. Should be held low during normal operation
	.2 .3			Door open/close via transistor H-bridge and latching relay

Port2:
	.0-.7	7-Segment: segment selection (A-G)
	
	
Port3:
	.0 .1	Crystal Clock circuit.
	

	
====================
2.Specification
	a.Hardware spec
====================
>>>



====================
2.Specification
	b.software spec
====================
>>>



====================
3.List of functions
====================

	a)main
		i	check_arrays
		ii	code_check
		iii	display_code
		iv	passcode_entering
		
	b)eeprom
		i	eeprom_read
		ii	eeprom_write
		iii	pass_read
		iv	pass_write
		v	eeprom_clear
		
		
	c)scan
		i	init_scan
		ii	scan
		
	d)interrupts
		i	settim0
		ii	timer0
		iii	delay_10ms
	
	e)LED
		i	Write
		ii	DisplayInit
	
	f)open_door
		i	init_door
		ii	open_door
		iii	close_door
		iv	door_routine

(details of function arguments and returns can be found in the appropriate .h and .c files.)


======================
4.List of Definitions
======================
PASS_LENGTH
	Defines length of passcode to be used. Given current coding, can be set between 1-125.
	Defined in: definitions.h
	Current Value: 8 (from specification)
	
KEY_ENTER
	Value assigned with the "Enter" key is pressed. Arbitrary value
	Defined in: definitions.h
	Current Value: 10
	
KEY_CHANGE
	Value assigned with the "Change" key is pressed. Arbitrary value
	Defined in: definitions.h
	Current Value: 13
	
PASS_NULL
	Null value for passcode arrays.
	Defined in: definitions.h
	Current Value: 32 (ascii for space character)
	
PASS_FLAG
	EEPROM address of pass_flag data
	Defined in: eeprom.c
	Current Value: 0 (pass_flag occupies the first byte of eeprom)
	
PASS_ADDR
	Start of EEPROM addresses for passcode data.
	Defined in: eeprom.c
	Current Value: 1 (starts after pass_flag)
	
PASS_1
	Flag value indicating pass_1 is the 'live' passcode. Arbitrary value
	Defined in: eeprom.c
	Current Value: 0x62
	
PASS_2
	Flag value indicating pass_2 is the 'live' passcode. Arbitrary value
	Defined in: eeprom.c
	Current Value: 0xA9
	
NO_KEY
	value used in scan() to determine that no key has been pressed this scan cycle.
	Defined in: scan.c
	Current Value: 0xFF

TBL_LENGTH
	length of table for ascii->LED conversions
	Defined in: LED.c
	Current Value: 37



===========================
5.List of Global Variables
===========================
clock_freq
	type:	code unsigned int 
	purpose:	This should be set to the frequency of timer0 interrupts. 
				It allows other time critical functions to maintain their
				timing while the timer0 can be changed.

clock
	type:	unsigned int
	purpose:	this counts +1 for every interrupt of timer0.
	
	
key_flag;
	type:	bit
	purpose: 	set to 1 when a key press is detected. Should be set to '0' by 
				software when key press has been processed.
	
display[4]
	type:	char
	purpose:	array to store values for current display.
				
key_pressed
	type:	unsigned char
	purpose:	stores the value of the key press detected.
	







=====
END
=====