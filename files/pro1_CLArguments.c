// tests opening and closing input and output files, including error handling

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#define BONUS_MODE 1

void close_input(FILE * inputPtr)
{
	if (fclose(inputPtr) != 0) fprintf(stdout, "ERROR: Failed to close input file\n");
	else {
		fprintf(stdout, "DIAGNOSTIC: Input file closed\n");
	}
}

void close_output(FILE * outputPtr)
{
        if (fclose(outputPtr) != 0) fprintf(stdout, "ERROR: Failed to close input file\n");
	else {
		fprintf(stdout, "DIAGNOSTIC: Output file closed\n");
	}
}

int main(int argc, char **argv)
{
	if (argc < 3)
	{
		fprintf(stdout, "ERROR: insufficient args. Usage: ./lab4 inputfile outputfile\n");
		return EXIT_FAILURE;
	}
	if (argc == 4 && !(BONUS_MODE))
	{
		fprintf(stdout, "ERROR: Bonus code is not present or graphics not enabled.\n");
		return EXIT_FAILURE;
	}
	// TODO: Other initialiation checks
	
	// Open input file
	
	FILE * inputPtr;

	inputPtr = fopen(argv[1], "r");

	if (inputPtr == NULL)
	{
		fprintf(stdout, "ERROR: Unable to open %s for reading\n", argv[1]);
		return EXIT_FAILURE;
	}
	fprintf(stdout, "DIAGNOSTIC: Successfully opened %s for reading\n", argv[1]);

	// Open output file

	FILE * outputPtr;

	outputPtr = fopen(argv[2], "w+");

	if (outputPtr == NULL)
	{
		fprintf(stdout, "ERROR: Unable to open %s for writing\n", argv[2]);
		close_input(inputPtr);
		return EXIT_FAILURE;
	}
	fprintf(stdout, "DIAGNOSTIC: Successfully opened %s for writing\n", argv[2]);
	
	close_input(inputPtr);
	
	close_output(outputPtr);	
	
	return EXIT_SUCCESS;
}
