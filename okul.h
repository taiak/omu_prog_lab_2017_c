/************************************/
/*  name: okul.h  date: 2017        */
/************************************/
#define ARG_SIZE_ERROR_MESSAGE    "Hata: sadece bir argüman girebilirsiniz!" 
#define UNKNOWN_ARG_ERROR_MESSAGE "Hata: tanınmayan argüman:"
#define SEMESTER_ERROR_MESSAGE    "Hata: devre numarası 1-4 aralığında olmalı!"
#define NOT_UNIQ_ERROR_MESSAGE    "Hata: Öğrencilerden bazıları tekrarlanıyor!"
#define STREAM_ERROR_MESSAGE      "Hata: Herhangi bir akış bulunamadı!"
#define STUDENT_ERROR_MESSAGE     "Hata: Öğrenci bulunamadı!"
#define FILE_ERROR_MESSAGE        "Hata: Verilen dosya bulunamadı!"
#define CSV_ERROR_MESSAGE         "Hata: CSV formatı hatalı!"

typedef struct student STU;

struct student {
	char *name;
	char *surname;
	char  gender;
	int   semester;
};

void not_uniq_warning (char* name, char* surname) {
	fprintf (stderr, "WARNING: %s %s is not uniq!\n", name, surname);
}
void stu_error (const char *err_msg) { /* general error */
	fprintf (stderr, "%s\n", err_msg);
	exit    (EXIT_FAILURE);	
}
void unknown_arg_error (char *arg) {
	fprintf (stderr, "%s %s\n", UNKNOWN_ARG_ERROR_MESSAGE, arg);
	exit    (EXIT_FAILURE);
}

void student_control (const STU * const stu) {
	if (stu == NULL)
		stu_error (STUDENT_ERROR_MESSAGE);
}
void stream_control (const FILE * const stream) {
	if (stream == NULL)
		stu_error (STREAM_ERROR_MESSAGE);
}

int puts_stu (const STU * const stu) {
	student_control (stu);
	printf ("%d %s %s %c\n", stu->semester, stu->name, stu->surname, stu->gender);
	return 0;
}

int puts_stu_gender (const STU * const stu, char gender) {
	student_control (stu);
	if (toupper (stu->gender) == gender)
		printf ("%d %s %s\n", stu->semester, stu->name, stu->surname);

  return 0;
}

int puts_stu_semester (const STU * const stu, char semester) {
	student_control (stu);

	if (stu->semester == (semester - '0'))
		printf ("%s %s %c\n", stu->name, stu->surname, stu->gender);
	return 0;
}

/* if type k,K,E,e,1,2,3,4 will print stus to screen    */
/* if type is 0, so print stus without any property     */
/* if type don't have type, print nothing and return 1  */
int puts_stu_selector (const STU * const stu, char type) {
	int result = 1;
	student_control (stu);  
	type = toupper (type);

	switch (type) {
		case 'K':
		case 'E':
			result = puts_stu_gender (stu, type);
			break;
		case '1':
		case '2':
		case '3':
		case '4':
			result = puts_stu_semester (stu, type);
			break;
		case 0:
			result = puts_stu (stu);  
	}
	return result;
}

/* print stus till limit number */
int puts_stus (const STU * stu, int limit, char type) {
	while (limit-- > 0)
		puts_stu_selector (stu++, type);

	return 0;
}

void free_stu (STU *stu) {
	student_control (stu);

	if (stu->name != NULL)
		free (stu->name);

	if (stu->surname != NULL)
		free (stu->surname);
}

/* free first n element */
void free_stus (STU *stu, int n) {
	while (n-- > 0)
		free_stu (stu++);
}

/* comparison for qsort */
int compare_for_stu (const void *s1, const void *s2)
{
	int name_comp, surname_comp, semester_comp, result;
	STU *stu1, *stu2;

	stu1 = (STU *) s1;
	stu2 = (STU *) s2;
	
	name_comp     = strcmp (stu1->name, stu2->name);
	surname_comp  = strcmp (stu1->surname, stu2->surname);
	semester_comp = stu1->semester - stu2->semester;

	/* select if any comparison not equal */
	/* same semester so sort by name and surname */
	if (semester_comp != 0) 
		result = semester_comp;
	else if (name_comp != 0)
		result = name_comp;
	else
		result = surname_comp;

	/* if any name and surname is both equal puts error and quit */
	if ((name_comp || surname_comp) == 0) {
		not_uniq_warning (stu1->name, stu1->surname);
		stu_error (NOT_UNIQ_ERROR_MESSAGE);
	}

	return result;
}

/* dosya açılırsa dosyayı açılmazsa hata döner */
FILE *read_file (char *file_name) {
	FILE *file = fopen (file_name, "r");
	
	if (file == NULL)
		stu_error (FILE_ERROR_MESSAGE);

	return file;
}

/* get line count from stream */
int fgetlinecount (FILE *stream){
	int line_count = 1, chr = 'y';

	while ((chr = fgetc (stream)) != EOF)
		if (chr == '\n')
			line_count++;

	rewind (stream);

	return line_count;
}

/* Delimiter controller */
int fdeliControl (FILE *stream, int deli, int deli_limit, int line_limit){
	int chr = 1, count = 0, flag = 1, line = 0;
	
	if ( !line_limit )
		line_limit = fgetlinecount (stream);

	while ((chr = fgetc (stream)) != EOF && flag) {
		if (chr == deli)
			count++;

		if (chr == '\n' || chr == EOF) {
			if (count != deli_limit)
				flag = 0;

			line++;
			count = 0;
		}
	}
	rewind (stream);
	return (flag)? 0: line;
}

/* argument controller for okul.c */
int argument_control (int argc, char *argv) {
	int val = 0;
	/* argument number control */
	if (argc < 1 || 2 < argc)
		stu_error (ARG_SIZE_ERROR_MESSAGE);
	
	/* argument size control */
	if (argc == 2) {
		if (strlen (argv) != 1)
			unknown_arg_error (argv);
		/* eğer değer varsa tek karaktere ata */
		val = *argv;
	}

	/* 1, 2, 3, 4 ? */
	if (isdigit (val) && (val < '1' || '4' < val))
		stu_error (SEMESTER_ERROR_MESSAGE);

	/* E, e, k , K ? */
	if (isalpha (val) && (val != 'k' && val != 'e' && val != 'K' && val != 'E'))
		unknown_arg_error (argv);

	return (EXIT_SUCCESS);
}

/* allocate memory token size and copy token into destination */
void cp_with_allocate (char *token, char **dest) {
	*dest = (char *) malloc (strlen (token) * sizeof (char));
	strcpy (*dest, token);
}

/* get a stu from stream */
int get_stu (FILE *stream, STU *stu, int buff_size, int chr_limit) {
	char *token, *line;
	line = (char *) malloc (buff_size * sizeof (char));

	stream_control  (stream);
	student_control (stu);
	
	/* read line from stream*/
	fscanf (stream, "%s", line);

  /* split and spill line */
	cp_with_allocate (strtok (line, ","), &(stu->name));
	cp_with_allocate (strtok (NULL, ","), &(stu->surname));
	stu->gender   =  *strtok (NULL, ",");
	stu->semester =  atoi (strtok (NULL, ","));

	free (line);
	return 0;
}

/* pull first count element from stream and close stream */
int pull_elements (FILE *stream, STU *pStu, int count) {
	int res = 0;
	stream_control (stream);

	while (count-- > 0) {
		if (get_stu (stream, pStu++, 100, 7))
			res++;
	}

	fclose (stream);
	return res;
}
