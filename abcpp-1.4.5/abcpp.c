/* ************************************************************************ *
 *
 * --- File: abcpp.c
 *
 * --- Purpose: simple preprocessor for abc music files.
 *
 * --- Copyright (C) Guido Gonzato, guido.gonzato at gmail.com
 *     Modifications by John Fattaruso, johnf@ti.com,
 *     Ewan A. Macpherson emacpher@umich.edu,
 *     D. Glenn Arthur Jr. dglenn@radix.net, Stuart Soloway 
 *     stuart.soloway@ieee.org, and Steve West steve.m.west@btinternet.com
 *     Code rewrite by Christopher David Lane, cdl at cdl.best.vwh.net
 *
 *     So many people for such a simple program!
 *
 * --- Last updated: 2-September-2012
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 * ************************************************************************ */

/* This is a no-brainer program. No efficient memory allocation,
 * no lists, no trees or some such. Only fixed-length arrays for now!
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdarg.h>
#include <assert.h> /* assert() macros can be compiled away via -DNDEBUG */

#define PROGNAME "abcpp"
#define DATE "2 September 2012"
#define MAX_TOKENS 20 /* # of tokens following #ifdef etc */
#define MAX_MACROS 512 /* # of defined symbols, hopefully enough for microabc */
#define NAME_LENGTH 50 /* maximum name length, ~ (LINE_LENGTH / MAX_TOKENS) */
#define DEFINITION_LENGTH 975 /* maximum definition length, ~ (LINE_LENGTH - NAME_LENGTH) */
#define LINE_LENGTH 1024 /* maximum length of input line */
#define MAX_EXPANSIONS (MAX_MACROS * LINE_LENGTH) /* to detect runaway recursive expansion */
#ifdef WIN32
#define LIB_DIR "C:\\ABCPP\\"
#else
#define LIB_DIR "/usr/share/abcpp/"
#endif

#ifndef TRUE
#define TRUE (1)
#endif
#ifndef FALSE
#define FALSE (0)
#endif
typedef int BOOLEAN;

#define BREAK "!break!"
#define UNBALANCED "Unbalanced '%c' character found - the line is probably wrong."

#define equal_string(s1, s2) (0 == strcmp(s1, s2))
#define copy_string(dst, src, len) ((strncpy(dst, src, (size_t) len - 1))[len - 1] = '\0')
#ifndef sizeofA
#define sizeofA(array) (sizeof(array) / sizeof(array[0])) /* from <c.h> */
#endif

/* function prototypes */

void define_macro (const char *, const char *, unsigned int);
void delete_string (char *, size_t);
char *detect_field (const char *);
void error (int, const char *, ...);
void handle_directive (const char *, FILE *);
void include_file (const char *, FILE *);
void insert_string (char *, size_t, const char *);
void output_line (char *, FILE *);
void preprocess_file (FILE *, FILE *);
void remove_bang (char *, BOOLEAN);
void remove_delimited (char *, char, const char *);
void replace_delimiter (char *, const char *, const char *);
int replace_string (char *, const char *, const char *);
void undefine_macro (const char *);
void usage (void);
void warning (int, const char *, ...);
unsigned get_terminated_line(char *line, unsigned line_length, FILE *in);

/* global variables */

size_t nmacros = 0; /* # size of macros[] array */
BOOLEAN condition = TRUE; /* condition after #ifdef */
BOOLEAN cond_else = FALSE; /* condition for #else */
BOOLEAN ifdef = FALSE; /* #ifdef was read */

/* program options */

BOOLEAN strip = FALSE; /* strip option */
BOOLEAN strip_chords = FALSE; /* strip chords option */
BOOLEAN strip_bang = FALSE; /* remove single '!' */
BOOLEAN change_bang_to_break = FALSE; /* change single '!' to !break! */
BOOLEAN change_plus_to_bracket = FALSE; /* change +abc+ to [abc] */
BOOLEAN change_plus_to_bang = FALSE; /* change +fermata+ to !fermata! */
BOOLEAN change_bang_to_plus = FALSE; /* change !fermata! to +fermata+ */
BOOLEAN undefine = FALSE; /* #undefine was read */
BOOLEAN doremi = FALSE; /* #define 'do', 're', 'mi'... */
BOOLEAN externals_override = FALSE; /* (DGA) command line definitions override #define */
BOOLEAN suppress_warnings = FALSE; /* turns off printing of warnings */
BOOLEAN warnings_fatal = FALSE; /* turns warnings into fatal errors (for scripting) */

enum directives {
	COMMENT, ABC, DEFINE, DOREMI, ELIFDEF,
	ELIFNDEF, ELSE, ENDIF, IFDEF, IFNDEF,
	INCLUDE, REDEFINE, RESUME, SUSPEND, UNDEFINE
	};

const char *directives[] = {
	"#", "#abc", "#define", "#doremi", "#elifdef",
	"#elifndef", "#else", "#endif", "#ifdef", "#ifndef",
	"#include", "#redefine", "#resume", "#suspend", "#undefine"
	};

#define ANNOTATION_CHARACTERS "^_<>@"

enum symbols { NONE = 0, MACRO = 1, SYMBOL = 2, SOLFEGE = 4, EXTERNAL = 8 };

struct macro {
	char name[NAME_LENGTH];
	char definition[DEFINITION_LENGTH];
	unsigned int flags;
} macros[MAX_MACROS];

#define is_macro(x) (MACRO == (MACRO & x.flags))
#define is_macro_ptr(x) (MACRO == (MACRO & x->flags))
#define is_external_ptr(x) (EXTERNAL == (EXTERNAL & x->flags))

/* this array contains Latin note macros */

struct macro solfege[] = {
	{"DO", "C", SOLFEGE}, {"RE", "D", SOLFEGE}, {"MI", "E", SOLFEGE}, {"FA", "F", SOLFEGE},
	{"SOL", "G", SOLFEGE}, {"LA", "A", SOLFEGE}, {"SI", "B", SOLFEGE},
	{"do", "c", SOLFEGE}, {"re", "d", SOLFEGE}, {"mi", "e", SOLFEGE}, {"fa", "f", SOLFEGE},
	{"sol", "g", SOLFEGE}, {"la", "a", SOLFEGE}, {"si", "b", SOLFEGE}
};

typedef int (*COMPARE)(const void *, const void *);

#define MACRO_SIZE sizeof(struct macro)
#define sort_macros() qsort(macros, nmacros, MACRO_SIZE, (COMPARE) strcmp)
#define find_macro(name) bsearch(name, macros, nmacros, MACRO_SIZE, (COMPARE) strcmp)

struct context {
	int line_number; /* # of line being processed */
	char filename[FILENAME_MAX]; /* name of file being processed */
} global = { 0, "stdin" }, *current = &global;

/* ----- */

int main (int argc, char *argv[])
{
	/* stdin is the default unless infile is specified on the command line;
	 * stdout is the default unless both infile and outfile are specified
	 * on the command line
	 */

	FILE *in = stdin, *out = stdout;
	char input[FILENAME_MAX] = "", output[FILENAME_MAX] = "";
	int i, nfilespec = 0;

	/* parse command line */
	for (i = 1; i < argc; i++) {
		if ('-' == argv[i][0]) {
			char *index;

			/* check if it's a built-in switch */
			if (equal_string("-s", argv[i])) strip = TRUE;
			else if (equal_string("-c", argv[i])) strip_chords = TRUE;
			else if (equal_string("-p", argv[i])) change_plus_to_bracket = TRUE;
			else if (equal_string("-n", argv[i])) change_plus_to_bang = TRUE;
			else if (equal_string("-a", argv[i])) change_bang_to_plus = TRUE;
			else if (equal_string("-b", argv[i])) strip_bang = TRUE;
			else if (equal_string("-k", argv[i])) change_bang_to_break = TRUE;
			else if (equal_string("-o", argv [i])) externals_override = TRUE;
			else if (equal_string("-w", argv [i])) suppress_warnings = TRUE;
			else if (equal_string("-e", argv [i])) warnings_fatal = TRUE;
			else if (equal_string("-h", argv[i])) usage();
			else if (NULL != (index = strchr(argv[i], '='))) {
				/* (DGA) command line macro definition, look for '='
				 * within token and divide into name and definition
				 */
				char name[NAME_LENGTH], definition[DEFINITION_LENGTH];
				int name_length = (1 + index - &argv[i][1]) % NAME_LENGTH;

				copy_string(name, &argv[i][1], (size_t) name_length);
				copy_string(definition, ++index, DEFINITION_LENGTH);

				define_macro(name, definition, MACRO | EXTERNAL);
			} else {
				/* it's a command-line define */
				define_macro(&argv[i][1], &argv[i][1], SYMBOL | EXTERNAL);
			}
		} else { /* it's a filespec */
			switch (nfilespec) {
				/* no files specified yet, so this is the infile spec */
				case 0:
					copy_string(input, argv[i], FILENAME_MAX);

					if (NULL == (in = fopen(input, "r"))) {
						error(0, "Can't open '%s' for input.", input);
					}

					copy_string(global.filename, input, FILENAME_MAX);

					nfilespec++;

					break;
				/* 1 file specified already, so this is the outfile spec */
				case 1:
					copy_string(output, argv[i], FILENAME_MAX);

					/* check if input and output are the same file */
					if (equal_string(input, output)) {
						error(0, "Input (%s) and output (%s) cannot be the same.", input, output);
					}

					if (NULL == (out = fopen (output, "w"))) {
						error(0, "Can't open '%s' for output.", output);
					}

					nfilespec++;

					break;
				/* 2 files specified already - error! */
				default:
					error(0, "Too many files specified.");
			} /* switch */
		} /* else */
	} /* for */

	if (suppress_warnings && warnings_fatal) {
		warning(0, "both -w and -e specified - unpredictable behaviour.");
	}

	if (strip_bang && change_bang_to_break) {
		warning(0, "both -b and -k specified - unpredictable behaviour.");
	}
	
	if (change_plus_to_bang && change_plus_to_bracket) {
		error(0, "both -n and -p specified - inconsistent behaviour.");
	} else if (change_plus_to_bang && change_bang_to_plus) {
		warning(0, "both -n and -a specified - unpredictable behaviour.");
	}

	preprocess_file(in, out);

	(void) fclose(in);
	(void) fclose(out);

	return EXIT_SUCCESS;
} /* main() */

/* ----- */

void usage (void)
{
        fprintf(stderr, "%s, %s  %s\n", PROGNAME,VERSION, DATE);
        fprintf(stderr, "Copyright 2001-2007 Guido Gonzato <guido.gonzato at gmail.com> and others\n");
	fprintf(stderr, "This is free software with ABSOLUTELY NO WARRANTY.\n\n");
	fprintf(stderr, "Usage:\n");
	fprintf(stderr, "%s [-s] [-c] [-p|-n|-a] [-b|-k] [-o] [-SYM -SYM=def ...] [input] [output]\n\n", PROGNAME);
	fprintf(stderr, "-s:\tstrip input of w: fields and decorations\n");
	fprintf(stderr, "-c:\tstrip input of accompaniment \"chords\"\n");
	fprintf(stderr, "-p:\tchange old +abc+ style chords to new [abc] style\n");
	fprintf(stderr, "-n:\tchange +plus+ style decorations to !plus! style\n");
	fprintf(stderr, "-a:\tchange !plus! style decorations to +plus+ style\n");
	fprintf(stderr, "-b:\tremove single '!'\n");
	fprintf(stderr, "-k:\tchange single '!' to !break! (or +break+ if -a is set)\n");
	fprintf(stderr, "-o:\tcommand line macros override defines in input\n");
	fprintf(stderr, "-w:\tsuppress warnings\n");
	fprintf(stderr, "-e:\tturn warnings into fatal errors\n");
	fprintf(stderr, "-h:\tshow usage\n");

	exit(EXIT_SUCCESS);
} /* usage() */

/* ----- */

/* Delete count characters off beginning of string. */
void delete_string (char *string, size_t count)
{
	size_t length = strlen(string);

	assert(0 != count && count <= length);

	(void) memmove(string, string + count, 1 + length - count);
} /* delete_string() */

/* ----- */

/* Insert onto beginning of string, replacing count characters. */
void insert_string (char *string, size_t count, const char *insert)
{
	size_t string_length = strlen(string), insert_length = strlen(insert);

	if (insert_length != count) { /* move characters out of the way */
		if (insert_length > count) assert(string_length + insert_length - count < LINE_LENGTH);
		(void) memmove(string + insert_length, string + count, 1 + string_length - count);
	}

	/* then insert */
	(void) strncpy(string, insert, insert_length);
} /* insert_string() */

/* ----- */

int replace_string (char *string, const char *old, const char *new)
{
	char *position;
	int count = 0, offset = 0;
	size_t old_length = strlen(old), new_length = strlen(new);

	assert(0 != old_length); /* if old is "", strstr() returns string! */

	/* replace all occurrances of 'old' in 'string' with 'new'. The
	 * character '\' can be used to prevent 'old' from being replaced,
	 * if need be. For example, if 'do' is #defined as 'c',
	 * Guido -> Guic, Gui\do -> Guido.
	 */
	while (NULL != (position = strstr(string + offset, old))) {
		int index = position - string;

		if (0 < index && '\\' == string[index - 1]) { /* remove the '\' */
			delete_string(string + index - 1, 1);
			offset = index; /* start searching from this position */
		} else { /* if the text to replace isn't preceded by '\', go ahead */
			insert_string(string + index, old_length, new);
			offset = index + (int) new_length;
			count++;
		}

	} /* while */

	return count;
} /* replace_string() */

/* ----- */

void define_macro (const char *name, const char *definition, unsigned int flags)
{
	const char *next = name;
	struct macro *itemptr = find_macro(name);

	/* find name to see if it's already there */

	if (NULL != itemptr) { /* found - already defined */
		if (is_external_ptr(itemptr) && externals_override) {
			warning(1, "Symbol '%s' can't be changed - redefinition ignored.", name);
		} else {
			itemptr->flags = flags;
			copy_string(itemptr->definition, definition, DEFINITION_LENGTH);
			if (is_macro_ptr(itemptr)) {
				(void) replace_string(itemptr->definition, "~", " ");
			}
			warning(1, "Symbol '%s' redefined.", name);
		}
	} else if (MAX_MACROS == nmacros) {
		warning(1, "Maximum macros reached (%d) - symbol '%s' ignored.", MAX_MACROS, name);
	} else {
		macros[nmacros].flags = flags;
		copy_string(macros[nmacros].definition, definition, DEFINITION_LENGTH);
		if (is_macro(macros[nmacros])) {
			(void) replace_string(macros[nmacros].definition, "~", " ");
		}
		copy_string(macros[nmacros].name, name, NAME_LENGTH);
		nmacros++;
		sort_macros(); /* sort macros[] whenever we add/delete one */
	}

	while (NULL != (itemptr = find_macro(next)) && is_macro_ptr(itemptr)) {
		if (NULL != strstr((next = itemptr->definition), name)) {
			error(1, "Infinitely expanding macro '%s' detected.", name);
		}
	}
} /* define_macro() */

/* ----- */

void undefine_macro (const char *name)
{
	struct macro *itemptr = find_macro(name);

	if (NULL != itemptr) {
		if (is_external_ptr(itemptr) && externals_override) {
			warning(1, "Symbol '%s' can't be changed - undefine ignored.", name);
		} else { /* found name so remove it */
			--nmacros;
			itemptr->flags = macros[nmacros].flags;
			copy_string(itemptr->definition, macros[nmacros].definition, DEFINITION_LENGTH);
			copy_string(itemptr->name, macros[nmacros].name, NAME_LENGTH);
			sort_macros(); /* sort macros[] whenever we add/delete one */
		}
	} else { /* not found */
		warning(1, "Symbol '%s' not defined - undefine ignored.", name);
	}
} /* undefine_macro() */

/* ----- */

void remove_delimited (char *string, char delimiter, const char *exceptions)
{
	char *begin, *end;
	int position = 0, removed = 0;

	while (NULL != (begin = strchr(string + position, delimiter))) {
		BOOLEAN is_exception = (exceptions != NULL && strspn(begin + 1, exceptions) > 0);

		position = begin - string; /* opening delimiter found */

		if (NULL != (end = strchr(begin + 1, delimiter))) {
			if (is_exception) { /* e.g. annotation string, "^Allegro", skip over it */
				position = end - string + 1;
			} else { /* closing delimiter found */
				int count = end - begin + 1;

				delete_string(string + position, (size_t) count);
				removed += count;
			}
		} else {
			if ('!' != delimiter) warning(position + removed + 1, UNBALANCED, delimiter);
			break; /* while */
		}
	} /* while */
} /* remove_delimited() */

/* ----- */

void replace_delimiter (char *string, const char *old, const char *new)
{
	int skip = 0;
	char *begin, *end;
	enum { BEGIN, END };

	while (NULL != (begin = strchr(string + skip, old[BEGIN]))) {
		/* old starting delimiter found */
		if (NULL != (end = strchr(begin + 1, old[END]))) {
			/* old ending delimiter found; replace with new delimiters */
			*begin = new[BEGIN];
			*end = new[END];

			skip = end - string + 1;
		} else {
			if ('!' != old[BEGIN]) warning(begin - string + 1, UNBALANCED, old[BEGIN]);
			break; /* while */
		}
	} /* while */
} /* replace_delimiter () */

/* ----- */

#define BREAK_LENGTH strlen(BREAK)

void remove_bang (char *string, BOOLEAN write_break)
{
	int skip = 0;
	char *pointer;

	while (NULL != (pointer = strchr(string + skip, '!'))) {
		int offset = pointer++ - string; /* '!' found */

		if ('\0' == *pointer || isspace(*pointer)) {
			/* replace or remove, it cannot be a !decoration! */
			skip = offset;

			if (write_break) {
				insert_string(string + offset, 1, BREAK);
				/* skip over !break! characters */
				skip += BREAK_LENGTH;
			}
			else {
				delete_string(string + offset, 1);
			}
		} else if (NULL != (pointer = strchr(pointer, '!'))) {
			/* it's a decoration, skip completely over it */
			skip = pointer - string + 1;
		} else {
			warning(offset + 1, UNBALANCED, '!');
			break; /* while */
		}
	} /* while */
} /* remove_bang() */

/* ----- */

void output_line (char *line, FILE *out)
{
	size_t i;
	char *marker, comment[LINE_LENGTH] = "";

	if (!condition) return;

	/* expand macros */

	if (!undefine) {
		int replaced, expansions = 0;

		do {
			for (i = 0, replaced = 0; i < nmacros; i++) {
				if (is_macro(macros[i])) {
					replaced += replace_string(line, macros[i].name, macros[i].definition);
				}
			}

			if (MAX_EXPANSIONS < ++expansions) {
				error(1, "Possible infinitely expanding macro detected.");
			}
		} while (0 < replaced);
	}

	if (NULL != (marker = strchr(line, '%'))) {
		(void) strcpy(comment, marker);
		*marker = '\0';
	}

	/* the following transformations do not apply to % comment lines */

	if (NULL != (marker = detect_field(line))) {
		if (('w' == *marker || 'W' == *marker) && strip) return;
		}
	else {
		/* the following transformations do not apply to : tagged fields */

		/* replace notes */
		if (doremi) {
			for (i = 0; i < sizeofA(solfege); i++) {
				(void) replace_string(line, solfege[i].name, solfege[i].definition);
			}
		}

		/* !!! ADD OPTIONS HERE !!! */

		/* process these before strip to avoid warnings */
		if (change_bang_to_break) remove_bang(line, TRUE);
		else if (strip_bang) remove_bang(line, FALSE);

		if (strip) {
			remove_delimited(line, '!', NULL);
			remove_delimited(line, '+', NULL);
		}

		/* use remove_delimited(line, '"', NULL) to remove annotations also */
		if (strip_chords) remove_delimited(line, '"', ANNOTATION_CHARACTERS);

		/* process change_plus_to_bracket before change_bang_to_plus */
		if (change_plus_to_bracket) replace_delimiter(line, "++", "[]");
	
		if (change_bang_to_plus) {
			replace_delimiter(line, "!!", "++");
		} else if (change_plus_to_bang) {
			replace_delimiter(line, "++", "!!");
		}
	} /* else */

	fprintf(out, "%s%s", line, comment);
} /* output_line() */

/* ----- */

char *detect_field (const char *line)
{
	while (isspace(*line)) line++;

	return ((isalpha(*line) && ':' == *(line + 1)) ? (char *) line : NULL);
}

/* ----- */

void preprocess_file (FILE *in, FILE *out)
{
	char line[LINE_LENGTH];
	current->line_number++;  /* make sure line_number is valid in 
				    get_terminated_line, in case we crash */
	while (get_terminated_line(line, LINE_LENGTH, in)) {
	        if ('#' == line[0]) {
			handle_directive(line, out);
		} else {
			output_line(line, out);
		}
		current->line_number++;
	} /* while */
} /* preprocess_file() */

/*
	get_terminated_line gets a line, fixing up the multiple kinds of line-termination.  It considers
	\r, \n, \r\n, and \n\r to be proper line termination. Upon finding one of these sequences, it 
	replaces it with \n, delimits the line with a null, and returns a 1, indicating success.  If there
	are no more lines to be read because we're at EOF, it returns NULL. For errors (ie, line too big)
	it calls the failure/crash routine error and crashes.

	Calling sequence:

		char *line, a buffer to hold the line.  It must hold a \0 terminator but no cr/lf
		unsigned line_length, the number of bytes it the above buffer.
		FILL *in, the input file descriptor.  It must be open.

	Return:
		0- We have read the entire file.  Nothing has been returned
		1- A line has been returned, delimited by \0
*/
#define IS_TERMINATOR(char_arg) (((char_arg)=='\n')||((char_arg)=='\r'))
unsigned get_terminated_line(char *line,unsigned line_length,FILE *in)
{	unsigned c; /*current character*/
        static unsigned last_terminator=EOF;   /* On entry, this is the cr or lf that terminated the last line.  Or EOF if this is 1st line or we are at EOF. */
	char *linepointer=line;
	c=fgetc(in);
	if(c==EOF)return 0;
	if(line_length<3 || line_length>32767)error(1, "get_terminated_line call invalid.");

	if(IS_TERMINATOR(c))
	{	if(c==last_terminator)
		{	/*  This character is the same as the one that ended the last line.  It indicates
			a blank line.  */
			*linepointer='\n';
			*(linepointer+1)='\0';
			return 1; /*successful completion (last_terminator is set correctly)*/
		}
	}
	else
	{	if(c=='\0') error(1,"Embedded NULL");  /* We currently don't handle embedded NULLs, 
							because \0 is the line terminator.*/ 
		*(linepointer++) =c;  /* this character wasn't a terminator; save it.  We know there is room; we are
				at the beginning of the buffer.*/
	}
	while((c=fgetc(in))!=EOF)
	  {     
	        if((linepointer-line)>=line_length-1)error(line_length, "Line too long");  /*line 2 big (need room for c and NULL*/
		if(c=='\0') error(linepointer-line+1,"Embedded NULL");
		if(IS_TERMINATOR(c))
		{       *(linepointer++)='\n'; /*Note - regardless of whether terminator was cr or lf, we use \n, so that we
						 will conform to whatever the OS convention is.*/
			break;
		}
		*(linepointer++)= c;
	}
	last_terminator=c;
	*linepointer='\0';
	return 1; /*successfully returning a line*/
}/*get_terminated_line*/


/* ----- */

#define LIBRARYNAME_MAX (FILENAME_MAX - strlen(LIB_DIR))

void include_file (const char *file, FILE *out)
{
	FILE *in;
	char included[FILENAME_MAX];
	size_t length = strlen(file);
	struct context *saved = current, local;

	if ('<' == *file) {
		/* if the file name starts with '<', then search for it in LIB_DIR */

		if (LIBRARYNAME_MAX <= length - 2) {
			error(1, "Included library name '%s' too long (%d characters maximum).", LIBRARYNAME_MAX, file);
		}

		(void) strncat(strcpy(included, LIB_DIR), file + 1, length - 2);
	} else {
		if ((size_t) FILENAME_MAX <= length) {
			error(1, "Included file name '%s' too long (%d characters maximum).", FILENAME_MAX, file);
		}

		copy_string(included, file, FILENAME_MAX);
	}

	if (NULL == (in = fopen(included, "r"))) {
		error(1, "Can't open included file '%s'.", included);
	}

	local.line_number = 0;
	copy_string(local.filename, included, FILENAME_MAX);

	/* swap the current context before processing so error message are relative to our included file */

	current = &local;

	preprocess_file(in, out);

	current = saved;

	(void) fclose(in);
} /* include_file() */

/* ----- */

void warning (int column, const char *format, ...)
{
	va_list ap;

	if (suppress_warnings && !warnings_fatal) return;

	fprintf(stderr, "\a%s: *** warning ", PROGNAME);

	if (current->line_number == 0) {
		fprintf(stderr, "on command line\n");
	} else {
		if (!equal_string(current->filename, global.filename)) {
			fprintf(stderr, "in included file %s ", current->filename);
		}

		fprintf(stderr, "on line %d:%d\n", current->line_number, column);
	}

	va_start(ap, format); {
		(void) vfprintf(stderr, format, ap);
		} va_end(ap);

	fprintf(stderr, "\n");

	if (warnings_fatal) exit(EXIT_FAILURE);
} /* warning() */

/* ----- */

void error (int column, const char *format, ...)
{
	va_list ap;

	fprintf(stderr, "\a%s: *** error ", PROGNAME);

	if (current->line_number == 0) {
		fprintf(stderr, "on command line\n");
	} else {
		if (!equal_string(current->filename, global.filename)) {
			fprintf(stderr, "in included file %s ", current->filename);
		}

		fprintf(stderr, "on line %d:%d\n", current->line_number, column);
	}

	va_start(ap, format); {
		(void) vfprintf(stderr, format, ap);
		} va_end(ap);

	fprintf(stderr, "\n");

	exit(EXIT_FAILURE);
} /* error() */

/* ----- */

void handle_directive (const char *line, FILE *out)
{
	size_t ntokens = 0, i;
	enum directives directive = -1;
	char tokens[MAX_TOKENS][LINE_LENGTH];
	while ('\0' != *line) {
		size_t k = 0;
		char token[LINE_LENGTH];
		BOOLEAN escape = FALSE;
		/*For each token, we need to consider 2 cases:
		1) The token begins with " so we expect it to end with " also. The token consists of everything 
 		   up to the next unescaped " or else ends with null with the end of the line.
		2) The token begins with anything else. Now the token consists of everything up to the next 
                   space character or end of line.
		In both cases, a single \ should escape the next character. In most cases, the \ is simply skipped,
	        except in the following cases:
		- \\ expands to \ and any character after the sequence is not escaped (ie, an escaped \ is treated
		as an ordinary character);
		- \" expands to " but is not considered as marking the beginning or end of a double quoted string;
		- \<space> is treated like <space> and will terminate an unquoted macro;
		- \ at the end of the line terminates the string. */

		if ('"' == *line) {						/*The token begins with unescaped " */
			line++;
			/*Do the next loop until we reach a null, an unescaped ", or a CR */
			while (('\n'!=*line) && ('\0' != *line) && !(('"' == *line) && (!escape))) {
				if (('\\' == *line)&&(!escape)) { 	/* We have an unescaped backslash */
					escape =TRUE;
					}
				else {					/*We have an ordinary character*/
					token[k++] = *line;
					escape = FALSE;
					}
				line++;
				}
			if ('"'==*line){line++;	} 			/*If pointing at last quote, move on one char */
			}
		else	{						/*Token begins with other than " */
			/* Do the loop until we reach a space or null */
			while (('\0' != *line) && (!isspace(*line))) {
				if (('\\' == *line)&&(!escape)) { 	/* We have an unescaped backslash */
					escape =TRUE;
					}
				else {					/*We have an ordinary character*/
					token[k++] = *line;
					escape = FALSE;
					}
				line++;

            			}
			}
		token[k] = '\0';


		if (MAX_TOKENS == ntokens) {
			warning(1, "Too many tokens (> %d) on directive line - line truncated.", MAX_TOKENS);

			break; /* while */
		} else {
			copy_string(tokens[ntokens++], token, LINE_LENGTH);
		}
		while (isspace(*line)) {line++;}				/*Strip  spaces*/
		
	} /* while */

	/* ok, now find out the directive and decide what to do
	 * no binary search for so few directives...
	 */

	for (i = 0; i < sizeofA(directives); i++) {
		if (equal_string(tokens[0], directives[i])) {
			directive = i;
			break; /* for */
		}
	}

	switch (directive) {

		case IFDEF:
		case IFNDEF:

			if (1 == ntokens) {
				error(1, "#if(n)def must be followed by at least 1 symbol.");
			}

			if (ifdef) {
				error(1, "Cannot nest #if(n)def.");
			}

			ifdef = cond_else = TRUE;
			condition = FALSE;

			/* if any of the tokens are defined, then TRUE */
			for (i = 1; i < ntokens; i++) {
				if (NULL != find_macro(tokens[i])) {
					condition = TRUE;
					cond_else = FALSE;
					break; /* for */
				}
			}

			if (IFNDEF == directive) {
				condition = !condition;
				cond_else = !cond_else;
			}

			break;

		case ELIFDEF:
		case ELIFNDEF:

			if (!ifdef) {
				warning(1, "#elif(n)def without #ifdef - unpredictable behaviour.");
			}

			if (1 == ntokens) {
				error(1, "#elif(n)def must be followed by at least 1 symbol.");
			}

			if (cond_else) {
				/* there was an #ifdef or an #elifdef */
				for (i = 1; i < ntokens; i++) {
					if (NULL != find_macro(tokens[i])) {
						condition = TRUE;
						cond_else = FALSE;
						break; /* for */
					}
				}
				if (ELIFNDEF == directive) {
					condition = !condition;
					cond_else = !cond_else;
				}
			} else {
				condition = FALSE;
			}

			break;

		case ELSE:

			if (!ifdef) {
				error(1, "#else without #ifdef.");
			}

			if (1 != ntokens) {
				warning(1, "#else should not be followed by any symbols - extra ignored.");
			}

			condition = cond_else;

			break;

		case ENDIF:

			if (1 != ntokens) {
				warning(1, "#endif should not be followed by any symbols - extra ignored.");
			}

			if (!ifdef) {
				warning(1, "#endif without #ifdef - ignored.");
			} else {
				condition = TRUE;
				ifdef = cond_else = FALSE;
			}

			break;

		case DEFINE:

			if (3 < ntokens) {
				error(1, "#define must be followed by 1 or 2 strings.");
			}

			if (condition) {
				if (2 == ntokens) {
					undefine_macro(tokens[1]);
				} else {
					define_macro(tokens[1], tokens[2], MACRO);
				}
			}

			break;

		case INCLUDE:

			if (2 != ntokens) {
				warning(1, "#include must be followed by 1 string - ignored.");
			} else if (condition) {
				include_file(tokens[1], out);
			}

			break;

		case UNDEFINE:
		case SUSPEND:

			if (condition) undefine = TRUE;

			break;

		case REDEFINE:
		case RESUME:

			if (condition) undefine = FALSE;

			break;

		case ABC:

			if (condition) doremi = FALSE;

			break;

		case DOREMI:

			if (condition) doremi = TRUE;

			break;

		case COMMENT:

			;

			break;

		default:

			warning(1, "Unknown preprocessor directive: '%s' - ignored.", tokens[0]);

			break;
	} /* switch (directive) */
} /* handle_directive () */

/* ----- */

/* --- End of file abcpp.c --- */
