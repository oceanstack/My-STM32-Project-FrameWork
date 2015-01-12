#include <stdio.h>

#define BYTE0(dwTemp)       (*(unsigned char *)(&dwTemp))
#define BYTE1(dwTemp)       (*((unsigned char *)(&dwTemp) + 1))

//Another method
#define LOWBYTE(w) ((unsigned char) ((w) & 0xff))
#define HIGHBYTE(w) ((unsigned char) ((w) >> 8))

int main()
{
    unsigned short a = 0xAA55;
    unsigned char b = 0;

    b  = (unsigned char)(a & 0x00FF);
    printf("%X \n",b);

    b  =  (unsigned char)( (a >> 8) & 0x00FF );
    printf("%X \n",b);

    b  =  BYTE0(a);
    printf("%X \n",b);

    b  =  BYTE1(a);
    printf("%X \n",b);

    b  =  LOWBYTE(a);
    printf("%X \n",b);

    b  =  HIGHBYTE(a);
    printf("%X \n",b);
    return 0;
}
