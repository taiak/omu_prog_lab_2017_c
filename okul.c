#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include <ctype.h>
#include "okul.h"

int main (int argc, char *argv[]) {
	FILE * file;
	int line_count, control;
	char file_name[] = "okul.csv", arg_char;

	control = argument_control (argc, argv[1]);

	/* dosyayı okumaya çalış, ulaşmazsan hata ver */
	file  = read_file (file_name);

	/* dosyadaki eleman sayısı */
	line_count = fgetlinecount (file);

  /* dosyada delimiter kontrolü (3 tane ',' ara) yap */
	control = fdeliControl (file, ',', 3, line_count);

  /* delimiter okumada problem varsa hata ver ve çık */
	if (control)
		stu_error (CSV_ERROR_MESSAGE);

  /* öğrenci dizisini oluştur */
	STU stus[line_count];

	/* elemanları verilen dosyadan çek*/
	control = pull_elements (file, stus, line_count);

	/* olası bir hatayı engellemek için */
	/* eğer çekilemeyen bir satır olursa bunu satır sayısından düş */
	line_count -= control;
	
	/* elemanları sıralat */
	qsort (stus, line_count, sizeof(STU), compare_for_stu);
	
	/* puts stu için arg_char parametresini hazırlar */
	arg_char = (argc > 1)? argv[1][0]: 0;

	/* verilen elemanları eleman sayısınca ekrana bas */
	puts_stus (stus, line_count, arg_char);

	/* bellekten alınan yeri geri ver */
	free_stus (stus, line_count);
	return 0;
}
