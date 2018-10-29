#ifndef _BSP_I2C_GPIO_H
#define _BSP_I2C_GPIO_H

#include "includes.h"


#define EEPROM_I2C_WR	0		/* д����bit */
#define EEPROM_I2C_RD	1		/* ������bit */


/* ����I2C�������ӵ�GPIO�˿�, �û�ֻ��Ҫ�޸�����4�д��뼴������ı�SCL��SDA������ */
#define I2C_GPIO_PORT				GPIOH			/* GPIO�˿� */
#define I2C_GPIO_CLK			 	RCC_AHB1Periph_GPIOH		/* GPIO�˿�ʱ�� */
#define I2C_SCL_PIN					GPIO_Pin_2			/* ���ӵ�SCLʱ���ߵ�GPIO */
#define I2C_SDA_PIN					GPIO_Pin_3			/* ���ӵ�SDA�����ߵ�GPIO */


#define I2C_SCL  PHout(2)		
#define I2C_SDA  PHout(3)	    	
#define I2C_SDA_READ  PHin(3)

/* ��������F4�Ķ˿�ת�������� ������Ҫһ������ʱ���� */
#define SDA_IN()  {GPIOH->MODER&=~(3<<(3*2));GPIOH->MODER|=0<<3*2;bsp_DelayUS(1);}	//PH2����ģʽ
#define SDA_OUT() {GPIOH->MODER&=~(3<<(3*2));GPIOH->MODER|=1<<3*2;bsp_DelayUS(1);} //PH2���ģʽ



void i2c_Start(void);
void i2c_Stop(void);
void i2c_SendByte(uint8_t _ucByte);
uint8_t i2c_ReadByte(u8 ack);
uint8_t i2c_WaitAck(void);
void i2c_Ack(void);
void i2c_NAck(void);
uint8_t i2c_CheckDevice(uint8_t _Address);
void i2c_CfgGpio(void);


#endif

