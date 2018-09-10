#ifndef __BSP_ADC_H
#define	__BSP_ADC_H

#include "includes.h"

#define RHEOSTAT_NOFCHANEL      2

/*=====================ͨ��1 IO======================*/
// PC3 ͨ����ñ�ӵ�λ��
// ADC IO�궨��
#define RHEOSTAT_ADC_GPIO_PORT1    GPIOA
#define RHEOSTAT_ADC_GPIO_PIN1     GPIO_Pin_7
#define RHEOSTAT_ADC_GPIO_CLK1     RCC_AHB1Periph_GPIOA
#define RHEOSTAT_ADC_CHANNEL1      ADC_Channel_7
/*=====================ͨ��2 IO ======================*/
// PA4 ͨ����ñ�ӹ�������
// ADC IO�궨��
#define RHEOSTAT_ADC_GPIO_PORT2    GPIOA
#define RHEOSTAT_ADC_GPIO_PIN2     GPIO_Pin_5
#define RHEOSTAT_ADC_GPIO_CLK2     RCC_AHB1Periph_GPIOA
#define RHEOSTAT_ADC_CHANNEL2      ADC_Channel_5
/*=====================ͨ��3 IO ======================*/
// PA6 ���գ����öŰ��߽�3V3����GND��ʵ��
// ADC IO�궨��
//#define RHEOSTAT_ADC_GPIO_PORT3    GPIOA
//#define RHEOSTAT_ADC_GPIO_PIN3     GPIO_Pin_6
//#define RHEOSTAT_ADC_GPIO_CLK3     RCC_AHB1Periph_GPIOA
//#define RHEOSTAT_ADC_CHANNEL3     ADC_Channel_6

// ADC ��ź궨��
#define RHEOSTAT_ADC              ADC1
#define RHEOSTAT_ADC_CLK          RCC_APB2Periph_ADC1
// ADC DR�Ĵ����궨�壬ADCת���������ֵ����������
#define RHEOSTAT_ADC_DR_ADDR    ((u32)ADC1+0x4c)


// ADC DMA ͨ���궨�壬��������ʹ��DMA����
#define RHEOSTAT_ADC_DMA_CLK      RCC_AHB1Periph_DMA2
#define RHEOSTAT_ADC_DMA_CHANNEL  DMA_Channel_0
#define RHEOSTAT_ADC_DMA_STREAM   DMA2_Stream0



void Rheostat_Init(void);

#endif /* __BSP_ADC_H */



