/*
  Copyright 1998-2003 Victor Wagner
  Copyright 2003 Alex Ott
  This file is released under the GPL.  Details can be
  found in the file COPYING accompanying this distribution.
*/
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <stdlib.h>
#include "catdoc.h"
#if defined(_WIN32)
#include <dir.h>
#include <dos.h>
#else
#include <glob.h>
#endif


/************************************************************************/
/*  Copies component of string starting with p and ending one char      */
/*  before q into path_buf, expanding ~ if neccessary                   */
/************************************************************************/
int prepare_path_buf(char *path_buf, const char *start, const char *end) {
	if (*start == '~' && start[1] == DIR_SEP) {
		char *home=getenv("HOME");
		start++;
		if (!home) {
			if (end-start>PATH_BUF_SIZE) return 0;
			strncpy(path_buf,start,end-start);
			path_buf[end-start]=0;
		} else {
			int l = strlen(home);
			if (l+(end-start)>PATH_BUF_SIZE) return 0;
			strcpy(path_buf,home);
			strncpy(path_buf+l,start,end-start);
			path_buf[end-start+l]=0;
		}	
	} else {	
			if (end-start>PATH_BUF_SIZE) return 0;
			strncpy(path_buf,start,end-start);
			path_buf[end-start]=0;
	}
	/* Empty list element means current directory */
	if (!*path_buf) {
		path_buf[0]='.';
		path_buf[1]=0;
	}
	return 1;

}	
/************************************************************************/
/* Searches for file name in specified list of directories. Sets        */
/* Returns dynamically allocated full path or NULL. if nothing          */
/* appropriate   Expects name to be dynamically allocated and frees it  */
/************************************************************************/
char *find_file(char *name, const char *path)
{ const char *p;
	char *q;
	char path_buf[PATH_BUF_SIZE];
	char dir_sep[2]={DIR_SEP,0};
	for (p=path;p;p=(q?(q+1):NULL)) {
		q=strchr(p,LIST_SEP);
		if (q) {
			if (!prepare_path_buf(path_buf,p,q)) continue;
		} else {
			if (!prepare_path_buf(path_buf,p,p+strlen(p))) continue;
		}
		strcat(path_buf,dir_sep); /* always one char */
		if (strlen(path_buf)+strlen(name)>=PATH_BUF_SIZE) 
			continue; /* Ignore too deeply nested directories */
		strcat(path_buf,name);
		if (access(path_buf,0)==0) {
			free(name); 
			return strdup(path_buf);
		}
	}
	/* if we are here, nothing found */
	free(name); 
	return NULL;
}

/************************************************************************/
/* Searches for charset with given name and put pointer to malloced copy*/
/* of its name into first arg if found. Otherwise leaves first arg      */
/*  unchanged. Returns non-zero on success                              */ 
/************************************************************************/
int check_charset(char **filename,const char *charset) {
	char *tmppath;
	if (charset == NULL ) {
		return 0;
	}
	if (!strncmp(charset,"utf-8",6)) {
		*filename=strdup("utf-8");
		return 1;
	}   
	tmppath=find_file(stradd(charset,CHARSET_EXT),charset_path);
	if (tmppath && *tmppath) {
			*filename=strdup(charset);
			free(tmppath);
			return 1;
	} 
	return 0;
}

/**********************************************************************/
/*  Returns malloced string containing concatenation of two           */
/*  arguments                                                         */
/**********************************************************************/
char *stradd(const char *s1,const char *s2) 
{ char *res;
	res=malloc(strlen(s1)+strlen(s2)+1);
	if (!res) {
		fprintf (stderr,"Out of memory!");
		exit(1);
	}
	strcpy(res,s1);
	strcat(res,s2);
	return res;
}  


/*********************************************************************/
/* Prints out list of available charsets, i.e. names without extension *
 * of all .txt files in the charset path + internally-supported utf-8  *
 ************************************************************************/ 

void list_charsets(void) {
	const char *p;
	char *q;
	char path_buf[PATH_BUF_SIZE];
	char dir_sep[2]={DIR_SEP,0};
	char **ptr;
	glob_t glob_buf;
	int count,glob_flags=GLOB_ERR;

	memset(&glob_buf,0,sizeof(glob_t));

	for (p=charset_path;p;p=(q?(q+1):NULL)) {
		q=strchr(p,LIST_SEP);
		if (q) {
			if (q-p>=PATH_BUF_SIZE) {
				/* Oops, dir name too long, perhabs broken config file */
				continue;
			}
			strncpy(path_buf,p,q-p);
			path_buf[q-p]=0;
		} else {
			if (strlen(p)>=PATH_BUF_SIZE) continue;
			strcpy(path_buf,p);
		}
		/* Empty list element means current directory */
		if (!*path_buf) {
			path_buf[0]='.';
			path_buf[1]=0;
		}
		strcat(path_buf,dir_sep); /* always one char */
		if (strlen(path_buf)+6>=PATH_BUF_SIZE)
			continue; /* Ignore too deeply nested directories */
		strcat(path_buf,"*.txt");
		switch (glob(path_buf,glob_flags,NULL,&glob_buf)) {
			case 0:
#ifdef GLOB_NOMATCH
			case GLOB_NOMATCH:
#endif
				break;
			default:
				perror("catdoc");
				exit(1);
		}
		glob_flags|=GLOB_APPEND;
	}
	count=0;printf("Available charsets:"); 
	for (ptr=glob_buf.gl_pathv;*ptr;ptr++) {
		printf("%c",(count++)%5?'\t':'\n');
		p=strrchr(*ptr,dir_sep[0]);
		if (!p) continue;
		p++;
		if ((q=strchr(p,'.'))) *q=0;
		fputs(p,stdout);
	}  
	printf("%c",(count++)%5?'\t':'\n');
	fputs("utf-8",stdout);
	printf("\n");
	globfree(&glob_buf);
}    
