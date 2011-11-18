/*
comparefiles.c - Binary file comparison
Author - John Kilfeather
Last Modified: 15-Dec-2010
*/

#include <stdio.h>
#include <stdlib.h>


int main(int argc, char *argv[])
{

	FILE *file[2];
	int endfile[2];/*end of each of the input files*/
	char current_byte[2];/*current byte in each file being considered*/
	int i;
	
	/*Open files and test they opened correctly*/
	file[0] = fopen(argv[1], "rb");
	file[1] = fopen(argv[2], "rb");
	
	if(file[0] == NULL || file[1] == NULL)
	{
		printf("files failed to open");
		exit(1);
	}
	
	/*Find the size of both files and output to screen*/
	fseek(file[0], 0, SEEK_END);
	fseek(file[1], 0, SEEK_END);
	endfile[0] = ftell(file[0]);
	endfile[1] = ftell(file[1]);
	printf("end file one is %x\n", endfile[0]);
	printf("end file two is %x\n", endfile[1]);
	
	if(endfile[0]!=endfile[1])
		printf("file lengths are not equal\n");
	rewind(file[0]);
	rewind(file[1]);
	
	/*Loop through the two files byte by byte checking if the bytes are equal*/
	for(i=0; i<endfile[0]; i++)
	{
		fread(&current_byte[0], 1, 1, file[0]);
		fread(&current_byte[1], 1, 1, file[1]);
		/*if the bytes do not match up alert user where they don't match up, what values are in each file and terminate the program*/
		if(current_byte[0]!=current_byte[1])
		{
			printf("not equal at %x, file 1 is %x, file 2 is %x\n", i, current_byte[0], current_byte[1]);
			exit(1);
		}
	}
	
	/*If program reaches this point tell user files are equal*/
	printf("files are equal\n");
	

	return 0;
}
