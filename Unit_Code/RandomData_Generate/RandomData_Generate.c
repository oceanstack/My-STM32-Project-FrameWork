/* This Code can generate random data as required with stm32 */
#include <stdlib.h>


int RandomData_Generate(unsigned int max)
{
    static  vu32 Rand_Seed = 0;
    srand(Rand_Seed++);
    return (int )(rand() % max);
}

