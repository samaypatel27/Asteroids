// Copyright 2025 Neil Kirby not for distribution without written permission
// Samay Patel has also made changes to this file

// system libraries
#include <stdio.h>
#include <sys/types.h>
#include "structs.h"
#include "debug.h"
#include "libll.h"

// C header files with functions called in this file
#include "rocks.h"
#include "ships.h"

// file specific header file
#include "text.h"

// Print out ship and rock data for a certain timestamp during the sim loop
void master_print(struct Sim * sim_ptr)
{
	if (DEBUG_ENEX) fprintf(stderr, "DEBUG: -> entering %s\n", __func__);
	
        if (TEXT) fprintf(sim_ptr->out,"Time: %7.4f\n", sim_ptr->elapsed_time);
        if (TEXT) rock_iterator(sim_ptr, print_a_rock);
	if (DO_SHIPS){
		fprintf(sim_ptr->out, "\n");
	       	ship_iterator(sim_ptr, print_a_ship);
		// let's print the bullets as well
	}
		
	if (sim_ptr->shot_list != NULL) iterate(sim_ptr->shot_list, print_a_shot);
	
	if (DEBUG_ENEX) fprintf(stderr, "DEBUG: <- exiting %s\n", __func__);
}

