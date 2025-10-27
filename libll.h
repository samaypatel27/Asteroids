

/* header file for list */
#include <stdbool.h>

/* these are the function pointer signatures needed */

typedef void (* ActionFunction)( void *data);
typedef bool (* ComparisonFunction)(void *data1, void *data2);
typedef bool (* CriteriaFunction)(void *data, void *helper);
typedef double (* NumericFunction)(void *data);


/* signatures that the list code provides to the outside world */

/* insert and delete need to be able to change the head pointer so you pass
 * in the address of the head pointer */

/* insert returns FALSE when it fails to do the insert */
bool insert(void *p2head, void *data, ComparisonFunction goesInFrontOf );
/* deleteSome returns the number of nodes deleted */
int deleteSome(void *p2head, CriteriaFunction mustGo, void *helper,
		ActionFunction disposal);

/* The functions below do not change the nodes, so you pass in the head
 * pointer and not the address of the head pointer */
bool any(void *head, CriteriaFunction yes, void *helper);
void *best (void *head, ComparisonFunction second_better);
int count (void *head, CriteriaFunction countThis, void *helper);
void iterate(void *head, ActionFunction doThis);
double least(void *head, NumericFunction compute_this);
bool some(void *head, CriteriaFunction someTest);
void sort(void *hptr, ComparisonFunction cf);


