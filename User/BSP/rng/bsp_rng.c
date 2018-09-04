
#include "bsp_rng.h"   

 /**
  * @brief  初始化RNG
  * @param  无
  * @retval 无
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
	while(RNG_GetFlagStatus(RNG_FLAG_DRDY) == RESET);  //等待随机数准备完毕
	data = RNG_GetRandomNumber();   //读数
	datato2 = data%100;
    return datato2;
}

/*********************************************END OF FILE**********************/
