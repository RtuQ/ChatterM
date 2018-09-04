
#include "bsp_rng.h"   

 /**
  * @brief  ��ʼ��RNG
  * @param  ��
  * @retval ��
  */
void RNG_Config(void)
{		
	RCC_AHB2PeriphClockCmd(RCC_AHB2Periph_RNG,ENABLE);
	RNG_Cmd(ENABLE);
}

uint8_t Read_Seed(void)
{
	uint32_t data;
	uint8_t datato2;
	while(RNG_GetFlagStatus(RNG_FLAG_DRDY) == RESET);  //�ȴ������׼�����
	data = RNG_GetRandomNumber();   //����
	datato2 = data%100;
    return datato2;
}

/*********************************************END OF FILE**********************/
