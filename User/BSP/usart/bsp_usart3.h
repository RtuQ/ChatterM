#ifndef __bsp_USART3_H
#define	__bsp_USART3_H

#include "stm32f4xx.h"
#include <stdio.h>



//引脚定义
/*******************************************************/
#define BSP_USART3                             USART3
#define BSP_USART3_CLK                         RCC_APB1Periph_USART3

#define BSP_USART3_RX_GPIO_PORT                GPIOB
#define BSP_USART3_RX_GPIO_CLK                 RCC_AHB1Periph_GPIOB
#define BSP_USART3_RX_PIN                      GPIO_Pin_11
#define BSP_USART3_RX_AF                       GPIO_AF_USART3
#define BSP_USART3_RX_SOURCE                   GPIO_PinSource11

#define BSP_USART3_TX_GPIO_PORT                GPIOB
#define BSP_USART3_TX_GPIO_CLK                 RCC_AHB1Periph_GPIOB
#define BSP_USART3_TX_PIN                      GPIO_Pin_10
#define BSP_USART3_TX_AF                       GPIO_AF_USART3
#define BSP_USART3_TX_SOURCE                   GPIO_PinSource10
/************************************************************/


//串口波特率
#define BSP_USART3_BAUDRATE                    115200



void BSP_USART3_Config(void);
void Usart_SendByte( USART_TypeDef * pUSARTx, uint8_t ch);
void Usart_SendString( USART_TypeDef * pUSARTx, char *str);
void Usart_SendHalfWord( USART_TypeDef * pUSARTx, uint16_t ch);

void    Usart_Senddec(USART_TypeDef * pUSARTx,int dec);
#endif /* __USART1_H */
