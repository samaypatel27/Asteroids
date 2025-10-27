// this prototype demoes the usage of creating shot structs, adding them to link lists, printing them, deleting some that are out of bounds, and then printing them out again
// some typedef function decleration are stasified as parameters with the creation of mock typedef functions (see libll.h)

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "altmem.h"
#include "linkedlist.h"
#include "pro2_LL.h"


struct PandV {
        double xPos, yPos, xVel, yVel;
};


struct Shot {
	struct PandV pv;
	// TODO: Color/Team and Mass	
};

// compare x positions. the earlier the x position, the earlier a shot should be placed in the linkedlist
bool compare_xPos(void * data1, void * data2)
{
	// the data is a pointer to a new node of type struct Shot
	// cannot dereference void pointers(when using the -> operator) so we make it automaticall cast to a rock struct
	struct Shot * shot1 = data1;
	struct Shot * shot2 = data2;

	return (shot1->pv.xPos < shot2->pv.xPos);
}	

void print_node(void * data)
{
	struct Shot * shot = data;

	fprintf(stdout, "Shot at position (%.3lf, %.3lf) \n", shot->pv.xPos, shot->pv.yPos);
}

// checks if shot is out of bounds in order to remove it (delete) form the linked list
bool out_of_bounds(void * data, void * helper)
{
	// here, we are not gonna use a helper data value to help us decide if the shot shall be deleted (examples would be if shot is too close to a ship)
	struct Shot * shot = data;
	
	int X_LIMIT = 30.0;
	int Y_LIMIT = 20.0;
	if (shot->pv.xPos >= X_LIMIT) return true;
	return false;
}

// free shot node data we allocated ynamically
void free_shot(void * data)
{
	// free a single node
	alternative_free(data);
}

void * allocate_node()
{
	void * ptr = malloc(sizeof(struct Shot));
	if (ptr != NULL)
	{
		return ptr;
	}
	return NULL;
}

int main()
{
	// Let's use shot_list as the pointer to the head of the linkedList. It will be null upon the first insert call so it'll add whatever value to the beginning of the list

	void * shot_list = NULL;

	// in actual lab: use alternative and include error handling (see prototype 4)	
	void * ptr1;
	ptr1 = allocate_node();
	if (ptr1 != NULL){
	       	struct Shot * shot1 = ptr1;
		shot1->pv.xPos = 6.0;
		insert(&shot_list, shot1, compare_xPos);
	}
	

	void * ptr2;
        ptr2 = allocate_node();
        if (ptr2 != NULL){
	       	struct Shot * shot2 = ptr2;
		shot2->pv.xPos = 5.0;
		insert(&shot_list, shot2, compare_xPos);
	}

	void * ptr3;
        ptr3 = allocate_node();
        if (ptr3 != NULL){
	       	struct Shot * shot3 = ptr3;
		shot3->pv.xPos = 12.0;
		insert(&shot_list, shot3, compare_xPos);
	}
	void * ptr4;
        ptr4 = allocate_node();
        if (ptr4 != NULL) {
		struct Shot * shot4 = ptr4;
		shot4->pv.xPos = 31.0;
		insert(&shot_list, shot4, compare_xPos);
	}



	// Iterate through the linkedlist
	
	iterate(shot_list, print_node);

	// Delete shot nodes that violate the out of bounds condition

	deleteSome(&shot_list, out_of_bounds, NULL, free_shot);

	// iterate again to see the out of bounds shots get removed
	
	iterate(shot_list, print_node);

	return EXIT_SUCCESS;
}

