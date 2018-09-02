
#include "bsp_DHT22.h"   

#include "math.h"
u16 conversion(u16 s1)
{
	u8 num=0;
	u16 sum=0;
	for(num=0;num<16;num++)
	{
		
		if(num==0&&s1&0x0001)
		{
			sum+=pow(2,0);
		}
		else if (s1>>num&0x0001)
		{
			sum+=pow(2,num);
		}
	}
	return sum;
}

u16 Turn_16(u8 *s)
{
    u16 f_data;
	f_data&=0x00;
	f_data|=*s;
	f_data=f_data<<8;
	f_data|=*(s+1);
	return f_data;
}

//复位DHT22
void DHT22_Rst(void)	   
{               
	DHT22_IO_OUT(); 	//设置为输出
	DHT22_DQ_OUT=0; 	//拉低DQ

	bsp_DelayMS(20);    	//拉低至少18ms

	DHT22_DQ_OUT=1; 	//DQ=1 

	bsp_DelayUS(30);     	//主机拉高20~40us
	
}

//等待DHT22的回应
//返回1:未检测到DHT22的存在
//返回0:存在
u8 DHT22_Check(void) 	   
{   
	
	u8 retry=0;
	DHT22_IO_IN();      //设置为输入	 
	while (DHT22_DQ_IN&&retry<100)//DHT22会拉低40~80us
	{
		retry++;
		
		bsp_DelayUS(1);
	
	};	 
	if(retry>=100)return 1;
	else retry=0;
	while (!DHT22_DQ_IN&&retry<100)//DHT22拉低后会再次拉高40~80us
	{
		retry++;
		
		bsp_DelayUS(1);
	
	};
	if(retry>=100)return 1;	    
	return 0;
}

//从DHT22读取一个位
//返回值：1/0
u8 DHT22_Read_Bit(void) 			 
{
 	u8 retry=0;
	while(DHT22_DQ_IN&&retry<100)//等待变为低电平
	{
		retry++;
		bsp_DelayUS(1);
	}
	retry=0;
	while(!DHT22_DQ_IN&&retry<100)//等待变高电平
	{
		retry++;
		bsp_DelayUS(1);
	}
	bsp_DelayUS(30);//等待40us
	if(DHT22_DQ_IN)return 1;
	else return 0;		   
}

//从DHT22读取一个字节
//返回值：读到的数据
u8 DHT22_Read_Byte(void)    
{        
	u8 i,dat;
	dat=0;
	for (i=0;i<8;i++) 
	{
   		dat<<=1; 
	    dat|=DHT22_Read_Bit();
    }						    
    return dat;
}

//从DHT22读取一次数据
//temp:温度值(范围:0~50°)
//humi:湿度值(范围:20%~90%)
//返回值：0,正常;1,读取失败
u8 DHT22_Read_Data(u16 *temp,u16 *humi) 
{        
 	u8 buf[5];
	u8 i;
	DHT22_Rst();
	if(DHT22_Check()==0)
	{
		for(i=0;i<5;i++)//读取40位数据
		{
			buf[i]=DHT22_Read_Byte();
		}
		if((buf[0]+buf[1]+buf[2]+buf[3])==buf[4])
		{
		
           
			
			*humi=conversion(Turn_16(&buf[0]));
			*temp=conversion(Turn_16(&buf[2]));
		}
		
	}else return 1;
	return 0;	    
}




u8 DHT_GPIO_Config(void)
{		
		/*定义一个GPIO_InitTypeDef类型的结构体*/
		GPIO_InitTypeDef GPIO_InitStructure;

		/*开启DHT相关的GPIO外设时钟*/
		RCC_AHB1PeriphClockCmd ( DHT_GPIO_CLK, ENABLE); 

		/*选择要控制的GPIO引脚*/															   
		GPIO_InitStructure.GPIO_Pin = DHT_PIN;	

		/*设置引脚模式为输出模式*/
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;   
    
        /*设置引脚的输出类型为推挽输出*/
        GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    
        /*设置引脚为上拉模式*/
        GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;

		/*设置引脚速率为2MHz */   
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 

		/*调用库函数，使用上面配置的GPIO_InitStructure初始化GPIO*/
		GPIO_Init(DHT_GPIO_PORT, &GPIO_InitStructure);	
		
		DHT22_Rst();
		
		return DHT22_Check();
}

/*********************************************END OF FILE**********************/
