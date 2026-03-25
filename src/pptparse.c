/**
 * @file   pptparse.c
 * @author Alex Ott <alexott@gmail.com>
 * @date   23 ??? 2004
 * Version: $Id: pptparse.c,v 1.2 2006-10-17 19:11:29 vitus Exp $
 * Copyright: Alex Ott
 *
 * @brief  .ppt parsing routines
 *
 *
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include "ppt.h"
#include "catdoc.h"
#include "ppttypes.h"

char *slide_separator = "\f";

static void process_item (int rectype, long reclen, FILE* input);

#if !defined(min)
#define min(x,y) ((x) < (y) ? (x) : (y))
#endif

static void start_text_out(void);

/**
 *
 *
 * @param input
 * @param filename
 */

enum {START_FILE,START_SLIDE,TEXTOUT,END_FILE} slide_state ;
static int in_slide = 0;  /* true when inside a Slide record, not a master or notes */

static void start_text_out(void) {
	if (slide_state == START_SLIDE) {
		fputs(slide_separator,stdout);
	}
	slide_state = TEXTOUT;
}
void do_ppt(FILE *input,char *filename) {
	int itemsread=1;
	int rectype;
	long reclen;
	unsigned char recbuf[8];
	slide_state = START_FILE;
	while(itemsread) {
		itemsread = catdoc_read(recbuf, 1, 8, input);

		if (catdoc_eof(input)) {
			process_item(DOCUMENT_END,0,input);
			return;
		}
		if(itemsread < 8)
			break;
		rectype=getshort(recbuf,2);
		reclen=getulong(recbuf,4);
		DBGPRINT("read record type=%d len=%ld", rectype, reclen);
		if (reclen < 0) {
			return;
		}
		process_item(rectype,reclen,input);
	}
}


/**
 *
 *
 * @param rectype
 * @param reclen
 * @param input
 */
static void process_item (int rectype, long reclen, FILE* input) {
	int i=0, u;
	static unsigned char buf[2];
	switch(rectype) {
	case DOCUMENT_END:
		DBGPRINT("End of document, ended at %ld", catdoc_tell(input));
		catdoc_seek(input, reclen, SEEK_CUR);
		if (slide_state == TEXTOUT) {
			fputs(slide_separator,stdout);
			slide_state = END_FILE;
		}
		break;

	case DOCUMENT:
		DBGPRINT("Start of document, reclen=%ld, started at %ld", reclen, catdoc_tell(input));
		break;

	case DOCUMENT_ATOM:
		DBGPRINT("DocumentAtom, reclen=%ld", reclen);
		catdoc_seek(input, reclen, SEEK_CUR);
		break;

	case SLIDE:
		DBGPRINT("Slide, reclen=%ld", reclen);
		in_slide = 1;
		break;

	case SLIDE_ATOM:
		DBGPRINT("SlideAtom, reclen=%ld", reclen);
		catdoc_seek(input, reclen, SEEK_CUR);
		break;

	case SLIDE_BASE:
		DBGPRINT("SlideBase, reclen=%ld", reclen);
		break;

	case SLIDE_BASE_ATOM:
		DBGPRINT("SlideBaseAtom, reclen=%ld", reclen);
		catdoc_seek(input, reclen, SEEK_CUR);
		break;

	case NOTES:
		DBGPRINT("Notes, reclen=%ld", reclen);
		in_slide = 0;
		break;

	case NOTES_ATOM:
		DBGPRINT("NotesAtom, reclen=%ld", reclen);
		catdoc_seek(input, reclen, SEEK_CUR);
		break;

	case HEADERS_FOOTERS:
		DBGPRINT("HeadersFooters, reclen=%ld", reclen);
		break;

	case HEADERS_FOOTERS_ATOM:
		DBGPRINT("HeadersFootersAtom, reclen=%ld", reclen);
		catdoc_seek(input, reclen, SEEK_CUR);
		break;

	case MAIN_MASTER:
		DBGPRINT("MainMaster, reclen=%ld", reclen);
		in_slide = 0;
		break;

	case TEXT_BYTES_ATOM: {
		DBGPRINT("TextBytesAtom, reclen=%ld", reclen);
		start_text_out();
		for(i=0; i < reclen; i++) {
			catdoc_read(buf,1,1,input);
			if((unsigned char)*buf!=0x0d)
				fputs(convert_char((unsigned char)*buf),stdout);
			else
				fputc('\n',stdout);
		}
		fputc('\n',stdout);
		}
		break;

	case TEXT_CHARS_ATOM:
	case CSTRING: {
		long text_len;

		DBGPRINT("TextCharsAtom/CString, reclen=%ld", reclen);
		start_text_out();
		text_len=reclen/2;
		for(i=0; i < text_len; i++) {
			catdoc_read(buf,2,1,input);
			u=(unsigned short)getshort(buf,0);
			if(u!=0x0d)
				fputs(convert_char(u),stdout);
			else
				fputc('\n',stdout);
		}
		fputc('\n',stdout);
		}
		break;

	case USER_EDIT_ATOM:
		DBGPRINT("UserEditAtom, reclen=%ld", reclen);
		catdoc_seek(input, reclen, SEEK_CUR);
		break;

	case COLOR_SCHEME_ATOM:
		DBGPRINT("ColorSchemeAtom, reclen=%ld", reclen);
		catdoc_seek(input, reclen, SEEK_CUR);
		break;

	case PPDRAWING:
		if (in_slide) {
			DBGPRINT("PPDrawing, reclen=%ld - descending into slide drawing", reclen);
		} else {
			DBGPRINT("PPDrawing, reclen=%ld - skipping (not in slide)", reclen);
			catdoc_seek(input, reclen, SEEK_CUR);
		}
		break;

	case ESCHER_DG_CONTAINER:
		DBGPRINT("Escher DgContainer, reclen=%ld", reclen);
		break;

	case ESCHER_SPGR_CONTAINER:
		DBGPRINT("Escher SpgrContainer, reclen=%ld", reclen);
		break;

	case ESCHER_SP_CONTAINER:
		DBGPRINT("Escher SpContainer, reclen=%ld", reclen);
		break;

	case ESCHER_CLIENT_TEXTBOX:
		DBGPRINT("Escher ClientTextbox, reclen=%ld - contains text", reclen);
		break;

	case ESCHER_CLIENT_DATA:
		DBGPRINT("Escher ClientData, reclen=%ld", reclen);
		break;

	case ENVIRONMENT:
		DBGPRINT("Environment, reclen=%ld", reclen);
		catdoc_seek(input, reclen, SEEK_CUR);
		break;

	case SSDOC_INFO_ATOM:
		DBGPRINT("SSDocInfoAtom, reclen=%ld", reclen);
		catdoc_seek(input, reclen, SEEK_CUR);
		break;

	case SSSLIDE_INFO_ATOM:
		DBGPRINT("SSSlideInfoAtom, reclen=%ld", reclen);
		catdoc_seek(input, reclen, SEEK_CUR);
		break;

	case PROG_TAGS:
		DBGPRINT("ProgTags, reclen=%ld", reclen);
		catdoc_seek(input, reclen, SEEK_CUR);
		break;

	case PROG_STRING_TAG:
		DBGPRINT("ProgStringTag, reclen=%ld", reclen);
		catdoc_seek(input, reclen, SEEK_CUR);
		break;

	case PROG_BINARY_TAG:
		DBGPRINT("ProgBinaryTag, reclen=%ld", reclen);
		catdoc_seek(input, reclen, SEEK_CUR);
		break;

	case LIST:
		DBGPRINT("List, reclen=%ld", reclen);
		break;

	case SLIDE_LIST_WITH_TEXT:
		DBGPRINT("SlideListWithText, reclen=%ld", reclen);
		break;

	case PERSIST_PTR_INCREMENTAL_BLOCK:
		DBGPRINT("PersistPtrIncrementalBlock, reclen=%ld", reclen);
		catdoc_seek(input, reclen, SEEK_CUR);
		break;

	case EX_OLE_OBJ_STG:
		DBGPRINT("ExOleObjStg, reclen=%ld", reclen);
		catdoc_seek(input, reclen, SEEK_CUR);
		break;

	case PPDRAWING_GROUP:
		DBGPRINT("PpdrawingGroup, reclen=%ld", reclen);
		catdoc_seek(input, reclen, SEEK_CUR);
		break;

	case EX_OBJ_LIST:
		DBGPRINT("ExObjList, reclen=%ld", reclen);
		catdoc_seek(input, reclen, SEEK_CUR);
		break;

	case TX_MASTER_STYLE_ATOM:
		DBGPRINT("TxMasterStyleAtom, reclen=%ld", reclen);
		catdoc_seek(input, reclen, SEEK_CUR);
		break;

	case HANDOUT:
		DBGPRINT("Handout, reclen=%ld", reclen);
		catdoc_seek(input, reclen, SEEK_CUR);
		break;

	case SLIDE_PERSIST_ATOM:
		DBGPRINT("SlidePersistAtom, reclen=%ld", reclen);
		if (slide_state != START_FILE) {
			slide_state = START_SLIDE;
		}
		catdoc_seek(input, reclen, SEEK_CUR);
		break;

	case TEXT_HEADER_ATOM:
		DBGPRINT("TextHeaderAtom, reclen=%ld", reclen);
		catdoc_seek(input, reclen, SEEK_CUR);
		break;

	case TEXT_SPEC_INFO:
		DBGPRINT("TextSpecInfo, reclen=%ld", reclen);
		catdoc_seek(input, reclen, SEEK_CUR);
		break;

	case STYLE_TEXT_PROP_ATOM:
		DBGPRINT("StyleTextPropAtom, reclen=%ld", reclen);
		catdoc_seek(input, reclen, SEEK_CUR);
		break;

	default:
		DBGPRINT("Default action for rectype=%d reclen=%ld", rectype, reclen);
		catdoc_seek(input, reclen, SEEK_CUR);

	}

}
