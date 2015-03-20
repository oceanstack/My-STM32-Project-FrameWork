#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef  unsigned int u32 ;
typedef  unsigned short u16 ; 
typedef  unsigned char u8 ;

/*A Simple mid average filter designed for ad sample . */
static void AD_MidAvg_Filter(u16 adc_value)
{
    static u8 cnt = 0 ; 
    static u16 adc_min = 0;
    static u16 adc_max = 0;
    static u16 target_value = 0;
    static u32 sum = 0 ;
    static u8 flag = 0 ;

    if(cnt < 3)
    {
        if(0 == flag)
        {
            flag = 1 ;
            adc_min = adc_value;
            adc_max = adc_value; 
        }

        if(adc_min > adc_value  )
        {
            adc_min = adc_value ;
        }

        if(adc_max < adc_value  )
        {
            adc_max = adc_value ;
        }

        sum += adc_value;
        cnt ++ ;

        if(3 == cnt)        
        {
            target_value = (u16)(sum - adc_min -adc_max);
            printf("MidValue : %d\n", target_value); //Unit Test 
            sum = 0;
            adc_max = 0;
            adc_min = 0;
            cnt = 0 ;
            flag = 0;
        }       
    }
}


int main(void)
{
    //unsigned char MidTest[3]= {0xFF,0x01,0x10};
    unsigned char MidTest[3];
    unsigned int i ;

    for(i = 0 ; i < 3*17; i++)
    {
        //srand((unsigned int)time(NULL)) ;
        srand((unsigned int) i*55) ;
        MidTest[i%3] = (unsigned char) (rand() % 99);
        printf("%d  \n",MidTest[i%3]);

        AD_Mid_Filter(MidTest[i%3]);
    }    

    return 0;
}

