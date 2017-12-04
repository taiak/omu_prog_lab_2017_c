#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include <ctype.h>
#include "okul.h"

int main (int argc, char *argv[]) {
	FILE *file;
	int line_count, control;
	char file_name[] = "okul.csv";

	/* puts_stus için arg_char parametresini hazırlar */
	char arg_char = (argc > 1)? argv[1][0]: 0;

	ANY_STU_ERROR = argument_control (argc, argv[1]);
	catch_error (NULL);

	/* dosyayı okumaya çalış, ulaşmazsan hata ver */
	file  = read_file (file_name);

	/* dosyadaki eleman sayısı */
	line_count = fgetlinecount (file);

	/* dosyada delimiter kontrolü (3 tane ',' ara) yap */
	ANY_STU_ERROR = fdelimitercontrol (file, ',', 3, line_count);

	/* delimiter okumada problem varsa hata ver ve çık */
	catch_error ("Hata: CSV formatı hatalı!");
 
	/* öğrenci dizisini oluştur */
	STUDENT stus[line_count];

	/* elemanları verilen dosyadan çek*/
	ANY_STU_ERROR = pull_elements (file, stus, line_count);
	catch_error ("HATA: CSV'deki dosyalar okunurken bir hatayla karşılaşıldı!");

	/* elemanları sıralat */
	qsort (stus, line_count, sizeof(STUDENT), compare_for_stu);
	catch_error (NULL);

	/* verilen elemanları eleman sayısınca ekrana bas */
	ANY_STU_ERROR = puts_stus (stus, line_count, arg_char);
	catch_error (NULL);
  
	/* bellekten alınan yeri geri ver */
	free_stus (stus, line_count);
	catch_error (NULL);

	return 0;
}
