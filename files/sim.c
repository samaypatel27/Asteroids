// Copyright (c) 2025 Neil Kirby not for distribution without written permission
// Samay Patel has also made changes to this file

// system libraries
#include <stdio.h>
#include <stdbool.h>

// custom libraries
#include "structs.h"
#include "constants.h"
#include "debug.h"
// remove this after bc we should have it in ships.c
#include "linkedlist.h"

// C header files with functions called in this file
#include "ships.h"
#include "rocks.h"
#include "input.h"
#include "output.h"
#include "commonGround.h"

// file specific header file
#include "sim.h"


// check if the elapsed time exceeds the time limit
static bool timed_out(struct Sim * sim_ptr)
{
	if (DEBUG_ENEX) fprintf(stderr, "DEBUG: -> entering %s\n", __func__);

	if (sim_ptr->elapsed_time >= sim_ptr->time_limit) 
	{
		if (DEBUG_ENEX) fprintf(stderr, "DEBUG: <- exiting %s with true\n", __func__);
	    	return true;
	}

	if (DEBUG_ENEX) fprintf(stderr, "DEBUG: <- exiting %s with false\n", __func__);
	
	// if we return false, The game still continues as we are under the time limit
	return false;
}
// check if there are any rocks, in order to prevent the game from running for an elapsed time with nothing to display or print
static bool no_rocks(struct Sim * sim_ptr)
{
	if (DEBUG_ENEX) fprintf(stderr, "DEBUG: -> entering %s\n", __func__);

	if (sim_ptr->belt->numRocks == 0)
	{
		if (DEBUG) fprintf(sim_ptr->out, "ERROR: There are no rocks present\n");
		return true;
	}
	return false;
}

// check if the game is still allowed to run
static bool game_is_on(struct Sim * sim_ptr)
{
	if (DEBUG_ENEX) fprintf(stderr, "DEBUG: -> entering %s\n", __func__);

	if(timed_out(sim_ptr) || no_rocks(sim_ptr))
	{
		if (DEBUG_ENEX) fprintf(stderr, "DEBUG: <- exiting %s with false\n", __func__);
		
		// If timed out, let's return false and therefore the game is over/the simulation should be stopped
	    	return false;
	}

	if (DEBUG_ENEX) fprintf(stderr, "DEBUG: <- exiting %s with true\n", __func__);
	return true;
}

// simulate the motion for rocks and ships
static void move_stuff(struct Sim * sim_ptr)
{
	if (DEBUG_ENEX) fprintf(stderr, "DEBUG: -> entering %s\n", __func__);

	rock_iterator(sim_ptr, move_a_rock);
	if (DO_SHIPS) ship_iterator(sim_ptr, move_a_ship);
	
	iterate(sim_ptr->shot_list, move_a_shot);	

	if (DEBUG_ENEX) fprintf(stderr, "DEBUG: <- exiting %s\n", __func__);
}


// clear memory for everything that is dynamically allocated (currently just rocks as of now)
static void call_cleanup(struct Sim * sim_ptr)
{
	if (DEBUG_ENEX) fprintf(stderr, "DEBUG: -> entering %s\n", __func__);

	// clear the array of rocks
	rock_cleanup(sim_ptr);

	// let's clear the linked list
	
	int deletedShots = deleteSome(&(sim_ptr->shot_list), always_true , NULL, free_shot);

	print_cleared_shots(&deletedShots);

	if (DEBUG_ENEX) fprintf(stderr, "DEBUG: -> entering %s\n", __func__);

}

// if DO_BUMPS is turned on in debug.h, bump checks will occur for collision detection
static void do_bump_checks(struct Sim * sim_ptr)
{
	// ship to rock collision checker
	if (DO_SHIPS) ship_iterator(sim_ptr, ship_to_rock_collision);

	// rock to rock collision checker
        all_rocks_bump_check(sim_ptr);

	// rock to shot checker
	if (DO_BONUS) shot_to_rock_bumps_check(sim_ptr);
}

// TODO: actually call the method in ships.c
static void manage_shots(struct Sim * sim_ptr)
{
	if (sim_ptr->shot_list != NULL)
	{
		// let's check if the shots are out of bounds
		deleteSome(&(sim_ptr->shot_list), out_of_bounds, NULL, free_shot);
		return;
	}

	// create the shots if the linked list is empty
		
	ship_iterator(sim_ptr, create_shot);	

	// output the ship firing all of the shots
		
}

// simulate the motion of ships and rocks while within the time limit
static void sim_loop(struct Sim * sim_ptr)
{
	if (DEBUG_ENEX) fprintf(stderr, "DEBUG: -> entering %s\n", __func__);
	
	// Let's keep track of elapsed time as a member of the Sim Struct
	while(game_is_on(sim_ptr))
	{
		// increase the time of the game at the start
		// Print out ships, rocks, and shots
	    	master_output(sim_ptr);
		
		manage_shots(sim_ptr);
		
		sim_ptr->elapsed_time += DELTA_T;
		// simulate motion of ships and rocks
	    	move_stuff(sim_ptr);

		fprintf(sim_ptr->out, "\n");

		// enable collision detection and effects as a result of those collisions
	    	if (DO_BUMPS) do_bump_checks(sim_ptr);
	}
	// final output when time is up
	final_output(sim_ptr);

	// clear memory allocated for ships and rock array
	call_cleanup(sim_ptr);
	if (DEBUG_ENEX) fprintf(stderr, "DEBUG: <- exiting %s\n", __func__);
}

static void close_input(FILE * inputPtr)
{
        if (fclose(inputPtr) != 0) fprintf(stdout, "ERROR: Failed to close input file\n");
        else {
                fprintf(stderr, "DIAGNOSTIC: Input file closed\n");
        }
}

static void close_output(FILE * outputPtr)
{
        if (fclose(outputPtr) != 0) fprintf(stdout, "ERROR: Failed to close input file\n");
        else {
                fprintf(stderr, "DIAGNOSTIC: Output file closed\n");
        }
}

static FILE * open_input(char * inputFile)
{
	FILE * ptr = fopen(inputFile, "r");
	if (ptr == NULL)
	{
                fprintf(stderr, "ERROR: Unable to open %s for reading\n", inputFile);
                return NULL;
        }
        fprintf(stderr, "DIAGNOSTIC: Successfully opened %s for reading\n", inputFile);
	return ptr;

}

static FILE * open_output(struct Sim * sim_ptr, char * outputFile)
{
	FILE * ptr = fopen(outputFile, "w+");

        if (outputFile == NULL)
        {
                fprintf(stderr, "ERROR: Unable to open %s for writing\n", outputFile);
                close_input(sim_ptr->in);
                return NULL;
        }
        fprintf(stderr, "DIAGNOSTIC: Successfully opened %s for writing\n", outputFile);
	return ptr;
}

// main function to complete all actions for ships and rocks
bool do_everything(char * inputFile, char * outputFile)
{
	if (DEBUG_ENEX) fprintf(stderr, "DEBUG: -> entering %s\n", __func__);

	// Automatically sets all members of the structs to 0 to initialize (except pointers which are set to NULL)
	struct Sim sim = {0};
	struct Belt belt = {0};

	struct Sim * sim_ptr = &sim;
	struct Belt * belt_ptr = &belt;

	// Assign the belt pointer as a member of Sim
	sim_ptr->belt = belt_ptr;

	// NEW: let's open the input and the output files

	sim_ptr->in = open_input(inputFile);
	if (sim_ptr->in == NULL) return false;

	sim_ptr->out = open_output(sim_ptr, outputFile);
	if (sim_ptr->out == NULL) return false;

	// NEW: let's declare the linked list as null
	sim_ptr->shot_list = NULL;

	double time_limit = master_input(sim_ptr);
	
	// Now let's assign the time limit to the sim pointer
	sim_ptr->time_limit = time_limit;

	// Now let's call our final function, the sim_loop	
	if (time_limit > 0.0) sim_loop(sim_ptr);
	else
        {
            if (DEBUG) fprintf(sim_ptr->out, "ERROR: Input failed or invalid time limit.\n");
            return false; 
        }
        if (DEBUG_ENEX) fprintf(stderr, "DEBUG: <- exiting %s\n", __func__);
	
	// now let's close the input and output files
	

	return true;
}

