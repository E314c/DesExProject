//main.h

char check_arrays(char *str_1, char *str_2);
//input: 2 arrays
//output: outputs 1 if code correct, else 0
//sys reqs: define PASS_LENGTH (should be in definitions.h);

//for the lock system, this function should only be called when enter is pressed.


char code_check(char *code_in);
//input: arrays containing code entered;
//output: outputs 1 if code correct, else 0
//sys reqs: define PASS_LENGTH (should be in definitions.h), define pass_read();

//This is written as a separate function so main doesn't see the stored pass code


void display_code(char *str, char entered);
//takes in a passcode string and displays the last 4 digits entered
//input: pointer to char array, 'entered' counting how many numbers have been entered
//output: calls the write() function to send last 4 digits of code to screen.
//pre-reqs: write() function defined, PASS_NULL defined

//Test: PASSED (compiled in DEVC with input/output driver)


char passcode_entering(char *pass, int idle_time);
//input: address of array to store passcode, idle time in seconds
//returns: 0 if timer interrupt causes exit. KEY_ENTER if enter causes exit. KEY_CHANGE if change causes exit.

