// Samay Patel has created and edited this file

// System Libraries
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// C header files with functions called in this file
#include "debug.h"

// declared constants specific to this file 
#define TIME_WIDTH 10
#define TIME_SHIFT 16

#define ROCK_WIDTH 5
#define ROCK_SHIFT 8

#define SHIP_WIDTH 3
#define SHIP_SHIFT 4

// file specific header file
#include "bits.h"

// Unmask the bits and obtain the time by shifting the bits
unsigned int getTime(unsigned int hex)
{
	 if (DEBUG_ENEX) fprintf(stderr, "DEBUG: -> entering %s\n", __func__);

         int width = 10;
         int shift = 16;
         // creates a mask that only has the bits marked as 1 at the appropriate
         int mask = ((1 << TIME_WIDTH) - 1) << TIME_SHIFT;
         // with the mask, we can use the & operator to get the values marked
         //  as 1 by shifting right to the 16, giving us the answer
         unsigned int time = (hex & mask) >> TIME_SHIFT;
	
	 if (DEBUG_ENEX) fprintf(stderr, "DEBUG: <- leaving %s\n", __func__);


         return time;
}

// Unmask the bits and obtain the number of rocks by shifting the bits
unsigned int getRocks(unsigned int hex)
{	
	if (DEBUG_ENEX) fprintf(stderr, "DEBUG: -> entering %s\n", __func__);


         int mask = ((1 << ROCK_WIDTH) - 1) << ROCK_SHIFT;
         unsigned int rocks = (hex & mask) >> ROCK_SHIFT;
	if (DEBUG_ENEX) fprintf(stderr, "DEBUG: <- leaving %s\n", __func__);


         return rocks;
}

// Unmask the bits and obtain the number of ships by shifting the bits
unsigned int getShips(unsigned int hex)
{
	if (DEBUG_ENEX) fprintf(stderr, "DEBUG: -> entering %s\n", __func__);
        
	int mask = ((1 << SHIP_WIDTH) - 1) << SHIP_SHIFT;
        unsigned int ships  = (hex & mask) >> SHIP_SHIFT;

	if (DEBUG_ENEX) fprintf(stderr, "DEBUG: <- leaving %s\n", __func__);

        return ships;
}

// ensure the bits are valid by checking that there are an even number of 1s in the binary version of the hex
bool validateBits(unsigned int bits)
{
	bool validBits = true;
	if (DEBUG_ENEX) fprintf(stderr, "DEBUG: -> entering %s\n", __func__);

	int numberOfOnes = 0;
	// edge case: if there are no bits to check, we automatically return false
	if (!(bits)) validBits = false;
	while (bits)
        {
                if (bits & 1)
                {
                        numberOfOnes += 1;
                        
                }
		bits >>= 1;
        }
        if( numberOfOnes % 2 != 0) validBits = false;

	if (DEBUG_ENEX) fprintf(stderr, "DEBUG: <- leaving  %s\n", __func__);

	return validBits;
}
