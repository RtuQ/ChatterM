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
  
#include "bsp_usart3.h"



 /**
  * @brief  ����Ƕ�������жϿ�����NVIC
  * @param  ��
  * @retval ��
  */
static void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  
//  /* Ƕ�������жϿ�������ѡ�� */
//  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  /* ����USARTΪ�ж�Դ */
  NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
  /* �������ȼ�Ϊ1 */
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  /* �����ȼ�Ϊ1 */
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  /* ʹ���ж� */
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  /* ��ʼ������NVIC */
  NVIC_Init(&NVIC_InitStructure);
}

 /**
  * @brief  BSP_USART3 GPIO ����,����ģʽ���á�115200 8-N-1
  * @param  ��
  * @retval ��
  */
 
void BSP_USART3_Config(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  USART_InitTypeDef USART_InitStructure;
		
  RCC_AHB1PeriphClockCmd( BSP_USART3_RX_GPIO_CLK|BSP_USART3_TX_GPIO_CLK, ENABLE);

  /* ʹ�� UART ʱ�� */
  RCC_APB1PeriphClockCmd(BSP_USART3_CLK, ENABLE);
  
  /* ���� PXx �� USARTx_Tx*/
  GPIO_PinAFConfig(BSP_USART3_RX_GPIO_PORT,BSP_USART3_RX_SOURCE, BSP_USART3_RX_AF);

  /*  ���� PXx �� USARTx__Rx*/
  GPIO_PinAFConfig(BSP_USART3_TX_GPIO_PORT,BSP_USART3_TX_SOURCE,BSP_USART3_TX_AF);

  /* ����Tx����Ϊ���ù���  */
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;

  GPIO_InitStructure.GPIO_Pin = BSP_USART3_TX_PIN  ;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(BSP_USART3_TX_GPIO_PORT, &GPIO_InitStructure);

  /* ����Rx����Ϊ���ù��� */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Pin = BSP_USART3_RX_PIN;
  GPIO_Init(BSP_USART3_RX_GPIO_PORT, &GPIO_InitStructure);
			
  /* ���ô�BSP_USART3 ģʽ */
  USART_InitStructure.USART_BaudRate = BSP_USART3_BAUDRATE;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No ;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  USART_Init(BSP_USART3, &USART_InitStructure); 
  
   /* Ƕ�������жϿ�����NVIC���� */
	NVIC_Configuration();
	
	/* ʹ�ܴ��ڽ����ж� */
	USART_ITConfig(BSP_USART3, USART_IT_RXNE, ENABLE);
  
  USART_Cmd(BSP_USART3, ENABLE);
}


/*****************  ����һ���ַ� **********************/
void Usart_SendByte( USART_TypeDef * pUSARTx, uint8_t ch)
{
	/* ����һ���ֽ����ݵ�USART */
	USART_SendData(pUSARTx,ch);
		
	/* �ȴ��������ݼĴ���Ϊ�� */
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);	
}

/*****************  �����ַ��� **********************/
void Usart_SendString( USART_TypeDef * pUSARTx, char *str)
{
	unsigned int k=0;
  do 
  {
      Usart_SendByte( pUSARTx, *(str + k) );
      k++;
  } while(*(str + k)!='\0');
  
  /* �ȴ�������� */
  while(USART_GetFlagStatus(pUSARTx,USART_FLAG_TC)==RESET)
  {}
}
/*****************  ����һ��16λ�� **********************/
void Usart_SendHalfWord( USART_TypeDef * pUSARTx, uint16_t ch)
{
	uint8_t temp_h, temp_l;
	
	/* ȡ���߰�λ */
	temp_h = (ch&0XFF00)>>8;
	/* ȡ���Ͱ�λ */
	temp_l = ch&0XFF;
	
	/* ���͸߰�λ */
	USART_SendData(pUSARTx,temp_h);	
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);
	
	/* ���͵Ͱ�λ */
	USART_SendData(pUSARTx,temp_l);	
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);	
}

void    Usart_Senddec(USART_TypeDef * pUSARTx,int dec)
{
    if(dec==0)
    {
        return;
    }
	Usart_SendByte( pUSARTx,(char)(dec%10 + '0'));
    Usart_Senddec(pUSARTx,dec/10);
    
}

///�ض���c�⺯��printf������DEBUG_USART���ض�����ʹ��printf����
int fputc(int ch, FILE *f)
{
		/* ����һ���ֽ����ݵ�����DEBUG_USART */
		USART_SendData(BSP_USART3, (uint8_t) ch);
		
		/* �ȴ�������� */
		while (USART_GetFlagStatus(BSP_USART3, USART_FLAG_TXE) == RESET);		
	
		return (ch);
}

///�ض���c�⺯��scanf������DEBUG_USART����д����ʹ��scanf��getchar�Ⱥ���
int fgetc(FILE *f)
{
		/* �ȴ������������� */
		while (USART_GetFlagStatus(BSP_USART3, USART_FLAG_RXNE) == RESET);

		return (int)USART_ReceiveData(BSP_USART3);
}
/*********************************************END OF FILE**********************/
