
/* this file is mandatory so we can run 3 ways:
 * GRAHPICS set to 1 - supresses ALL text output, draws instead.
 * GRAPHICS set to 0 - text mode, VERBOSE controls what prints.
 * TEXT has 2 modes: 
 * VERBOSE to 1 to get DEBUG output 
 * VERBOSE to 0 to get only the required text output
 * Do not set TEXT or DEBUG directly; change GRAHPICS and VERBOSE instead.
 */


#define VERBOSE 0 	/* 1: all messages, 0: only required mesages */
#define TEXT (! GRAPHICS)	/* do not change */
#define GRAPHICS 1 /* 1 for graphics and no text, 0 for text only */

#define DEBUG (VERBOSE && TEXT)	/* do not change this */

#define DEBUG_ENEX (DEBUG && 1)	//Enter/Exit messages, 1 enables, 0 suppresses

#define DO_BUMPS 1 // Determines whether rock on rock collisions occur

#define DO_SHIPS 1 // Determines whether ships should be read in and be used

#define DO_BONUS 1 // Determines when rock to shot collision detection occurs (results in rock fission)
