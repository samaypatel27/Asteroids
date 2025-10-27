// This file was created and edited by Samay Patel

// standard C libraries
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

// custom libraries
#include "altmem.h"
#include "debug.h"
#include "structs.h"

// File's own header files
#include "linkedlist.h"


// allocate a pointer to node
static Node * allocate_node()
{
	static int numNodes = 0;
	void * ptr = alternative_malloc(sizeof(Node));
	if (ptr == NULL)
	{
		fprintf(stderr, "ERROR: Failed to allocate shot data.\n");
		return NULL;
	}
	numNodes++;
	fprintf(stderr, "DIAGNOSTIC: %i nodes allocated.\n", numNodes);
	if (DEBUG) fprintf(stderr, "DEBUG: linkedlist: allocated pointer is %p\n", ptr);
	Node * n = ptr;
	return n;
}

// free the space the node allocates in memory
static void free_node(Node * node)
{
	if (node != NULL)
	{
		fprintf(stderr, "DEBUG: linkedlist: about to free %p\n", (void *) node);
		free(node);
		node = NULL;

	}
}

// add a new node with shot data into the linked list
bool insert(void *p2head, void *shot, bool (*doComparison) (void * data1, void * data2))
{
	// let's first allocate space to hold the new thing we want to insert
	Node * newNode = allocate_node();
	if (newNode == NULL) return false;
	
	// let's add the shot as an actual data value
	newNode->data = shot;

	// Remember, the void * shot_list holds pointers to Nodes, not nodes itself. So we have a pointer to a pointer, but we are also holding a pointer as well
	// now we need to figure out when the insert this shot (we gotta do this in order). We are given void *p2head but p2p2 head (with two **s) is what is actually passed
	Node **p2p2change = (Node**)p2head;
	
	while (*p2p2change != NULL && doComparison((**p2p2change).data, newNode->data))
	{
		p2p2change = &((**p2p2change).next);	
	}
		
	newNode->next = *p2p2change;
	
	*p2p2change = newNode;
	return true;
}

// iterate through all of the elements in the linked list, which are pointer to nodes
void iterate(void * head, void (*doThis) (void * data))
{
	Node *p2Node = (Node*)head;
	while (p2Node != NULL)
	{
		// call the pointer to function
		doThis(p2Node->data);
		// next is already of type pointer to node
		p2Node = (p2Node->next);
	}	
}

// delete elements from the linked list that fail to meet a particular requirment, if provided
int deleteSome(void *p2head, bool (*mustGo)(void * data, void * helper), void *helper, void (*doDispose) (void * data))
{
	Node **p2p2change = (Node**)p2head;
	int deleted = 0;
	while (*p2p2change != NULL)
	{
		// lets skip to the next because it is allowed to stay in the linked list
		if (!(mustGo((**p2p2change).data, helper))) p2p2change = &((**p2p2change).next);

		else
		{
			// let's declare a pointer to the node we want to delete
			Node *tempNode = *p2p2change;           
                        *p2p2change = (**p2p2change).next;  
                        doDispose(tempNode->data);              
                        free_node(tempNode);                    
                        deleted++;
		}
	}
	return deleted;
}






