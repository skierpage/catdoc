#ifndef HAVE_STDINT_H
#define HAVE_STDINT_H
/* If there is no stdint.h, let's define some common integer types
 * ourselves */
#define int16_t	short int
#define uint16_t unsigned short int
#ifdef __MSDOS__
/* MS-DOS is only supported platform where int is 16-bit */
#define int32_t  long int
#define uint32_t unsigned long int
#else
#define int32_t int
#define uint32_t unsigned int
#endif
#endif
