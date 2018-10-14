#include "mpu6050.h"
/**
  * @brief   写数据到MPU6050寄存器
  * @param   reg_add:寄存器地址
	* @param	 reg_data:要写入的数据
  * @retval  
  */
u8 MPU6050_WriteReg(u8 reg_add,u8 reg_dat)
{
	i2c_Start();		/* 发送启动信号 */
	/* 发送设备地址+读写控制bit（0 = w， 1 = r) bit7 先传 */
	i2c_SendByte((MPU6050_ADDRESS<<1) | EEPROM_I2C_WR);
	if(i2c_WaitAck())	//等待应答
	{
		i2c_Stop();		
//		Debug_printf("no ack!\n");		
	}
	i2c_SendByte(reg_add);
	i2c_WaitAck();
	i2c_SendByte(reg_dat);
	if(i2c_WaitAck())	//等待应答
	{
		i2c_Stop();
        return 1;		
	}
	
	i2c_Stop();
	return 0;
}

/**
  * @brief   从MPU6050寄存器读取数据
  * @param   reg_add:寄存器地址
	* @param	 Read：存储数据的缓冲区
	* @param	 num：要读取的数据量
  * @retval  
  */
uint8_t MPU6050_ReadData(u8 reg_add)
{
	u8 MPU6050_data;
	i2c_Start();		/* 发送启动信号 */
	/* 发送设备地址+读写控制bit（0 = w， 1 = r) bit7 先传 */
	i2c_SendByte((MPU6050_ADDRESS<<1) | EEPROM_I2C_WR);
	i2c_WaitAck();
	i2c_SendByte(reg_add);
	i2c_WaitAck();
	i2c_Start();		/* 发送启动信号 */
	/* 发送设备地址+读写控制bit（0 = w， 1 = r) bit7 先传 */
	i2c_SendByte((MPU6050_ADDRESS<<1) | EEPROM_I2C_RD);
	i2c_WaitAck();
	MPU6050_data = i2c_ReadByte(0);
	i2c_Stop();
	return MPU6050_data;
}


/**
  * @brief   初始化MPU6050芯片
  * @param   
  * @retval  
  */
void MPU6050_Init(void)
{
  	MPU6050_WriteReg(MPU6050_RA_PWR_MGMT_1,0X80);	//复位MPU6050
    bsp_DelayMS(200);
	MPU6050_WriteReg(MPU6050_RA_PWR_MGMT_1, 0x00);	     //解除休眠状态
	MPU_Set_Gyro_Fsr(3);
	MPU_Set_Accel_Fsr(0);
	MPU_Set_Rate(50);
	MPU6050_WriteReg(MPU6050_RA_INT_ENABLE,0X00);	//关闭所有中断
	MPU6050_WriteReg(MPU6050_RA_USER_CTRL,0X00);	//I2C主模式关闭
	MPU6050_WriteReg(MPU6050_RA_FIFO_EN,0x00);	//关闭FIFO
	MPU6050_WriteReg(MPU6050_RA_INT_PIN_CFG,0X80);	//INT引脚低电平有效
	MPU6050_ReadData(MPU6050_WHO_AM_I);
	
	MPU6050_WriteReg(MPU6050_RA_PWR_MGMT_1, 0x01);	     
	MPU6050_WriteReg(MPU6050_RA_PWR_MGMT_2, 0x00);	     
	MPU_Set_Rate(50);
}



/**
  * @brief   设置MPU6050陀螺仪传感器满量程范围
  * @param   fsr:0,±250dps;1,±500dps;2,±1000dps;3,±2000dps
  * @retval  正常返回0，异常返回1
  */
u8 MPU_Set_Gyro_Fsr(u8 fsr)
{
	return MPU6050_WriteReg(MPU6050_RA_GYRO_CONFIG,fsr<<3);//设置陀螺仪满量程范围  
}

/**
  * @brief   设置MPU6050加速度传感器满量程范围
  * @param   fsr:0,±2g;1,±4g;2,±8g;3,±16g
  * @retval  正常返回0，异常返回1
  */
u8 MPU_Set_Accel_Fsr(u8 fsr)
{
	return MPU6050_WriteReg(MPU6050_RA_ACCEL_CONFIG,fsr<<3);//设置加速度传感器满量程范围  
}

/**
  * @brief   设置MPU6050的数字低通滤波器
  * @param   lpf:数字低通滤波频率(Hz)
  * @retval  正常返回0，异常返回1
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
	return MPU6050_WriteReg(MPU6050_RA_CONFIG,data);//设置数字低通滤波器  
}
/**
  * @brief   设置MPU6050的采样率(假定Fs=1KHz)
  * @param   rate:4~1000(Hz)
  * @retval  正常返回0，异常返回1
  */
u8 MPU_Set_Rate(u16 rate)
{
	u8 data;
	if(rate>1000)rate=1000;
	if(rate<4)rate=4;
	data=1000/rate-1;
	data=MPU6050_WriteReg(MPU6050_RA_SMPLRT_DIV,data);	//设置数字低通滤波器
 	return MPU_Set_LPF(rate/2);	//自动设置LPF为采样率的一半
}
/**
  * @brief   IIC连续写
  * @param   addr:器件地址
             reg:寄存器地址
			 len:写入长度
			 buf:数据区
  * @retval  正常返回0，异常返回1
  */
u8 MPU_Write_Len(u8 addr,u8 reg,u8 len,u8 *buf)
{
	u8 i; 
    i2c_Start(); 
	i2c_SendByte((addr<<1)|0);//发送器件地址+写命令	
	if(i2c_WaitAck())	//等待应答
	{
		i2c_Stop();		 
		return 1;		
	}
    i2c_SendByte(reg);	//写寄存器地址
    i2c_WaitAck();		//等待应答
	for(i=0;i<len;i++)
	{
		i2c_SendByte(buf[i]);	//发送数据
		if(i2c_WaitAck())		//等待ACK
		{
			i2c_Stop();	 
			return 1;		 
		}		
	}    
    i2c_Stop();	 
	return 0;	
} 

/**
  * @brief   IIC连续读
  * @param   
			addr:器件地址
			reg:要读取的寄存器地址
			len:要读取的长度
			buf:读取到的数据存储区
  * @retval  正常返回0，异常返回1
  */
u8 MPU_Read_Len(u8 addr,u8 reg,u8 len,u8 *buf)
{ 
 	i2c_Start(); 
	i2c_SendByte((addr<<1)|0);//发送器件地址+写命令	
	if(i2c_WaitAck())	//等待应答
	{
		i2c_Stop();		 
		return 1;		
	}
    i2c_SendByte(reg);	//写寄存器地址
    i2c_WaitAck();		//等待应答
    i2c_Start();
	i2c_SendByte((addr<<1)|1);//发送器件地址+读命令	
    i2c_WaitAck();		//等待应答 
	while(len)
	{
		if(len==1)*buf=i2c_ReadByte(0);//读数据,发送nACK 
		else *buf=i2c_ReadByte(1);		//读数据,发送ACK  
		len--;
		buf++; 
	}    
    i2c_Stop();	//产生一个停止条件 
	return 0;	
}

/**
  * @brief   读取MPU6050的ID
  * @param   
  * @retval  正常返回1，异常返回0
  */
uint8_t MPU6050ReadID(void)
{
	unsigned char Re = 0;
    Re=MPU6050_ReadData(MPU6050_RA_WHO_AM_I);    //读器件地址
	if(Re != 0x68)
	{
		Debug_printf("MPU6050 dectected error!\r\n检测不到MPU6050模块，请检查模块与开发板的接线");
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
  * @brief   读取MPU6050的加速度数据
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
  * @brief   读取MPU6050的角加速度数据
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
  * @brief   读取MPU6050的原始温度数据
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
  * @brief   读取MPU6050的温度数据，转化成摄氏度
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
