#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include <ctype.h>
#include "okul.h"

int main (int argc, char *argv[]) {
	FILE * file;
	int line_count, control;
	char file_name[] = "okul.csv", argument;

	argument_control (argc, argv[1]);
	
	/* arguman verildi mi diye bakar */
	argument = (argc > 1)? argv[1][0]: 0;

	/* dosyayı okumaya çalış ulaşmazsan hata ver */
	file  = read_file (file_name);

	/* dosyadaki eleman sayısı */
	line_count = fgetlinecount (file);

  control = fdeliControl (file, ',', 3, line_count);
	
	if (control)
		csv_error ();

	STU stus[line_count];

	/* elemanları verilen dosyadan çek*/
	control = pull_elements (file, stus, line_count);

  /* olası bir hatayı engellemek için */
  /* eğer çekilemeyen bir satır olursa bunu satır sayısından düş */
  line_count -= control;
	
	/* elemanları sıralat */
	qsort (stus, line_count, sizeof(STU), compare_for_stu);	


	/* verilen elemanları eleman sayısınca ekrana bas */
	puts_stus (stus, line_count, argument);

	/* bellekten alınan yeri geri ver */
	free_stus (stus, line_count);
	return 0;
}