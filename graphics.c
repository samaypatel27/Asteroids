// Copyright (c) 2025 Neil Kirby not for distribution without written permission
// This file has also been edited by Samay Patel

// system libraries
#include <stdio.h>
#include "structs.h"

// custom libraries
#include "n2.h"
#include "rx.h"
#include "debug.h"
#include "constants.h"
#include "libll.h"

// C header files with functions called in this file
#include "ships.h"
#include "rocks.h"

// Constants specific to this file
// Set the graphics to continue displaying for 4 seconds after the sim loop is complete for viewability
#define LAG_TIME 4

#include "graphics.h"

// Draw rocks and ships in on the graphics representing their position during a certain timestamp
void master_draw(struct Sim * sim_ptr)
{
	if (DEBUG_ENEX) fprintf(stderr, "DEBUG: -> entering %s\n", __func__);
	// Clear the screen
	rx_clear(); 
	rx_sim_time(sim_ptr->elapsed_time * 1000);
	
	// draw the rocks
	rock_iterator(sim_ptr, draw_a_rock);
	
	// draw the ships
	if (DO_SHIPS) ship_iterator(sim_ptr, draw_a_ship);
	
	// draw the shots
	iterate(sim_ptr->shot_list, draw_a_shot);	

	rx_refresh();
	microsleep(1000000 * DELTA_T);

	if (DEBUG_ENEX) fprintf(stderr, "DEBUG: <- exiting %s\n", __func__);
}   

// Drawing to occur after the sim loop is complete
void final_draw(struct Sim * sim_ptr)
{
	if (DEBUG_ENEX) fprintf(stderr, "DEBUG: -> entering %s\n", __func__);

	double wait_time = 0.0; // seconds waited so far 

	while(wait_time < LAG_TIME) {
	    master_draw(sim_ptr);
	    wait_time += DELTA_T; // increment the wait time
	}   

	if (DEBUG_ENEX) fprintf(stderr, "DEBUG: <- exiting %s\n", __func__);
}

