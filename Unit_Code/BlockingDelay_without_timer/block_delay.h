#ifndef __BLOCK_DELAY_H
#define __BLOCK_DELAY_H

#define Delayms(msec)         Delayus(msec*1000)  //������ʱ���뼶��ֻ��Ҫ����һ����

void DelayInit(u32 clk);
void Delayus(u32 usec);
void Delay_Test(void);


#endif /*__BLOCK_DELAY_H*/

