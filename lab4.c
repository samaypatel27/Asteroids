// Copyright 2024 Neil Kirby 
//
// Not for disclosure without permission

// system libraries rarely have bugs, do them first
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// custom libraries and things like custom libraries go here
#include "n2.h"
#include "debug.h"
#include "rx.h"

// do C code headers last, this file.s header dead last.
#include "sim.h"

// my own header file is included dead last.  IT is MANDATORY!
#include "lab4.h"



/* I own all inits.  In future labs, I might morph to master init and call
 * my init minions to do the various subtasks.  For now, I'm simple enough
 * to do it all easily myself.  I shield main for all details about
 * initializations */
bool init()
{
        // Our graphics library is a bit of a hog, so we only initialize it 
        return(TEXT || ( GRAPHICS && rx_initialize()));

}

/* Put all of the toys away.  In future labs I will own more stuff and
 * call my minions to do those things. */
void teardown()
{
        if(GRAPHICS)rx_teardown();
}

/* Avoid details and call upon my minions to make it everything happen.  I own
 * those highest level things that I must own: performance timing and the
 * value we return to linux */

#define RVAL_INIT_FAIL (1)
#define RVAL_BAD_INPUT (2)

bool valid_arguments(int argc, char ** argv)
{
	if (argc < 3)
        {
                fprintf(stdout, "ERROR: insufficient args. Usage: ./lab4 inputfile outputfile.\n");
                return false;
        }
	// todo: implement bonus mode check
	
	if (argc == 4 && !(GRAPHICS))
	{
		fprintf(stderr, "ERROR: Bonus code is not present or graphics not enabled.\n");
		return false;
	}

	// todo: implement the other bonus check (the 4th one listed)
	
	return true;
}

int main(int argc, char ** argv)
{
	
	if (!(valid_arguments(argc, argv))) return EXIT_FAILURE;

        int rval = EXIT_SUCCESS;
        double start, runtime;

        start = now();  // this is the very first executable statement

	char * inputFile = argv[1];
	char * outputFile = argv[2];

        if( init())
        {
            if( !do_everything(inputFile, outputFile)) rval = RVAL_BAD_INPUT;
            teardown();
        }
        else
        {
            rval = RVAL_INIT_FAIL;       // non zero, indicates an error
        }

        /* at this point we are done, graphics has been torn down*/
        printf("Returning %d\n", rval);
        runtime = now() - start;
        /* after graphics has been torn down, we can freely print */
        printf("Total runtime is %.9lf seconds\n", runtime);

        return rval;
}

