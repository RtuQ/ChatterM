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
//UINT fnum;            					  /* �ļ��ɹ���д���� */
//BYTE ReadBuffer[1024]={0};        /* �������� */
//BYTE WriteBuffer[] =              /* д������*/
//"��ӭʹ��Ұ��STM32 F429������ �����Ǹ������ӣ��½��ļ�ϵͳ�����ļ�\r\n";  


OS_MEM  mem;                    //�����ڴ�������
uint8_t ucArray [ 70 ] [ 4 ];   //�����ڴ������С

uint8_t Voice_Mode = 0;


// ADC1���ĵ�ѹֵͨ��MDA��ʽ����SRAM
extern __IO uint16_t ADC_ConvertedValue;

#define People_Mode PGin(12)

/*
*********************************************************************************************************
*                                                 TCB
*********************************************************************************************************
*/

static  OS_TCB   AppTaskStartTCB;

static 	OS_TCB	 AppTaskShowBQTCB;
static  OS_TCB   AppTaskPowerTCB;
static  OS_TCB   AppTaskLed1TCB;
        OS_TCB   AppTasktalkTCB;
static  OS_TCB   AppTaskLed3TCB;
static 	OS_TCB	 AppTaskCheckPeopleTCB;
static  OS_TCB   SystemDatasBroadcast_TCB; 


/*
*********************************************************************************************************
*                                                STACKS
*********************************************************************************************************
*/

static  CPU_STK  AppTaskStartStk[APP_TASK_START_STK_SIZE];

static  CPU_STK  AppTaskShowBQStk [ APP_TASK_ShowBQ_STK_SIZE ];
static  CPU_STK  AppTaskPowerStk [ APP_TASK_Power_STK_SIZE ];
static  CPU_STK  AppTaskLed1Stk [ APP_TASK_LED1_STK_SIZE ];
static  CPU_STK  AppTasktalkStk [ APP_TASK_TALK_STK_SIZE ];
static  CPU_STK  AppTaskLed3Stk [ APP_TASK_LED3_STK_SIZE ];
static  CPU_STK  AppTaskCheckPeopleStk [ APP_TASK_CheckPeople_STK_SIZE ];
static  CPU_STK  SystemDatasBroadcast_STK [SystemDatasBroadcast_STK_SIZE];


/*
*********************************************************************************************************
*                                         FUNCTION PROTOTYPES
*********************************************************************************************************
*/

static  void  AppTaskStart  (void *p_arg);

static  void  AppTaskShowBQ  ( void * p_arg );
static  void  AppTaskPower  ( void * p_arg );
static  void  AppTaskLed1  ( void * p_arg );
static  void  AppTasktalk  ( void * p_arg );
static  void  AppTaskLed3  ( void * p_arg );
static  void  AppTaskCheckPeople  ( void * p_arg );
static void  SystemDatasBroadcast (void *p_arg);


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
	
   //���ⲿSPI Flash�����ļ�ϵͳ���ļ�ϵͳ����ʱ���SPI�豸��ʼ��
	res_sd = f_mount(&fs,"0:",1);

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
								 
	OSTaskCreate((OS_TCB     *)&AppTaskShowBQTCB,                /* Create the Led1 task                                */
                 (CPU_CHAR   *)"App Task ShowBQ",
                 (OS_TASK_PTR ) AppTaskShowBQ,
                 (void       *) 0,
                 (OS_PRIO     ) APP_TASK_ShowBQ_PRIO,
                 (CPU_STK    *)&AppTaskShowBQStk[0],
                 (CPU_STK_SIZE) APP_TASK_ShowBQ_STK_SIZE / 10,
                 (CPU_STK_SIZE) APP_TASK_ShowBQ_STK_SIZE,
                 (OS_MSG_QTY  ) 5u,
                 (OS_TICK     ) 0u,
                 (void       *) 0,
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR     *)&err);


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
				 
	OSTaskCreate((OS_TCB     *)&AppTaskPowerTCB,                /* Create the Power task                                */
                 (CPU_CHAR   *)"App Task Power",
                 (OS_TASK_PTR ) AppTaskPower,
                 (void       *) 0,
                 (OS_PRIO     ) APP_TASK_Power_PRIO,
                 (CPU_STK    *)&AppTaskPowerStk[0],
                 (CPU_STK_SIZE) APP_TASK_Power_STK_SIZE / 10,
                 (CPU_STK_SIZE) APP_TASK_Power_STK_SIZE,
                 (OS_MSG_QTY  ) 5u,
                 (OS_TICK     ) 0u,
                 (void       *) 0,
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR     *)&err);
	OSTaskCreate((OS_TCB     *)&AppTaskCheckPeopleTCB,                /* Create the Power task                                */
                 (CPU_CHAR   *)"App Task CheckPeople",
                 (OS_TASK_PTR ) AppTaskCheckPeople,
                 (void       *) 0,
                 (OS_PRIO     ) APP_TASK_CheckPeople_PRIO,
                 (CPU_STK    *)&AppTaskCheckPeopleStk[0],
                 (CPU_STK_SIZE) APP_TASK_CheckPeople_STK_SIZE / 10,
                 (CPU_STK_SIZE) APP_TASK_CheckPeople_STK_SIZE,
                 (OS_MSG_QTY  ) 5u,
                 (OS_TICK     ) 0u,
                 (void       *) 0,
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR     *)&err);
	OSTaskCreate( (OS_TCB     *)&SystemDatasBroadcast_TCB,
                (CPU_CHAR   *)"SystemDatasBroadcast",
                (OS_TASK_PTR ) SystemDatasBroadcast,
                (void       *) 0,
                (OS_PRIO     ) SystemDatasBroadcast_PRIO,
                (CPU_STK    *)&SystemDatasBroadcast_STK[0],
                (CPU_STK_SIZE) SystemDatasBroadcast_STK_SIZE/10,
                (CPU_STK_SIZE) SystemDatasBroadcast_STK_SIZE,
                (OS_MSG_QTY  ) 0,
                (OS_TICK     ) 0,
                (void       *) 0,
                (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR), 
                (OS_ERR     *) &err);	
		
		
		OSTaskDel ( & AppTaskStartTCB, & err );
		
		
}

/*
*********************************************************************************************************
*                                          ������ʾ TASK
*********************************************************************************************************
*/
static void AppTaskShowBQ (void * p_arg )
{
	OS_ERR      err;
    u8 RandData = 0;
   (void)p_arg;
	
	  while (DEF_TRUE) {                                          /* Task body, always written as an infinite loop.       */
		    RandData = Read_Seed()%3;
			if(RandData==0)	_ShowJPEG2("0:pc.jpg",0,0);
			if(RandData==1)	_ShowJPEG2("0:pc2.jpg",0,0);
			if(RandData==2)	_ShowJPEG2("0:pc3.jpg",0,0);
			OSTimeDlyHMSM(0, 0, 20,0,OS_OPT_TIME_DLY,&err);
		    
    }
	
}

/*
*********************************************************************************************************
*                                          ϵͳ����ָʾ TASK
*********************************************************************************************************
*/

static  void  AppTaskLed1 ( void * p_arg )
{
    OS_ERR      err;
  
   (void)p_arg;

//	WM_MULTIBUF_Enable(1);
	
		
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
	u16 * pMsg;
	
	u8 i = 0 ;
	
   (void)p_arg;


    while (DEF_TRUE) {                                          /* Task body, always written as an infinite loop.       */
		
		
		
		/* �������񣬵ȴ�������Ϣ */
		pMsg = (u16 *)OSTaskQPend ((OS_TICK        )0,                    //�����޵ȴ�
											  (OS_OPT         )OS_OPT_PEND_BLOCKING, //û����Ϣ����������
											  (OS_MSG_SIZE   *)&msg_size,            //������Ϣ����
											  (CPU_TS        *)0,                    //������Ϣ��������ʱ���
											  (OS_ERR        *)&err);                //���ش�������

	

     	print ( "ReceiveData = %d\n", * pMsg );                                   //��ӡ��Ϣ����
		Voice_Mode = 1;
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
						}break;
						
	        case 0x06:
						{
							print("�¶�\n");
							if(DHT22_Read_Data(&tem,&hu) == 0)
								{
									printf("@TextToSpeech#���ڵ��¶���[n2]%d��ʪ���ǰٷ�֮[n2]%d$",tem/10,hu/10);
									OSTaskSuspend((OS_TCB *)&AppTaskShowBQTCB,&err);
									_ShowJPEG2("0:xiao.jpg",0,0);
									if(tem >=28)
									{
										printf("@TextToSpeech#�����̫���ˣ�$");
										_ShowJPEG2("0:ku.jpg",0,0);
									}
									OSTimeDly(20000,OS_OPT_TIME_DLY,&err);
								}
							else printf("@TextToSpeech#��ѽ�ҵ��¶ȴ���������$");
						}break;
			case 0x0f:
						{
							print("����\n");
							OSTaskSuspend((OS_TCB *)&AppTaskShowBQTCB,&err);  //������ʾ��������
							_ShowJPEG2("0:fahuo.jpg",0,0);
						  
						}break;
			case 0x0c:
			            {
							print("Ц��\n");
							OSTaskSuspend((OS_TCB *)&AppTaskShowBQTCB,&err);
									_ShowJPEG2("0:hanxiao.jpg",0,0);
							if(i==0)printf("@TextToSpeech#��һѧ���������ҵȫ�����꣬�����һҳ����д��һ�仰:�ҵ���ҵ��17�����⣬74������֣��鷳��ʦ�������һ�ҳ������벻Ҫ��һ�����ġ���������һ���£�$");
						    if(i==1)printf("@TextToSpeech#�ڹ�����.����ţȥ��ҩ������2��Сʱ.��ţ��û�������ڹ꼱�����:�ٲ������Ҿ�����!��ʱ���⴫������ţ������:����˵�Ҳ�ȥ��!$");
						    OSTimeDlyHMSM(0,0,25,0,OS_OPT_TIME_DLY,&err);
							i++; 
							if(i>=2)i = 0;
							
						}break;
			case 0x14:
						{
							print("����\n");
							OSTaskSuspend((OS_TCB *)&AppTaskShowBQTCB,&err);
							
							GUI_SelectLayer(1);
							GUI_SetBkColor(GUI_BLACK);
							GUI_Clear();
//							WM_MULTIBUF_Enable(1);
							_ShowGIF2("0:renzhu.gif",2,1);
													
						}break;
				      
				
			
		}

         OSTaskResume((OS_TCB *)&AppTaskShowBQTCB,&err);  //�ָ�������ʾ��������
		
		Voice_Mode = 0;
		
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
		
		if(DHT22_Read_Data(&tem,&hum) == 0)
								{
									tem = tem/10;
									hum = hum/10;
									Usart_Senddec(USART1,tem);
									OSTimeDly(1000,OS_OPT_TIME_DLY,&err);
									Usart_Senddec(USART1,hum);
								}
			OSTimeDlyHMSM(0, 0, 10,0,OS_OPT_TIME_DLY,&err);
    }
		
		
}

/*
*********************************************************************************************************
*                                          ������� TASK
*********************************************************************************************************
*/
static void  AppTaskPower  ( void * p_arg )
{
	OS_ERR err;
	 	 
    float ADC_Vol; // ���ڱ���ת�������ĵ�ѹֵ
	
	(void)p_arg;
	while (DEF_TRUE){
      	    
			ADC_Vol =(float) ADC_ConvertedValue/4096*(float)13.3; // ��ȡת����ADֵ
	        print("\r\n The current AD value = %f V \r\n",ADC_Vol); 
			if (ADC_Vol <= 10&&Voice_Mode == 0)
			{
				_ShowJPEG2("0:meidian.jpg",0,0);
				printf("@TextToSpeech#��û���ˣ�Ҫ���һ���ؿ���$");
			}
		    OSTimeDlyHMSM(0,5,0,0,OS_OPT_TIME_DLY,&err); //ÿ��5���Ӽ��һ�ε�ѹֵ
		    
	}
}

/*
*********************************************************************************************************
*                                          �˼�� TASK
*********************************************************************************************************
*/
static void  AppTaskCheckPeople  ( void * p_arg )
{
	OS_ERR err;
	 	 
    u8 Mode = 0;
	u8 Last_Mode = 0;
	(void)p_arg;
	while (DEF_TRUE){
      	    
		    if(People_Mode == 1&&Voice_Mode == 0) {
				
				OSTaskSuspend((OS_TCB *)&AppTaskShowBQTCB,&err);
				OSTaskSuspend((OS_TCB *)&AppTasktalk,&err);
				Mode = 1;
				if(Last_Mode == 0)
				{
					printf("@TextToSpeech#��ã���ʲô�ҿ��԰������$");
				    OSTimeDlyHMSM(0,0,5,0,OS_OPT_TIME_DLY,&err); 
				}
		        Last_Mode = ~Last_Mode;
				print("on\n");
		    }
			if(Mode == 1)
			{
				OSTaskResume((OS_TCB *)&AppTaskShowBQTCB,&err);  //�ָ�������ʾ��������
				OSTaskResume((OS_TCB *)&AppTasktalk,&err);  //�ָ��Ի���������
				Mode = 0 ;
			}
		    OSTimeDlyHMSM(0,0,0,250,OS_OPT_TIME_DLY,&err); //ÿ��250ms���һ��
	}
	
}
/*
*********************************************************************************************************
*                                          ��������ջʹ���� TASK  
*********************************************************************************************************
*/

static void  SystemDatasBroadcast (void *p_arg)
{
	OS_ERR err;
//  CPU_STK_SIZE free,used;
//  (void)p_arg;
//  while(DEF_TRUE)
//  {
//	OSTaskStkChk (&SystemDatasBroadcast_TCB,&free,&used,&err);//  ��ͳ��������Ķ�ջʹ����Ҳ��ӡ����
//	print("SystemDatasBroadcast      used/free:%d/%d  usage:%d%%\r\n",used,free,(used*100)/(used+free));
//		
//	OSTaskStkChk (&AppTaskShowBQTCB,&free,&used,&err);
//	print("AppTaskShowBQ             used/free:%d/%d  usage:%d%%\r\n",used,free,(used*100)/(used+free));
//		
//	OSTaskStkChk (&AppTaskPowerTCB,&free,&used,&err);
//	print("AppTaskPower              used/free:%d/%d  usage:%d%%\r\n",used,free,(used*100)/(used+free));
//		
//	OSTaskStkChk (&AppTaskLed1TCB,&free,&used,&err);
//	print("AppTaskLed1               used/free:%d/%d  usage:%d%%\r\n",used,free,(used*100)/(used+free));
//		
//	OSTaskStkChk (&AppTasktalkTCB,&free,&used,&err);
//	print("AppTasktalk               used/free:%d/%d  usage:%d%%\r\n",used,free,(used*100)/(used+free));
//		
//	OSTaskStkChk (&AppTaskLed3TCB,&free,&used,&err);
//	print("AppTaskLed3               used/free:%d/%d  usage:%d%%\r\n",used,free,(used*100)/(used+free));
// 
// 
//	OSTaskStkChk (&AppTaskCheckPeopleTCB,&free,&used,&err);
//	print("AppTaskCheckPeople        used/free:%d/%d  usage:%d%%\r\n",used,free,(used*100)/(used+free));
//	
//	print("\r\n\r\n\r\n");
//	OSTimeDlyHMSM(0,0,5,0,(OS_OPT)OS_OPT_TIME_DLY,(OS_ERR*)&err);
//   }


	OS_TCB      *p_tcb;	        /* ����һ��������ƿ�ָ��, TCB = TASK CONTROL BLOCK */
	float CPU = 0.0f;
    (void)p_arg;
	
	while(DEF_TRUE){
		
    CPU_SR_ALLOC();  //���������У���չ������ͬucos-ii��
	CPU_CRITICAL_ENTER();
    p_tcb = OSTaskDbgListPtr;
    CPU_CRITICAL_EXIT();
	
	/* ��ӡ���� */
	print("===============================================================\r\n");
	print(" ���ȼ� ʹ��ջ ʣ��ջ �ٷֱ� ������   ������\r\n");
	print("  Prio   Used  Free   Per    CPU     Taskname\r\n");

	/* ����������ƿ��б�(TCB list)����ӡ���е���������ȼ������� */
	while (p_tcb != (OS_TCB *)0) 
		{
			CPU = (float)p_tcb->CPUUsage / 100;
	print("   %d      %d    %d    %d%%   %f%%    %s\r\n", 
			p_tcb->Prio, 
			p_tcb->StkUsed, 
			p_tcb->StkFree, 
			(p_tcb->StkUsed * 100) / (p_tcb->StkUsed + p_tcb->StkFree),
			CPU,
			p_tcb->NamePtr);		
			
			CPU_CRITICAL_ENTER();
			p_tcb = p_tcb->DbgNextPtr;
			CPU_CRITICAL_EXIT();
		}
	OSTimeDlyHMSM(0,0,5,0,(OS_OPT)OS_OPT_TIME_DLY,(OS_ERR*)&err);
   }
}




