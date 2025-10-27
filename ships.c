// Samay Patel created and edited this file

// system libraries
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>

// custom libraries
#include "structs.h"
#include "constants.h"
#include "debug.h"
#include "rx.h"
#include "altmem.h"
#include "libll.h"

// C header files with functions called in this file
#include "physics.h"
#include "commonGround.h"

// constants specific to this file
#define TEAM_COUNT 8
#define SCORE_INCREMENT 10
#define HALF_CIRCLE 180.0

// file specific header file
#include "ships.h"

// iterate through all consecutive memory spaces that hold ship Structs; the second parameter is a pointer to a function that takes a pointer to a struct Ship as a parameter
void ship_iterator(struct Sim * sim_ptr, void (*callback) (struct Ship *))
{
	if(DEBUG_ENEX) fprintf(stderr, "DEBUG: -> entering %s\n", __func__);

	for (int i = 1; i < TEAM_COUNT; i++)
	{
		// return if no ship is present at that particular subscript
		// earlier, we allocated 7 spaces in the memory. In C, when you allocate memory, the values at each spot default to 0. so we can just see if the color is 0, which is what the lab manual says to do
		if (sim_ptr->shipArray[i].color == 0) continue;
		
		callback(&(sim_ptr->shipArray[i]));
	}

	if(DEBUG_ENEX) fprintf(stderr, "DEBUG: <- exiting %s\n", __func__);

}

// adds a ship to a consecutive space in memory
bool add_a_ship(struct Sim * sim_ptr, struct Ship * ship)
{
	if(DEBUG_ENEX) fprintf(stderr, "DEBUG: -> entering %s\n", __func__);
	
	if (ship->color <= 0 || ship->color >= TEAM_COUNT)
	{
		if (DEBUG) fprintf(sim_ptr->out, "Error in adding ship, color does not meet requirements\n");
		return false;	
	}
	// the color determines the subscript. we passed as a ship pointer but lets store as just a regular ship by dereferencing
	sim_ptr->shipArray[ship->color] = *ship;
	
	if(DEBUG_ENEX) fprintf(stderr, "DEBUG: <- exiting %s\n", __func__);
	return true;

}

// prints the members of a ship Struct, detailing information about it at a particular elapsed time
void print_a_ship(struct Ship * ship)
{
	if(DEBUG_ENEX) fprintf(stderr, "DEBUG: -> entering %s\n", __func__);

	// We never read in a score value, but it is 0 by default when they are intialized as struct Ship ship = {0} which we did
	FILE * out = ship->sim_ptr->out;
	if (TEXT) fprintf(out, "%7i Ship at (%7.4lf, %7.4lf), Velocity: (%6.2lf, %6.2lf), Angle: %3.0i Score: %3i\n", ship->color, ship->pv.xPos, ship->pv.yPos, ship->pv.xVel, ship->pv.yVel, (int)ship->angle, ship->score);

	if(DEBUG_ENEX) fprintf(stderr, "DEBUG: <- exiting %s\n", __func__);
}

// draws the ship to the graphics at a particular elapsed time
void draw_a_ship(struct Ship * ship)
{
	if(DEBUG_ENEX) fprintf(stderr, "DEBUG: -> entering %s\n", __func__);

	rx_ship((int) ship->color, ship->score, ship->pv.xPos, ship->pv.yPos, ship->angle);
	
	if(DEBUG_ENEX) fprintf(stderr, "DEBUG: <- exiting  %s\n", __func__);

}

// simulates the movement of a ship by calculating the new position
void move_a_ship(struct Ship * ship)
{
	if(DEBUG_ENEX) fprintf(stderr, "DEBUG: -> entering %s\n", __func__);

	// pointer to struct of PandV for easy reference to memory address location
	struct PandV * pv = &(ship->pv);

	pv->xPos += pv->xVel * DELTA_T;
	pv->yPos += pv->yVel * DELTA_T;

	apply_wrapping(pv);

	if(DEBUG_ENEX) fprintf(stderr, "DEBUG: -> entering %s\n", __func__);

}
static void crunch_message( struct Ship *ship, struct Rock *rock)
{
	if (DEBUG_ENEX) fprintf(stderr, "DEBUG: -> entering %s\n", __func__);

	if(TEXT)
	{
	    FILE *fp = ship->sim_ptr->out;
	    fprintf(fp, "CRUNCH! %s ship at (%.4lf, %.4lf) hits %s rock at (%.4lf, %.4lf)\n",
		team(ship->color), ship->pv.xPos, ship->pv.yPos,
		team(rock->color), rock->pv.xPos, rock->pv.yPos);
	}
	if(GRAPHICS)
	{
	    char buffer[100];
	    sprintf(buffer, "CRUNCH! %s ship at (%.0lf, %.0lf) hits %s rock at (%.0lf, %.0lf)",
		team(ship->color), ship->pv.xPos, ship->pv.yPos,
		team(rock->color), rock->pv.xPos, rock->pv.yPos);
	    rx_status(buffer);
	}

	if (DEBUG_ENEX) fprintf(stderr, "DEBUG: <- exiting %s\n", __func__);
}

// function called by ship iterator to determines if a ship deserved to be crunched by a rock
void ship_to_rock_collision(struct Ship * ship)
{
	// get a reference to the array in order to loop through and check against all rocks
	int numRocks = ship->sim_ptr->belt->numRocks;
	// the rock array is already an address of type struct Rock * array, and the individual elements are struct Rock rock (no pointer). No need for & to get address of something that's already a pointer
	struct Rock * array = ship->sim_ptr->belt->array;	
	for (int i = 0; i < numRocks; i++)
	{
		struct Rock * rock = &(array[i]);
		double rock_radius = rock->mass;

		double x1 = rock->pv.xPos;
		double y1 = rock->pv.yPos;

		double x2 = ship->pv.xPos;
		double y2 = ship->pv.yPos;

		double center_distance = distance(x1, y1, x2, y2);

		if (DEBUG) print_distance_debug(ship->sim_ptr->out, x1, y2, x2, y2, center_distance);

		// If this condition is met, then the rock and ship are not colliding, and we don't need to take account for any collisions
		if (center_distance > rock_radius) return;	
		crunch_message(ship, rock);	
	

		// Now that the ship and rock collided we should take it out of the game by setting the ship color to 0. That way, in the ship iterator it won't notice the ship at all. Reminder that the ship array struct defaults to 0 anyway so it will be treated as if there are no ships there
		ship->color = 0;
		// Now let's update the score. This ship is destroyed, but if there is another ship of the same color (color represents subscripts) we can award points to that surviving ship. So you can get points as long as another ship has the same color and is still alive.
		
		ship->sim_ptr->shipArray[rock->color].score += SCORE_INCREMENT;
	}
	
			
}

// compares x Positions of two structs with PandV properties
bool compare_xPos(void * data1, void * data2)
{
        // the data is a pointer to a new node of type struct Shot
        // cannot dereference void pointers(when using the -> operator) so we make it automatically cast to a shot struct
        struct Shot * shot1 = data1;
        struct Shot * shot2 = data2;

        return (shot1->pv.xPos < shot2->pv.xPos);
}

// adds a shot to the dynamically allocated linkedlist
static void add_a_shot(struct Shot * shot)
{
	// now let's attempt to add the shot into the linked list

	// this shouldn't fail if using the linked list library
        if (!(insert(&(shot->sim_ptr->shot_list), shot, compare_xPos)))
        {
                fprintf(stderr, "DIAGNOSTIC: Failed to insert shot into linked list\n");
                if (DEBUG) fprintf(shot->sim_ptr->out, "Shot: About to free %p\n", (void *) shot);
                alternative_free(shot);
        }

}

// prints the output for when a ship fires a bullet
void fire_message( struct Ship *ship)
{
	if (DEBUG_ENEX) fprintf(stderr, "DEBUG: -> entering %s\n", __func__);

	if(TEXT)
	{
	    FILE *fp = ship->sim_ptr->out;
	    fprintf(fp, "FIRE! %s ship at (%.4lf, %.4lf) fires at angle %.1f\n",
		team(ship->color), ship->pv.xPos, ship->pv.yPos,
		ship->angle);
	}

	if(GRAPHICS)
	{
	    char buffer[100];
	    sprintf(buffer, "%s ship fires at angle %.1f degrees",
			team(ship->color), ship->angle);
	    rx_status(buffer);
	}

	if (DEBUG_ENEX) fprintf(stderr, "DEBUG: <- exiting %s\n", __func__);
}

// fills a shot by using/modifying data from a ship struct
static void fill_shot(struct Ship * ship, struct Shot * shot)
{
        // First, let's update the positions
        shot->pv.xPos = ship->pv.xPos;
        shot->pv.yPos = ship->pv.yPos;

        // Now, let's calculate the velocities to be added

        double RADIAN_FACTOR = M_PI / HALF_CIRCLE;

        double newX = TEAM_COUNT * sin(ship->angle * RADIAN_FACTOR);
        double newY = TEAM_COUNT * cos(ship->angle * RADIAN_FACTOR);

        // actually updating the velocity
        shot->pv.xVel = newX;
        shot->pv.yVel = newY;

        // make the shot point back to the sim
        shot->sim_ptr = ship->sim_ptr;

        // and finally the color (only stored as an integer though)
        shot->color = ship->color;

}


// dynamically allocates a shot and is repsonsible for calling other functions to add data, add to the linked list, and print output
void create_shot(struct Ship * ship)
{
	// this should have failures with the unreliable.o library
	void * ptr = alternative_malloc(sizeof(struct Shot));
	if (ptr == NULL)
        {
                fprintf(stderr, "DIAGNOSTIC: Failed to allocate shot Data\n");
        	return;
        }
	struct Shot * shot = ptr;

	// NOW lets fill the shot up
	
	fill_shot(ship, shot);
	add_a_shot(shot);
	// when the shot gets created, the ship should fire it
	fire_message(ship);

}
// same logic as ships, and we can apply wrapping here as well
void move_a_shot(void * data)
{
	struct Shot * shot = data;
	struct PandV * pv = &(shot->pv);
	pv->xPos += pv->xVel * DELTA_T;
        pv->yPos += pv->yVel * DELTA_T;

}
// outputs the members and details of a particular shot
void print_a_shot(void * data)
{
        struct Shot * shot = data;
	fprintf(shot->sim_ptr->out,"%7s Shot at (%7.4lf, %7.4lf), Velocity: (%6.2lf, %6.2lf)\n", team(shot->color), shot->pv.xPos, shot->pv.yPos, shot->pv.xVel, shot->pv.yVel);
}
// draws a shot to the screen in graphics mode
void draw_a_shot(void * data)
{
	struct Shot * shot = data;
	rx_shot(shot->color, shot->pv.xPos, shot->pv.yPos);
}

// checks if shot is out of bounds in order to remove it (delete) form the linked list
bool out_of_bounds(void * data, void * helper)
{
        // here, we are not gonna use a helper data value to help us decide if the shot shall be deleted (examples would be if shot is too close to a ship)
        struct Shot * shot = data;

	// if the shot is out of bounds that means it leaves. The deleteSome linkedlist will automatically call the free_shot method
        if (shot->pv.xPos >= X_LIMIT || shot->pv.xPos < 0 || shot->pv.yPos >= Y_LIMIT || shot->pv.yPos < 0)
	{
		leaves_message(shot);		
		return true;
	}
        return false;
}

// handles the printing of output message if a shot leaves the screen
void leaves_message( struct Shot *shot)
{
	if (DEBUG_ENEX) fprintf(stderr, "DEBUG: -> entering %s\n", __func__);

	if(TEXT)
	{
	    FILE *fp = shot->sim_ptr->out;
	    fprintf(fp, "%s shot at leaves play at (%.4lf, %.4lf)\n",
		team(shot->color), shot->pv.xPos, shot->pv.yPos);
	}

	if(GRAPHICS)
	{
	    char buffer[100];
	    sprintf(buffer, "%s shot at leaves play at (%.4lf, %.4lf)",
		team(shot->color), shot->pv.xPos, shot->pv.yPos);
	    rx_status(buffer);
	}

	if (DEBUG_ENEX) fprintf(stderr, "DEBUG: <- exiting %s\n", __func__);
}
// function that always returns true, used as a function pointer when calling the deleteSome() linkedlist function
bool always_true(void * data, void * helper) {return true;}
