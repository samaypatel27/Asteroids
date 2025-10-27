#define _GNU_SOURCE
// The purpose of this prototype is to simulate the logic of the alternate memory library, which will fail at random times, and so we will have to account for this. one time we have to do this when we are trying to create a shot, and allocate for space in the linked list (to add it to the linked list) which we don't have to worry about because we won't be implmenting the linked list yet anyways.

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "altmem.h"
#include "libll.h"

struct PandV {
        double xPos, yPos, xVel, yVel;
};


struct Shot {
        struct PandV pv;
        // TODO: Color/Team and Mass
};

// function from prototype 2 (nessecary as a pointer to function for the insert function from the linkedlist library)
// compare x positions. the earlier the x position, the earlier a shot should be placed in the linkedlist
bool compare_xPos(void * data1, void * data2)
{
        // the data is a pointer to a new node of type struct Shot
        // cannot dereference void pointers(when using the -> operator) so we make it automaticall cast to a rock struct
        struct Shot * shot1 = data1;
        struct Shot * shot2 = data2;

        return (shot1->pv.xPos < shot2->pv.xPos);
}


int main()
{
	void * shot_list = NULL;
	// let's simulate creating 6 shots and attempting to add it the shot_list
	for (int i = 1; i <= 6; i++)
	{
		printf("Shot #%i\n", i);
		struct Shot shot = {0};
		void * ptr = alternative_malloc(sizeof(struct Shot));
		if (ptr == NULL)
		{
			printf("DIAGNOSTIC: Failed to allocate shot Data\n");
			continue;
		}
		
		struct Shot * shot_ptr = &shot;
		shot_ptr = ptr;

		if (!(insert(&shot_list, shot_ptr, compare_xPos)))
		{
			printf("DIAGNOSTIC: Failed to insert shot into linked list\n");
			// do an if (DEBUG)
			printf("Shot: About to free %p\n", (void *) shot_ptr);
			alternative_free(shot_ptr);
		}
	}	

}
