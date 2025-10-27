// Copyright (c) 2025 Neil Kirby not for distribution without written permission
// Samay Patel has also made changes to this file

#define _GNU_SOURCE

// standard libraries in C
#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <stdbool.h>

// declared constants in header files
#include "structs.h"
#include "debug.h"

// declared constants specific to this file
#define ROCK_ITEMS 6
#define SHIP_ITEMS 6

// Header files supporting functions declarations that are used
#include "bits.h"
#include "rocks.h"
#include "ships.h"

// file specific header file
#include "input.h"

// clear the comment at the end of each line per input file
static void clear_comment(FILE *fp)
{
        // use getline to clear the comment
        char *line = NULL;
        size_t len = 0;
        ssize_t read;

        if(DEBUG_ENEX) fprintf(stderr, "DEBUG: -> entering %s\n", __func__);

        read = getline(&line, &len, fp);
        if (read == -1) 
	{
		fprintf(stderr, "ERROR: clear_comment failed to read or end of file\n");
        }
	else
        {
	    	if(DEBUG)fprintf(stderr, "DEBUG: clear_comment read %s", line);
        }

        free(line);
        if(DEBUG_ENEX) fprintf(stderr, "DEBUG: <- exiting %s\n", __func__);
        return;
}



// reads in ship data to a temporary ship Struct; passes a pointer to this struct to add the ship to the shipArray
static bool read_a_ship(struct Sim * sim_ptr)
{
        if(DEBUG_ENEX) fprintf(stderr, "DEBUG: -> entering %s\n", __func__);

        struct Ship tempShip = {0};

        if (fscanf(sim_ptr->in, "%i %lf %lf %lf %lf %lf", &tempShip.color, &tempShip.pv.xPos, &tempShip.pv.yPos, &tempShip.pv.xVel, &tempShip.pv.yVel, &tempShip.angle) != SHIP_ITEMS)
        {
                if (DEBUG) fprintf(sim_ptr->out, "Error returning ship data because of incorect number of conversions\n");
                return false;
        }

        // set the sim Pointer to point back to the sim
        tempShip.sim_ptr = sim_ptr;

        // lets get a pointer to the ship so that we only pass the address into the function for better performance, and copying the whole ship struct is inefficient
        struct Ship * ship_ptr = &tempShip;

        // this add_rock function is in the rocks.c file
        if (!(add_a_ship(sim_ptr, ship_ptr))) return false;

        clear_comment(sim_ptr->in);
        if(DEBUG_ENEX) fprintf(stderr, "DEBUG: <- exiting %s\n", __func__);

        return true;
}

// iterator function to call reads on ship data from an input file
static bool read_ships(struct Sim * sim_ptr, int shipsToRead)
{
	if(DEBUG_ENEX) fprintf(stderr, "DEBUG: -> entering %s\n", __func__);

	// ship memory should already be statically allocated upon initalized in the structs.h file
	//if (!(allocate_ship_memory(sim_ptr))) return false;

	// in all the files, after all rock data is read, only ship data is left, so we can read that now
	for (int i = 0; i < shipsToRead; i++)
	{
		if (!(read_a_ship(sim_ptr)))
		{
			if (DEBUG) fprintf(sim_ptr->out, "A particular ship could not be read\n");
			return false;
		}
	}
	
	if(DEBUG_ENEX) fprintf(stderr, "DEBUG: <- exiting %s\n", __func__);
	return true;
}


// reads in rock data to a temporary rock Struct; passes a pointer to this struct to add the rock to the belt
static bool read_a_rock(struct Sim * sim_ptr)
{
        if(DEBUG_ENEX) fprintf(stderr, "DEBUG: -> entering %s\n", __func__);

        // creating a temporary rock Structure to read in Data, then we'll pass in to another method in order to actually add it to the array
        struct Rock tempRock = {0};

        // Read the rock data from the file

        if (fscanf(sim_ptr->in, "%d %lf %lf %lf %lf %lf",
                &tempRock.color, &tempRock.pv.xPos, &tempRock.pv.yPos,
                &tempRock.pv.xVel, &tempRock.pv.yVel, &tempRock.mass)!= ROCK_ITEMS)

        {
                 if (DEBUG) fprintf(sim_ptr->out, "Error reading rock data because of incorrect number of conversions\n");
                 return false;
        }
        // set the sim Pointer to point back to the sim
        tempRock.sim_ptr = sim_ptr;

        // this add_rock function is in the rocks.c file

        // let's pass a pointer to add a rock. only passing an address is more efficent than copying the entire rock struct data
        struct Rock * rock_ptr = &tempRock;
        if (!(add_a_rock(sim_ptr, rock_ptr))) return false;

        clear_comment(sim_ptr->in);
        if(DEBUG_ENEX) fprintf(stderr, "DEBUG: <- exiting %s\n", __func__);

        return true;
}



// iterator function to call reads on rock data from an input file
static bool read_rocks(struct Sim * sim_ptr, int rocksToRead)
{
	if(DEBUG_ENEX) fprintf(stderr, "DEBUG: -> entering %s\n", __func__);

        // Read the rock data from the file
        for (int i = 0; i < rocksToRead; i++)
        {
                // sim_ptr is type pointer to Sim and *sim_ptr is type Sim
            if (!(read_a_rock(sim_ptr)))
            {
                if (DEBUG) fprintf(sim_ptr->out, "A particular rock could not be read\n");
                return false;
            }
        }
	if(DEBUG_ENEX) fprintf(stderr, "DEBUG: <- exiting %s\n", __func__);

        return true;
}


// Read in bits as the first item from the file input
static unsigned int read_bits(struct Sim * sim_ptr)
{
        if(DEBUG_ENEX) fprintf(stderr, "DEBUG: -> entering %s\n", __func__);
        // we don't know how to interpret the bits, but we know that they can't all be zero
        unsigned int bits = 0;

	FILE * fp = sim_ptr->in;

        if (fscanf(fp, "%x", &bits) != 1) 
	{
    		if (DEBUG) fprintf(sim_ptr->out, "ERROR: read_bits failed to read bits\n");
	    	return 0;
        }

        clear_comment(fp);
	if(DEBUG_ENEX) fprintf(stderr, "DEBUG: <- exiting %s with bits = %08X\n", __func__, bits);
        return bits;   
}

// Main input function to decode hexcode to retrieve data form rocks, ships, and time
int master_input(struct Sim * sim_ptr)
{
        if(DEBUG_ENEX) fprintf(stderr, "DEBUG: -> entering %s\n", __func__);
	// this repersents stdin (or the file input)
        if (sim_ptr->in == NULL) 
	{
    		if (DEBUG) fprintf(sim_ptr->out, "ERROR: master_input received a NULL file pointer\n");
	    	return -1;
        }

	// Read the bits from the file pointer. if no bits are read in the file, we return -1
        unsigned int bits = read_bits(sim_ptr);
	if (bits == 0) return -1;
	
	// if the bits read from the file input are not valid bits, then we also return -1
        if(!(validateBits(bits)))
	{
    		if (DEBUG) fprintf(sim_ptr->out, "ERROR: master_input received invalid bits %08X\n", bits);
	    	return -1;
	}
	// Get the number of rocks in the file input by decoding the hex/reading the bits
	unsigned int numRocks = getRocks(bits);
	
	// Get the number of ships in the file input be decoding the hex
	unsigned int numShips;
	if (DO_SHIPS)
	{
	       	numShips = getShips(bits);
	}
	// get the Time by reading the bits
	unsigned int time = getTime(bits);
	// Now that we have rocks and bits, let's update the Sim struct to store these variables
	// going to do value conversion unsigned int -> int
	sim_ptr->time_limit = time;

	// let's not set initial numRocks bc we use it as an index
	
	// we have grasped the bits from the file input, now let's grasp all the rocks
        if(!(read_rocks(sim_ptr, numRocks))) return -1;

	if (DO_SHIPS) 
	{
		if(!(read_ships(sim_ptr, numShips))) return -1;
  	}
        if(DEBUG_ENEX) fprintf(stderr, "DEBUG: <- exiting %s with time = %X\n", __func__, time);
        return time;   
}

