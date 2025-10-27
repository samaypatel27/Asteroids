// Copyright (c) 2025 Neil Kirby not for distribution without written permission
// Samay Patel has also made changes to this file

// system libraries
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

// custom libraries
#include "structs.h"
#include "debug.h"
#include "constants.h"
#include "rx.h"

// C header files with functions called in this file
#include "text.h"
#include "graphics.h"

// file specific header file
#include "output.h"

// ensure that the text is ok to print by being in DEBUG mode
/*static bool ok_to_print(struct Sim * sim_ptr)
{
	return(DEBUG);
}*/

// main output stream while the sim loop is running, in charge of printing out text or graphics
void master_output(struct Sim * sim_ptr)
{
	if (DEBUG_ENEX) fprintf(stderr, "DEBUG: -> entering %s\n", __func__);

	if(TEXT) master_print(sim_ptr);
	
	if(GRAPHICS) master_draw(sim_ptr);

	if (DEBUG_ENEX) fprintf(stderr, "DEBUG: <- exiting %s\n", __func__);
}

// output to occur after the sim loop is done running
void final_output(struct Sim * sim_ptr)
{
	if (DEBUG_ENEX) fprintf(stderr, "DEBUG: -> entering %s\n", __func__);

	// output the final state of the game
	if(TEXT)master_print(sim_ptr);

	if(GRAPHICS) final_draw(sim_ptr);

	if (DEBUG_ENEX) fprintf(stderr, "DEBUG: <- exiting %s\n", __func__);
}




