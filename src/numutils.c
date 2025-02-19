/*****************************************************************/
/* Utilities to convert various numeric types from the Windows   */
/* (Little endian) format to native types                        */
/*                                                               */
/* This file is part of catdoc project                           */
/* (c) Victor Wagner 1996-2003, (c) Alex Ott 2003	             */
/*****************************************************************/
#include "catdoc.h"

/********************************************************************/
/* Reads 2-byte LSB  int from buffer at given offset platfom-indepent
 * way
 *********************************************************************/ 
uint16_t getshort(unsigned char *buffer,int offset) {
	return (uint16_t)buffer[offset]|((uint16_t)buffer[offset+1]<<8);
}  
/********************************************************************/
/* Reads 4-byte LSB  int from buffer at given offset almost platfom-indepent
 * way
 *********************************************************************/ 
int32_t getlong(unsigned char *buffer,int offset) {
	return (int32_t)buffer[offset]|((int32_t)buffer[offset+1]<<8L)
		|((int32_t)buffer[offset+2]<<16L)|((int32_t)buffer[offset+3]<<24L);
}  

uint32_t getulong(unsigned char *buffer,int offset) {
	return (uint32_t)buffer[offset]|((uint32_t)buffer[offset+1]<<8L)
		|((uint32_t)buffer[offset+2]<<16L)|((uint32_t)buffer[offset+3]<<24L);
}  
