/*
*********************************************************************************************************
*                                              EXAMPLE CODE
*
*                             (c) Copyright 2013; Micrium, Inc.; Weston, FL
*
*                   All rights reserved.  Protected by international copyright laws.
*                   Knowledge of the source code may not be used to write a similar
*                   product.  This file may only be used in accordance with a license
*                   and should not be redistributed in any way.
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*
*                                            EXAMPLE CODE
*
*                                       IAR Development Kits
*                                              on the
*
*                                    STM32F429II-SK KICKSTART KIT
*
* Filename      : app.c
* Version       : V1.00
* Programmer(s) : YS
*                 DC
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                             INCLUDE FILES
*********************************************************************************************************
*/

#include <includes.h>


/*
*********************************************************************************************************
*                                            LOCAL DEFINES
*********************************************************************************************************
*/
FATFS fs;													/* FatFs�ļ�ϵͳ���� */
FIL fnew;													/* �ļ����� */
FRESULT res_sd;                /* �ļ�������� */
UINT fnum;            					  /* �ļ��ɹ���д���� */
BYTE ReadBuffer[1024]={0};        /* �������� */
BYTE WriteBuffer[] =              /* д������*/
"��ӭʹ��Ұ��STM32 F429������ �����Ǹ������ӣ��½��ļ�ϵͳ�����ļ�\r\n";  


OS_MEM  mem;                    //�����ڴ�������
uint8_t ucArray [ 70 ] [ 4 ];   //�����ڴ������С
/*
*********************************************************************************************************
*                                                 TCB
*********************************************************************************************************
*/

static  OS_TCB   AppTaskStartTCB;

static  OS_TCB   AppTaskLed1TCB;
        OS_TCB   AppTasktalkTCB;
static  OS_TCB   AppTaskLed3TCB;


/*
*********************************************************************************************************
*                                                STACKS
*********************************************************************************************************
*/

static  CPU_STK  AppTaskStartStk[APP_TASK_START_STK_SIZE];

static  CPU_STK  AppTaskLed1Stk [ APP_TASK_LED1_STK_SIZE ];
static  CPU_STK  AppTasktalkStk [ APP_TASK_TALK_STK_SIZE ];
static  CPU_STK  AppTaskLed3Stk [ APP_TASK_LED3_STK_SIZE ];


/*
*********************************************************************************************************
*                                         FUNCTION PROTOTYPES
*********************************************************************************************************
*/

static  void  AppTaskStart  (void *p_arg);

static  void  AppTaskLed1  ( void * p_arg );
static  void  AppTasktalk  ( void * p_arg );
static  void  AppTaskLed3  ( void * p_arg );


/*
*********************************************************************************************************
*                                                main()
*
* Description : This is the standard entry point for C code.  It is assumed that your code will call
*               main() once you have performed all necessary initialization.
*
* Arguments   : none
*
* Returns     : none
*********************************************************************************************************
*/

int  main (void)
{
    OS_ERR  err;


    OSInit(&err);                                               /* Init uC/OS-III.                                      */

    OSTaskCreate((OS_TCB     *)&AppTaskStartTCB,                /* Create the start task                                */
                 (CPU_CHAR   *)"App Task Start",
                 (OS_TASK_PTR ) AppTaskStart,
                 (void       *) 0,
                 (OS_PRIO     ) APP_TASK_START_PRIO,
                 (CPU_STK    *)&AppTaskStartStk[0],
                 (CPU_STK_SIZE) APP_TASK_START_STK_SIZE / 10,
                 (CPU_STK_SIZE) APP_TASK_START_STK_SIZE,
                 (OS_MSG_QTY  ) 5u,
                 (OS_TICK     ) 0u,
                 (void       *) 0,
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR     *)&err);

    OSStart(&err);                                              /* Start multitasking (i.e. give control to uC/OS-III). */
		
		
}


/*
*********************************************************************************************************
*                                          STARTUP TASK
*
* Description : This is an example of a startup task.  As mentioned in the book's text, you MUST
*               initialize the ticker only once multitasking has started.
*
* Arguments   : p_arg   is the argument passed to 'AppTaskStart()' by 'OSTaskCreate()'.
*
* Returns     : none
*
* Notes       : 1) The first line of code is used to prevent a compiler warning because 'p_arg' is not
*                  used.  The compiler should not generate any code for this statement.
*********************************************************************************************************
*/

static  void  AppTaskStart (void *p_arg)
{
 
    OS_ERR      err;


   (void)p_arg;

    CPU_Init();
    BSP_Init();                                                 /* Initialize BSP functions                             */
    BSP_Tick_Init();
    

    Mem_Init();                                                 /* Initialize Memory Management Module                  */

#if OS_CFG_STAT_TASK_EN > 0u
    OSStatTaskCPUUsageInit(&err);                               /* Compute CPU capacity with no task running            */
#endif

#ifdef CPU_CFG_INT_DIS_MEAS_EN
    CPU_IntDisMeasMaxCurReset();
#endif
	
	/* �����ڴ������� mem */
		OSMemCreate ((OS_MEM      *)&mem,             //ָ���ڴ�������
								 (CPU_CHAR    *)"Mem For Test",   //�����ڴ�������
								 (void        *)ucArray,          //�ڴ�������׵�ַ
								 (OS_MEM_QTY   )70,               //�ڴ�������ڴ����Ŀ
								 (OS_MEM_SIZE  )4,                //�ڴ����ֽ���Ŀ
								 (OS_ERR      *)&err);            //���ش�������


    OSTaskCreate((OS_TCB     *)&AppTaskLed1TCB,                /* Create the Led1 task                                */
                 (CPU_CHAR   *)"App Task Led1",
                 (OS_TASK_PTR ) AppTaskLed1,
                 (void       *) 0,
                 (OS_PRIO     ) APP_TASK_LED1_PRIO,
                 (CPU_STK    *)&AppTaskLed1Stk[0],
                 (CPU_STK_SIZE) APP_TASK_LED1_STK_SIZE / 10,
                 (CPU_STK_SIZE) APP_TASK_LED1_STK_SIZE,
                 (OS_MSG_QTY  ) 5u,
                 (OS_TICK     ) 0u,
                 (void       *) 0,
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR     *)&err);
								 
    OSTaskCreate((OS_TCB     *)&AppTasktalkTCB,                /* Create the talk task                                */
                 (CPU_CHAR   *)"App Task talk",
                 (OS_TASK_PTR ) AppTasktalk,
                 (void       *) 0,
                 (OS_PRIO     ) APP_TASK_TALK_PRIO,
                 (CPU_STK    *)&AppTasktalkStk[0],
                 (CPU_STK_SIZE) APP_TASK_TALK_STK_SIZE / 10,
                 (CPU_STK_SIZE) APP_TASK_TALK_STK_SIZE,
                 (OS_MSG_QTY  ) 5u,
                 (OS_TICK     ) 0u,
                 (void       *) 0,
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR     *)&err);

    OSTaskCreate((OS_TCB     *)&AppTaskLed3TCB,                /* Create the Led3 task                                */
                 (CPU_CHAR   *)"App Task Led3",
                 (OS_TASK_PTR ) AppTaskLed3,
                 (void       *) 0,
                 (OS_PRIO     ) APP_TASK_LED3_PRIO,
                 (CPU_STK    *)&AppTaskLed3Stk[0],
                 (CPU_STK_SIZE) APP_TASK_LED3_STK_SIZE / 10,
                 (CPU_STK_SIZE) APP_TASK_LED3_STK_SIZE,
                 (OS_MSG_QTY  ) 5u,
                 (OS_TICK     ) 0u,
                 (void       *) 0,
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR     *)&err);
		
		
		OSTaskDel ( & AppTaskStartTCB, & err );
		
		
}


/*
*********************************************************************************************************
*                                          LED1 TASK
*********************************************************************************************************
*/

static  void  AppTaskLed1 ( void * p_arg )
{
    OS_ERR      err;


   (void)p_arg;
	//���ⲿSPI Flash�����ļ�ϵͳ���ļ�ϵͳ����ʱ���SPI�豸��ʼ��
	res_sd = f_mount(&fs,"0:",1);
//	WM_MULTIBUF_Enable(1);
//    show_jpg("0:pc.jpg");
//	OSTimeDly ( 1000, OS_OPT_TIME_DLY, & err );
//	printf("@TextToSpeech#���!$");
   _ShowJPEG2("0:ku.jpg",20,30);	
	
		
    while (DEF_TRUE) {                                          /* Task body, always written as an infinite loop.       */
			LED1_TOGGLE;
		    
			
			OSTimeDly ( 1000, OS_OPT_TIME_DLY, & err );
    }
		
		
}


/*
*********************************************************************************************************
*                                          TALK TASK
*********************************************************************************************************
*/

static  void  AppTasktalk ( void * p_arg )
{
    OS_ERR      err;

    u16 tem,hu;
	OS_MSG_SIZE    msg_size;
	char * pMsg;
	
   (void)p_arg;


    while (DEF_TRUE) {                                          /* Task body, always written as an infinite loop.       */
		
		
		
		/* �������񣬵ȴ�������Ϣ */
		pMsg = OSTaskQPend ((OS_TICK        )0,                    //�����޵ȴ�
											  (OS_OPT         )OS_OPT_PEND_BLOCKING, //û����Ϣ����������
											  (OS_MSG_SIZE   *)&msg_size,            //������Ϣ����
											  (CPU_TS        *)0,                    //������Ϣ��������ʱ���
											  (OS_ERR        *)&err);                //���ش�������

	

     	print ( "ReceiveData = %x\n", * pMsg );                                   //��ӡ��Ϣ����
		switch(* pMsg)
		{
			case 0x01:{
						print("���\n");
						OSTimeDly(2000,OS_OPT_TIME_DLY,&err);
						printf("@TextToSpeech#��ʲô���ܰ����$");
				
			          }break;
			case 0x02:
                      {
					    print("����\n");
					  }break;
			case 0x04:
						{
							print("�Ա�\n");
							OSTimeDly(1000,OS_OPT_TIME_DLY,&err);
							printf("@TextToSpeech#��Ȼ�ҵ�������Ů�ģ�[m51]���ҿ����Ǹ��е�[m3]$");
						}break;
						
	        case 0x06:
						{
							print("�¶�\n");
							if(DHT22_Read_Data(&tem,&hu) == 0)
								{
									printf("@TextToSpeech#���ڵ��¶���[n2]%d��ʪ���ǰٷ�֮[n2]%d$",tem/10,hu/10);
									
								}
							else printf("@TextToSpeech#��ѽ�ҵ��¶ȴ���������$");
						}break;
		}


		
		/* �˻��ڴ�� */
		OSMemPut ((OS_MEM  *)&mem,                                 //ָ���ڴ�������
							(void    *)pMsg,                                 //�ڴ����׵�ַ
							(OS_ERR  *)&err);		                             //���ش�������

		
    }
		
		
}


/*
*********************************************************************************************************
*                                          LED3 TASK
*********************************************************************************************************
*/

static  void  AppTaskLed3 ( void * p_arg )
{
    OS_ERR      err;

   
	u16 tem,hum;
   (void)p_arg;


    while (DEF_TRUE) {                                          /* Task body, always written as an infinite loop.       */
//	        _ShowJPEG2("0:pc.jpg",40,30);
//		OSTimeDly ( 1000, OS_OPT_TIME_DLY, & err );
//		    _ShowJPEG2("0:ku.jpg",40,30);
//		if(PGin(12)==1) print("RED Work\n");
//		    else print("RED not work\n");
		
		if(DHT22_Read_Data(&tem,&hum) == 0)
								{
									tem = tem/10;
									hum = hum/10;
									Usart_Senddec(USART1,tem);
									
									OSTimeDly(1000,OS_OPT_TIME_DLY,&err);
									Usart_Senddec(USART1,hum);
								}
			OSTimeDly ( 10000, OS_OPT_TIME_DLY, & err );
    }
		
		
}






