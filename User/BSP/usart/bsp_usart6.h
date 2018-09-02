#ifndef __bsp_USART6_H
#define	__bsp_USART6_H

#include "stm32f4xx.h"
#include <stdio.h>



//引脚定义
/*******************************************************/
#define BSP_USART6                             USART6
#define BSP_USART6_CLK                         RCC_APB2Periph_USART6

// #define BSP_USART6_RX_GPIO_PORT                GPIOB
// #define BSP_USART6_RX_GPIO_CLK                 RCC_AHB1Periph_GPIOB
// #define BSP_USART6_RX_PIN                      GPIO_Pin_11
// #define BSP_USART6_RX_AF                       GPIO_AF_USART6
// #define BSP_USART6_RX_SOURCE                   GPIO_PinSource11

#define BSP_USART6_TX_GPIO_PORT                GPIOG
#define BSP_USART6_TX_GPIO_CLK                 RCC_AHB1Periph_GPIOG
#define BSP_USART6_TX_PIN                      GPIO_Pin_14
#define BSP_USART6_TX_AF                       GPIO_AF_USART6
#define BSP_USART6_TX_SOURCE                   GPIO_PinSource14
/************************************************************/


//串口波特率
#define BSP_USART6_BAUDRATE                    115200



void BSP_USART6_Config(void);
#endif /* __USART1_H */
