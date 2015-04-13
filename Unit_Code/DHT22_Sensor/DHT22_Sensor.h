#ifndef  __TH_SENSOR_H
#define  __TH_SENSOR_H

#define DHT22_DEBUG 1


#if(DHT22_DEBUG)
    #define     DHT22_Print  printf
#else
    #define  DHT22_Print    // 
#endif 



#define DHT22_ERROR_VALUE -995

// This should be 40, but the sensor is adding an extra bit at the start
//#define DHT22_DATA_BIT_COUNT 41

#define DHT22_DATA_BIT_COUNT 40

typedef enum { DHT_GETTING_RDY = 0,  DHT_GET_IN  = ! DHT_GETTING_RDY} DHT_Status;

#define DHT_Pin  GPIOB
#define DHT_Con_Pin  GPIO_Pin_7  //PA13
#define DHT_In_Status  PBin(7)
#define DHT_Out_Con   PBout(7)

typedef enum
{
  DHT_ERROR_NONE = 0,
  DHT_BUS_HUNG,
  DHT_ERROR_NOT_PRESENT,
  DHT_ERROR_ACK_TOO_LONG,
  DHT_ERROR_SYNC_TIMEOUT,
  DHT_ERROR_DATA_TIMEOUT,
  DHT_ERROR_CHECKSUM,
  DHT_ERROR_TOOQUICK
} DHT22_ERROR_t;


typedef struct Temperature
{
    u8 status ;
    float tem;  //Temperature
    float hum;  //Humidity

    short int lastHumidity;
    short int lastTemperature;
}DHT_t  ;  //DHT_t ;


extern DHT_t DHT ;

void DHT22_Init(void);
void DHT22_GetData_Process(void);

#endif
