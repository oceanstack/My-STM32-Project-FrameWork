#ifndef __SYS_CONF_
#define __SYS_CONF_ 

#ifdef __cplusplus
 extern "C" {
#endif 

#include "includes.h"

#define USART_TIMEOUT  500000  //10ms  for(;;)

typedef enum {SYS_ERROR = 0, SYS_SUCCESS = !SYS_ERROR} SYS_ErrorStatus;

typedef unsigned int word;

typedef uint8_t boolean;
typedef uint8_t byte;


#ifdef __cplusplus
}
#endif

#endif /*__SYS_CONF_*/

