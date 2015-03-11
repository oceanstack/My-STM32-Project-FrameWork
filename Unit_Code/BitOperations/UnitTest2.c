
/*get any bit status of any byte */

#include <stdio.h>

int main(void)
{
    unsigned char bittest[8] = {0xFF,0x01,0x10,0xBF,0xFB,0xFE,0xEF,0xFF};
    unsigned int i ;
    unsigned char bitcache = 0 ;
  
    for(i = 0 ; i < 8; i++)
    {
    	printf("%X ",bittest[i]);
    }    
    
    for(i = 0 ; i < 64; i++)
    {
    	if(i%8 == 0) printf("\n");
    	bitcache= bittest[i/8];
    	printf("%d ",_get_8_bits(&bitcache,7-(i%8)) ? 1 : 0 );
    }
    return 0;
}

