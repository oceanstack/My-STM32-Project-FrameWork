#include "includes.h"


char *itoa(int val, char *buf, unsigned radix)
{
    char *p;
    char *firstdig;
    char temp;
    unsigned digval;
    p = buf;
    if(val<0)
    { 
        *p++ = '-';
        val = (unsigned long)(-(long)val);
    }
    firstdig = p;
    do
    {
        digval = (unsigned)(val%radix); 
        val /= radix;
        if(digval>9)
        {
            *p++ = (char)(digval-10+'a');
        }
        else
        {
            *p++=(char)(digval+'0');
        }
    }while(val>0); 
    *p--='\0';
    do
    {
        temp=*p;
        *p=*firstdig;
        *firstdig=temp;
        --p;
        ++firstdig;
    }while(firstdig<p);
    return buf; 
}



//char * Get_UID_BYTE(void)

void Get_UID_BYTE(void)
{
	u8 UID_BYTE_Buf[UID_BYTE_LEN] ;
    char UID_Str[UID_BYTE_LEN*2] ;
	u8 i; 
	printf("UID :");  // itoa(num, str, 10);
	for(i = 0; i < 12 ; i++){
		UID_BYTE_Buf[i] = *((vu8 *)UID_ADDRESS + i);
        
		printf("%02X,",UID_BYTE_Buf[i]);
//        itoa(UID_BYTE_Buf[i] ,&UID_Str[i*2],16);	  // Cannot work well 1219	
		snprintf(&UID_Str[i*2],3,"%02X",UID_BYTE_Buf[i]);

		printf("%s  ",&UID_Str[i*2]);
	
	}
	printf("\n"); 
    printf("UID_Str : "); 
    printf("%s ",UID_Str);
}



void Get_UID_HALF(void)
{
	vu16 UID_HALF_Buf[UID_HALF_LEN] ;
	u8 i; 
	printf("UID :");  // itoa(num, str, 10);
	
	printf("\n"); 
    
	for(i = 0; i < UID_HALF_LEN ; i++){
		UID_HALF_Buf[i] = *((vu16 *)UID_ADDRESS + i);       
		printf("%X,",UID_HALF_Buf[i]);
	
	}
	printf("\n");            
      

    for(i = 0; i < UID_HALF_LEN ; i++){
		     
		printf("%X,",BigtoLittle16(UID_HALF_Buf[i]));
	
	}
	printf("\n"); 
}


void Get_UID_WORD(void)
{
	vu32 UID_WORD_Buf[UID_WORD_LEN] ;
	u8 i; 
	printf("UID :");  // itoa(num, str, 10);
	
	printf("\n"); 
    
	for(i = 0; i < UID_WORD_LEN ; i++){
		UID_WORD_Buf[i] = *((vu32 *)UID_ADDRESS + i);       
		printf("%X,",UID_WORD_Buf[i]);
	
	}
	printf("\n"); 
    
    	UID_WORD_Buf[0] = *(vu32 *)(0x1ffff7e8);
  		UID_WORD_Buf[1] = *(vu32 *)(0x1ffff7ec);       
		UID_WORD_Buf[2] = *(vu32 *)(0x1ffff7f0);
        
        printf("%X,",UID_WORD_Buf[0]);
		printf("%X,",UID_WORD_Buf[1]);
    	printf("%X,",UID_WORD_Buf[1]);
       
    
    printf("\n"); 

    for(i = 0; i < UID_WORD_LEN ; i++){
		     
		printf("%X,",BigtoLittle32(UID_WORD_Buf[i]));
	
	}
	printf("\n"); 
}



