#ifndef __UID_GET_H
#define __UID_GET_H_

#include "includes.h"

#define UID_ADDRESS  0x1FFFF7E8
#define FLASH_SIZE_ADDRESS  0X1FFFF7E0 //Get The size of chip 


#define BigtoLittle32(A)   ((( (uint32_t)(A) & 0xff000000) >> 24) | \
                                       (( (uint32_t)(A) & 0x00ff0000) >> 8)   | \
                                       (( (uint32_t)(A) & 0x0000ff00) << 8)   | \
                                       (( (uint32_t)(A) & 0x000000ff) << 24)) 


#define BigtoLittle16(A)   (( ((uint16_t)(A) & 0xff00) >> 8)    | \
                                       (( (uint16_t)(A) & 0x00ff) << 8))  
   


#define UID_BYTE_LEN 12
#define UID_HALF_LEN 6
#define UID_WORD_LEN 3

void Get_UID_BYTE(void);
void Get_UID_HALF(void);
void Get_UID_WORD(void);

#endif  /*__UID_GET_H_*/


