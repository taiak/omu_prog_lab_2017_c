#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include <ctype.h>
#include "okul.h"

int main (int argc, char* argv[]) {

	FILE* file = fopen("okul.csv", "r"); 
	buffer_on ();
	get_stu (file);
	printf ("%s\n", read_buffer);

	/*
	artık yeni elemanlar read_buffer'da
	henüz bir öğrenciye aktarılmamış bir string şekilde
	*/
	STUDENT test;
	chomp_into_stu (&test);

	puts_stu (&test);
	buffer_off ();
	
	return 0;
}
