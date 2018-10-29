#ifndef _BSP_I2C_GPIO_H
#define _BSP_I2C_GPIO_H

#include "includes.h"


#define EEPROM_I2C_WR	0		/* 写控制bit */
#define EEPROM_I2C_RD	1		/* 读控制bit */


/* 定义I2C总线连接的GPIO端口, 用户只需要修改下面4行代码即可任意改变SCL和SDA的引脚 */
#define I2C_GPIO_PORT				GPIOH			/* GPIO端口 */
#define I2C_GPIO_CLK			 	RCC_AHB1Periph_GPIOH		/* GPIO端口时钟 */
#define I2C_SCL_PIN					GPIO_Pin_2			/* 连接到SCL时钟线的GPIO */
#define I2C_SDA_PIN					GPIO_Pin_3			/* 连接到SDA数据线的GPIO */


#define I2C_SCL  PHout(2)		
#define I2C_SDA  PHout(3)	    	
#define I2C_SDA_READ  PHin(3)

/* 可能由于F4的端口转换有问题 这里需要一定的延时才行 */
#define SDA_IN()  {GPIOH->MODER&=~(3<<(3*2));GPIOH->MODER|=0<<3*2;bsp_DelayUS(1);}	//PH2输入模式
#define SDA_OUT() {GPIOH->MODER&=~(3<<(3*2));GPIOH->MODER|=1<<3*2;bsp_DelayUS(1);} //PH2输出模式



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

