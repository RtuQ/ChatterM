#include "show_jpg.h"

FATFS fst;													/* FatFs文件系统对象 */
FIL file;                                                   /* 文件对象 */
FRESULT f_result;                                               /* 文件读取结果 */
UINT bw;
GUI_JPEG_INFO JpegInfo;
GUI_HMEM hMem;
static char acBuffer[1024*20];

int _GetData(void * p, const U8 ** ppData, unsigned NumBytesReq, U32 Off) 
{
	static int FileAddress = 0;
	UINT NumBytesRead;
	FIL *PicFile;

	PicFile = (FIL *)p;

	/*
	* 检测缓存大小
	*/
	if (NumBytesReq > sizeof(acBuffer)) {
	NumBytesReq = sizeof(acBuffer);
	}

	/*
	* 设置读取位置
	*/
	if(Off == 1) FileAddress = 0;
	else FileAddress = Off;
	f_result =f_lseek(PicFile, FileAddress);

	/*
	* 读取数据到缓存
	*/
	f_result = f_read(PicFile, acBuffer, NumBytesReq, &NumBytesRead);

	/*
	* 让指针ppData指向读取的数据
	*/
	*ppData = (const U8 *)acBuffer;

	/*
	* 返回读取的字节数
	*/
	return NumBytesRead;
}

void _ShowJPEG1(const char *sFilename) 
{
	/* 打开文件 */		
	f_result = f_open(&file, sFilename, FA_OPEN_EXISTING | FA_READ | FA_OPEN_ALWAYS);
	if (f_result != FR_OK)
	{
		return;
	}
	GUI_SelectLayer(1);
	/* 绘制JPEG图片 */
	GUI_JPEG_DrawEx(_GetData, &file, 0, 0);
	
	/* 关闭文件 */
	f_close(&file);
}

void _ShowJPEG2(const char *sFilename, int x, int y) 
{
	char *_acBuffer;
	

	GUI_MEMDEV_Handle hMemJPEG;
	
	
//	f_result = f_mount(&fst,"1:",1);     //挂载FATFS

	/* 打开文件 */		
	f_result = f_open(&file,sFilename, FA_OPEN_EXISTING | FA_READ | FA_OPEN_ALWAYS);
	
	if (f_result != FR_OK)
	{
//		return 0;
	}
	 
	/* 申请一块内存空间 并且将其清零 */
	hMem = GUI_ALLOC_AllocZero(file.fsize);
	
	
	/* 将申请到内存的句柄转换成指针类型 */
	_acBuffer = GUI_ALLOC_h2p(hMem);

	/* 读取文件到动态内存 */
	f_result = f_read(&file, _acBuffer, file.fsize, &bw);
	if (f_result != FR_OK)
	{
//		return 0;
	}
	
	GUI_JPEG_GetInfo(_acBuffer, file.fsize, &JpegInfo);
	
	/* 创建内存设备，并将JPEG图片绘制到此内存设备里面，此内存设备要在主程序中用到
	   所以退出此函数前，不要释放。
	*/
	hMemJPEG = GUI_MEMDEV_CreateEx(0, 0, JpegInfo.XSize, JpegInfo.YSize, GUI_MEMDEV_HASTRANS);
	GUI_MEMDEV_Select(hMemJPEG);

	GUI_JPEG_Draw(_acBuffer, file.fsize, x, y);

	GUI_MEMDEV_Select(0);
	
	GUI_SelectLayer(1);
    GUI_MEMDEV_WriteAt(hMemJPEG,10,0);
	
	GUI_MEMDEV_Delete(hMemJPEG);
	/* 释放动态内存hMem */
	GUI_ALLOC_Free(hMem);
	
	/* 关闭文件 */
	f_close(&file);
	
}

//void show_jpg(const char*JPGname) 
//{
//	
//	/*
//	 关于多缓冲和窗口内存设备的设置说明
//	   1. 使能多缓冲是调用的如下函数，用户要在LCDConf_Lin_Template.c文件中配置了多缓冲，调用此函数才有效：
//		  WM_MULTIBUF_Enable(1);
//	   2. 窗口使能使用内存设备是调用函数：WM_SetCreateFlags(WM_CF_MEMDEV);
//	   3. 如果emWin的配置多缓冲和窗口内存设备都支持，二选一即可，且务必优先选择使用多缓冲，实际使用
//		  STM32F429BIT6 + 32位SDRAM + RGB565/RGB888平台测试，多缓冲可以有效的降低窗口移动或者滑动时的撕裂
//		  感，并有效的提高流畅性，通过使能窗口使用内存设备是做不到的。
//	   4. 所有emWin例子默认是开启三缓冲。
//	*/
////	WM_MULTIBUF_Enable(1);
//	
//	/*
//       触摸校准函数默认是注释掉的，电阻屏需要校准，电容屏无需校准。如果用户需要校准电阻屏的话，执行
//	   此函数即可，会将触摸校准参数保存到EEPROM里面，以后系统上电会自动从EEPROM里面加载。
//	*/
//    //TOUCH_Calibration();

//		GUI_MEMDEV_Handle hMemJPEG;
////		uint32_t t0, t1, i, count = 0;
////		char buf[50];
//		
//		/* 加载JPEG图片到内存设备 */
//		_ShowJPEG1(JPGname);
//		
////		/*刷新20次，串口打印速度数值，时间单位ms */
////		for(i = 0; i < 20; i++)
////		{
////			t0 = GUI_GetTime();
//         
//			GUI_SelectLayer(1);
//			/* 用到JPEG图片的时候，调用此函数即可 */
//			GUI_MEMDEV_WriteAt(hMemJPEG, -10, 0);
////			t1 = GUI_GetTime() - t0;
////			printf("速度 = %dms\r\n", t1);
////			count += t1;
////		}
//		
////		/* 求出刷新20次的平均速度 */
////		printf(buf, "speed = %dms/frame", count/i);
////		GUI_DispStringAt(buf, 10, 10);

//	
////	_ShowJPEG2("1:xb.jpg", 0, 0);
//	
//		GUI_Delay(100);
//		GUI_ALLOC_Free(hMemJPEG);//释放内存（不释放的情况下，后面的guishow将不能进行）
//}
