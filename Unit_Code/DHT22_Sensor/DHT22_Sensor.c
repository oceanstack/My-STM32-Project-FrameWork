#include "includes.h"

DHT_t DHT ;


//__enable_irq();
// __disable_irq();


static void DHT_Mode_IPU(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    //        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB , ENABLE);
    GPIO_InitStructure.GPIO_Pin = DHT_Con_Pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU ;
    GPIO_Init(DHT_Pin, &GPIO_InitStructure);
}

static void DHT_Mode_Out_PP(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    //        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB , ENABLE);
    GPIO_InitStructure.GPIO_Pin = DHT_Con_Pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(DHT_Pin, &GPIO_InitStructure);
}



void DHT22_Init(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB , ENABLE);
    GPIO_InitStructure.GPIO_Pin = DHT_Con_Pin ;  //PA13 FT 5V Tolerant.
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz ;
    GPIO_Init(DHT_Pin, &GPIO_InitStructure);
    GPIO_SetBits(DHT_Pin, DHT_Con_Pin);

    DHT.status = DHT_GET_IN ;
}



//
// Read the 40 bit data stream from the DHT 22
// Store the results in private member data to be read by public member functions
//

//
DHT22_ERROR_t DHT22_ReadData(void)
{
    u8 retryCount;
    u8 bitTimes[DHT22_DATA_BIT_COUNT];
    int currentHumidity;
    int currentTemperature;
    u8 checkSum, csPart1, csPart2, csPart3, csPart4;
    int i;
    currentHumidity = 0;
    currentTemperature = 0;
    checkSum = 0;

    if (DHT_GETTING_RDY == DHT.status)
    {
        DHT.status = DHT_GET_IN ;
        for (i = 0; i < DHT22_DATA_BIT_COUNT; i++)
        {
            bitTimes[i] = 0;
        }
        DHT_Mode_Out_PP();
        DHT_Out_Con = 0 ;
        Delay_us(1100);
        DHT_Out_Con = 1;
        Delay_us(30);
#if 1       
        DHT_Mode_IPU();
        // Find the start of the ACK Pulse
        retryCount = 0;

        do
        {
            if (retryCount > 100)  //(Spec is 80 us, 50*2 == 100 us)//(Spec is 20 to 40 us, 25*2 == 50 us)
            {
                return DHT_ERROR_NOT_PRESENT;
            }

            retryCount++;
            Delay_us(2);
        }
        while (0 == DHT_In_Status);

        // Find the end of the ACK Pulse
        retryCount = 0;

        do
        {
            if (retryCount > 100) //(Spec is 80 us, 50*2 == 100 us)
            {
                return DHT_ERROR_ACK_TOO_LONG;
            }

            retryCount++;
            Delay_us(2);
        }
        while (1 == DHT_In_Status);

        for (i = 0; i < DHT22_DATA_BIT_COUNT; i++)
        {
            // Find the start of the sync pulse
            retryCount = 0;

            do
            {
                if (retryCount > 35) //(Spec is 50 us, 35*2 == 70 us)
                {
                    return DHT_ERROR_SYNC_TIMEOUT;
                }

                retryCount++;
                Delay_us(2);
            }
            while (0 == DHT_In_Status);

            // Measure the width of the data pulse
            retryCount = 0;

            do
            {
                if (retryCount > 50) //(Spec is 80 us, 50*2 == 100 us)
                {
                    return DHT_ERROR_DATA_TIMEOUT;
                }

                retryCount++;
                Delay_us(2);
            }
            while (1 == DHT_In_Status);

            bitTimes[i] = retryCount;
        }

        // Now bitTimes have the number of retries (us *2)
        // that were needed to find the end of each data bit
        // Spec: 0 is 26 to 28 us
        // Spec: 1 is 70 us
        // bitTimes[x] <=  is a 0
        // bitTimes[x] >   is a 1
        // Note: the bits are offset by one from the data sheet, not sure why
        for (i = 0; i < 16; i++)
        {
            if (bitTimes[i] > 14)
            {
                currentHumidity |= (1 << (15 - i));
            }
        }

        for (i = 0; i < 16; i++)
        {
            if (bitTimes[i + 16] > 14)
            {
                currentTemperature |= (1 << (15 - i));
            }
        }

        for (i = 0; i < 8; i++)
        {
            if (bitTimes[i + 32] > 11)
            {
                checkSum |= (1 << (7 - i));
            }
        }

        csPart1 = currentHumidity >> 8;
        csPart2 = currentHumidity & 0xFF;
        csPart3 = currentTemperature >> 8;
        csPart4 = currentTemperature & 0xFF;

        if (checkSum == ((csPart1 + csPart2 + csPart3 + csPart4) & 0xFF))
        {
            DHT.lastHumidity = currentHumidity & 0x7FFF;

            if (currentTemperature & 0x8000)
            {
                // Below zero, non standard way of encoding negative numbers!
                // Convert to native negative format.
                DHT.lastTemperature = -(currentTemperature & 0x7FFF);
            }
            else
            {
                DHT.lastTemperature = currentTemperature;
            }

            return DHT_ERROR_NONE;
        }

        return DHT_ERROR_CHECKSUM;
#endif         
    }

    else
    {
        // Caller needs to wait 2 seconds between each call to readData
        return DHT_ERROR_TOOQUICK;
    }
}


void DHT22_GetData_Process(void)
{ 
    DHT22_ERROR_t errorCode;

    // The sensor can only be read from every 1-2s, and requires a minimum
    // 2s warm-up after power-on.

    errorCode = DHT22_ReadData();
    switch(errorCode)
    {
        case DHT_ERROR_NONE:
            DHT22_Print("Got Data ");
            DHT.tem = (float)(DHT.lastTemperature)/10;
            DHT22_Print("T:%3.1f C   ",DHT.tem);

            //      DHT22_Print("C ");

            DHT.hum = (float)(DHT.lastHumidity )/10;
            DHT22_Print("H:%3.1f%%",DHT.hum);
            // Alternately, with integer formatting which is clumsier but more compact to store and
            // can be compared reliably for equality:
            //	 
            break;
        case DHT_ERROR_CHECKSUM:
            DHT22_Print("check sum error ");
            break;
        case DHT_BUS_HUNG:
            DHT22_Print("BUS Hung ");
            break;
        case DHT_ERROR_NOT_PRESENT:
            DHT22_Print("Not Present ");
            break;
        case DHT_ERROR_ACK_TOO_LONG:
            DHT22_Print("ACK time out ");
            break;
        case DHT_ERROR_SYNC_TIMEOUT:
            DHT22_Print("Sync Timeout ");
            break;
        case DHT_ERROR_DATA_TIMEOUT:
            DHT22_Print("Data Timeout ");
            break;
        case DHT_ERROR_TOOQUICK:
            //DHT22_Print("Polled to quick ");
            break;
        default :
            break ;
    }
}



