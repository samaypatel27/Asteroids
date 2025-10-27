// Samay Patel has created an edited this file

// system libraries
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

// custom libraries
#include "structs.h"
#include "n2.h"
#include "rx.h"
#include "math.h"
#include "constants.h"
#include "debug.h"
#include "libll.h"

// C header files with functions called in this file 
#include "graphics.h"
#include "commonGround.h"
#include "physics.h"

// constants specific to this file
#define TWO 2.0

// file specific header file
#include "rocks.h"


// iterate through all consecutive memory spaces that hold rock Structs; the second parameter is a pointer to a function that takes a pointer to a struct Rock as a parameter
void rock_iterator (struct Sim * sim_ptr, void (*callback) (struct Rock *))
{
	if (DEBUG_ENEX) fprintf(stderr, "DEBUG: -> entering %s\n", __func__);
 
	for (int i = 0; i < sim_ptr->belt->numRocks; i++)
	{
		// Here, we pass the address of the Rock struct because it isn't a pointer (in order to make it a pointer)
		callback(&(sim_ptr->belt->array[i]));
	}
	if (DEBUG_ENEX) fprintf(stderr, "DEBUG: <- exiting  %s\n", __func__);


}

// print a rock's members at a particular elapsed time
void print_a_rock(struct Rock * rock)
{
	if (DEBUG_ENEX) fprintf(stderr, "DEBUG: -> entering %s\n", __func__);

	// We point the rock back to the sim to get the stdout/stderr (it's a pointer now because we got the address in the previous function)
	if (TEXT) fprintf(rock->sim_ptr->out, "%7d Rock at (%7.4lf, %7.4lf), Velocity: (%6.2lf, %6.2lf), Mass: %4.1lf\n", rock->color, rock->pv.xPos, rock->pv.yPos, rock->pv.xVel, rock->pv.yVel, rock->mass);

	if (DEBUG_ENEX) fprintf(stderr, "DEBUG: <- exiting %s\n", __func__);

}

// draw a rock in graphics mode during a particular elapsed time
void draw_a_rock(struct Rock *rock)
{
        if (DEBUG_ENEX) fprintf(rock->sim_ptr->out, "DEBUG: -> entering %s\n", __func__);

        rx_rock((int)rock->color, rock->pv.xPos, rock->pv.yPos, rock->mass);

        if (DEBUG_ENEX) fprintf(stderr, "DEBUG: <- exiting %s\n", __func__);
}

// Copyright (c) 2025 Neil Kirby not for distribution without written permission
// The following function has copyright mentioned above, but has been edited by Samay Patel 

// Simulate the motion of a rock by updating its position
void move_a_rock(struct Rock * rock)
{
        if (DEBUG_ENEX) fprintf(stderr, "DEBUG: -> entering %s\n", __func__);

	struct PandV * pv = &(rock->pv);

        // simulate rock motion by increasing by a factor of velocity and DELTA_T, used to represent a single frame
        pv->xPos += pv->xVel * DELTA_T;
        pv->yPos += pv->yVel * DELTA_T;
        
	apply_wrapping(pv);


        if (DEBUG_ENEX) fprintf(stderr, "DEBUG: <- exiting %s\n", __func__);
}

// update velocities after calculations using elasticity formula
static void update_velocities_by_adjustment(struct PandV * pv1, struct PandV * pv2, double x1, double y1, double x2, double y2)
{
	pv1->xVel = x1;
	pv1->yVel = y1;
	pv2->xVel = x2;
	pv2->yVel = y2;
}

// Calculate new velocities after rocks bump, using the concept of elastic collision
static void adjust_velocities_bump(struct Rock * rock1, struct Rock * rock2)
{
        // pointers to struct’s memory addresses for easy implementation and changes
        struct PandV * pv1 = &(rock1->pv);
        struct PandV * pv2 = &(rock2->pv);

        double vx1 = rock1->pv.xVel;
        double vy1 = rock1->pv.yVel;

        double vx2 = rock2->pv.xVel;
        double vy2 = rock2->pv.yVel;

        double m1 = rock1->mass;
        double m2 = rock2->mass;

        if (DEBUG) fprintf(rock1->sim_ptr->out, "rock_on_rock velocities: old velocities (%.4lf, %.4lf) and (%.4lf, %.4lf)\n", vx1, vy1, vx2, vy2);

        // New X Velocity for Rock 1
        double rock1XVel = elastic_va_final(vx1, m1, vx2, m2);
        // New Y Velocity for Rock 1
        double rock1YVel = elastic_va_final(vy1, m1, vy2, m2);
        // New X Velocity for Rock 2
        double rock2XVel = elastic_va_final(vx2, m2, vx1, m1);
        // New Y Velocity for Rock 2
        double rock2YVel = elastic_va_final(vy2, m2, vy1, m1);
        
	update_velocities_by_adjustment(pv1, pv2, rock1XVel, rock1YVel, rock2XVel, rock2YVel);
	
	if (DEBUG) fprintf(rock1->sim_ptr->out, "rock_on_rock velocities: new velocities (%.4lf, %.4lf) and (%.4lf, %.4lf)\n", rock1->pv.xVel, rock1->pv.yVel, rock2->pv.xVel, rock2->pv.yVel);
}

// update the positions of both rocks after calculating both x and y adjustments
static void update_positions_by_adjustment(struct PandV * pv1, struct PandV * pv2, double x_adjustment, double y_adjustment)
{
	pv1->xPos += x_adjustment;
	pv1->yPos += y_adjustment;
	pv2->xPos -= x_adjustment;
	pv2->yPos -= y_adjustment;
}
// Calculate new positions after rocks bump and applying wrapping in case they go outside the bounds of the screen
static void adjust_positions_bump(struct Rock * rock1, struct Rock * rock2, double desired_distance, double center_distance)
{
        // pointers to structs
        struct PandV * pv1 = &(rock1->pv);
        struct PandV * pv2 = &(rock2->pv);

        // let’s print the original distance that we found to be concerning again
        if (DEBUG) print_distance_debug(rock1->sim_ptr->out, pv1->xPos, pv1->yPos, pv2->xPos, pv2->yPos, center_distance);

        double ratio = desired_distance / center_distance;
        double adjustment_factor = ((ratio - 1.0) / 2.0);

        double xDiff = pv1->xPos - pv2->xPos;
        double yDiff = pv1->yPos - pv2->yPos;

        double x_adjustment = xDiff * adjustment_factor;
        double y_adjustment = yDiff * adjustment_factor;

        if (DEBUG) fprintf(rock1->sim_ptr->out,"rock on rock positions: adjusted by (+/-) %.2lf x, (+/-) %.2lf y\n", x_adjustment, y_adjustment);

        // update to the new positions
	update_positions_by_adjustment(pv1, pv2, x_adjustment, y_adjustment);

        // we changed the positions, so let’s apply the wrapping just in case the rocks went out of bounds
        apply_wrapping(pv1);
        apply_wrapping(pv2);

        // Now that we updated the positions, Let’s check the new distance, and it should be equal to the sum of the rocks masses
        double new_distance = distance(pv1->xPos, pv1->yPos, pv2->xPos, pv2->yPos);
        if (DEBUG) print_distance_debug(rock1->sim_ptr->out, pv1->xPos, pv1->yPos, pv2->xPos, pv2->yPos, new_distance);
        if (DEBUG) fprintf(rock1->sim_ptr->out, "New C2C distance is %.2lf\n", new_distance);
}

// check if two specific rocks bump by comparing their radiuses and distances
static void two_rocks_bump_check(struct Rock * rock1, struct Rock * rock2)
{
        // Let's calculate the desired_distance and see if its greater than center_distance

        double x1 = rock1->pv.xPos;
        double x2 = rock2->pv.xPos;
        double y1 = rock1->pv.yPos;
        double y2 = rock2->pv.yPos;

	double center_distance = distance(x1, y1, x2, y2);

        double m1 = rock1->mass;
        double m2 = rock2->mass;

        double desired_distance = m1 + m2;

        if (desired_distance <= center_distance) return;

        // if we are here the rocks bump
        if (DEBUG) fprintf(rock1->sim_ptr->out, "rock_too_close: %.2lf compared to %.2lf + %.2lf\n", center_distance, m1, m2);
        if (TEXT) fprintf(rock1->sim_ptr->out, "BUMP! Rock color %d at (%.4lf, %.4lf) hits rock color %i at (%.4lf,  %.4lf)\n", rock1->color, rock1->pv.xPos, rock1->pv.yPos, rock2->color, rock2->pv.xPos, rock2->pv.yPos);

        if (GRAPHICS)
        {
                char status_buffer[100];
                sprintf(status_buffer, "BUMP! Rock %d at (%.4lf, %.4lf) hits rock %d at (%.4lf, %.4lf) \n",
                rock1->color, rock1->pv.xPos, rock1->pv.yPos, rock2->color, rock2->pv.xPos, rock2->pv.yPos);
                rx_status(status_buffer);
        }
        adjust_positions_bump(rock1, rock2, desired_distance, center_distance);
        adjust_velocities_bump(rock1, rock2);

}


// check if all rocks bump with each other, algorithm runs in O(n^2) time as we are checking every single rock against each other
void all_rocks_bump_check(struct Sim * sim_ptr)
{
	// pointer to belt structure
	struct Belt * belt = sim_ptr->belt;
	for (int i = 0; i < belt->numRocks; i++)
	{
		for (int j = i + 1; j < belt->numRocks; j++)
		{
			struct Rock * rock1 = &(belt->array[i]);
			struct Rock * rock2 = &(belt->array[j]);
			two_rocks_bump_check(rock1, rock2);
		}
	}
}

// allocates extra consecutive memory storage to hold rock Structs
static bool allocate_rocks_memory(struct Sim * sim_ptr, struct Belt * belt_ptr)
{
	void * ptr;
	if (belt_ptr->capacity == 0)
	{
		ptr = realloc(belt_ptr->array, 4 * sizeof(struct Rock));
		belt_ptr->capacity = 4;
        }
        else
	{
		ptr = realloc(belt_ptr->array, (belt_ptr->capacity) * 2 * sizeof(struct Rock));
        	belt_ptr->capacity *= 2;
	}

	if (ptr == NULL)
	{
        	if (DEBUG) fprintf(sim_ptr->out, "ERROR: Failed to allocate extra memory");
		return false;
	}
	// if memory allocation didn't fail, we can assign the new address with the extra space to the rock array
	// array is of type struct Rock *. Because void pointer is a generic type, it can automatically be converted to any other pointer type without an explicit cast.
	// however, if we want to convert it back to a rock pointer, we will have to do that
        belt_ptr->array = ptr;

	// Now let's do the debug messages
	if (DEBUG) fprintf(sim_ptr->out, "Resizing rock array to %i\n", belt_ptr->capacity);
	if (DEBUG) fprintf(sim_ptr->out, "realloc returned %p\n", (void *) belt_ptr->array);
	
	return true;
}	

// adds rock to a consecutive memory space with other rock structs to represent an "array"
bool add_a_rock(struct Sim * sim_ptr, struct Rock * tempRock)
{
	if(DEBUG_ENEX) fprintf(stderr, "DEBUG: -> entering %s\n", __func__);

        // pointer to struct for belt for easier access and usage; belt is already a pointer so we don't have to use & to get address
        struct Belt * belt_ptr = sim_ptr->belt;

        // before we can add the rock, we have to check if we have to reallocate space
        int capacity = belt_ptr->capacity;
	int currentRocks = belt_ptr->numRocks;

	if (DEBUG) fprintf(sim_ptr->out, "This is the current rock capacity: %i\n", capacity);
        if (capacity == 0 || currentRocks == capacity)
        {
		if (!(allocate_rocks_memory(sim_ptr, belt_ptr))) return false;
        }
	// Only add a non-pointer rock Struct, so we dereference the struct
        belt_ptr->array[currentRocks] = *tempRock;
	
	// now lets update numRocks (starts at 0)
	belt_ptr->numRocks += 1;
	
	// When we get the rock data, we already get numRocks, so we don't need to update numRocks += 1 to count it
	
	if(DEBUG_ENEX) fprintf(stderr, "DEBUG: -> exiting %s\n", __func__);

	return true;
}

// free the allocated space for a pointer in memory address pointing to rock structs stored in consecutive memory spaces
void rock_cleanup(struct Sim * sim_ptr)
{
	free(sim_ptr->belt->array);
	sim_ptr->belt->array = NULL;
	int numRocks = sim_ptr->belt->numRocks;
	int capacity = sim_ptr->belt->capacity;
	if (TEXT) fprintf(stderr, "Freeing rock array size %i holding %i rocks\n", capacity, numRocks);
	sim_ptr->belt->numRocks = 0;
	sim_ptr->belt->capacity = 0;
}


// removes a rock from the rockArray, and the simulation
static void remove_rock(struct Rock * rock)
{
        struct Belt * belt_ptr = rock->sim_ptr->belt;

        // here we subtract the memory addresses. In C, since everything is sizeof(obj) bytes away in memory, it will automatically divide expression below by it, giving us the valid answer
        int index = rock - belt_ptr->array;

        // assign the rock we want to disappear to the very last rock in the array
        belt_ptr->array[index] = belt_ptr->array[belt_ptr->numRocks - 1];

        // decrement the capacity, so we won't loop all the way to that last rock now
        (belt_ptr->numRocks)--;
}

// calculates the offsets and update the new rock PandV velocities for the new rocks
static void do_offsets(struct Rock * left, struct Rock * right, const struct Rock original, const struct Shot shot, const double center_distance)
{
	double DELTA_Y = split_offset(original.pv.xPos, shot.pv.xPos, center_distance, original.mass);
        double DELTA_X = split_offset(original.pv.yPos, shot.pv.yPos, center_distance, original.mass);

        // applying the seperation to the left and the right rocks

        left->pv.xPos = original.pv.xPos - DELTA_X;
        left->pv.yPos = original.pv.yPos + DELTA_Y;
        left->pv.xVel = original.pv.xVel - DELTA_X;
        left->pv.yVel = original.pv.yVel + DELTA_Y;

        right->pv.xPos = original.pv.xPos + DELTA_X;
        right->pv.yPos = original.pv.yPos - DELTA_Y;
        right->pv.xVel = original.pv.xVel + DELTA_X;
        right->pv.yVel = original.pv.yVel - DELTA_Y;

}

// fill the left and right rocks properties
static void fill_split_rock_details(struct Rock * left, struct Rock * right, const struct Rock original, const struct Shot shot, const double center_distance)
{
        // lets add properties from the original rock to the new rock

        left->mass = (original.mass) / TWO;
        right->mass = (original.mass) / TWO;

        left->color = original.color;
        right->color = original.color;

        struct Sim * sim_ptr = shot.sim_ptr;

        left->sim_ptr = sim_ptr;
        right->sim_ptr = sim_ptr;

	do_offsets(left, right, original, shot, center_distance);
}

// handles rock deletion after getting hit by a shot, and rock splitting if certain requirements are met
static bool apply_fission_effects(struct Shot * shot, struct Rock * rock, double center_distance)
{
        // regardless of if the mass is below 2.0 or not, we have to delete the original rock

        struct Rock original = *rock;

        // we remove the original rock now. above, we dereferenced it because its pointer will pointer to the last rock in the array now (which we don't want)
        // if you want to change something, you pass the pointer to the type you want to change. we are changing a type rock so we already have a rock * being passed
        remove_rock(rock);

        // procces completed, return true
        if (original.mass < 2.0) return true;

        // if not let's split the rock in half

        struct Rock left = {0};
        struct Rock right = {0};

        // lets add properties from the original rock to the new rock
	
	fill_split_rock_details(&left, &right, original, *shot, center_distance);
	
	// now lets add the rock
        if (!(add_a_rock(rock->sim_ptr, &left)) || !(add_a_rock(rock->sim_ptr, &right))) return false;

        return true;
}

// checks if a particular rock and shot collide
static bool rocks_shots_collide(void * data, void * helper)
{
        struct Shot * shot = data;
        struct Rock * rock = helper;

        double distance_between = distance(shot->pv.xPos, shot->pv.yPos, rock->pv.xPos, rock->pv.yPos);

        if (distance_between > rock->mass) return false;

        // at this point, this means the collison occured

        rocks_shots_hit_message(shot, rock);

        // now let's increment the score ship of a particular color by the mass of the rock
        // we can't pass the helper pointer as the last parameter of deleteSome() so let's handle the fission stuff here

        ((rock->sim_ptr->shipArray)[shot->color]).score += (int) rock->mass;

        // now that we did this, the shot hitting the rock causes the rock to split in half

        // if add a rock fails what do we do?
        if (!(apply_fission_effects(shot, rock, distance_between))) return false;

        return true;

}

// called by the simulation to check all possible combinations of rocks and shots and see if they have collided
void shot_to_rock_bumps_check(struct Sim * sim_ptr)
{
        // traverse backwards. because if we delete a rock we are removing the last index and adding it to somewhere in the middle (so we could get to last index if we went forward and this would cause some issues)
        for (int i = sim_ptr->belt->numRocks - 1; i >= 0; i--)
        {
                struct Rock * rock = &(sim_ptr->belt->array[i]);

                int deletedShots = deleteSome(&(sim_ptr->shot_list), rocks_shots_collide, rock, free_shot);
                if (deletedShots > 0){
                        print_cleared_shots(&deletedShots);
                }

        }
}


// handles output if rock and shots collide
void rocks_shots_hit_message( struct Shot *shot, struct Rock *rock)
{
        if (DEBUG_ENEX) fprintf(stderr, "DEBUG: -> entering %s\n", __func__);

        if(TEXT)
        {
            FILE *fp = shot->sim_ptr->out;
            fprintf(fp, "HIT! %s shot at (%.2lf, %.2lf) hits %s rock size %.2lf at (%.4lf, %.4lf)\n",
                team(shot->color), shot->pv.xPos, shot->pv.yPos,
                team(rock->color), rock->mass, rock->pv.xPos, rock->pv.yPos);
        }

        if(GRAPHICS)
        {
            char buffer[100];
            sprintf(buffer, "HIT! %s shot hits %s rock at (%.0lf, %.0lf)",
                team(shot->color), team(rock->color), rock->pv.xPos, rock->pv.yPos);
            rx_status(buffer);
        }

        if (DEBUG_ENEX) fprintf(stderr, "DEBUG: <- exiting %s\n", __func__);
}

