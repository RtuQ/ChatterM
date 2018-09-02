
#include "show_BMP.h"

extern FATFS fst;													/* FatFs�ļ�ϵͳ���� */
extern FIL file;                                                   /* �ļ����� */
extern FRESULT f_result;                                               /* �ļ���ȡ��� */
extern UINT bw;
/*
*********************************************************************************************************
*	                              	 �궨��
*********************************************************************************************************
*/
/* ֱ�Ӽ��ص�SDRAM������ʾѡ��Method1
   �߼��ر���ʾ�ķ�ʽѡ��      :Method2
*/
#define Method1


/*
*********************************************************************************************************
*	                                	����
*********************************************************************************************************
*/
/* ���ú���GUI_BMP_DrawEx��ʾBMPͼƬʱ��������Ĵ�Сһ��Ҫ�ܹ��洢ͼƬ��һ���������ݣ��мǣ�����
   ���磺BMPͼƬ��32λ��ɫ��ȣ����֧�ֳ�����1024�����أ���ô�洢һ���������ݾ���Ҫ1024*4�ֽڡ�
*/
//static char acBuffer[1024*4];


/*
*********************************************************************************************************
*	�� �� ��: _GetData
*	����˵��: ������GUI_BMP_DrawEx����
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
*	�� �� ��: _ShowBMP1
*	����˵��: ��ʾBMPͼƬ
*	��    ��: sFilename  Ҫ��ȡ���ļ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void _ShowBMP1(const char *sFilename) 
{
	/* ���ļ� */		
	f_result = f_open(&file, sFilename, FA_OPEN_EXISTING | FA_READ | FA_OPEN_ALWAYS);
	if (f_result != FR_OK)
	{
		return;
	}
	
	/* ����BMPͼƬ */
	GUI_BMP_DrawEx(_GetData, &file, 0, 0);
	
	/* �ر��ļ� */
	f_close(&file);
}

/*
*********************************************************************************************************
*	�� �� ��: _ShowBMP2
*	����˵��: ��ʾBMPͼƬ
*	��    ��: sFilename  Ҫ��ȡ���ļ���
*                     x  Ҫ��ʾ��x������λ��
*                     y  Ҫ��ʾ��y������λ��
*	�� �� ֵ: ���ػ�����BMPͼƬ���ڴ��豸�����
*********************************************************************************************************
*/
GUI_HMEM _ShowBMP2(const char *sFilename, int x, int y) 
{
	char *_acBuffer;
	int XSize, YSize;
	GUI_HMEM hMem;
	GUI_MEMDEV_Handle hMemBMP;

	/* ���ļ� */		
	f_result = f_open(&file, sFilename, FA_OPEN_EXISTING | FA_READ | FA_OPEN_ALWAYS);
	if (f_result != FR_OK)
	{
		return 0;
	}
	 
	/* ����һ���ڴ�ռ� ���ҽ������� */
	hMem = GUI_ALLOC_AllocZero(file.fsize);
	
	/* �����뵽�ڴ�ľ��ת����ָ������ */
	_acBuffer = GUI_ALLOC_h2p(hMem);

	/* ��ȡ�ļ�����̬�ڴ� */
	f_result = f_read(&file, _acBuffer, file.fsize, &bw);
	if (f_result != FR_OK)
	{
		return 0;
	}
	
	XSize = GUI_BMP_GetXSize(_acBuffer);
	YSize = GUI_BMP_GetYSize(_acBuffer);
	
	/* �����ڴ��豸������BMPͼƬ���Ƶ����ڴ��豸���棬���ڴ��豸Ҫ�����������õ�
	   �����˳��˺���ǰ����Ҫ�ͷš�
	*/
	hMemBMP = GUI_MEMDEV_CreateEx(0, 0, XSize, YSize, GUI_MEMDEV_HASTRANS);
	GUI_MEMDEV_Select(hMemBMP);
	GUI_BMP_Draw(_acBuffer, 0, 0);
	GUI_MEMDEV_Select(0);

	/* �ͷŶ�̬�ڴ�hMem */
	GUI_ALLOC_Free(hMem);
	
	/* �ر��ļ� */
	f_close(&file);
	
	return hMemBMP;
}

/*
*********************************************************************************************************
*	�� �� ��: show_BMP
*	����˵��: GUI������
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void show_BMP(void) 
{
	
	/* ��ʼ�� */
	GUI_Init();
	
	/* �������壬�ı�ģʽ��ǰ��ɫ */
	GUI_SetFont(&GUI_Font24B_ASCII);
	GUI_SetTextMode(GUI_TM_TRANS);
	GUI_SetColor(GUI_RED);	
	
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

	/* BMPͼƬ��ʾ��ʽһ������15msһ֡��ʵ����Ŀǿ���Ƽ� */
#if defined Method1
	{
		GUI_MEMDEV_Handle hMemBMP;
		uint32_t t0, t1, i, count = 0;
		char buf[50];
			
		GUI_DispStringAt("Load BMP File to Memory Devices....", 10, 10);
		
		/* ����BMPͼƬ���ڴ��豸 */
		hMemBMP = _ShowBMP2("1.bmp", 0, 0);
		
		/*ˢ��20�Σ����ڴ�ӡ�ٶ���ֵ��ʱ�䵥λms */
		for(i = 0; i < 20; i++)
		{
			t0 = GUI_GetTime();
			/* �õ�BMPͼƬ��ʱ�򣬵��ô˺������� */
			GUI_MEMDEV_WriteAt(hMemBMP, 0, 0);
			t1 = GUI_GetTime() - t0;
			printf("�ٶ� = %dms\r\n", t1);
			count += t1;
		}
		
		/* ���ˢ��20�ε�ƽ���ٶ� */
		sprintf(buf, "speed = %dms/frame", count/i);
		GUI_DispStringAt(buf, 10, 10);
	}
	/* BMPͼƬ��ʾ��ʽ����ʵ����Ŀ���Ƽ������ü��� */
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

/***************************** ���������� www.armfly.com (END OF FILE) *********************************/
