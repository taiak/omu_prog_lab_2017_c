/************************************/
/*  name: okul.h  date: 2017        */
/************************************/

/* Error messages must be fixed */
#define ARG_SIZE_ERROR_MESSAGE    "Hata: sadece bir argüman girebilirsiniz!" 
#define UNKNOWN_ARG_ERROR_MESSAGE "Hata: tanınmayan argüman:"
#define SEMESTER_ERROR_MESSAGE    "Hata: devre numarası 1-4 aralığında olmalı!"
#define NOT_UNIQ_ERROR_MESSAGE    "ERROR: Student's aren't uniq!"
#define NO_STREAM_ERROR_MESSAGE   "ERROR: There is no stream!"
#define NO_STUDENT_ERROR_MESSAGE  "ERROR: There is no student!"
#define NO_FILE_ERROR_MESSAGE     "ERROR: There is no file!"
#define CSV_ERROR_MESSAGE         "ERROR: CSV is not in the desired format!!!"

typedef struct student STU;

struct student {
	char *name;
	char *surname;
	char  gender;
	int   semester;
};

/* warnings*/
void not_uniq_warning (char* name, char* surname) {
	fprintf (stderr, "WARNING: %s %s is not uniq!\n", name, surname);
}
/* errors */
void stu_err (char *err_msg) { /* general error */
	fprintf (stderr, "%s\n", err_msg);
	exit    (EXIT_FAILURE);	
}
void csv_error (void) {
	fprintf (stderr, "%s\n", CSV_ERROR_MESSAGE);
	exit    (EXIT_FAILURE);
}
void not_uniq_err (void) {
	fprintf (stderr, "%s\n", NOT_UNIQ_ERROR_MESSAGE);
	exit    (EXIT_FAILURE);
}
void no_stream_err (void) {
	fprintf (stderr, "%s\n", NO_STREAM_ERROR_MESSAGE);
	exit    (EXIT_FAILURE);
}
void no_stu_err (void) {
	fprintf (stderr, "%s\n", NO_STUDENT_ERROR_MESSAGE);
	exit    (EXIT_FAILURE);
}
void no_file_err (void) {
	fprintf (stderr, "%s\n", NO_FILE_ERROR_MESSAGE);
	exit    (EXIT_FAILURE);
}
void arg_size_error (void) {
	fprintf (stderr, "%s\n", ARG_SIZE_ERROR_MESSAGE);
	exit    (EXIT_FAILURE);
}
void semester_error (void) {
	fprintf (stderr, "%s\n", SEMESTER_ERROR_MESSAGE);
	exit    (EXIT_FAILURE);
}
void unknown_arg_error (char *arg) {
	fprintf (stderr, "%s %s\n", UNKNOWN_ARG_ERROR_MESSAGE, arg);
	exit    (EXIT_FAILURE);
}
/*
	READ FIRST:
		unsafe command not control parameter's nullity
		for safety usage, stu must controlled!
*/
int puts_stu_unsafe_common (const STU * const stu) {
	printf ("%d %s %s %c\n", stu->semester, stu->name, stu->surname, stu->gender);
	return 0;
}
int puts_stu_unsafe_gender (const STU * const stu, char gender) {
	if (toupper (stu->gender) == gender)
		printf ("%d %s %s\n", stu->semester, stu->name, stu->surname);
	return 0;
}
int puts_stu_unsafe_semester (const STU * const stu, char semester) {
	if (semester > 4)
		semester = semester - '0';

	if (stu->semester == semester)
		printf ("%s %s %c\n", stu->name, stu->surname, stu->gender);
	return 0;
}
int puts_stu (const STU * const stu) {
	if (stu == NULL)
		no_stu_err ();
	return puts_stu_unsafe_common (stu);
}
int puts_stu_gender (const STU * const stu, char gender) {
	if (stu == NULL)
		no_stu_err ();
	return puts_stu_unsafe_gender (stu, gender);
}
/* list stu if not null and */
int puts_stu_semester (const STU * const stu, char semester) {
	if (stu == NULL)
		no_stu_err ();
	return puts_stu_unsafe_semester (stu, semester);
}
/********************************************************/
/* select function for puts_stu                         */
/* parameter type must ascii                            */
/* if type k,K,E,e,1,2,3,4 will print stus to screen    */
/* if type is 0, so print stus without any property     */
/* if type don't have type, print nothing and return 1  */
/********************************************************/
int puts_stu_selector (const STU * const stu, char type) {
	int result = 1;
	if (stu == NULL)
		no_stu_err ();  
	
	switch (type){
		case 'K':
		case 'k':
		case 'E':
		case 'e':
			result = puts_stu_unsafe_gender (stu, toupper(type));
			break;
		case '1':
		case '2':
		case '3':
		case '4':
			result = puts_stu_unsafe_semester (stu, type);
			break;
		case 0:
		result = puts_stu_unsafe_common (stu);  
	}
	return result;
}
/* print stus till limit number */
int puts_stus (const STU * stu, int limit, char type) {
	while (limit-- > 0)
		puts_stu_selector (stu++, type);

	return 0;
}

/* free functions */
void free_stu (STU *stu) {
	if (stu == NULL)
		no_stu_err ();

	if (stu->name != NULL)
		free (stu->name);

	if (stu->surname != NULL)
		free (stu->surname);
}
/* free first n element*/
void free_stus (STU *stu, int n) {
	while (n-- > 0)
		free_stu (stu++);
}

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
	if (semester_comp != 0) { 
		result = semester_comp;
	}
	else if (name_comp != 0) {
		result = name_comp;
	}
	else {
		result = surname_comp;
	}

	/* if any name and surname is both equal puts error and quit */
	if ((name_comp || surname_comp) == 0) {
		not_uniq_warning (stu1->name, stu1->surname);
		not_uniq_err ();
	}

	return result;
}
/* dosya açılırsa dosyayı açılmazsa hata döner */
FILE *read_file (char *file_name) {
	FILE *file = fopen (file_name, "r");
	
	if (file == NULL)
		no_file_err ();

	return file;
}
/* get line count from stream */
int fgetlinecount (FILE *in_stream){
	int line_count = 1, chr = 'y';
	while (chr != EOF) {
		chr = fgetc(in_stream);
		if (chr == '\n')
			line_count++;
	}
	rewind (in_stream);
	return line_count;
}
/* Delimiter controller */
int fdeliControl (FILE *in_stream, int deli, int deli_limit, int line_limit){
	int chr = 1, count = 0, flag = 1, line = 0;
	
	if ( !line_limit )
		line_limit = fgetlinecount (in_stream);

	while (chr != EOF && flag) {
		chr = fgetc (in_stream);
		
		if (chr == deli)
			count++;

		if (chr == '\n' || chr == EOF){
			if (count != deli_limit)
				flag = 0;
			line++;
			count = 0;
		}
	}
	rewind (in_stream);
	return (flag)? 0: line;
}
/* argument controller for okul.c */
int argument_control (int argc, char *argv) {
	int val = 0;
	/* argument number control */
	if (argc < 1 || 2 < argc)
		arg_size_error ();
	
	/* argument size control */
	if (argc == 2) {
		if (strlen (argv) != 1)
			unknown_arg_error (argv);
		/* eğer değer varsa tek karaktere ata */
		val = *argv;
	}

	/* 1, 2, 3, 4 ? */
	if (isdigit(val) && (val < '1' || '4' < val))
		semester_error ();

	/* E, e, k , K ? */
	if (isalpha(val) && (val != 'k' && val != 'e' && val != 'K' && val != 'E'))
		unknown_arg_error (argv);

	return (EXIT_SUCCESS);
}
/* parse helper */
char *parse_from_comma (void) {
	return strtok (NULL, ",");
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

	if (stream == NULL)
		no_stream_err ();
	
	if (stu == NULL)
		no_stu_err ();
	
	/* MAY-DO: for using stdin, get char by char */
	fscanf (stream, "%s", line);

	if ( strlen(line) < chr_limit )
		return (EXIT_FAILURE);

	cp_with_allocate (strtok (line, ","),  &(stu->name));
	cp_with_allocate (parse_from_comma (), &(stu->surname));
	stu->gender   = *parse_from_comma ();
	stu->semester =  atoi (parse_from_comma ());

	free (line);
	return 0;
}
/* pull first count element from stream and close stream */
int pull_elements (FILE *stream, STU *pStu, int count) {
	if (stream == NULL)
		no_stream_err ();
	int res = 0;

	while (count-- > 0){
		if (get_stu (stream, pStu++, 100, 7))
			res++;
	}

	fclose (stream);
	return res;
}