#include<stdio.h>

static inline char * asm_sub_str(char * dest, char * src, int s_idx, int edix)
{
	__asm__ __volatile__
	(
		// Clear the content in the CX register.
		"cld\n\t"

		// CPU checks CX to see if it contains zero. If not, it moves the byte from SI to DI and decrements the cx register.
		// This process repeats until CX becomes zero.
		"rep\n\t"

		// moves the byte from SI to DI
		"movsb"

		:

		// Bond the addr of src + s_idx to SI register. Bond the addr of dest to DI register. Bond the word length to CX register.
		:"S"(src + s_idx), "D"(dest), "c"(edix - s_idx + 1)
        );

	return dest;
}

static inline char * sub_str(char * dest, char * src, int s_idx, int edix)
{
	// Calculate the source word's length.
	int len = edix - s_idx + 1;
	int i;

	// Copy the string.
	for(i = 0; i < len; i++)
	{
		* (dest + i) = * (src + s_idx + i);
	}
 
	// Fill the end char.
	* (dest + len) = '\0';

	return dest;
}


int main(int argc, char **argv)
{
	// Enter the source string from command line.
	char *str = malloc(sizeof(char) * 50);
	printf("Enter a source value : \n");
 	gets(str);

	// Enter the start index and end index from command line.
	int s_idx, edix;
	printf("Enter start index : \n");
	scanf("%d", &s_idx);
	printf("Enter end index : \n");
	scanf("%d", &edix);

	// Molloc some memory spaces for destination1.
	char * d1 = malloc(sizeof(char) * 50);

	// Molloc some memory spaces for destination2.
	char * d2 = malloc(sizeof(char) * 50);

	// Copy the string by asm function.
	d1 = asm_sub_str(d1, str, s_idx, edix);

	// Copy the string by C function.
	d2 = sub_str(d2, str, s_idx, edix);

	// print the two destination results.
	printf("The substring got by function asm_sub_str : %s\n", d1);
	printf("The substring got by function sub_str : %s\n", d2);

	// Compare the two copied strings.
	if (strcmp(d1, d2) == 0)
		printf("Above two substrings are equal.\n");
	else
		printf("Above two substrings are not equal.\n");

	// free the memory spaces we molloced before.
	free(d1);
	free(d2);
	free(str);
}
