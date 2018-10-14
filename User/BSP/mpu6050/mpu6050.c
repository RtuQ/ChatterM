#include "mpu6050.h"
/**
  * @brief   д���ݵ�MPU6050�Ĵ���
  * @param   reg_add:�Ĵ�����ַ
	* @param	 reg_data:Ҫд�������
  * @retval  
  */
u8 MPU6050_WriteReg(u8 reg_add,u8 reg_dat)
{
	i2c_Start();		/* ���������ź� */
	/* �����豸��ַ+��д����bit��0 = w�� 1 = r) bit7 �ȴ� */
	i2c_SendByte((MPU6050_ADDRESS<<1) | EEPROM_I2C_WR);
	if(i2c_WaitAck())	//�ȴ�Ӧ��
	{
		i2c_Stop();		
//		Debug_printf("no ack!\n");		
	}
	i2c_SendByte(reg_add);
	i2c_WaitAck();
	i2c_SendByte(reg_dat);
	if(i2c_WaitAck())	//�ȴ�Ӧ��
	{
		i2c_Stop();
        return 1;		
	}
	
	i2c_Stop();
	return 0;
}

/**
  * @brief   ��MPU6050�Ĵ�����ȡ����
  * @param   reg_add:�Ĵ�����ַ
	* @param	 Read���洢���ݵĻ�����
	* @param	 num��Ҫ��ȡ��������
  * @retval  
  */
uint8_t MPU6050_ReadData(u8 reg_add)
{
	u8 MPU6050_data;
	i2c_Start();		/* ���������ź� */
	/* �����豸��ַ+��д����bit��0 = w�� 1 = r) bit7 �ȴ� */
	i2c_SendByte((MPU6050_ADDRESS<<1) | EEPROM_I2C_WR);
	i2c_WaitAck();
	i2c_SendByte(reg_add);
	i2c_WaitAck();
	i2c_Start();		/* ���������ź� */
	/* �����豸��ַ+��д����bit��0 = w�� 1 = r) bit7 �ȴ� */
	i2c_SendByte((MPU6050_ADDRESS<<1) | EEPROM_I2C_RD);
	i2c_WaitAck();
	MPU6050_data = i2c_ReadByte(0);
	i2c_Stop();
	return MPU6050_data;
}


/**
  * @brief   ��ʼ��MPU6050оƬ
  * @param   
  * @retval  
  */
void MPU6050_Init(void)
{
  	MPU6050_WriteReg(MPU6050_RA_PWR_MGMT_1,0X80);	//��λMPU6050
    bsp_DelayMS(200);
	MPU6050_WriteReg(MPU6050_RA_PWR_MGMT_1, 0x00);	     //�������״̬
	MPU_Set_Gyro_Fsr(3);
	MPU_Set_Accel_Fsr(0);
	MPU_Set_Rate(50);
	MPU6050_WriteReg(MPU6050_RA_INT_ENABLE,0X00);	//�ر������ж�
	MPU6050_WriteReg(MPU6050_RA_USER_CTRL,0X00);	//I2C��ģʽ�ر�
	MPU6050_WriteReg(MPU6050_RA_FIFO_EN,0x00);	//�ر�FIFO
	MPU6050_WriteReg(MPU6050_RA_INT_PIN_CFG,0X80);	//INT���ŵ͵�ƽ��Ч
	MPU6050_ReadData(MPU6050_WHO_AM_I);
	
	MPU6050_WriteReg(MPU6050_RA_PWR_MGMT_1, 0x01);	     
	MPU6050_WriteReg(MPU6050_RA_PWR_MGMT_2, 0x00);	     
	MPU_Set_Rate(50);
}



/**
  * @brief   ����MPU6050�����Ǵ����������̷�Χ
  * @param   fsr:0,��250dps;1,��500dps;2,��1000dps;3,��2000dps
  * @retval  ��������0���쳣����1
  */
u8 MPU_Set_Gyro_Fsr(u8 fsr)
{
	return MPU6050_WriteReg(MPU6050_RA_GYRO_CONFIG,fsr<<3);//���������������̷�Χ  
}

/**
  * @brief   ����MPU6050���ٶȴ����������̷�Χ
  * @param   fsr:0,��2g;1,��4g;2,��8g;3,��16g
  * @retval  ��������0���쳣����1
  */
u8 MPU_Set_Accel_Fsr(u8 fsr)
{
	return MPU6050_WriteReg(MPU6050_RA_ACCEL_CONFIG,fsr<<3);//���ü��ٶȴ����������̷�Χ  
}

/**
  * @brief   ����MPU6050�����ֵ�ͨ�˲���
  * @param   lpf:���ֵ�ͨ�˲�Ƶ��(Hz)
  * @retval  ��������0���쳣����1
  */
u8 MPU_Set_LPF(u16 lpf)
{
	u8 data=0;
	if(lpf>=188)data=1;
	else if(lpf>=98)data=2;
	else if(lpf>=42)data=3;
	else if(lpf>=20)data=4;
	else if(lpf>=10)data=5;
	else data=6; 
	return MPU6050_WriteReg(MPU6050_RA_CONFIG,data);//�������ֵ�ͨ�˲���  
}
/**
  * @brief   ����MPU6050�Ĳ�����(�ٶ�Fs=1KHz)
  * @param   rate:4~1000(Hz)
  * @retval  ��������0���쳣����1
  */
u8 MPU_Set_Rate(u16 rate)
{
	u8 data;
	if(rate>1000)rate=1000;
	if(rate<4)rate=4;
	data=1000/rate-1;
	data=MPU6050_WriteReg(MPU6050_RA_SMPLRT_DIV,data);	//�������ֵ�ͨ�˲���
 	return MPU_Set_LPF(rate/2);	//�Զ�����LPFΪ�����ʵ�һ��
}
/**
  * @brief   IIC����д
  * @param   addr:������ַ
             reg:�Ĵ�����ַ
			 len:д�볤��
			 buf:������
  * @retval  ��������0���쳣����1
  */
u8 MPU_Write_Len(u8 addr,u8 reg,u8 len,u8 *buf)
{
	u8 i; 
    i2c_Start(); 
	i2c_SendByte((addr<<1)|0);//����������ַ+д����	
	if(i2c_WaitAck())	//�ȴ�Ӧ��
	{
		i2c_Stop();		 
		return 1;		
	}
    i2c_SendByte(reg);	//д�Ĵ�����ַ
    i2c_WaitAck();		//�ȴ�Ӧ��
	for(i=0;i<len;i++)
	{
		i2c_SendByte(buf[i]);	//��������
		if(i2c_WaitAck())		//�ȴ�ACK
		{
			i2c_Stop();	 
			return 1;		 
		}		
	}    
    i2c_Stop();	 
	return 0;	
} 

/**
  * @brief   IIC������
  * @param   
			addr:������ַ
			reg:Ҫ��ȡ�ļĴ�����ַ
			len:Ҫ��ȡ�ĳ���
			buf:��ȡ�������ݴ洢��
  * @retval  ��������0���쳣����1
  */
u8 MPU_Read_Len(u8 addr,u8 reg,u8 len,u8 *buf)
{ 
 	i2c_Start(); 
	i2c_SendByte((addr<<1)|0);//����������ַ+д����	
	if(i2c_WaitAck())	//�ȴ�Ӧ��
	{
		i2c_Stop();		 
		return 1;		
	}
    i2c_SendByte(reg);	//д�Ĵ�����ַ
    i2c_WaitAck();		//�ȴ�Ӧ��
    i2c_Start();
	i2c_SendByte((addr<<1)|1);//����������ַ+������	
    i2c_WaitAck();		//�ȴ�Ӧ�� 
	while(len)
	{
		if(len==1)*buf=i2c_ReadByte(0);//������,����nACK 
		else *buf=i2c_ReadByte(1);		//������,����ACK  
		len--;
		buf++; 
	}    
    i2c_Stop();	//����һ��ֹͣ���� 
	return 0;	
}

/**
  * @brief   ��ȡMPU6050��ID
  * @param   
  * @retval  ��������1���쳣����0
  */
uint8_t MPU6050ReadID(void)
{
	unsigned char Re = 0;
    Re=MPU6050_ReadData(MPU6050_RA_WHO_AM_I);    //��������ַ
	if(Re != 0x68)
	{
		Debug_printf("MPU6050 dectected error!\r\n��ⲻ��MPU6050ģ�飬����ģ���뿪����Ľ���");
		Debug_printf("MPU6050 ID = %X\r\n",Re);
		return 0;
	}
	else
	{
		Debug_printf("MPU6050 ID = %X\r\n",Re);
		return 1;
	}
		
}

/**
  * @brief   ��ȡMPU6050�ļ��ٶ�����
  * @param   
  * @retval  
  */
void MPU6050ReadAcc(short *accData)
{
    u8 buf[6];
    buf[0]=MPU6050_ReadData(MPU6050_RA_ACCEL_XOUT_H);
	buf[1]=MPU6050_ReadData(MPU6050_RA_ACCEL_XOUT_L);
	buf[2]=MPU6050_ReadData(MPU6050_RA_ACCEL_YOUT_H);
	buf[3]=MPU6050_ReadData(MPU6050_RA_ACCEL_YOUT_L);
	buf[4]=MPU6050_ReadData(MPU6050_RA_ACCEL_ZOUT_H);
	buf[5]=MPU6050_ReadData(MPU6050_RA_ACCEL_ZOUT_L);
    accData[0] = (buf[0] << 8) | buf[1];
    accData[1] = (buf[2] << 8) | buf[3];
    accData[2] = (buf[4] << 8) | buf[5];
}

/**
  * @brief   ��ȡMPU6050�ĽǼ��ٶ�����
  * @param   
  * @retval  
  */
void MPU6050ReadGyro(short *gyroData)
{
    u8 buf[6];
    buf[0]=MPU6050_ReadData(MPU6050_RA_GYRO_XOUT_H);
	buf[1]=MPU6050_ReadData(MPU6050_RA_GYRO_XOUT_L);
	buf[2]=MPU6050_ReadData(MPU6050_RA_GYRO_YOUT_H);
	buf[3]=MPU6050_ReadData(MPU6050_RA_GYRO_YOUT_L);
	buf[4]=MPU6050_ReadData(MPU6050_RA_GYRO_ZOUT_H);
	buf[5]=MPU6050_ReadData(MPU6050_RA_GYRO_ZOUT_L);
    gyroData[0] = (buf[0] << 8) | buf[1];
    gyroData[1] = (buf[2] << 8) | buf[3];
    gyroData[2] = (buf[4] << 8) | buf[5];
}

/**
  * @brief   ��ȡMPU6050��ԭʼ�¶�����
  * @param   
  * @retval  
  */
void MPU6050ReadTemp(short *tempData)
{
	u8 buf[2];
    buf[0]=MPU6050_ReadData(MPU6050_RA_TEMP_OUT_H);
	buf[1]=MPU6050_ReadData(MPU6050_RA_TEMP_OUT_L);
    *tempData = (buf[0] << 8) | buf[1];
}

/**
  * @brief   ��ȡMPU6050���¶����ݣ�ת�������϶�
  * @param   
  * @retval  
  */
void MPU6050_ReturnTemp(float *Temperature)
{
	short temp3;
	u8 buf[2];
	
	buf[0]=MPU6050_ReadData(MPU6050_RA_TEMP_OUT_H);
	buf[1]=MPU6050_ReadData(MPU6050_RA_TEMP_OUT_L);
  temp3= (buf[0] << 8) | buf[1];	
	*Temperature=((double) temp3/340.0)+36.53;

}
