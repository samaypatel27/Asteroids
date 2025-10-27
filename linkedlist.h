/* Samay Patel */

int deleteSome(void *p2head, bool (*mustGo)(void * data, void * helper), void *helper, void (*doDispose) (void * data));
bool insert(void *p2head, void *shot, bool (*doComparison) (void * data1, void * data2));
void iterate(void * head, void (*doThis) (void * data));
