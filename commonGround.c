// This file has been created and edited by Samay Patel

// system libraries
#include <stdlib.h>
#include <stdio.h>

// custom libraries
#include "structs.h"
#include "constants.h"
#include "debug.h"
#include "altmem.h"

// file specific header file
#include "commonGround.h"

// This file serves as a header for rocks.c and ships.c, which have common functions used by both rocks and ships below

// apply wrapping if ships and rocks travel outside of the screen after velocities are updated 
void apply_wrapping(struct PandV * pv)
{
        // wrap the position to keep it within bounds
        if (pv->xPos < 0.0) pv->xPos += X_LIMIT;
        if (pv->yPos < 0.0) pv->yPos += Y_LIMIT;
        if (pv->xPos >= X_LIMIT) pv->xPos -= X_LIMIT;
        if (pv->yPos >= Y_LIMIT) pv->yPos -= Y_LIMIT;
}

// print out the distance between two coordinates, comparing either rock to rock or ship to ship
void print_distance_debug(FILE * out, double x1, double y1, double x2, double y2, double center_distance)
{
        fprintf(out, "DEBUG: distance(%.6lf, %.6lf to %.6lf, %.6lf) = %.6lf\n", x1, y2, x2, y2, center_distance);
}


// free shot node data we allocated dynamically
void free_shot(void * data)
{
        // free a single node
        struct Shot * shot = data;
        alternative_free(data);
        shot = NULL;
}

// determines the color based on index value
char * team(int color)
{
        // probably define this static array somewhere else
        char * arr[8] = {"BLACK", "RED", "GREEN", "YELLOW", "BLUE", "MAGENTA", "CYAN", "WHITE"};
        return arr[color];
}

// prints total number of shots cleared from the linkedlist, after deleteSome() is called
void print_cleared_shots(int * clearedShots)
{
        fprintf(stderr, "DIAGNOSTIC: cleared %d shots from list\n", *clearedShots);
}


