/************************************/
/*  name: okul.h  date: 2017        */
/************************************/
#define BUFF_LIMIT                100
#define check_control(control)    (control > 0)?0:-1

int ANY_STU_ERROR = 0;
char *read_buffer;

typedef struct student STUDENT;

struct student {
	char *name;
	char *surname;
	char  gender;
	int   semester;
};

void stu_error (const char *err_msg) { /* general error */
	fprintf (stderr, "%s\n", err_msg);
	ANY_STU_ERROR += 1;
}

void catch_error (const char *err_msg) {
	if (ANY_STU_ERROR){
		if (err_msg != NULL)
			stu_error (err_msg);
		exit (EXIT_FAILURE);
	}
}

void unknown_arg_error (char *arg) {
	fprintf (stderr, "Hata: tanınmayan argüman: %s\n", arg);
	ANY_STU_ERROR += 1;
}

void student_control (const STUDENT * const stu) {
	if (stu == NULL)
		stu_error ("Hata: Öğrenci bulunamadı!");
}

void stream_control (const FILE * const stream) {
	if (stream == NULL)
		stu_error ("Hata: Herhangi bir akış bulunamadı!");
}

int puts_stu (const STUDENT * const stu) {
	student_control (stu);
	int control = -1;
	/* if printf fail, it return negative number */
	control = printf ("%d %s %s %c\n", stu->semester, stu->name,
					stu->surname, stu->gender);

	return check_control (control);
}

int puts_stu_gender (const STUDENT * const stu, char gender) {
	student_control (stu);

	int control = -1;

	/* if printf fail, it return negative number */
	if (toupper (stu->gender) == gender)
		control = printf ("%d %s %s\n", stu->semester, 
					stu->name, stu->surname);

	return check_control (control);
}

int puts_stu_semester (const STUDENT * const stu, char semester) {
	student_control (stu);

	int control = -1;

	/* if printf fail, it return negative number */
	if (stu->semester == (semester - '0'))
		control = printf ("%s %s %c\n", stu->name,
					stu->surname, stu->gender);

	return check_control (control);
}

/* if type k,K,E,e,1,2,3,4 will print stus to screen    */
/* if type is 0, so print stus without any property     */
/* if type don't have type, print nothing and return -1  */
int puts_stu_selector (const STUDENT * const stu, char type) {
	int result = -1;
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
		default:
			result = puts_stu (stu);
			break;
	}
	return result;
}

/* print stus till limit number */
int puts_stus (const STUDENT * stu, int limit, char type) {
	student_control (stu);

	int control = 0;

	while (limit-- > 0 && stu != NULL){
		control = puts_stu_selector (stu++, type);
	}

	return control;
}

int free_stu (STUDENT *stu) {
	student_control (stu);
	if (ANY_STU_ERROR) {
		return 1;
	}

	if (stu->name != NULL)
		free (stu->name);

	if (stu->surname != NULL)
		free (stu->surname);

	return 0;
}

/* free first n element */
int free_stus (STUDENT *stu, int n) {
	int control = 0;
	student_control (stu);

	if (ANY_STU_ERROR)
		return 1;

	while (n-- > 0 && stu != NULL && !control)
		control += free_stu (stu++);

	return control;
}

/* comparison for qsort */
int compare_for_stu (const void *s1, const void *s2)
{
	int name_comp, surname_comp, semester_comp, result;
	STUDENT *stu1, *stu2;

	stu1 = (STUDENT *) s1;
	stu2 = (STUDENT *) s2;
	
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

	/* if any name and surname is both equal puts error*/
	if ((name_comp || surname_comp) == 0)
		stu_error ("Hata: Öğrencilerden bazıları tekrarlanıyor!");

	return result;
}

/* dosya açılırsa dosyayı açılmazsa hata döner */
FILE *read_file (char *file_name) {
	FILE *file = fopen (file_name, "r");

	if (file == NULL)
		stu_error ("Hata: Verilen dosya bulunamadı!");

	return file;
}


/* argument controller for okul.c */
int argument_control (int argc, char *argv) {
	int val = 0;
	/* argument number control */
	if (2 < argc || argc < 1) {
		stu_error ("Hata: sadece bir argüman girebilirsiniz!");
		return 1;
	}
	
	/* argument size control */
	if (argc == 2) { 
		if (strlen (argv) != 1) {
			unknown_arg_error (argv);
			return 2;
		}
		/* only one character acceptable */
		val = *argv;
	}

	/* 1, 2, 3, 4 ? */
	if (isdigit (val) && (val < '1' || '4' < val)) {
		stu_error ("Hata: devre numarası 1-4 aralığında olmalı!");
		return 3;
	}

	/* E, e, k , K ? */
	if (isalpha (val) && (val != 'k' && val != 'e' && val != 'K' && val != 'E')) {
		unknown_arg_error (argv);
		return 4;
	}

	return (EXIT_SUCCESS);
}

/* allocate memory token size and copy token into destination */
int cp_with_allocate (char *token, char **dest) {
	*dest = (char *) malloc (strlen (token) * sizeof (char));

	/* error handling for segmentation fault */
	if (*dest == NULL) {
		stu_error ("Hata: Bellekten yer almada sorun yaşandı!");
		return -1;
	}
	
	strcpy (*dest, token);
	return 0;

}

/* get a stu from stream */
int get_stu (FILE *stream) {
	stream_control (stream);
	
	if (ANY_STU_ERROR)
		return 1;
	
	/* read line from stream and write into read_buffer*/
	if( !fscanf (stream, "%s", read_buffer) ) {
		stu_error ("Hata: Akıştan veri okumada sorun yaşandı!");
		return 2;
	}
	return 0;
}
/* read from read_buffer and split it into stu's part */
int chomp_into_stu (STUDENT* stu) {
	int control = 0;

	control += cp_with_allocate (strtok (read_buffer, ","), &(stu->name));
	control += cp_with_allocate (strtok (NULL, ","), &(stu->surname));
	stu->gender   =  *strtok (NULL, ",");
	stu->semester =  atoi (strtok (NULL, ","));

	return control;
}
/* activate buffer */
int buffer_on () {
	read_buffer = (char *) malloc (BUFF_LIMIT * sizeof (char));
}
/* free buffer */
int buffer_off () {
	free (read_buffer);
}

/* pull first count element from stream and close stream */
int pull_elements (FILE *stream, STUDENT *pStu, int count) {
	int warning = 0;
	stream_control (stream);
	/* open buffer for read */
	buffer_on ();

	if (read_buffer == NULL) {
		stu_error ("Hata: Bellekten yer almada sorun yaşandı!");
		return 1;
	}

	while (count-- > 0) {
		if (get_stu (stream) || chomp_into_stu (pStu++))
			warning++;
	}

	/* close read buffer */
	buffer_off ();
	fclose (stream);
	return warning;
}
/* get line count from stream */
int fgetlinecount (FILE *stream){
	int line_count = 1, chr = 'y';

	while ((chr = fgetc (stream)) != EOF)
		if (chr == '\n')
			line_count++;

	/* back to start of stream */
	rewind (stream);

	return line_count;
}

/* Delimiter controller */
int fdelimitercontrol (FILE *stream, int deli, int deli_limit, int line_limit){
	int chr = 1, count = 0, flag = 1, line = 0;
	
	if (!line_limit)
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
