#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include <ctype.h>
#include "okul.h"
#include "minunit.h"

FILE *file;
int line_count;
STUDENT stus[23];
STUDENT student1 = {"Ibrahim", "Avcu", 'e', 1};
STUDENT student2 = {"Ibrahim", "Avcu", 'e', 1};
STUDENT student3 = {"Yasir", "Kiroglu", 'e', 2};
STUDENT student4 = {"Hulya", "Macaroglu", 'k', 3};
STUDENT student5 = {"Erkan", "Gunduz", 'e', 4};

void test_setup (void) {
	file  = read_file ("okul.csv");
	line_count = fgetlinecount (file);
}

void test_teardown (void) {
	/* Nothing */
}

MU_TEST (readfile) {
	fprintf (stderr, "\n->Dosya okuma hataları:\n\n");
	mu_assert (file != NULL, "read file should not be null");
}

MU_TEST (ispullstus) {
	fprintf (stderr, "\n->Öğrenci çekme hataları:\n\n");
	mu_assert_int_eq (0, pull_elements  (file, stus, line_count));
}

MU_TEST (argument_controlling) {
	fprintf (stderr, "\n->Argüman kontrol hataları:\n\n");

	/* iki argüman */
	mu_assert_int_eq (1, argument_control (3, "deneme cumlesi"));

	/* anlamsız kelime */
	mu_assert_int_eq (2, argument_control (2, "5tre"));

	/* gecersiz sayi */
	mu_assert_int_eq (3, argument_control (2, "5"));

	/* geçersiz harf */
	mu_assert_int_eq (4, argument_control (2, "u"));

	/* basarili parametreler */
	mu_assert_int_eq (0, argument_control (1, NULL));
	mu_assert_int_eq (0, argument_control (2, "e"));
	mu_assert_int_eq (0, argument_control (2, "k"));
	mu_assert_int_eq (0, argument_control (2, "E"));
	mu_assert_int_eq (0, argument_control (2, "k"));
	mu_assert_int_eq (0, argument_control (2, "1"));
	mu_assert_int_eq (0, argument_control (2, "2"));
	mu_assert_int_eq (0, argument_control (2, "3"));
	mu_assert_int_eq (0, argument_control (2, "4"));

	/* argc ağırlıklı tasarım yüzünden, argc == 1 ise argv önemsizdir */
	mu_assert_int_eq (0, argument_control (1, "rastgele ifade"));	
}

MU_TEST (checkcollusion) {
	fprintf (stderr, "\n->Karşılaştırma fonksiyonu hataları:\n\n");

	/* student1 = student2 */
	mu_assert_int_eq (0, compare_for_stu  (&student1, &student2));

	/* student3 < student2 ise */
	mu_assert_int_eq (-1, compare_for_stu (&student2, &student3));

	/* student4 > student3 ise */
	mu_assert_int_eq (1, compare_for_stu  (&student4, &student3));	
}

MU_TEST (puts_test) {
	fprintf (stderr, "\n->Çıktı karşılaştırma fonksiyonu hataları:\n\n");
	buffer_on (&puts_buffer, PUTS_BUFFER_LIMIT);
	
	/* başarısızlar */
	mu_assert_int_eq (-1, student_gender (&student1, puts_buffer, 'k'));
	mu_assert_string_eq ("", puts_buffer);

	mu_assert_int_eq (-1, student_semester (&student2, puts_buffer, '2'));
	mu_assert_string_eq ("", puts_buffer);

	mu_assert_int_eq (-1, student_semester (&student2, puts_buffer, 1));
	mu_assert_string_eq ("", puts_buffer);

	/* başarılılar */
	mu_assert_int_eq (0, student (&student1, puts_buffer));
	mu_assert_string_eq ("1 Ibrahim Avcu e\n", puts_buffer);

	mu_assert_int_eq (0, student_gender (&student3, puts_buffer, 'e'));
	mu_assert_string_eq ("2 Yasir Kiroglu\n", puts_buffer);

	mu_assert_int_eq (0, student_gender (&student4, puts_buffer, 'k'));
	mu_assert_string_eq ("3 Hulya Macaroglu\n", puts_buffer);

	mu_assert_int_eq (0, student_semester (&student5, puts_buffer, '4'));
	mu_assert_string_eq ("Erkan Gunduz e\n", puts_buffer);

	buffer_off (&puts_buffer);
}

MU_TEST_SUITE (test_suite) {
	MU_SUITE_CONFIGURE (&test_setup, &test_teardown);

	MU_RUN_TEST (readfile);
	MU_RUN_TEST (ispullstus);
	MU_RUN_TEST (argument_controlling);
	MU_RUN_TEST (checkcollusion);
	MU_RUN_TEST (puts_test);
}

int main (int argc, char *argv[]) {
	MU_RUN_SUITE (test_suite);
	MU_REPORT ();
	return 0;
}
