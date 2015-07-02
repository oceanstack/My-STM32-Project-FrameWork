#include "includes.h"


#define  DWT_CR      *(volatile u32 *)0xE0001000
#define  DWT_CYCCNT  *(volatile u32 *)0xE0001004
#define  DEM_CR      *(volatile u32 *)0xE000EDFC
#define  DEM_CR_TRCENA                   (1 << 24)
#define  DWT_CR_CYCCNTENA                (1 <<  0)


static u32 cpuclkfeq;     //用于保存cpu运行频率，可运行时动态修改


//初始化延时系统，参数为CPU频率
//SystemCoreClock
void DelayInit(u32 clk)
{
    cpuclkfeq = clk;
    //打开CYCCNT功能,并把计数器清零，最后打开计数器对cpu时钟进行向上计数
    DEM_CR         |=  DEM_CR_TRCENA; 
    //    DWT_CYCCNT      = 0u;    //根据需要如果调试，或其他程序要使用CYCCNT时注释掉，否则可直接清零 
    DWT_CR         |= DWT_CR_CYCCNTENA;
}


//延时函数，参数为需要延时的微秒数
void Delayus(u32 usec)
{
    u32 startts,endts,ts;
    //保存进入函数时的计数器值
    startts = DWT_CYCCNT;
    ts =  usec * (cpuclkfeq  / (1000*1000) );        //计算达到所需延时值的cpu时钟数,^-^如果想要更精确此处可以减去运行前面代码所需的时钟数。
    endts = startts + ts;           //计算达到所需延时时间的DWT_CYCCNT计数值，超过32bit所能表达的最大值2的32次方-1是自动绕回丢弃进位
    if(endts > startts)            //判断是否跨越最大值边界
    {
        while(DWT_CYCCNT < endts);        //等到计数到所需延时值的cpu时钟数值
    }
    else
    {
        while(DWT_CYCCNT > endts);       //等待跨域32bit的最大值，2的32次方-1
        while(DWT_CYCCNT < endts);        //等到计数到所需延时值的cpu时钟数值
    }

}

void Delay_Test(void)
{
    DelayInit(SystemCoreClock);
    USART1_Debug_Init(115200);

    while(1)
    {
        Delayms(1000);
        printf("Delay Tick \n");
    }

}
