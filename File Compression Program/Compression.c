/* *******************************************************************************************
**Compression.c                                                                                **
**Author: John Kilfeather                                                                   **
**Last Modified: 15/12/10                                                                   **
**Function:	To compress an input file into a smaller output file, and decompress a          **
**compressed file back into its original form.                                              **
**                                                                                          **
**USER INPUT                                                                                **
**                                                                                          **
**User inputs 3 arguments along with the program:                                           **
** 1 - either 'c' or 'd' - tells the program whether to compress or decompress.             **
** 2 - input file                                                                           **
** 3 - output file                                                                          **
**                                                                                          **
**COMPRESSION                                                                               **
**                                                                                          **
**Program compresses an input file into an output file by finding wherever there are bytes 	**
**in the input file that occur multiple times in a row and instead of outputting them to    **
**the output file multiple times, it outputs the recurring byte once and then the number of	**
**times it occurs.                                                                          **
**                                                                                          **
**To signify multiplication a particular value is chosen that does not occur often or       **
**at all in the input file. Then instead of outputting a particular byte that occurs        **
**multiple times, the "multiplier value" is first output, then the byte that occurs	multiple**
**times and then the number of times it occurs.                                             **
**                                                                                          **
**For example if the multiplier value was set to 0x23 and the byte FF had occurred 30 times	**
**(0x1e times in hex) in a row the output would be:	23 FF 1e                                **
**The 23 signifies the beginning of a "multiplication", FF is the byte that is to be        **
**written, and 1e is the number of times it should be written.                              **
**                                                                                          **
**Using this method instead of having 30 bytes of information there are now just three.     **
**                                                                                          **
**The "multiplier value" is output to the start of the file, so the decompressor will know  **
**what it is.                                                                               **
**                                                                                          **
**DECOMPRESSION                                                                             **
**                                                                                          **
**The decompressor reads the multiplier value from the first byte in the file. Then whenever**
**it encounters that value in the input file, it will output the suceeding character to the	**
**decompressed file the number of times signified by the next succeeding character          **
**                                                                                          **
**For example if the first byte in the file is 0x24, the decompressor knows this signifies  **
**"multiplication". Then if it encountered the bytes 24 00 07 in succession it would output:**
**00 00 00 00 00 00 00                                                                      **
**(the byte 00 seven times).                                                                **
**                                                                                          **
**COMMENTS                                                                                  **
**                                                                                          **
**This type of compression will not work for files that don't have repeating bytes and is   **
**not designed with any of the common file types in mind                                    **
******************************************************************************************* */


#include <stdio.h>
#include <stdlib.h>

/*Global variable to signify a recurring character within the compressed file*/
char multiplier;

void compress(FILE* input_file, FILE* output_file);/*Compresses input file into output file*/
void decompress(FILE* input_file, FILE* output_file);/*Decompresses input file into output file*/

/*writes 'character' to the compressed file(output_file), based on how many times it has appeared in succession (recurrance_count)
recurrance count is passed as a pointer as it needs to be reset after printing*/
void write_to_file(int *recurrance_count, char character, FILE* output_file);



int main(int argc, char *argv[])
{
	FILE  *input_file, *output_file;
	
	if(argc != 4)
	{
		printf("Incorrect number of arguments - correct arugments form is:\nCompression [compress/decompress] [inputfile] [outputfile]\n");
		exit(1);
	}
		
	/*Open input and output files and test to see if they opened correctly*/
	input_file = fopen(argv[2], "rb");
	if(input_file == NULL)
	{
		printf("input file %s failed to open", argv[2]);
		exit(1);
	}
	output_file = fopen(argv[3], "wb");
	if(output_file == NULL)
	{
		printf("output file %s failed to open", argv[3]);
		exit(1);
	}
	
	/*Check whether user wants to compress or decompress and call respective functions*/
	if(argv[1][0] == 'c')
	{
		compress(input_file, output_file);
	}
	else if(argv[1][0] == 'd')
	{
		decompress(input_file, output_file);
	}
	else
	{
		printf("invalid input %s, input should be either compress/decompress\n", argv[1]);
		exit(1);
	}
	
	/*Close files*/
	fclose(output_file);
	fclose(input_file);
	
	return 0;
	
	
}

/*Assigns the least frequently occuring byte value in the input file to the multiplier variable, then creates the compressed file as described 
in the program description*/
void compress(FILE* input_file, FILE* output_file)
{
	int input_file_end;	
	
	char *bytes;/*an array that will store all the input bytes*/
	int byte_index;/*used to index bytes in the file*/
	
	int recurrance_count;/*the number of times a byte value has occurred in a row*/
	int i;
	
	
	int byte_value_count[256];/*A count of the number of times each byte appears in the file. 
	The index represents the byte value, and the integer is the count of how many times it occurs
	E.g. byte_value_count[13] = 4, would mean that the value '0D' (DEC 13) appears four times in the file*/
	char byte_value_count_index;/*index of above numbers*/
	int lowest_byte_value_count;/*how often the least frequently occuring byte in the file occurs.*/
	
	/*Find size of input file*/
	fseek(input_file, 0, SEEK_END);
	input_file_end = ftell(input_file);
	rewind(input_file);
	
	/*allocate memory to hold all the input data - reads entire file */
	bytes = (char*) malloc(input_file_end);
	
	if(bytes == NULL)
	{
		printf("ERROR: Not enough memory");
		exit(2);
	}
	
	
	/*store data from input file in an array*/
	for(byte_index = 0; byte_index<input_file_end; byte_index++)
	{
		fread(&bytes[byte_index], 1, 1, input_file);
	}
		
	
	/*Set the number of times each byte value occurs in the file to 0*/
	for(i=0;i<256;i++)
	{
		byte_value_count[i] = 0;
	}
	
	/*Count the number of times each byte value occurs in the file*/
	for(byte_index = 0; byte_index<input_file_end; byte_index++)
	{
		/*Set the index to whatever the current byte is
		e.g if the current byte if 0x15 index is set to 21*/
		byte_value_count_index = bytes[byte_index];
		if(byte_value_count_index<0)
			byte_value_count_index+=256;
		
		/*Increment the count of the number of times that byte occurs by 1*/
		byte_value_count[byte_value_count_index]++;
	}
	
	/*Find the lowest byte count and set multpilier to that byte value*/
	lowest_byte_value_count = byte_value_count[0];
	multiplier = 0;
	for(i=0;i<256;i++)
	{
		if(byte_value_count[i]<lowest_byte_value_count)
		{
			lowest_byte_value_count = byte_value_count[i];
			multiplier = i;
		}
	}
	
	/*Print multiplier to output file*/
	fwrite(&multiplier, 1, 1, output_file);
	

	recurrance_count = 1;
	
	/*loop through input checking for areas where there are recurring bytes*/
	for(byte_index = 1; byte_index < input_file_end+1; byte_index++)
	{
		
		/*if the byte is the same as the previous byte increase the recurrance count and do not write to file, except in 2 special cases*/
		if(bytes[byte_index] == bytes[byte_index-1])
		{
			/*If statement deals with two special cases
				1-if there is a recurring character at the end of the file make sure it is printed to the file
				2-if a character occurs more than 255 times in a row print it to the file as being multiplied by 255 then reset the recurrance count
				and start counting it again. This is necessary because the byte that represents the number it is to be multiplied by
				can't store values > 255 (the maximum number that can be stored in a byte).
			*/			
			if((byte_index == input_file_end) || recurrance_count==255)
			{
				write_to_file(&recurrance_count, bytes[byte_index-1], output_file);
				/*reset recurrance to zero instead of one because it will be incremented just below*/
				recurrance_count = 0;
			}
			recurrance_count++;
		}
		
		/*if the byte is not the same as the previous byte write to file*/
		else
		{
			write_to_file(&recurrance_count, bytes[byte_index-1], output_file);
		}
	}
	
	free(bytes);
}

/*Writes 'character' to compressed file ('output_file') based on how many time it has occurred in succession ('recurrance count')*/
void write_to_file(int *recurrance_count, char character, FILE* output_file)
{
	int i;
	/*if recurrance_count is greater than 3 instead of printing the byte out multiple times print the "multiplier value"
	followed by the character itself and then the number of times the character occurs in succession
	There is no gain in using the multiplyer for sequences <= 3*/
	
	/*There is a special case where the character that has been read from the file is the character designated as the "multiplier value".
	In that instance you can't simply print the character to the file as the decompressor will think it signifies multiplication of
	the character that follows it.
	
	To get around this, whenever the multiplier needs to be printed it is printed as if it is part of a
	sequence of recurring characters, so the decompressor will still read it as signifying multiplication of the
	character that follows it, but this character will be the "multiplier value" itself.*/
	
	if(*recurrance_count>3 || character == multiplier)
	{
		fwrite(&multiplier, 1, 1, output_file);
		fwrite(&character, 1, 1, output_file);
		fwrite(recurrance_count, 1, 1, output_file);
		
		/*reset recurrance count*/
		*recurrance_count = 1;
	}
	/*if the byte appears three or fewer times in sucession print out the byte the number of times it appears*/
	else
	{
		for(i=0;i<*recurrance_count;i++)
		{
			fwrite(&character, 1, 1, output_file);	
		}
		/*reset recurrance_count*/
		*recurrance_count = 1;
	}
}


void decompress(FILE* input_file, FILE* output_file)
{
	int input_file_end;
	int byte_index;/*Index of which where byte is in the file*/
	
	unsigned char multiply_quantity;/*the number of times a byte is to be output*/
	char output_char;/*the byte that is to be output*/
	int i;/*generic loop iterator*/
	
	/*Find size of input file*/
	fseek(input_file, 0, SEEK_END);
	input_file_end = ftell(input_file);
	rewind(input_file);
	
	/*Read multiplier from input file*/
	fread(&multiplier, 1, 1, input_file);
	
	/*Loop through input file, outputting characters to output file*/
	for(byte_index=1; byte_index<input_file_end;byte_index++)
	{
		fread(&output_char, 1, 1, input_file);
		/*If byte is the multiplier character act accordingly*/
		if(output_char == multiplier)
		{
			/*Find the byte that is to be outputted*/
			fread(&output_char, 1, 1, input_file);
			byte_index++;
			
			/*find how many times it is to be outputted*/
			fread(&multiply_quantity, 1, 1, input_file);
			byte_index++;
			
			/*output that amount of bytes*/
			for(i=0; i<multiply_quantity; i++)
			{
				fwrite(&output_char, 1, 1, output_file);
			}
		}
		/*Otherwise output the input directly*/
		else
		{
			fwrite(&output_char, 1, 1, output_file);
		}
	}
}
