// add shot data based on ship data. Then, print it out (team() function included as well)

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>

#include "constants.h"

struct PandV {
        double xPos, yPos, xVel, yVel;
};


struct Shot {
        struct PandV pv;
        // TODO: Color/Team and Mass 
	int color;   
};

struct Ship {
	struct PandV pv;
	double angle;
	int color;	
};

char * team(int color)
{
	// probably define this static array somewhere else
	char * arr[8] = {"BLACK", "RED", "GREEN", "YELLOW", "BLUE", "MAGENTA", "CYAN", "WHITE"};
	return arr[color];	
}

// fills the shot with appropriate data based on the ship it is coming from
void create_shot(struct Ship * ship, struct Shot * shot)
{
	// First, let's update the positions
	shot->pv.xPos = ship->pv.xPos;
	shot->pv.yPos = ship->pv.yPos;

	// Now, let's calculate the velocities to be added
	// MAGIC NUMBERS
	int TEAM_COUNT = 8;
	
	double RADIAN_FACTOR = M_PI / 180.0;

	double newX = TEAM_COUNT * sin(ship->angle * RADIAN_FACTOR);
	double newY = TEAM_COUNT * cos(ship->angle * RADIAN_FACTOR);

	// actually updating the velocity
	shot->pv.xVel = newX;
	shot->pv.yVel = newY;

	// and finally the color (only stored as an integer though)
	shot->color = ship->color;

}

// prints shot data
void print_shot(struct Shot * shot)
{
	fprintf(stdout, "%7s Shot at (%7.4lf, %7.4lf), Velocity: (%6.2lf, %6.2lf)\n",
        team(shot->color), shot->pv.xPos, shot->pv.yPos,
        shot->pv.xVel, shot->pv.yVel);
}


// In the actual lab, when the list is null, all ships fire 1 shot (all ships have 1 shot fired at a time) and then when ALL go out of bounds (or all shots get destroyed) the linked list is once again empty and ONLY THEN you all the ships fire another shot
int main()
{
	// let's assume the linked list is empty and now we want to fire a shot for a particular ship
	
	struct Shot * shot = malloc(sizeof(struct Shot));

	struct Ship ship = {{5.0, 5.0, 7.0, -6.0}, 72.0};

	struct Ship * ship_ptr = &ship;

	create_shot(ship_ptr, shot);

	print_shot(shot);
	// TODO: actual lab: add shot to the linkedlist of shots (1 ship has one shot each in the linked list)
	
	return EXIT_SUCCESS;
}
