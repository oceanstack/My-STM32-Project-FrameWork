/*A clever bits set clear and get method for  any length I ever seen online */

#include <stdio.h>

#define _BITS_SHIFT_8 3
#define _BITS_SHIFT_16 4
#define _BITS_SHIFT_32 5
#define _BITS_SHIFT_64 6
#define _BITSWIDTH_8 (1<<_BITS_SHIFT_8)
#define _BITSWIDTH_16 (1<<_BITS_SHIFT_16)
#define _BITSWIDTH_32 (1<<_BITS_SHIFT_32)
#define _BITSWIDTH_64 (1<<_BITS_SHIFT_64)

#define _BITSMASK(n) ((1<<(n)) - 1)
#define _BITS_MASK_8 (_BITSWIDTH_8 - 1)
#define _BITS_MASK_16 (_BITSWIDTH_16 - 1)
#define _BITS_MASK_32 (_BITSWIDTH_32 - 1)
#define _BITS_MASK_64 (_BITSWIDTH_64 - 1)




#define _get_bits(p,k,shift,mask) (((p)[(k)>>shift] & (1<<((k)& mask)))? 1 : 0)
#define _set_bits(p,k,shift,mask) do{(p)[(k)>>shift] |= (1<<((k)& mask));}while (0)
#define _clr_bits(p,k,shift,mask) do{(p)[(k)>>shift] &= (~(1<<((k)& mask)));}while (0)
#define _get_8_bits(p,k) _get_bits(p,k,_BITS_SHIFT_8,_BITS_MASK_8)
#define _set_8_bits(p,k) _set_bits(p,k,_BITS_SHIFT_8,_BITS_MASK_8)
#define _clr_8_bits(p,k) _clr_bits(p,k,_BITS_SHIFT_8,_BITS_MASK_8)
#define _get_16_bits(p,k) _get_bits(p,k,_BITS_SHIFT_16,_BITS_MASK_16)
#define _set_16_bits(p,k) _set_bits(p,k,_BITS_SHIFT_16,_BITS_MASK_16)
#define _clr_16_bits(p,k) _clr_bits(p,k,_BITS_SHIFT_16,_BITS_MASK_16)
#define _get_32_bits(p,k) _get_bits(p,k,_BITS_SHIFT_32,_BITS_MASK_32)
#define _set_32_bits(p,k) _set_bits(p,k,_BITS_SHIFT_32,_BITS_MASK_32)
#define _clr_32_bits(p,k) _clr_bits(p,k,_BITS_SHIFT_32,_BITS_MASK_32)
#define _get_64_bits(p,k) _get_bits(p,k,_BITS_SHIFT_64,_BITS_MASK_64)
#define _set_64_bits(p,k) _set_bits(p,k,_BITS_SHIFT_64,_BITS_MASK_64)
#define _clr_64_bits(p,k) _clr_bits(p,k,_BITS_SHIFT_64,_BITS_MASK_64)


int main(void)
{
    unsigned int bittest = 0xFFFFFFFF;
    unsigned int get = 9;

    printf("%X \n",bittest);

    _clr_64_bits(&bittest,6);
    printf("%X \n",bittest);

    get = _get_64_bits(&bittest,6);
    printf("%X \n",get );

    _set_64_bits(&bittest,6);
    get = _get_64_bits(&bittest,6);
    printf("%X \n",get );
    return 0;
}
