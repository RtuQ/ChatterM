#include "ctiic.h"

//����I2C�ٶȵ���ʱ
void CT_Delay(void)
{
	bsp_DelayUS(2);
} 
//���ݴ���оƬIIC�ӿڳ�ʼ��
void CT_IIC_Init(void)
{					     
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOH,ENABLE);			//����GPIOHʱ��
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOI,ENABLE);						//����GPIOIʱ��
    
    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_6;            //PH6
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT;  //�������
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;       //����
    GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;     //����
    GPIO_Init(GPIOH,&GPIO_InitStructure);     //��ʼ��
	
    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_3;            //PI3
    GPIO_Init(GPIOI,&GPIO_InitStructure);     //��ʼ��
    CT_IIC_SDA=1;	  	  
	CT_IIC_SCL=1;
}
//����IIC��ʼ�ź�
void CT_IIC_Start(void)
{
	CT_SDA_OUT();     //sda�����
	CT_IIC_SDA=1;	  	  
	CT_IIC_SCL=1;
	bsp_DelayUS(30);
 	CT_IIC_SDA=0;//START:when CLK is high,DATA change form high to low 
	CT_Delay();
	CT_IIC_SCL=0;//ǯסI2C���ߣ�׼�����ͻ�������� 
}	  
//����IICֹͣ�ź�
void CT_IIC_Stop(void)
{
	CT_SDA_OUT();//sda�����
	CT_IIC_SCL=0; 
	CT_IIC_SDA=0;//STOP:when CLK is high DATA change form low to high
	bsp_DelayUS(30);
	CT_IIC_SCL=1; 
	CT_Delay(); 
	CT_IIC_SDA=1;//����I2C���߽����ź�  
}
//�ȴ�Ӧ���źŵ���
//����ֵ��1������Ӧ��ʧ��
//        0������Ӧ��ɹ�
u8 CT_IIC_Wait_Ack(void)
{
	u8 ucErrTime=0;
	CT_SDA_IN();      //SDA����Ϊ����  
	CT_IIC_SDA=1;	   
	CT_IIC_SCL=1;
	CT_Delay();
	while(CT_READ_SDA)
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			CT_IIC_Stop();
			return 1;
		} 
		CT_Delay();
	}
	CT_IIC_SCL=0;//ʱ�����0 	   
	return 0;  
} 
//����ACKӦ��
void CT_IIC_Ack(void)
{
	CT_IIC_SCL=0;
	CT_SDA_OUT(); 
	CT_IIC_SDA=0;
	CT_Delay();
	CT_IIC_SCL=1;
	CT_Delay();
	CT_IIC_SCL=0;
}
//������ACKӦ��		    
void CT_IIC_NAck(void)
{
	CT_IIC_SCL=0;
	CT_SDA_OUT(); 
	CT_IIC_SDA=1;
	CT_Delay();
	CT_IIC_SCL=1;
	CT_Delay();
	CT_IIC_SCL=0;
}					 				     
//IIC����һ���ֽ�
//���شӻ�����Ӧ��
//1����Ӧ��
//0����Ӧ��			  
void CT_IIC_Send_Byte(u8 txd)
{                        
    u8 t;   
	CT_SDA_OUT(); 	    
    CT_IIC_SCL=0;//����ʱ�ӿ�ʼ���ݴ��� 
	for(t=0;t<8;t++)
    {              
        CT_IIC_SDA=(txd&0x80)>>7;
        txd<<=1; 
		CT_Delay();	      
		CT_IIC_SCL=1; 
		CT_Delay();
		CT_IIC_SCL=0;	 
		CT_Delay();
    }	 
} 	    
//��1���ֽڣ�ack=1ʱ������ACK��ack=0������nACK   
u8 CT_IIC_Read_Byte(unsigned char ack)
{
	u8 i,receive=0;
 	CT_SDA_IN();//SDA����Ϊ���� 
	bsp_DelayUS(30);
	for(i=0;i<8;i++ )
	{ 
		CT_IIC_SCL=0; 	    	   
		CT_Delay();
		CT_IIC_SCL=1;	 
		receive<<=1;
		if(CT_READ_SDA)receive++; 
		CT_Delay();  
	}	  				 
	if (!ack)CT_IIC_NAck();//����nACK
	else CT_IIC_Ack(); //����ACK   
 	return receive;
}





























