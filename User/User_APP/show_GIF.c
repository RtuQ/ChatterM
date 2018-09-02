
#include "show_GIF.h"

extern FATFS fst;													/* FatFs�ļ�ϵͳ���� */
extern FIL file;                                                   /* �ļ����� */
extern FRESULT f_result;                                               /* �ļ���ȡ��� */
extern UINT bw;

/*
*********************************************************************************************************
*	                              	 �궨��
*********************************************************************************************************
*/
/* �߼��ر���ʾ�ķ�ʽѡ��      ��Method1
   ֱ�Ӽ��ص�SDRAM������ʾѡ�� ��Method2
*/
#define Method2


/*
*********************************************************************************************************
*	                                	����
*********************************************************************************************************
*/
static char acBuffer[8192];

static GUI_GIF_INFO InfoGif;
static GUI_GIF_IMAGE_INFO ImagInfoGif;

/*
*********************************************************************************************************
*	�� �� ��: _GetData
*	����˵��: ������GUI_JPEG_DrawEx����
*	��    �Σ�p             FIL��������
*             NumBytesReq   �����ȡ���ֽ���
*             ppData        ����ָ��
*             Off           ���Off = 1����ô�����´���ʼλ�ö�ȡ                 
*	�� �� ֵ: ���ض�ȡ���ֽ���
*********************************************************************************************************
*/
extern int _GetData(void * p, const U8 ** ppData, unsigned NumBytesReq, U32 Off); 
//{
//	static int FileAddress = 0;
//	UINT NumBytesRead;
//	FIL *PicFile;

//	PicFile = (FIL *)p;

//	/*
//	* ��⻺���С
//	*/
//	if (NumBytesReq > sizeof(acBuffer)) {
//	NumBytesReq = sizeof(acBuffer);
//	}

//	/*
//	* ���ö�ȡλ��
//	*/
//	if(Off == 1) FileAddress = 0;
//	else FileAddress = Off;
//	result =f_lseek(PicFile, FileAddress);

//	/*
//	* ��ȡ���ݵ�����
//	*/
//	result = f_read(PicFile, acBuffer, NumBytesReq, &NumBytesRead);

//	/*
//	* ��ָ��ppDataָ���ȡ������
//	*/
//	*ppData = (const U8 *)acBuffer;

//	/*
//	* ���ض�ȡ���ֽ���
//	*/
//	return NumBytesRead;
//}

/*
*********************************************************************************************************
*	�� �� ��: _ShowGIF1
*	����˵��: ��ʾGIFƬ
*	��    ��: sFilename Ҫ��ʾ��ͼƬ����
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void _ShowGIF1(const char * sFilename) 
{
	uint16_t i = 0;
	uint32_t t0, t1;
	char buf[50];
	
	 f_result = f_mount(&fst,"1:",1);     //����FATFS

	/* ���ļ� */		
	f_result = f_open(&file, sFilename, FA_OPEN_EXISTING | FA_READ | FA_OPEN_ALWAYS);
	if (f_result != FR_OK)
	{
		return;
	}
   
	/* ��ȡGIFͼƬ��Ϣ */
   GUI_GIF_GetInfoEx(_GetData, &file,&InfoGif);
	
   while(1)
   {
	    /* �����������õ�ǰ���ŵ�֡����InfoGif.NumImages��GIFͼƬ�ܵ�֡�� */
		if(i <=InfoGif.NumImages)
	    {
			/* ��ȡ��ǰ֡GIFͼƬ��Ϣ��ע���4�������Ǵ�0��ʼ������ */
			GUI_GIF_GetImageInfoEx(_GetData, &file, &ImagInfoGif, i);
			
			/* �����֡�ӳ�ʱ����0��Ĭ�����ӳ�100ms */
			if(ImagInfoGif.Delay == 0)
			{
				GUI_Delay(100);
			}
			else
			{
				t0 = GUI_GetTime();
				/* ��ʾ��ǰ���ŵ�֡�� */
				sprintf(buf, "     Frame:%d/%d     ", i+1, InfoGif.NumImages);
				GUI_DispStringHCenterAt(buf, LCD_GetXSize()/2, 0);
				
				/* ���벢��ʾ��֡GIFͼƬ��ע���5�������Ǵ�0��ʼ������ */
				GUI_GIF_DrawSubEx(_GetData, 
								  &file, 
								  (LCD_GetXSize() - InfoGif.xSize)/2,
								  (LCD_GetYSize() - InfoGif.ySize)/2, 
								  i++);
				
				/* ��ȡ���ν������ʾ���ĵ�ʱ�� */
				t1 = GUI_GetTime() - t0;
				
				/* ���GIF�Ľ������ʾ��ʱ�䳬ʱ�Ͳ����ӳ� */
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

//	/* ʵ��Ӧ���У�����ʵ������ͷŶ�̬�ڴ� */		
// 	GUI_ALLOC_Free(hMem);
// 	f_close(&file);
}

/*
*********************************************************************************************************
*	�� �� ��: _ShowGIF2
*	����˵��: ��ʾGIFƬ
*	��    ��: sFilename Ҫ��ʾ��ͼƬ����
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void _ShowGIF2(const char * sFilename) 
{
	uint16_t i = 0;
	uint32_t t0, t1;
	char *_acBuffer;
	GUI_HMEM hMem;
	char buf[50];
	

	/* ���ļ� */		
	f_result = f_open(&file, sFilename, FA_OPEN_EXISTING | FA_READ | FA_OPEN_ALWAYS);
	if (f_result != FR_OK)
	{
		return;
	}
	 
	/* ����һ���ڴ�ռ� ���ҽ������� */
	hMem = GUI_ALLOC_AllocZero(file.fsize);
	
	/* �����뵽�ڴ�ľ��ת����ָ������ */
	_acBuffer = GUI_ALLOC_h2p(hMem);

	/* ��ȡ�ļ�����̬�ڴ� */
	f_result = f_read(&file, _acBuffer, file.fsize, &bw);
	if (f_result != FR_OK)
	{
		return;
	}
   
	/* ��ȡGIFͼƬ��Ϣ */
   GUI_GIF_GetInfo(_acBuffer, file.fsize, &InfoGif);
	
   while(1)
   {
	    /* �����������õ�ǰ���ŵ�֡����InfoGif.NumImages��GIFͼƬ�ܵ�֡�� */
		if(i <= InfoGif.NumImages)
	    {
			/* ��ȡ��ǰ֡GIFͼƬ��Ϣ��ע���4�������Ǵ�0��ʼ������ */
	        GUI_GIF_GetImageInfo(_acBuffer, file.fsize, &ImagInfoGif, i);
			
			/* �����֡�ӳ�ʱ����0��Ĭ�����ӳ�100ms */
			if(ImagInfoGif.Delay == 0)
			{
				GUI_Delay(100);
			}
			else
			{
				t0 = GUI_GetTime();
				/* ��ʾ��ǰ���ŵ�֡�� */
//				sprintf(buf, "     Frame:%d/%d     ", i+1, InfoGif.NumImages);
//				GUI_DispStringHCenterAt(buf, LCD_GetXSize()/2, 0);
				
				
				
				GUI_SelectLayer(1);
				
				/* ���벢��ʾ��֡GIFͼƬ��ע���5�������Ǵ�0��ʼ������ */
				GUI_GIF_DrawSub(_acBuffer, 
								file.fsize, 
								(LCD_GetXSize() - InfoGif.xSize)/2,
								(LCD_GetYSize() - InfoGif.ySize)/2, 
								i++);
				
				/* ��ȡ���ν������ʾ���ĵ�ʱ�� */
				t1 = GUI_GetTime() - t0;
				
				/* ���GIF�Ľ������ʾ��ʱ�䳬ʱ�Ͳ����ӳ� */
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

	/* ʵ��Ӧ���У�����ʵ������ͷŶ�̬�ڴ� */		
 	GUI_ALLOC_Free(hMem);
 	f_close(&file);
}

/*
*********************************************************************************************************
*	�� �� ��: show_GIF
*	����˵��: GUI������
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void show_GIF(void) 
{
	/*
	 ���ڶ໺��ʹ����ڴ��豸������˵��
	   1. ʹ�ܶ໺���ǵ��õ����º������û�Ҫ��LCDConf_Lin_Template.c�ļ��������˶໺�壬���ô˺�������Ч��
		  WM_MULTIBUF_Enable(1);
	   2. ����ʹ��ʹ���ڴ��豸�ǵ��ú�����WM_SetCreateFlags(WM_CF_MEMDEV);
	   3. ���emWin�����ö໺��ʹ����ڴ��豸��֧�֣���ѡһ���ɣ����������ѡ��ʹ�ö໺�壬ʵ��ʹ��
		  STM32F429BIT6 + 32λSDRAM + RGB565/RGB888ƽ̨���ԣ��໺�������Ч�Ľ��ʹ����ƶ����߻���ʱ��˺��
		  �У�����Ч����������ԣ�ͨ��ʹ�ܴ���ʹ���ڴ��豸���������ġ�
	   4. ����emWin����Ĭ���ǿ��������塣
	*/
	WM_MULTIBUF_Enable(1);
	
	/*
       ����У׼����Ĭ����ע�͵��ģ���������ҪУ׼������������У׼������û���ҪУ׼�������Ļ���ִ��
	   �˺������ɣ��Ὣ����У׼�������浽EEPROM���棬�Ժ�ϵͳ�ϵ���Զ���EEPROM������ء�
	*/
    //TOUCH_Calibration();


	/* GIFͼƬ��ʾ��ʽһ��ʵ����Ŀ���Ƽ������ü��� */
#if defined Method1
_ShowGIF1("1:1.gif");
	
    /* GIFͼƬ��ʾ��ʽ����ʵ����Ŀ�Ƽ� */
#elif defined Method2
_ShowGIF2("1:jingya.gif");	
	
#endif
	
	/* ������뵽���˵��GIFͼƬ����ʧ�� */
	GUI_DispStringAt("GIF Load failed!!", 0, 0);
	
//	while(1) 
//	{
//		GUI_Delay(10);
//	}
}

/***************************** ���������� www.armfly.com (END OF FILE) *********************************/
