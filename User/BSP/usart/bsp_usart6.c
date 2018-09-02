/**
  ******************************************************************************
  * @file    bsp_debug_usart.c
  * @author  fire
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   重定向c库printf函数到usart端口
  ******************************************************************************
  * @attention
  *
  * 实验平台:秉火  STM32 F429 开发板  
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :https://fire-stm32.taobao.com
  *
  ******************************************************************************
  */ 
  
#include "bsp_usart6.h"


 /**
  * @brief  BSP_USART3 GPIO 配置,工作模式配置。115200 8-N-1
  * @param  无
  * @retval 无
  */
 
void BSP_USART6_Config(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  USART_InitTypeDef USART_InitStructure;
		
  RCC_AHB1PeriphClockCmd(BSP_USART6_TX_GPIO_CLK, ENABLE);

  /* 使能 UART 时钟 */
  RCC_APB2PeriphClockCmd(BSP_USART6_CLK, ENABLE);

  /*  连接 PXx 到 USARTx__Rx*/
  GPIO_PinAFConfig(BSP_USART6_TX_GPIO_PORT,BSP_USART6_TX_SOURCE,BSP_USART6_TX_AF);

  /* 配置Tx引脚为复用功能  */
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;

  GPIO_InitStructure.GPIO_Pin = BSP_USART6_TX_PIN  ;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(BSP_USART6_TX_GPIO_PORT, &GPIO_InitStructure);

			
  /* 配置串BSP_USART6 模式 */
  USART_InitStructure.USART_BaudRate = BSP_USART6_BAUDRATE;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No ;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Tx;
  USART_Init(BSP_USART6, &USART_InitStructure); 
 
  USART_Cmd(BSP_USART6, ENABLE);
}



/*********************************************END OF FILE**********************/
