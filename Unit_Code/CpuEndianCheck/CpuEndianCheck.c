#include <stdio.h>
// two methods about check the CPU  endian with c .
#define Little_Endian do{printf("Little Endian");}while(0)
#define Big_Endian  do{printf("Big Endian");}while(0)

#define USED_METHOD_1  1
#define USED_METHOD_2  0

#if USED_METHOD_1  // one method
void CpuEndianCheck(void) 
{ 
    int a = 0x12345678;
    char *p = (char*)(&a);
    
    if(0x78 == *p){
         Little_Endian; 
    }else{
         Big_Endian; 
    }  
}
#endif

#if USED_METHOD_2  // another method
void CpuEndianCheck(void) 
{ 
    union check 
    { 
        int Word; 
        char Byte;  
    } Endian; 
  
    Endian.Word=1; 
  
    if(1 == Endian.Byte) { 
         Little_Endian; 
    }else{
         Big_Endian; 
    }          
}
#endif

int main(void)
{
	CpuEndianCheck();
	return 0;
}
