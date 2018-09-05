#include "mpu6050.h"
#define MPU_ERROR 		I2C_ERROR
#define MPU_INFO 		I2C_INFO
/**
  * @brief   写数据到MPU6050寄存器
  * @param   reg_add:寄存器地址
	* @param	 reg_data:要写入的数据
  * @retval  
  */
void MPU6050_WriteReg(u8 reg_add,u8 reg_dat)
{
//	Sensors_I2C_WriteRegister(MPU6050_ADDRESS,reg_add,1,&reg_dat);
	i2c_Start();		/* 发送启动信号 */
	/* 发送设备地址+读写控制bit（0 = w， 1 = r) bit7 先传 */
	i2c_SendByte(MPU6050_ADDRESS | EEPROM_I2C_WR);
	i2c_SendByte(reg_add);
	i2c_SendByte(reg_dat);
	
	i2c_Stop();
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
	i2c_SendByte(MPU6050_ADDRESS | EEPROM_I2C_WR);
	i2c_SendByte(reg_add);
	i2c_Start();		/* 发送启动信号 */
	/* 发送设备地址+读写控制bit（0 = w， 1 = r) bit7 先传 */
	i2c_SendByte(MPU6050_ADDRESS | EEPROM_I2C_RD);
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
  int i=0,j=0;
  //在初始化之前要延时一段时间，若没有延时，则断电后再上电数据可能会出错
  for(i=0;i<1000;i++)
  {
    for(j=0;j<1000;j++)
    {
      ;
    }
  }
	MPU6050_WriteReg(MPU6050_RA_PWR_MGMT_1, 0x00);	     //解除休眠状态
	MPU6050_WriteReg(MPU6050_RA_SMPLRT_DIV , 0x07);	    //陀螺仪采样率
	MPU6050_WriteReg(MPU6050_RA_CONFIG , 0x06);	
	MPU6050_WriteReg(MPU6050_RA_ACCEL_CONFIG , 0x01);	  //配置加速度传感器工作在16G模式
	MPU6050_WriteReg(MPU6050_RA_GYRO_CONFIG, 0x18);     //陀螺仪自检及测量范围，典型值：0x18(不自检，2000deg/s)
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
		print("MPU6050 dectected error!\r\n检测不到MPU6050模块，请检查模块与开发板的接线");
		print("MPU6050 ID = %X\r\n",Re);
		return 0;
	}
	else
	{
		print("MPU6050 ID = %d\r\n",Re);
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
