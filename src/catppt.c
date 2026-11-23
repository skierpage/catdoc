/**
 * @file   ppt2text.c
 * @author Alex Ott <alexott@gmail.com>
 * @date   Dec 23 2004
 * Version: $Id: catppt.c,v 1.2 2006-10-17 19:11:29 vitus Exp $
 * Copyright: Alex Ott 
 * 
 * @brief  main module for text extracting from .ppt
 * 
 * 
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <ctype.h>
#include <string.h>
#include <unistd.h>
#include "ppt.h"
#include "catdoc.h"
#include <stdlib.h>
#include "float.h"

#ifdef __TURBOC__
#define strcasecmp(a,b) strcmpi(a,b)
#endif

/** 
 * Displays  help message
 * 
 */
void help (void) {
	printf("Usage:\n catppt [-lV] [-b string] [-s charset] [-d charset] files\n");
}

extern char *slide_separator;
char *input_buffer, *output_buffer;

/*
 * Prints the presentation of an OLE file, which may be standard input.
 * Returns 0 on success, 1 if it is not an OLE file.
 */
static int process_ppt(FILE *input, char *filename) {
	return ole_process_streams(input,filename,"PowerPoint Document",NULL,do_ppt);
}

/** 
 * 
 * 
 * @param argc 
 * @param argv 
 * 
 * @return 
 */
int main(int argc, char *argv[]) {
	short int *tmp_charset;
	int c;
	char *tempname;
	read_config_file(SYSTEMRC);
#ifdef USERRC
	tempname=find_file(strdup(USERRC),getenv("HOME"));
	if (tempname) {
		read_config_file(tempname);
		free(tempname);
	}
#endif
#ifdef HAVE_LANGINFO
	get_locale_charset();
#endif
	
	check_charset(&dest_csname,dest_csname); 

	while ((c=getopt(argc,argv,"Vls:d:b:"))!=-1) {
		switch(c)  {
		case 'l':
			list_charsets(); exit(0);
		case 's':
			check_charset(&source_csname,optarg);
			source_charset=read_charset(source_csname);
			break;
		case 'd':
			check_charset(&dest_csname,optarg);
			break;
		case 'b':
			slide_separator = strdup(optarg);
			break;
		case 'V': printf("Catdoc Version %s\n",CATDOC_VERSION);
			exit(0);
		default:
			help();
			exit(1);
		}	
	}
	/* If we are using system strftime, we need to  set LC_TIME locale
	 * category unless choosen charset is not same as system locale
	 */ 
#if defined(HAVE_LANGINFO) && defined(HAVE_STRFTIME) && !defined(__TURB0C__)
	set_time_locale();
#endif	
	/* charset conversion init*/
	input_buffer=malloc(FILE_BUFFER);
	if (strcmp(dest_csname,"utf-8")) {
		tmp_charset=read_charset(dest_csname);
		if (!tmp_charset) {
			fprintf(stderr,"Cannot load target charset %s\n",dest_csname);
			exit(1);
		}	
		target_charset=make_reverse_map(tmp_charset);
		free(tmp_charset);
	} else { 
		target_charset=NULL;
	} 
	spec_chars=read_substmap(stradd("ascii",SPEC_EXT));
	if (!spec_chars) {
		fprintf(stderr,"Cannot read substitution map ascii%s\n",
						SPEC_EXT);
		exit(1);
	}  
	replacements=read_substmap(stradd("ascii",REPL_EXT));
	if (!replacements) {
		fprintf(stderr,"Cannot read substitution map ascii%s\n",
						REPL_EXT);
		exit(1);
	}  
	set_std_func();
	if (optind>=argc && isatty(fileno(stdin))) {
		help();
		exit(0);
	}
	return process_files(argc,argv,optind,process_ppt);
}
