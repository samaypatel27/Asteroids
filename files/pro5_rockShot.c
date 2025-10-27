// simulates rock and shot iteraction, as of the instructions for lab 4 (bonus #1)
// should do fission logic after

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "physics.h"

struct PandV {
        double xPos, yPos, xVel, yVel;
};


struct Shot {
        struct PandV pv;
        int color;
};

struct Rock {
        struct PandV pv;
        double mass;
	int color;
};

// from prototype #3
char * team(int color)
{
        // probably define this static array somewhere else
        char * arr[8] = {"BLACK", "RED", "GREEN", "YELLOW", "BLUE", "MAGENTA", "CYAN", "WHITE"};
        return arr[color];
}



int main()
{
	// remember, shot gets its color from the ship (prototype 3)
	struct Shot sho = {{3.0, 5.0, 1.0, 1.0}, 5};

	struct Rock roc = {{3.1, 4.9, 1.0, 1.0}, 12.0, 4};

	// pointers not nessecary for prototype but will probably be used anyway in the actual lab
	struct Shot * shot = &sho;
	struct Rock * rock = &roc;

	double distance_between = distance(shot->pv.xPos, shot->pv.yPos, rock->pv.xPos, rock->pv.yPos);

	if (distance_between > rock->mass) return EXIT_SUCCESS;

	// at this point, this means the collison occured
	
	printf("HIT! %s shot at (%.2lf, %.2lf) hits %s rock size %.2lf at (%.4lf, %.4lf)\n", team(shot->color), shot->pv.xPos, shot->pv.yPos, team(rock->color), rock->mass, rock->pv.xPos, rock->pv.yPos);
	// TODO: add rock mass as many points to the ship at the shot index

}
