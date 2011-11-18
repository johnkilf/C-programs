/*reversesentence.c: Program to reverse order of letters within words in a sentence
Authors: John Kilfeather
Last Modified: 13/Dec/2010
***/

#include <stdio.h>

const int SENTENCE_LENGTH = 100;/*Maximum user input*/

/*Function that reverses the letters within a particular word*/
void reverse_characters_in_word(char *words, int start_of_word);


int main(void)
{
	char sentence[SENTENCE_LENGTH];
	int word_start = 0;/*index of where an individual word starts*/
	int sentence_position = 0;/*index of position within the sentence*/
	int i = 0;/*loop variable*/

	/*retreiving user input*/
	printf("Input some words\n");
	fgets(sentence, SENTENCE_LENGTH, stdin);
	
	
	
	/*loop through sentence until you reach the end, checking for spaces and return characters and acting accordingly*/
	for(sentence_position=0; sentence[sentence_position] != '\0'; sentence_position++)
	{
		/*upon finding a space or return character, go about reversing the preceding word*/
		if(sentence[sentence_position] == ' ' || sentence[sentence_position] == '\n')
		{
			/*call function only if there has been at least one letter*/
			if(word_start<sentence_position)
			{
				reverse_characters_in_word(&sentence[word_start],word_start);		
			}
			
				
		}
		/*If next character is a space keep iterating through sentence*/
		while(sentence[sentence_position] == ' ')
		{
			sentence_position++;
			word_start = sentence_position;	
		}
	}
	
	printf("%s", sentence);	
	return 0;
}


void reverse_characters_in_word(char *words, int start_of_word)
{

	/*array used for duration of function to copy reversed word into
	/*If I had time, I would replace [SENTENCE LENGTH] this with a call to malloc to allocate memory based on how many letters were in the word*/
	char temp_sentence[SENTENCE_LENGTH];
	int letter_count = 0;
	int i = 0; /*loop variable*/

	/*Find length of word*/
	for(i=0; (words[i]!= ' ') && (words[i]!= '\n'); i++)
	{		
		letter_count++;		
	}	
	
	/*Iterate through each letter in that word putting the last letter of that word into the first letter of that
	word in the temp_sentence array and so on*/
	for(i=0; i<letter_count; i++)
	{
		temp_sentence[i] = words[letter_count-1-i];
	}
	for(i=0; i<letter_count; i++)
	{
		words[i] = temp_sentence[i];
	}	
}
