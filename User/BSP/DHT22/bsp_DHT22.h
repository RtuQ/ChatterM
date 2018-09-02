#ifndef __bsp_DHT22_H
#define	__bsp_DHT22_H

#include "stm32f4xx.h"
#include "includes.h"


/*******************************************************/

#define DHT_PIN                  GPIO_Pin_1                 
#define DHT_GPIO_PORT            GPIOC                
#define DHT_GPIO_CLK             RCC_AHB1Periph_GPIOC

/************************************************************/


/* �������IO�ĺ� */
#define DHT_TOGGLE		digitalToggle(DHT_GPIO_PORT,DHT_PIN)
#define DHT_OFF			digitalLo(DHT_GPIO_PORT,DHT_PIN)
#define DHT_ON				digitalHi(DHT_GPIO_PORT,DHT_PIN)


/*IO��������*/
#define DHT22_IO_IN()  {GPIOC->MODER&=~(3<<(1*2));GPIOC->MODER|=0<<(1*2);}	//PC1����ģʽ
#define DHT22_IO_OUT() {GPIOC->MODER&=~(3<<(1*2));GPIOC->MODER|=1<<(1*2);} 	//PC1���ģʽ
 
/*IOλ��������*/										   
#define	DHT22_DQ_OUT    PCout(1)//���ݶ˿�	PC1
#define	DHT22_DQ_IN     PCin(1) //���ݶ˿�	PC1 


u8 DHT_GPIO_Config(void);
u8 DHT22_Read_Data(u16 *temp,u16 *humi);
#endif
