
struct Sim;

struct PandV {
        double xPos, yPos, xVel, yVel;
};

struct Shot {
	struct PandV pv;
	int color;
	struct Sim * sim_ptr;
};

struct Rock {
        struct PandV pv;
        double mass;
        int color;
        struct Sim * sim_ptr;
};

struct Ship {
	struct PandV pv;
	double angle;
	int color;
	int score;
	struct Sim * sim_ptr;
};
struct Belt {
        struct Rock * array;
        int numRocks;
        int capacity;

};

struct Sim {
	void * shot_list;
        double elapsed_time;
        double time_limit;
        FILE * in;
        FILE * out;
        struct Belt * belt;
	struct Ship shipArray[8];
};

typedef struct Node{
        struct Node * next;
        void * data;
}Node;


