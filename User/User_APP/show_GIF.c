
#include "show_GIF.h"

extern FATFS fst;													/* FatFs文件系统对象 */
extern FIL file;                                                   /* 文件对象 */
extern FRESULT f_result;                                               /* 文件读取结果 */
extern UINT bw;

/*
*********************************************************************************************************
*	                              	 宏定义
*********************************************************************************************************
*/
/* 边加载边显示的方式选择      ：Method1
   直接加载到SDRAM进行显示选择 ：Method2
*/
#define Method2


/*
*********************************************************************************************************
*	                                	变量
*********************************************************************************************************
*/
static char acBuffer[8192];

static GUI_GIF_INFO InfoGif;
static GUI_GIF_IMAGE_INFO ImagInfoGif;

/*
*********************************************************************************************************
*	函 数 名: _GetData
*	功能说明: 被函数GUI_JPEG_DrawEx调用
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
*	函 数 名: _ShowGIF1
*	功能说明: 显示GIF片
*	形    参: sFilename 要显示的图片名字
*	返 回 值: 无
*********************************************************************************************************
*/
void _ShowGIF1(const char * sFilename) 
{
	uint16_t i = 0;
	uint32_t t0, t1;
	char buf[50];
	
	 f_result = f_mount(&fst,"1:",1);     //挂载FATFS

	/* 打开文件 */		
	f_result = f_open(&file, sFilename, FA_OPEN_EXISTING | FA_READ | FA_OPEN_ALWAYS);
	if (f_result != FR_OK)
	{
		return;
	}
   
	/* 获取GIF图片信息 */
   GUI_GIF_GetInfoEx(_GetData, &file,&InfoGif);
	
   while(1)
   {
	    /* 变量用来设置当前播放的帧数，InfoGif.NumImages是GIF图片总的帧数 */
		if(i <=InfoGif.NumImages)
	    {
			/* 获取当前帧GIF图片信息，注意第4个参数是从0开始计数的 */
			GUI_GIF_GetImageInfoEx(_GetData, &file, &ImagInfoGif, i);
			
			/* 如果此帧延迟时间是0，默认是延迟100ms */
			if(ImagInfoGif.Delay == 0)
			{
				GUI_Delay(100);
			}
			else
			{
				t0 = GUI_GetTime();
				/* 显示当前播放的帧数 */
				sprintf(buf, "     Frame:%d/%d     ", i+1, InfoGif.NumImages);
				GUI_DispStringHCenterAt(buf, LCD_GetXSize()/2, 0);
				
				/* 解码并显示此帧GIF图片，注意第5个参数是从0开始计数的 */
				GUI_GIF_DrawSubEx(_GetData, 
								  &file, 
								  (LCD_GetXSize() - InfoGif.xSize)/2,
								  (LCD_GetYSize() - InfoGif.ySize)/2, 
								  i++);
				
				/* 获取本次解码和显示消耗的时间 */
				t1 = GUI_GetTime() - t0;
				
				/* 如果GIF的解码和显示的时间超时就不做延迟 */
				if (t1 < ImagInfoGif.Delay * 10) 
				{
					GUI_Delay(ImagInfoGif.Delay * 10 - t1);
				}
			}			  
	    }
	    else
	    {
	        i = 0;
	    }
	}

//	/* 实际应用中，根据实际情况释放动态内存 */		
// 	GUI_ALLOC_Free(hMem);
// 	f_close(&file);
}

/*
*********************************************************************************************************
*	函 数 名: _ShowGIF2
*	功能说明: 显示GIF片
*	形    参: sFilename 要显示的图片名字
*	返 回 值: 无
*********************************************************************************************************
*/
void _ShowGIF2(const char * sFilename) 
{
	uint16_t i = 0;
	uint32_t t0, t1;
	char *_acBuffer;
	GUI_HMEM hMem;
	char buf[50];
	

	/* 打开文件 */		
	f_result = f_open(&file, sFilename, FA_OPEN_EXISTING | FA_READ | FA_OPEN_ALWAYS);
	if (f_result != FR_OK)
	{
		return;
	}
	 
	/* 申请一块内存空间 并且将其清零 */
	hMem = GUI_ALLOC_AllocZero(file.fsize);
	
	/* 将申请到内存的句柄转换成指针类型 */
	_acBuffer = GUI_ALLOC_h2p(hMem);

	/* 读取文件到动态内存 */
	f_result = f_read(&file, _acBuffer, file.fsize, &bw);
	if (f_result != FR_OK)
	{
		return;
	}
   
	/* 获取GIF图片信息 */
   GUI_GIF_GetInfo(_acBuffer, file.fsize, &InfoGif);
	
   while(1)
   {
	    /* 变量用来设置当前播放的帧数，InfoGif.NumImages是GIF图片总的帧数 */
		if(i <= InfoGif.NumImages)
	    {
			/* 获取当前帧GIF图片信息，注意第4个参数是从0开始计数的 */
	        GUI_GIF_GetImageInfo(_acBuffer, file.fsize, &ImagInfoGif, i);
			
			/* 如果此帧延迟时间是0，默认是延迟100ms */
			if(ImagInfoGif.Delay == 0)
			{
				GUI_Delay(100);
			}
			else
			{
				t0 = GUI_GetTime();
				/* 显示当前播放的帧数 */
//				sprintf(buf, "     Frame:%d/%d     ", i+1, InfoGif.NumImages);
//				GUI_DispStringHCenterAt(buf, LCD_GetXSize()/2, 0);
				
				
				
				GUI_SelectLayer(1);
				
				/* 解码并显示此帧GIF图片，注意第5个参数是从0开始计数的 */
				GUI_GIF_DrawSub(_acBuffer, 
								file.fsize, 
								(LCD_GetXSize() - InfoGif.xSize)/2,
								(LCD_GetYSize() - InfoGif.ySize)/2, 
								i++);
				
				/* 获取本次解码和显示消耗的时间 */
				t1 = GUI_GetTime() - t0;
				
				/* 如果GIF的解码和显示的时间超时就不做延迟 */
				if (t1 < ImagInfoGif.Delay * 10) 
				{
					GUI_Delay(ImagInfoGif.Delay * 10 - t1);
				}
			}			  
	    }
	    else
	    {
	        i = 0;
	    }
	}

	/* 实际应用中，根据实际情况释放动态内存 */		
 	GUI_ALLOC_Free(hMem);
 	f_close(&file);
}

/*
*********************************************************************************************************
*	函 数 名: show_GIF
*	功能说明: GUI主函数
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void show_GIF(void) 
{
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


	/* GIF图片显示方式一：实际项目不推荐，会用即可 */
#if defined Method1
_ShowGIF1("1:1.gif");
	
    /* GIF图片显示方式二：实际项目推荐 */
#elif defined Method2
_ShowGIF2("1:jingya.gif");	
	
#endif
	
	/* 如果进入到这里，说明GIF图片加载失败 */
	GUI_DispStringAt("GIF Load failed!!", 0, 0);
	
//	while(1) 
//	{
//		GUI_Delay(10);
//	}
}

/***************************** 安富莱电子 www.armfly.com (END OF FILE) *********************************/
