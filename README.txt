DesExProject
============

Design Exercise Codes (Door Security module)

This code is primarily for the operation of a door lock system using a Phillips P89LPC932 micro controller, under a specification given later in the read-me.


========
CONTENTS
========
1.Pin out of design

2.Specification
	a.Hardware spec
	b.software spec
	
3.List of functions
	a.Main
		i	check_arrays(char *str_1, char *str_2)
		ii	code_check(char *code_in);
		iii	display_code(char *str, char entered);
		iv	passcode_entering(char *pass, int idle_time);
	b.

4.




====================
1.Pin Out of Design
====================
Port0: 
	.0-.2	Matrix keyboard columns
	.3-.6	Matrix keyboard rows

Port1:
	.0 .1 .6 .7		7-Segment common cathodes (for display digit selection)

Port2:
	.0-.7	7-Segment: segment selection (A-G)
	
Port3:
	.0 .1	Crystal Clock circuit.
	

====================
2.Specification
	a.Hardware spec
====================


====================
2.Specification
	b.software spec
====================

