/*
  Copyright 1998-2003 Victor Wagner
  Copyright 2003 Alex Ott
  This file is released under the GPL.  Details can be
  found in the file COPYING accompanying this distribution.
*/
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include "catdoc.h"
extern char ole_sign[], zip_sign[]; /* from ole.c */
char rtf_sign[]="{\\rtf";
char old_word_sign[]={0xdb,0xa5,0};
char write_sign[]={0x31,0xBE,0};
int verbose=0;
/********************************************************************* 
 * Determines format of input file and calls parse_word_header or 
 * process_file if
 * it is word processor file or copy_out if it is plain text file
 * return not 0 when error
 ********************************************************************/ 
int analyze_format(FILE *f) {
	unsigned char buffer[129];
	long offset=0;
	FILE *new_file, *ole_file;
	int ret_code=69;

	if (!signature_check) {
		/* forced parsing */
		/* no autodetect possible. Assume 8-bit if not overriden on
		 * command line */ 
		if (!get_unicode_char) 
			get_unicode_char=get_8bit_char;
		return process_file(f,LONG_MAX);
	}
	catdoc_read(buffer,4,1,f);
	buffer[4]=0;
	if (strncmp((char *)&buffer,write_sign,2)==0) {
		printf("[Windows Write file. Some garbage expected]\n");
		get_unicode_char=get_8bit_char;
		return process_file(f,LONG_MAX);
	} else if (strncmp((char *)&buffer,rtf_sign,4)==0) {
		return parse_rtf(f);
	} else if (strncmp((char *)&buffer, zip_sign,4) == 0) {
		fprintf(stderr,"This file looks like ZIP archive or Office 2007 "
		"or later file.\nNot supported by catdoc\n");
		exit(1);
	} else if (strncmp((char *)&buffer,old_word_sign,2)==0) {
	   fread(buffer+4,1,124,f);	
	   return parse_word_header(buffer,f,128,0);
	}	
	fread(buffer+4,1,4,f);
	if (strncmp((char *)&buffer,ole_sign,8)==0) {
		if ((new_file=ole_init(f, buffer, 8)) != NULL) {
			set_ole_func();
			while((ole_file=ole_readdir(new_file)) != NULL) {
				int res=ole_open(ole_file);
				if (res >= 0) {
					if (strcmp(((oleEntry*)ole_file)->name , "WordDocument") == 0) {
						offset=catdoc_read(buffer, 1, 128, ole_file);
						ret_code=parse_word_header(buffer,ole_file,-offset,offset);
					}
				} 
				ole_close(ole_file);
			}
			set_std_func();
			ole_finish();
		} else {
			fprintf(stderr,"Broken OLE file. Try using -b switch\n");
			exit(1);
		}	
	} else {

		copy_out(f,(char *)&buffer);
		return 0;
	}
	
	return ret_code;
}   
#define fDot 0x0001   
#define fGlsy 0x0002
#define fComplex 0x0004
#define fPictures 0x0008 
#define fEncrypted 0x100
#define fReadOnly 0x400
#define fReserved 0x800
#define fExtChar 0x1000

/*******************************************************************/
/* parses word file info block passed in buffer.  
 * Determines actual size of text stream and calls process_file  
 ********************************************************************/
int parse_word_header(unsigned char * buffer,FILE *f,int offset,long curpos) {
	int flags,charset, ret_code=0;
	long textstart,textstop,textlen,i;
	char buf[2];
	
	if (verbose) {
		printf("File Info block version %d\n",getshort(buffer,2));
		printf("Found at file offset %ld (hex %lx)\n",curpos,curpos);
		printf("Written by product version %d\n",getshort(buffer,4));
		printf("Language %d\n",getshort(buffer,6));
	}
	flags = getshort(buffer,10);
	if (verbose) {
		if ((flags & fDot)) {
			printf("This is template (DOT) file\n");
		} else {
			printf("This is document (DOC) file\n");
		}
		if (flags & fGlsy) {
			printf("This is glossary file\n");
		}
	}
	if (flags & fComplex) {
		fprintf(stderr,"[This was fast-saved %2d times. Some information is lost]\n",
				(flags & 0xF0)>>4);
/*		ret_code=69;*/
	}
	if (verbose) {
		if (flags & fReadOnly) {
			printf("File is meant to be read-only\n");
		}
		if (flags & fReserved) {
			printf("File is write-reserved\n");
		}
	}
	if (flags & fExtChar) {
		if (verbose) {
			printf ("File uses extended character set\n");
		}
		if (!get_unicode_char) 
			get_unicode_char=get_word8_char;

	} else if (!get_unicode_char) 
		get_unicode_char=get_8bit_char;

	if (verbose) {
		if (buffer[18]) {
			printf("File created on Macintosh\n");
		} else {
			printf("File created on Windows\n");
		} 
	}
	if (flags & fEncrypted) {
		fprintf(stderr,"[File is encrypted. Encryption key = %08lx]\n",
				getlong(buffer,14));
		return 69;
	}
	if (verbose) {
		charset=getshort(buffer,20);
		if (charset&&charset !=256) {
			printf("Using character set %d\n",charset);
		} else {
			printf("Using default character set\n");
		}
	}
	/* skipping to textstart and computing textend */
	textstart=getlong(buffer,24);
	textstop=getlong(buffer,28);
	textlen=(textstart < textstop)? textstop - textstart : 0;

	textstart+=offset;
	if (verbose) {
		printf ("Textstart = %ld (hex %lx)\n",textstart+curpos,textstart+curpos);
		printf ("Textlen =   %ld (hex %lx)\n",textlen,textlen);
	}   
	for (i=0;i<textstart;i++) {
		catdoc_read(buf, 1, 1, f);
		if (catdoc_eof(f)) {
			fprintf(stderr,"File ended before textstart. Probably it is broken. Try -b switch\n");
			exit(1);
		}
	}    
	return process_file(f,textlen) || ret_code;
}   

