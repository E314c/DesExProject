void init_door(void);
//initialises ports for the door


void open_door(void);
//Purpose: Opens the door. For our build, this means setting P1.2 high for atleast 3ms (the specified latching time for the relay)


void close_door(void);
//Purpose: Closes the door. For our build, this means setting P1.3 high for atleast 3ms (the specified latching time for the relay)


void door_routine(void);
//This function opens the door and exits when the door is locked
