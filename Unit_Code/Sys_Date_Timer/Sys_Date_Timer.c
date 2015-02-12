#include "Sys_Date_Timer.h"

Sys_Date_Time_Ticks_Struct Sys_Time ;

void SysTime_Ticks_Init(void)
{
    Sys_Time.Day = 1;	
    Sys_Time.Month = 1;
	Sys_Time.Year = 2015;
	Sys_Time.Second  = 0;
	Sys_Time.Minute  = 0;
	Sys_Time.Hour  = 0;
}



void SysTime_Ticks_IRQ(void)
{
    if(Sys_Time.Second >= 60)
    {
        Sys_Time.Second=0;
        Sys_Time.Minute++;
        if(Sys_Time.Minute>=60)
        {
            Sys_Time.Minute=0;
            Sys_Time.Hour++;
        }
        if(Sys_Time.Hour>=24)
        {
            Sys_Time.Hour=0;
            Sys_Time.Day++;
            switch(Sys_Time.Month)
            {
                case 1:
                case 3:
                case 5:
                case 7:
                case 8:
                case 10:
                case 12:
                    if(Sys_Time.Day > 31)
                    {
                        Sys_Time.Day	=	1;
                        Sys_Time.Month++;
                        if(Sys_Time.Month>12)
                        {
                            Sys_Time.Month=1;
                            Sys_Time.Year++;
                        }
                    }
                    break;		
                case 4:
                case 6:
                case 9:
                case 11:
                    if(Sys_Time.Day > 30)
                    {
                        Sys_Time.Day	=	1;
                        Sys_Time.Month++;
                    }
                    break;
                case 2:
                    if((Sys_Time.Year % 4 == 0 && Sys_Time.Year %100 != 0) || (Sys_Time.Year % 400 == 0))
                    {
                        if(Sys_Time.Day > 29)
                        {
                            Sys_Time.Day	=	1;
                            Sys_Time.Month++;
                        }
                    }
                    else
                    {
                        if(Sys_Time.Day > 28)
                        {
                            Sys_Time.Day	=	1;
                            Sys_Time.Month++;
                        }
                    }
                    break;
                default:Sys_Time.Month=1;break;
            }
        }
    }
}

