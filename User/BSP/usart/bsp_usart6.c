/**
  ******************************************************************************
  * @file    bsp_debug_usart.c
  * @author  fire
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   �ض���c��printf������usart�˿�
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:����  STM32 F429 ������  
  * ��̳    :http://www.firebbs.cn
  * �Ա�    :https://fire-stm32.taobao.com
  *
  ******************************************************************************
  */ 
  
#include "bsp_usart6.h"


 /**
  * @brief  BSP_USART3 GPIO ����,����ģʽ���á�115200 8-N-1
  * @param  ��
  * @retval ��
  */
 
void BSP_USART6_Config(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  USART_InitTypeDef USART_InitStructure;
		
  RCC_AHB1PeriphClockCmd(BSP_USART6_TX_GPIO_CLK, ENABLE);

  /* ʹ�� UART ʱ�� */
  RCC_APB2PeriphClockCmd(BSP_USART6_CLK, ENABLE);

  /*  ���� PXx �� USARTx__Rx*/
  GPIO_PinAFConfig(BSP_USART6_TX_GPIO_PORT,BSP_USART6_TX_SOURCE,BSP_USART6_TX_AF);

  /* ����Tx����Ϊ���ù���  */
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;

  GPIO_InitStructure.GPIO_Pin = BSP_USART6_TX_PIN  ;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(BSP_USART6_TX_GPIO_PORT, &GPIO_InitStructure);

			
  /* ���ô�BSP_USART6 ģʽ */
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
