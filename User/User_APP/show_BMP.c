
#include "show_BMP.h"

extern FATFS fst;													/* FatFs文件系统对象 */
extern FIL file;                                                   /* 文件对象 */
extern FRESULT f_result;                                               /* 文件读取结果 */
extern UINT bw;
/*
*********************************************************************************************************
*	                              	 宏定义
*********************************************************************************************************
*/
/* 直接加载到SDRAM进行显示选择：Method1
   边加载边显示的方式选择      :Method2
*/
#define Method1


/*
*********************************************************************************************************
*	                                	变量
*********************************************************************************************************
*/
/* 采用函数GUI_BMP_DrawEx显示BMP图片时，此数组的大小一定要能够存储图片的一行像素数据，切记！！！
   比如：BMP图片是32位颜色深度，最大支持长度是1024个像素，那么存储一行像素数据就需要1024*4字节。
*/
//static char acBuffer[1024*4];


/*
*********************************************************************************************************
*	函 数 名: _GetData
*	功能说明: 被函数GUI_BMP_DrawEx调用
*	形    参：p             FIL类型数据
*             NumBytesReq   请求读取的字节数
*             ppData        数据指针
*             Off           如果Off = 1，那么将重新从起始位置读取                 
*	返 回 值: 返回读取的字节数
*********************************************************************************************************
*/
extern int _GetData(void * p, const U8 ** ppData, unsigned NumBytesReq, U32 Off); 
//{
//	static int FileAddress = 0;
//	UINT NumBytesRead;
//	FIL *PicFile;

//	PicFile = (FIL *)p;

//	/*
//	* 检测缓存大小
//	*/
//	if (NumBytesReq > sizeof(acBuffer)) {
//	NumBytesReq = sizeof(acBuffer);
//	}

//	/*
//	* 设置读取位置
//	*/
//	if(Off == 1) FileAddress = 0;
//	else FileAddress = Off;
//	result =f_lseek(PicFile, FileAddress);

//	/*
//	* 读取数据到缓存
//	*/
//	result = f_read(PicFile, acBuffer, NumBytesReq, &NumBytesRead);

//	/*
//	* 让指针ppData指向读取的数据
//	*/
//	*ppData = (const U8 *)acBuffer;

//	/*
//	* 返回读取的字节数
//	*/
//	return NumBytesRead;
//}

/*
*********************************************************************************************************
*	函 数 名: _ShowBMP1
*	功能说明: 显示BMP图片
*	形    参: sFilename  要读取的文件名
*	返 回 值: 无
*********************************************************************************************************
*/
void _ShowBMP1(const char *sFilename) 
{
	/* 打开文件 */		
	f_result = f_open(&file, sFilename, FA_OPEN_EXISTING | FA_READ | FA_OPEN_ALWAYS);
	if (f_result != FR_OK)
	{
		return;
	}
	
	/* 绘制BMP图片 */
	GUI_BMP_DrawEx(_GetData, &file, 0, 0);
	
	/* 关闭文件 */
	f_close(&file);
}

/*
*********************************************************************************************************
*	函 数 名: _ShowBMP2
*	功能说明: 显示BMP图片
*	形    参: sFilename  要读取的文件名
*                     x  要显示的x轴坐标位置
*                     y  要显示的y轴坐标位置
*	返 回 值: 返回绘制了BMP图片的内存设备句柄。
*********************************************************************************************************
*/
GUI_HMEM _ShowBMP2(const char *sFilename, int x, int y) 
{
	char *_acBuffer;
	int XSize, YSize;
	GUI_HMEM hMem;
	GUI_MEMDEV_Handle hMemBMP;

	/* 打开文件 */		
	f_result = f_open(&file, sFilename, FA_OPEN_EXISTING | FA_READ | FA_OPEN_ALWAYS);
	if (f_result != FR_OK)
	{
		return 0;
	}
	 
	/* 申请一块内存空间 并且将其清零 */
	hMem = GUI_ALLOC_AllocZero(file.fsize);
	
	/* 将申请到内存的句柄转换成指针类型 */
	_acBuffer = GUI_ALLOC_h2p(hMem);

	/* 读取文件到动态内存 */
	f_result = f_read(&file, _acBuffer, file.fsize, &bw);
	if (f_result != FR_OK)
	{
		return 0;
	}
	
	XSize = GUI_BMP_GetXSize(_acBuffer);
	YSize = GUI_BMP_GetYSize(_acBuffer);
	
	/* 创建内存设备，并将BMP图片绘制到此内存设备里面，此内存设备要在主程序中用到
	   所以退出此函数前，不要释放。
	*/
	hMemBMP = GUI_MEMDEV_CreateEx(0, 0, XSize, YSize, GUI_MEMDEV_HASTRANS);
	GUI_MEMDEV_Select(hMemBMP);
	GUI_BMP_Draw(_acBuffer, 0, 0);
	GUI_MEMDEV_Select(0);

	/* 释放动态内存hMem */
	GUI_ALLOC_Free(hMem);
	
	/* 关闭文件 */
	f_close(&file);
	
	return hMemBMP;
}

/*
*********************************************************************************************************
*	函 数 名: show_BMP
*	功能说明: GUI主函数
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void show_BMP(void) 
{
	
	/* 初始化 */
	GUI_Init();
	
	/* 设置字体，文本模式和前景色 */
	GUI_SetFont(&GUI_Font24B_ASCII);
	GUI_SetTextMode(GUI_TM_TRANS);
	GUI_SetColor(GUI_RED);	
	
	/*
	 关于多缓冲和窗口内存设备的设置说明
	   1. 使能多缓冲是调用的如下函数，用户要在LCDConf_Lin_Template.c文件中配置了多缓冲，调用此函数才有效：
		  WM_MULTIBUF_Enable(1);
	   2. 窗口使能使用内存设备是调用函数：WM_SetCreateFlags(WM_CF_MEMDEV);
	   3. 如果emWin的配置多缓冲和窗口内存设备都支持，二选一即可，且务必优先选择使用多缓冲，实际使用
		  STM32F429BIT6 + 32位SDRAM + RGB565/RGB888平台测试，多缓冲可以有效的降低窗口移动或者滑动时的撕裂
		  感，并有效的提高流畅性，通过使能窗口使用内存设备是做不到的。
	   4. 所有emWin例子默认是开启三缓冲。
	*/
	WM_MULTIBUF_Enable(1);
	
	/*
       触摸校准函数默认是注释掉的，电阻屏需要校准，电容屏无需校准。如果用户需要校准电阻屏的话，执行
	   此函数即可，会将触摸校准参数保存到EEPROM里面，以后系统上电会自动从EEPROM里面加载。
	*/
    //TOUCH_Calibration();

	/* BMP图片显示方式一：做到15ms一帧，实际项目强烈推荐 */
#if defined Method1
	{
		GUI_MEMDEV_Handle hMemBMP;
		uint32_t t0, t1, i, count = 0;
		char buf[50];
			
		GUI_DispStringAt("Load BMP File to Memory Devices....", 10, 10);
		
		/* 加载BMP图片到内存设备 */
		hMemBMP = _ShowBMP2("1.bmp", 0, 0);
		
		/*刷新20次，串口打印速度数值，时间单位ms */
		for(i = 0; i < 20; i++)
		{
			t0 = GUI_GetTime();
			/* 用到BMP图片的时候，调用此函数即可 */
			GUI_MEMDEV_WriteAt(hMemBMP, 0, 0);
			t1 = GUI_GetTime() - t0;
			printf("速度 = %dms\r\n", t1);
			count += t1;
		}
		
		/* 求出刷新20次的平均速度 */
		sprintf(buf, "speed = %dms/frame", count/i);
		GUI_DispStringAt(buf, 10, 10);
	}
	/* BMP图片显示方式二：实际项目不推荐，会用即可 */
#elif defined Method2
	{
		uint32_t t0, t1;
		char buf[50];
		
		t0 = GUI_GetTime();
		_ShowBMP1("1.bmp");
		t1 = GUI_GetTime() - t0;

		sprintf(buf, "speed = %dms/frame", t1);
		GUI_DispStringAt(buf, 10, 10);
	}
#endif
	
//	while(1) 
//	{
//		GUI_Delay(10);
//	}
}

/***************************** 安富莱电子 www.armfly.com (END OF FILE) *********************************/
