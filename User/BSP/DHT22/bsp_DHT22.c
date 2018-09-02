
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

//��λDHT22
void DHT22_Rst(void)	   
{               
	DHT22_IO_OUT(); 	//����Ϊ���
	DHT22_DQ_OUT=0; 	//����DQ

	bsp_DelayMS(20);    	//��������18ms

	DHT22_DQ_OUT=1; 	//DQ=1 

	bsp_DelayUS(30);     	//��������20~40us
	
}

//�ȴ�DHT22�Ļ�Ӧ
//����1:δ��⵽DHT22�Ĵ���
//����0:����
u8 DHT22_Check(void) 	   
{   
	
	u8 retry=0;
	DHT22_IO_IN();      //����Ϊ����	 
	while (DHT22_DQ_IN&&retry<100)//DHT22������40~80us
	{
		retry++;
		
		bsp_DelayUS(1);
	
	};	 
	if(retry>=100)return 1;
	else retry=0;
	while (!DHT22_DQ_IN&&retry<100)//DHT22���ͺ���ٴ�����40~80us
	{
		retry++;
		
		bsp_DelayUS(1);
	
	};
	if(retry>=100)return 1;	    
	return 0;
}

//��DHT22��ȡһ��λ
//����ֵ��1/0
u8 DHT22_Read_Bit(void) 			 
{
 	u8 retry=0;
	while(DHT22_DQ_IN&&retry<100)//�ȴ���Ϊ�͵�ƽ
	{
		retry++;
		bsp_DelayUS(1);
	}
	retry=0;
	while(!DHT22_DQ_IN&&retry<100)//�ȴ���ߵ�ƽ
	{
		retry++;
		bsp_DelayUS(1);
	}
	bsp_DelayUS(30);//�ȴ�40us
	if(DHT22_DQ_IN)return 1;
	else return 0;		   
}

//��DHT22��ȡһ���ֽ�
//����ֵ������������
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

//��DHT22��ȡһ������
//temp:�¶�ֵ(��Χ:0~50��)
//humi:ʪ��ֵ(��Χ:20%~90%)
//����ֵ��0,����;1,��ȡʧ��
u8 DHT22_Read_Data(u16 *temp,u16 *humi) 
{        
 	u8 buf[5];
	u8 i;
	DHT22_Rst();
	if(DHT22_Check()==0)
	{
		for(i=0;i<5;i++)//��ȡ40λ����
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
		/*����һ��GPIO_InitTypeDef���͵Ľṹ��*/
		GPIO_InitTypeDef GPIO_InitStructure;

		/*����DHT��ص�GPIO����ʱ��*/
		RCC_AHB1PeriphClockCmd ( DHT_GPIO_CLK, ENABLE); 

		/*ѡ��Ҫ���Ƶ�GPIO����*/															   
		GPIO_InitStructure.GPIO_Pin = DHT_PIN;	

		/*��������ģʽΪ���ģʽ*/
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;   
    
        /*�������ŵ��������Ϊ�������*/
        GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    
        /*��������Ϊ����ģʽ*/
        GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;

		/*������������Ϊ2MHz */   
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 

		/*���ÿ⺯����ʹ���������õ�GPIO_InitStructure��ʼ��GPIO*/
		GPIO_Init(DHT_GPIO_PORT, &GPIO_InitStructure);	
		
		DHT22_Rst();
		
		return DHT22_Check();
}

/*********************************************END OF FILE**********************/
