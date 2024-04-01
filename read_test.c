#include <stdio.h>
int main()
{
	FILE *fptr;

	// Open a file in read mode
	fptr = fopen("/sys/kernel/debug/kernelcare/jiffies", "r"); 
	return (0);
}