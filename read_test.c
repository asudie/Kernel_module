#include <stdio.h>
int main()
{
	FILE *fptr;

	// Open a file in read mode
	fptr = fopen("/sys/kernel/debug/kernelcare/data", "r"); 
	if(fptr != NULL)
		fclose(fptr);
	fptr = fopen("/sys/kernel/debug/kernelcare/data", "w");

	// Write some text to the file
	if(fptr != NULL)
		fprintf(fptr, "Some text");

	if(fptr != NULL)
		fclose(fptr);
	return (0);
}