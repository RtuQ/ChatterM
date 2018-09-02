#include "show_jpg.h"

FATFS fst;													/* FatFs�ļ�ϵͳ���� */
FIL file;                                                   /* �ļ����� */
FRESULT f_result;                                               /* �ļ���ȡ��� */
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
	* ��⻺���С
	*/
	if (NumBytesReq > sizeof(acBuffer)) {
	NumBytesReq = sizeof(acBuffer);
	}

	/*
	* ���ö�ȡλ��
	*/
	if(Off == 1) FileAddress = 0;
	else FileAddress = Off;
	f_result =f_lseek(PicFile, FileAddress);

	/*
	* ��ȡ���ݵ�����
	*/
	f_result = f_read(PicFile, acBuffer, NumBytesReq, &NumBytesRead);

	/*
	* ��ָ��ppDataָ���ȡ������
	*/
	*ppData = (const U8 *)acBuffer;

	/*
	* ���ض�ȡ���ֽ���
	*/
	return NumBytesRead;
}

void _ShowJPEG1(const char *sFilename) 
{
	/* ���ļ� */		
	f_result = f_open(&file, sFilename, FA_OPEN_EXISTING | FA_READ | FA_OPEN_ALWAYS);
	if (f_result != FR_OK)
	{
		return;
	}
	GUI_SelectLayer(1);
	/* ����JPEGͼƬ */
	GUI_JPEG_DrawEx(_GetData, &file, 0, 0);
	
	/* �ر��ļ� */
	f_close(&file);
}

void _ShowJPEG2(const char *sFilename, int x, int y) 
{
	char *_acBuffer;
	

	GUI_MEMDEV_Handle hMemJPEG;
	
	
//	f_result = f_mount(&fst,"1:",1);     //����FATFS

	/* ���ļ� */		
	f_result = f_open(&file,sFilename, FA_OPEN_EXISTING | FA_READ | FA_OPEN_ALWAYS);
	
	if (f_result != FR_OK)
	{
//		return 0;
	}
	 
	/* ����һ���ڴ�ռ� ���ҽ������� */
	hMem = GUI_ALLOC_AllocZero(file.fsize);
	
	
	/* �����뵽�ڴ�ľ��ת����ָ������ */
	_acBuffer = GUI_ALLOC_h2p(hMem);

	/* ��ȡ�ļ�����̬�ڴ� */
	f_result = f_read(&file, _acBuffer, file.fsize, &bw);
	if (f_result != FR_OK)
	{
//		return 0;
	}
	
	GUI_JPEG_GetInfo(_acBuffer, file.fsize, &JpegInfo);
	
	/* �����ڴ��豸������JPEGͼƬ���Ƶ����ڴ��豸���棬���ڴ��豸Ҫ�����������õ�
	   �����˳��˺���ǰ����Ҫ�ͷš�
	*/
	hMemJPEG = GUI_MEMDEV_CreateEx(0, 0, JpegInfo.XSize, JpegInfo.YSize, GUI_MEMDEV_HASTRANS);
	GUI_MEMDEV_Select(hMemJPEG);

	GUI_JPEG_Draw(_acBuffer, file.fsize, x, y);

	GUI_MEMDEV_Select(0);
	
	GUI_SelectLayer(1);
    GUI_MEMDEV_WriteAt(hMemJPEG,10,0);
	
	GUI_MEMDEV_Delete(hMemJPEG);
	/* �ͷŶ�̬�ڴ�hMem */
	GUI_ALLOC_Free(hMem);
	
	/* �ر��ļ� */
	f_close(&file);
	
}

//void show_jpg(const char*JPGname) 
//{
//	
//	/*
//	 ���ڶ໺��ʹ����ڴ��豸������˵��
//	   1. ʹ�ܶ໺���ǵ��õ����º������û�Ҫ��LCDConf_Lin_Template.c�ļ��������˶໺�壬���ô˺�������Ч��
//		  WM_MULTIBUF_Enable(1);
//	   2. ����ʹ��ʹ���ڴ��豸�ǵ��ú�����WM_SetCreateFlags(WM_CF_MEMDEV);
//	   3. ���emWin�����ö໺��ʹ����ڴ��豸��֧�֣���ѡһ���ɣ����������ѡ��ʹ�ö໺�壬ʵ��ʹ��
//		  STM32F429BIT6 + 32λSDRAM + RGB565/RGB888ƽ̨���ԣ��໺�������Ч�Ľ��ʹ����ƶ����߻���ʱ��˺��
//		  �У�����Ч����������ԣ�ͨ��ʹ�ܴ���ʹ���ڴ��豸���������ġ�
//	   4. ����emWin����Ĭ���ǿ��������塣
//	*/
////	WM_MULTIBUF_Enable(1);
//	
//	/*
//       ����У׼����Ĭ����ע�͵��ģ���������ҪУ׼������������У׼������û���ҪУ׼�������Ļ���ִ��
//	   �˺������ɣ��Ὣ����У׼�������浽EEPROM���棬�Ժ�ϵͳ�ϵ���Զ���EEPROM������ء�
//	*/
//    //TOUCH_Calibration();

//		GUI_MEMDEV_Handle hMemJPEG;
////		uint32_t t0, t1, i, count = 0;
////		char buf[50];
//		
//		/* ����JPEGͼƬ���ڴ��豸 */
//		_ShowJPEG1(JPGname);
//		
////		/*ˢ��20�Σ����ڴ�ӡ�ٶ���ֵ��ʱ�䵥λms */
////		for(i = 0; i < 20; i++)
////		{
////			t0 = GUI_GetTime();
//         
//			GUI_SelectLayer(1);
//			/* �õ�JPEGͼƬ��ʱ�򣬵��ô˺������� */
//			GUI_MEMDEV_WriteAt(hMemJPEG, -10, 0);
////			t1 = GUI_GetTime() - t0;
////			printf("�ٶ� = %dms\r\n", t1);
////			count += t1;
////		}
//		
////		/* ���ˢ��20�ε�ƽ���ٶ� */
////		printf(buf, "speed = %dms/frame", count/i);
////		GUI_DispStringAt(buf, 10, 10);

//	
////	_ShowJPEG2("1:xb.jpg", 0, 0);
//	
//		GUI_Delay(100);
//		GUI_ALLOC_Free(hMemJPEG);//�ͷ��ڴ棨���ͷŵ�����£������guishow�����ܽ��У�
//}
