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
FATFS fs;													/* FatFs文件系统对象 */
FIL fnew;													/* 文件对象 */
FRESULT res_sd;                                             /* 文件操作结果 */
//UINT fnum;            					  /* 文件成功读写数量 */
//BYTE ReadBuffer[1024]={0};        /* 读缓冲区 */
//BYTE WriteBuffer[] =              /* 写缓冲区*/
//"";  


OS_MEM  mem;                    //声明内存管理对象
//OS_Q  Task_Msg;                 //声明消息队列

uint8_t ucArray [ 70 ] [ 4 ];     //声明内存分区大小

uint8_t Voice_Mode = 0;



// ADC1换的电压值通过MDA方式传到SRAM
extern __IO uint16_t ADC_ConvertedValue[RHEOSTAT_NOFCHANEL];

u8 Touch_TimeMode = 1;
u8 settime_mode  = 1;

#define People_Mode PGin(12)


/*
*********************************************************************************************************
*                                                 TCB
*********************************************************************************************************
*/

static  OS_TCB   AppTaskStartTCB;

    	OS_TCB	 AppTaskShowBQTCB;
static  OS_TCB   AppTaskPowerTCB;
static  OS_TCB   AppTaskLed1TCB;
        OS_TCB   AppTasktalkTCB;
static  OS_TCB   AppTaskLed3TCB;
static 	OS_TCB	 AppTaskCheckPeopleTCB;
static  OS_TCB   AppTaskTouchScanTCB; 
static  OS_TCB   AppTaskTouchTCB; 
static  OS_TCB   AppTaskLightTCB; 
        OS_TCB   AppTaskWindowTCB; 

#if SystemData == 1                           //在includes.h中定义
static  OS_TCB   SystemDatasBroadcast_TCB; 
#endif

/*
*********************************************************************************************************
*                                                STACKS
*********************************************************************************************************
*/

static  CPU_STK  AppTaskStartStk[APP_TASK_START_STK_SIZE];

static  CPU_STK  AppTaskShowBQStk [ APP_TASK_ShowBQ_STK_SIZE ];
static  CPU_STK  AppTaskPowerStk [ APP_TASK_Power_STK_SIZE ];
static  CPU_STK  AppTaskLightStk [ APP_TASK_Light_STK_SIZE ];
static  CPU_STK  AppTaskLed1Stk [ APP_TASK_LED1_STK_SIZE ];
static  CPU_STK  AppTasktalkStk [ APP_TASK_TALK_STK_SIZE ];
static  CPU_STK  AppTaskLed3Stk [ APP_TASK_LED3_STK_SIZE ];
static  CPU_STK  AppTaskCheckPeopleStk [ APP_TASK_CheckPeople_STK_SIZE ];
static  CPU_STK  AppTaskTouchScanStk [APP_TASK_TouchScan_STK_SIZE];
static  CPU_STK  AppTaskTouchStk [APP_TASK_Touch_STK_SIZE];
static  CPU_STK  AppTaskWindowStk [APP_TASK_Window_STK_SIZE];

#if SystemData == 1
static  CPU_STK  SystemDatasBroadcast_STK [SystemDatasBroadcast_STK_SIZE];
#endif
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
static void  AppTaskTouchScan (void *p_arg);
static void  AppTaskTouch (void *p_arg);
static void  AppTaskLight (void *p_arg);
static void  AppTaskWindow (void*p_arg);

#if SystemData == 1
static void  SystemDatasBroadcast (void *p_arg);
#endif

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

    u8 i;
	u8 num=0;
	u8 len = 2;
   (void)p_arg;

    CPU_Init();
    BSP_Init();                                                 
    BSP_Tick_Init();
    Mem_Init();                                               
	
	res_sd = f_mount(&fs,"0:",1);                  //挂载SD卡           

#if OS_CFG_STAT_TASK_EN > 0u
    OSStatTaskCPUUsageInit(&err);                               /* Compute CPU capacity with no task running            */
#endif

#ifdef CPU_CFG_INT_DIS_MEAS_EN
    CPU_IntDisMeasMaxCurReset();
#endif

     _ShowJPEG2("0:kaiji.jpg",0,0);
	GUI_SetBkColor(GUI_WHITE);
	GUI_SetColor(GUI_BLACK);
	GUI_SetFont(&GUI_Font8x18);
	GUI_DispStringAt("System Starting...",0,220);
	GUI_DispStringAt("%",175,220);
	GUI_DispDecAt(num,150,220,len);
	GUI_DispStringAt("[..........................................................]",0,241);
	bsp_DelayMS(2000);
	for(i=0;i<58;i++)
	{
		GUI_DispStringAt("=",8+(i*8),241);
		GUI_DispDecAt(num,150,220,len);
		num = num+2;
		if(num >=100) { num =100; len = 3;}
		bsp_DelayMS(100);
	}
	printf("@PlayFLASH#0023$");
	bsp_DelayMS(2000);
	
	
     CPU_SR_ALLOC();                               //进入临界区域防止被其他任务打断
	 OS_CRITICAL_ENTER();
	
	/* 创建内存管理对象 mem */
		OSMemCreate ((OS_MEM      *)&mem,                         //指向内存管理对象
								 (CPU_CHAR    *)"Mem For Test",   //命名内存管理对象
								 (void        *)ucArray,          //内存分区的首地址
								 (OS_MEM_QTY   )70,               //内存分区中内存块数目
								 (OS_MEM_SIZE  )4,                //内存块的字节数目
								 (OS_ERR      *)&err);            //返回错误类型             
								 
	OSTaskCreate((OS_TCB     *)&AppTaskShowBQTCB,                /* Create the ShowBQ task                                */
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
	OSTaskCreate((OS_TCB     *)&AppTaskLightTCB,                /* Create the Light task                                */
                 (CPU_CHAR   *)"App Task Light",
                 (OS_TASK_PTR ) AppTaskLight,
                 (void       *) 0,
                 (OS_PRIO     ) APP_TASK_Light_PRIO,
                 (CPU_STK    *)&AppTaskLightStk[0],
                 (CPU_STK_SIZE) APP_TASK_Light_STK_SIZE / 10,
                 (CPU_STK_SIZE) APP_TASK_Light_STK_SIZE,
                 (OS_MSG_QTY  ) 5u,
                 (OS_TICK     ) 0u,
                 (void       *) 0,
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR     *)&err);
//	OSTaskCreate((OS_TCB     *)&AppTaskCheckPeopleTCB,                /* Create the CheckPeople task                                */
//                 (CPU_CHAR   *)"App Task CheckPeople",
//                 (OS_TASK_PTR ) AppTaskCheckPeople,
//                 (void       *) 0,
//                 (OS_PRIO     ) APP_TASK_CheckPeople_PRIO,
//                 (CPU_STK    *)&AppTaskCheckPeopleStk[0],
//                 (CPU_STK_SIZE) APP_TASK_CheckPeople_STK_SIZE / 10,
//                 (CPU_STK_SIZE) APP_TASK_CheckPeople_STK_SIZE,
//                 (OS_MSG_QTY  ) 5u,
//                 (OS_TICK     ) 0u,
//                 (void       *) 0,
//                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
//                 (OS_ERR     *)&err);
    OSTaskCreate((OS_TCB     *)&AppTaskTouchScanTCB,                /* Create the TouchScan task                                */
                 (CPU_CHAR   *)"App Task TouchScan",
                 (OS_TASK_PTR ) AppTaskTouchScan,
                 (void       *) 0,
                 (OS_PRIO     ) APP_TASK_TouchScan_PRIO,
                 (CPU_STK    *)&AppTaskTouchScanStk[0],
                 (CPU_STK_SIZE) APP_TASK_TouchScan_STK_SIZE / 10,
                 (CPU_STK_SIZE) APP_TASK_TouchScan_STK_SIZE,
                 (OS_MSG_QTY  ) 5u,                                      //消息任务大小，需要时改
                 (OS_TICK     ) 0u,
                 (void       *) 0,
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR     *)&err);
    OSTaskCreate((OS_TCB     *)&AppTaskTouchTCB,                /* Create the Touch task                                */
                 (CPU_CHAR   *)"App Task Touch",
                 (OS_TASK_PTR ) AppTaskTouch,
                 (void       *) 0,
                 (OS_PRIO     ) APP_TASK_Touch_PRIO,
                 (CPU_STK    *)&AppTaskTouchStk[0],
                 (CPU_STK_SIZE) APP_TASK_Touch_STK_SIZE / 10,
                 (CPU_STK_SIZE) APP_TASK_Touch_STK_SIZE,
                 (OS_MSG_QTY  ) 5u,                                      //消息任务大小，需要时改
                 (OS_TICK     ) 0u,
                 (void       *) 0,
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR     *)&err);
				 
    OSTaskCreate((OS_TCB     *)&AppTaskWindowTCB,                /* Create the Window task                                */
                 (CPU_CHAR   *)"App Task Window",
                 (OS_TASK_PTR ) AppTaskWindow,
                 (void       *) 0,
                 (OS_PRIO     ) APP_TASK_Window_PRIO,
                 (CPU_STK    *)&AppTaskWindowStk[0],
                 (CPU_STK_SIZE) APP_TASK_Window_STK_SIZE / 10,
                 (CPU_STK_SIZE) APP_TASK_Window_STK_SIZE,
                 (OS_MSG_QTY  ) 5u,                                      //消息任务大小，需要时改
                 (OS_TICK     ) 0u,
                 (void       *) 0,
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR     *)&err);
#if SystemData == 1
	OSTaskCreate( (OS_TCB     *)&SystemDatasBroadcast_TCB,                 /* Create the SystemDatasBroadcast task       */
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
#endif				 
		OSTaskSuspend((OS_TCB *)&AppTaskTouchScanTCB,&err);               //挂起画板任务
		OSTaskSuspend((OS_TCB *)&AppTaskWindowTCB,&err);               //挂起画板任务
		OSTaskDel ( & AppTaskStartTCB, & err );                           //删除启动任务
        OS_CRITICAL_EXIT();	                                              //退出临界
		
		
}

/*
*********************************************************************************************************
*                                          表情显示 TASK
*********************************************************************************************************
*/
static void AppTaskShowBQ (void * p_arg )
{
	OS_ERR      err;
    u8 RandData = 0;
	u8 RandData2 = 0;
   (void)p_arg;
	
	  while (DEF_TRUE) {                                          /* Task body, always written as an infinite loop.       */
		    RandData = Read_Seed();
		    RandData2 = RandData%3;
		    RandData = RandData%11;
		   Debug_printf("RandData = %d\n",RandData);
//		  _ShowGIF2("0:yinyue.gif",1,2,5);
		  switch(RandData)
		  {
			  case 0:
				{
				  _ShowJPEG2("0:pc.jpg",0,0);

				}	break;
			  case 1:	
				{ 
					_ShowJPEG2("0:pc2.jpg",0,0);

				}break;
			  case 2:
				{
					_ShowJPEG2("0:pc3.jpg",0,0);

				}break;
			  case 3:
				{
//					GUI_SelectLayer(1);
					GUI_SetBkColor(GUI_BLACK);
					GUI_Clear();
					OSTimeDly ( 50, OS_OPT_TIME_DLY, & err );
					_ShowGIF2("0:xiaoji3.gif",2,1,5);
					if(RandData2==0)	_ShowJPEG2("0:pc.jpg",0,0);
					if(RandData2==1)	_ShowJPEG2("0:pc2.jpg",0,0);
					else	_ShowJPEG2("0:pc3.jpg",0,0);

				}break;
			  case 4:
				{
//					GUI_SelectLayer(1);
					GUI_SetBkColor(GUI_BLACK);
					GUI_Clear();
					OSTimeDly ( 50, OS_OPT_TIME_DLY, & err );
					_ShowGIF2("0:xiaoji4.gif",2,1,5);
					if(RandData2==0)	_ShowJPEG2("0:pc.jpg",0,0);
					if(RandData2==1)	_ShowJPEG2("0:pc2.jpg",0,0);
					else	_ShowJPEG2("0:pc3.jpg",0,0);

				}break;
			  case 5:
				{
//					GUI_SelectLayer(1);
					GUI_SetBkColor(GUI_BLACK);
					GUI_Clear();
					OSTimeDly ( 50, OS_OPT_TIME_DLY, & err );
					_ShowGIF2("0:xiaoji5.gif",2,1,5);
					if(RandData2==0)	_ShowJPEG2("0:pc.jpg",0,0);
					if(RandData2==1)	_ShowJPEG2("0:pc2.jpg",0,0);
					else	_ShowJPEG2("0:pc3.jpg",0,0);

				}break;
			  case 6:
				{
//					GUI_SelectLayer(1);
					GUI_SetBkColor(GUI_BLACK);
					GUI_Clear();
					OSTimeDly ( 50, OS_OPT_TIME_DLY, & err );
					_ShowGIF2("0:xiaoji6.gif",2,1,5);
					if(RandData2==0)	_ShowJPEG2("0:pc.jpg",0,0);
					if(RandData2==1)	_ShowJPEG2("0:pc2.jpg",0,0);
					else	_ShowJPEG2("0:pc3.jpg",0,0);

				}break;
			  case 7:
				{
//					GUI_SelectLayer(1);
					GUI_SetBkColor(GUI_BLACK);
					GUI_Clear();
					OSTimeDly ( 50, OS_OPT_TIME_DLY, & err );
					_ShowGIF2("0:xiaoji7.gif",2,1,5);
					if(RandData2==0)	_ShowJPEG2("0:pc.jpg",0,0);
					if(RandData2==1)	_ShowJPEG2("0:pc2.jpg",0,0);
					else	_ShowJPEG2("0:pc3.jpg",0,0);

				}break;
			  case 8:
				{
//					GUI_SelectLayer(1);
					GUI_SetBkColor(GUI_BLACK);
					GUI_Clear();
					OSTimeDly ( 50, OS_OPT_TIME_DLY, & err );
					_ShowGIF2("0:xiaoji8.gif",2,1,5);
					if(RandData2==0)	_ShowJPEG2("0:pc.jpg",0,0);
					if(RandData2==1)	_ShowJPEG2("0:pc2.jpg",0,0);
					else	_ShowJPEG2("0:pc3.jpg",0,0);

				}break;
			  case 9:
				{
//					GUI_SelectLayer(1);
					GUI_SetBkColor(GUI_BLACK);
					GUI_Clear();
					OSTimeDly ( 50, OS_OPT_TIME_DLY, & err );
					_ShowGIF2("0:xiaoji9.gif",2,1,5);
					if(RandData2==0)	_ShowJPEG2("0:pc.jpg",0,0);
					if(RandData2==1)	_ShowJPEG2("0:pc2.jpg",0,0);
					else	_ShowJPEG2("0:pc3.jpg",0,0);

				}break;
			  case 10:
				{
//					GUI_SelectLayer(1);
					GUI_SetBkColor(GUI_BLACK);
					GUI_Clear();
					OSTimeDly ( 50, OS_OPT_TIME_DLY, & err );
					_ShowGIF2("0:xiaoji10.gif",2,1,5);
					if(RandData2==0)	_ShowJPEG2("0:pc.jpg",0,0);
					if(RandData2==1)	_ShowJPEG2("0:pc2.jpg",0,0);
					else	_ShowJPEG2("0:pc3.jpg",0,0);

				}break;
			}
          
			OSTimeDlyHMSM(0, 0, 25,0,OS_OPT_TIME_DLY,&err);			
    }
}

/*
*********************************************************************************************************
*                                          系统运行指示 TASK
*********************************************************************************************************
*/

static  void  AppTaskLed1 ( void * p_arg )
{
    OS_ERR      err;

	float pitch,roll,yaw; 		//欧拉角
  
   (void)p_arg;
		
    while (DEF_TRUE) {                                          /* Task body, always written as an infinite loop.       */
			
		    LED1_TOGGLE;
		   while(mpu_dmp_get_data(&pitch,&roll,&yaw)!=0)
			{ 
				if(roll<0)
				{
					roll = roll*(-1);
					Debug_printf("roll = -%d        pitch = %d          yaw = %d          \n",(int)(roll*100),(int)(pitch*100),(int)(yaw*10));
				}
				else
					Debug_printf("roll = %d        pitch = %d          yaw = %d          \n",(int)(roll*100),(int)(pitch*100),(int)(yaw*10));
			}
//			RTC_TimeAndDate_Show();
			OSTimeDly ( 2000, OS_OPT_TIME_DLY, & err );                                       
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
	u8 Led_Mode = 0;
	u8 HH_Mode = 0;
	
	u8 i = 0 ;
	
   (void)p_arg;


    while (DEF_TRUE) {                                          /* Task body, always written as an infinite loop.       */
		
		
		
		/* 阻塞任务，等待任务消息 */
		pMsg = (u16 *)OSTaskQPend ((OS_TICK        )0,                    //无期限等待
											  (OS_OPT         )OS_OPT_PEND_BLOCKING, //没有消息就阻塞任务
											  (OS_MSG_SIZE   *)&msg_size,            //返回消息长度
											  (CPU_TS        *)0,                    //返回消息被发布的时间戳
											  (OS_ERR        *)&err);                //返回错误类型

     	Debug_printf ( "ReceiveData = %d\n", * pMsg );                                   //打印消息内容
		Voice_Mode = 1;
		switch(* pMsg)
		{
			case 0x01:{
						Debug_printf("你好\n");
						OSTimeDly(2000,OS_OPT_TIME_DLY,&err);
						printf("@TextToSpeech#有什么我能帮你的$");
			          }break;
			case 0x02:
                      {
					    Debug_printf("你好\n");
						
					  }break;
			case 0x04:
						{
							Debug_printf("性别\n");
							OSTimeDly(1000,OS_OPT_TIME_DLY,&err);
						}break;
						
	        case 0x06:
						{
							Debug_printf("温度\n");
							if(DHT22_Read_Data(&tem,&hu) == 0)
								{
									tem = tem/10;
									hu = hu/10;
									
									OSTaskSuspend((OS_TCB *)&AppTaskShowBQTCB,&err);

									if(tem >=28)
									{
										printf("@TextToSpeech#这真的太热了！温度都到[n2]%dD度了$",tem);
										_ShowJPEG2("0:ku.jpg",0,0);
									}
									else
									{
										_ShowJPEG2("0:xiao.jpg",0,0);
										printf("@TextToSpeech#现在的温度是[n2]%d，湿度是百分之[n2]%d$",tem,hu);
									    OSTimeDly(20000,OS_OPT_TIME_DLY,&err);
									}
								}
							else printf("@TextToSpeech#啊呀我的温度传感器坏了$");
							OSTaskResume((OS_TCB *)&AppTaskShowBQTCB,&err);  //恢复空闲显示表情任务
						}break;
			case 0x09:
						{
							Debug_printf("听歌\n");
							OSTaskSuspend((OS_TCB *)&AppTaskShowBQTCB,&err);
//							GUI_SelectLayer(1);
							GUI_SetBkColor(GUI_BLACK);
							GUI_Clear();
							OSTimeDlyHMSM(0, 0, 0,50,OS_OPT_TIME_DLY,&err);
							printf("@TextToSpeech#我摘下星星给你，我摘下月亮给你，我摘下太阳给你，你想要的我都给你，都给你呀都给你$");
							_ShowGIF2("0:g1.gif",1,2,3);
							_ShowGIF2("0:g2.gif",1,2,3);
							_ShowGIF2("0:g3.gif",1,2,3);
							_ShowGIF2("0:g4.gif",1,2,3);
							_ShowGIF2("0:g5.gif",1,2,3);
							_ShowGIF2("0:g6.gif",1,2,3);
							OSTaskResume((OS_TCB *)&AppTaskShowBQTCB,&err);  
						}break;
			case 0x0f:
						{
							Debug_printf("生气\n");
							OSTaskSuspend((OS_TCB *)&AppTaskShowBQTCB,&err);  //挂起显示表情任务
							_ShowJPEG2("0:fahuo.jpg",0,0);
						  OSTaskResume((OS_TCB *)&AppTaskShowBQTCB,&err);  //恢复空闲显示表情任务
						}break;
			case 0x0c:
			            {
							Debug_printf("笑话\n");
							OSTaskSuspend((OS_TCB *)&AppTaskShowBQTCB,&err);
							
							_ShowJPEG2("0:hanxiao.jpg",0,0);
							if(i==0)printf("@TextToSpeech#有一学渣，暑假作业全部做完，在最后一页上面写了一句话:我的作业有17道错题，74个错别字，麻烦老师认真的逐一找出来，请不要用一个“阅”，换我们一个月！$");
						    if(i==1)printf("@TextToSpeech#乌龟受伤.让蜗牛去买药。过了2个小时.蜗牛还没回来。乌龟急了骂道:再不回来我就死了!这时门外传来了蜗牛的声音:你再说我不去了!$");
						    OSTimeDlyHMSM(0,0,25,0,OS_OPT_TIME_DLY,&err);
							i++; 
							if(i>=2)i = 0;
							OSTaskResume((OS_TCB *)&AppTaskShowBQTCB,&err);  //恢复空闲显示表情任务
						}break;
			case 0x1c:
						{
							Debug_printf("笨蛋\n");
							OSTaskSuspend((OS_TCB *)&AppTaskShowBQTCB,&err);
							

							GUI_SetBkColor(GUI_BLACK);
							GUI_Clear();
							_ShowJPEG2("0:fahuo.jpg",0,0);	
							
							OSTaskResume((OS_TCB *)&AppTaskShowBQTCB,&err);							
						}break;
			case 0x16:
						{
							Debug_printf("画画\n");
							HH_Mode = 1 ;
							OSTaskSuspend((OS_TCB *)&AppTaskShowBQTCB,&err);    //挂起表情显示
							OSTimeDlyHMSM(0, 0, 0,100,OS_OPT_TIME_DLY,&err);
							
							OSTaskSuspend((OS_TCB *)&AppTaskTouchTCB,&err);     //挂起触摸
							OSTimeDlyHMSM(0, 0, 0,100,OS_OPT_TIME_DLY,&err);
							
							OSTaskResume((OS_TCB *)&AppTaskTouchScanTCB,&err);  
							OSTimeDlyHMSM(0, 0, 0,100,OS_OPT_TIME_DLY,&err);
				
							
//							GUI_SelectLayer(1);
							GUI_SetBkColor(GUI_BLACK);
							GUI_Clear();
							
							GUI_SetFont(&GUI_Font24_ASCII);
							GUI_SetColor(GUI_RED);
							GUI_DispStringHCenterAt("RST",456,10);
							
							OSTimeDlyHMSM(0, 0, 0,100,OS_OPT_TIME_DLY,&err);
							
							printf("@TextToSpeech#开始画吧，请说退出画画退出$");
						}break;
			case 0x17:
						{
							Debug_printf("退出画画\n");
							if(HH_Mode == 1)
							{
								OSTaskResume((OS_TCB *)&AppTaskShowBQTCB,&err);
								OSTimeDlyHMSM(0, 0, 0,100,OS_OPT_TIME_DLY,&err);
							
								OSTaskSuspend((OS_TCB *)&AppTaskTouchScanTCB,&err);  //挂起任务
								OSTimeDlyHMSM(0, 0, 0,100,OS_OPT_TIME_DLY,&err);
							
								OSTaskResume((OS_TCB *)&AppTaskTouchTCB,&err);
								OSTimeDlyHMSM(0, 0, 0,100,OS_OPT_TIME_DLY,&err);
							
								printf("@TextToSpeech#你画的真难看$");
								HH_Mode = 0;
								
							}
						}break;
			case 0x18:
						{
							Debug_printf("清屏\n");
							
							if(HH_Mode == 1)
							{
//								GUI_SelectLayer(1);
								GUI_SetBkColor(GUI_BLACK);
								GUI_Clear();
								
								GUI_SetFont(&GUI_Font24_ASCII);
								GUI_SetColor(GUI_RED);
								GUI_DispStringHCenterAt("RST",456,10);
								
								OSTimeDlyHMSM(0, 0, 0,50,OS_OPT_TIME_DLY,&err);
								printf("@TextToSpeech#好了你可以重新开始画了$");
							}
						}break;
			case 0x19:
						{
							Debug_printf("开灯\n");
							Led_Mode = 1;
							
							Usart_SendByte(USART1,'D');
							Usart_Senddec(USART1,Led_Mode);
							Usart_SendByte(USART1,'\r');
							
							OSTimeDlyHMSM(0, 0, 0,50,OS_OPT_TIME_DLY,&err);
							printf("@TextToSpeech#已开灯$");
							
						}break;
			case 0x1A:
						{
							Debug_printf("关灯\n");
							Led_Mode = 2;
							
							Usart_SendByte(USART1,'D');
							Usart_Senddec(USART1,Led_Mode);
							Usart_SendByte(USART1,'\r');
							
							OSTimeDlyHMSM(0, 0, 0,50,OS_OPT_TIME_DLY,&err);
							printf("@TextToSpeech#已关灯$");
						}break;
			case 0x1B:
						{
				           	OSTaskSuspend((OS_TCB *)&AppTaskShowBQTCB,&err);    //挂起表情显示
							OSTimeDlyHMSM(0, 0, 0,100,OS_OPT_TIME_DLY,&err);
							
							Touch_TimeMode = 0;
							settime_mode = 1;
							OSTaskResume((OS_TCB *)&AppTaskWindowTCB,&err);  //恢复
						}break;
			
				
			
		}

		
		Voice_Mode = 0;
		
		/* 退还内存块 */
		OSMemPut ((OS_MEM  *)&mem,                                 //指向内存管理对象
							(void    *)pMsg,                                 //内存块的首地址
							(OS_ERR  *)&err);		                             //返回错误类型

		
    }
		
		
}


/*
*********************************************************************************************************
*                                          温度 TASK
*********************************************************************************************************
*/

static  void  AppTaskLed3 ( void * p_arg )
{
    OS_ERR      err;
	
	u16 tem,hum;
	char head[3] = {'T','H','\r'};
   (void)p_arg;


    while (DEF_TRUE) {                                          /* Task body, always written as an infinite loop.       */
		if(DHT22_Read_Data(&tem,&hum) == 0)
								{
									tem = tem/10;
									hum = hum/10;
									
									Usart_SendByte(USART1,head[0]);
									Usart_Senddec(USART1,tem);
									Usart_SendByte(USART1,head[1]);
									Usart_Senddec(USART1,hum);
									Usart_SendByte(USART1,head[2]);
									Debug_printf("hum = %d\ntem= %d\n",hum,tem);
									
								}
			OSTimeDlyHMSM(0, 0, 10,0,OS_OPT_TIME_DLY,&err);
    }
		
		
}

/*
*********************************************************************************************************
*                                          电量检测 TASK
*********************************************************************************************************
*/
static void  AppTaskPower  ( void * p_arg )
{
	OS_ERR err;
	 	 
    float ADC_Vol; // 用于保存转换计算后的电压值
	
	(void)p_arg;
	while (DEF_TRUE){
      	    
			ADC_Vol =(float)ADC_ConvertedValue[0]/4096*(float)13.3; // 读取转换的AD值
	        Debug_printf("\r\n The current AD value = %f V \r\n",ADC_Vol); 
			if (ADC_Vol <= 10&&Voice_Mode == 0)
			{
				OSTaskSuspend((OS_TCB *)&AppTaskShowBQTCB,&err);
				_ShowJPEG2("0:meidian.jpg",0,0);
				printf("@TextToSpeech#快没电了，要给我充电咯！$");
				OSTimeDlyHMSM(0, 0, 3,0,OS_OPT_TIME_DLY,&err);
				OSTaskResume((OS_TCB *)&AppTaskShowBQTCB,&err);
			}
		    OSTimeDlyHMSM(0,5,0,0,OS_OPT_TIME_DLY,&err); //每隔5分钟读取一次电压值
		    
	}
}



/*********************************************************************************************************
*                                          光照检测 TASK
*********************************************************************************************************
*/
static void  AppTaskLight  ( void * p_arg )
{
	OS_ERR err;
	 	 
    float Light_ADC_Vol;                     // 用于保存转换计算后的电压值，电压值越小--->越暗
	
	u8 Light_Mode = 0;
	
	(void)p_arg;
	while (DEF_TRUE){
      	    
			Light_ADC_Vol =(float)ADC_ConvertedValue[1]/4096*(float)3.3;    // 读取转换的AD值
		
		    Debug_printf("\r\n The current Light_ADC_Vol value = %f V \r\n",Light_ADC_Vol); 
		 
			if(Light_ADC_Vol > 2.7&&Light_Mode == 0)
			{
				OSTaskSuspend((OS_TCB *)&AppTaskShowBQTCB,&err);
				OSTaskSuspend((OS_TCB *)&AppTasktalkTCB,&err);
				OSTaskSuspend((OS_TCB *)&AppTaskPowerTCB,&err);
				
				printf("@TextToSpeech#天怎么突然黑了$");
				OSTimeDlyHMSM(0,0,10,0,OS_OPT_TIME_DLY,&err); //每隔一段时间读取一次电压值
				
				OSTaskResume((OS_TCB *)&AppTaskShowBQTCB,&err);  //恢复空闲显示表情任务
				OSTaskResume((OS_TCB *)&AppTasktalkTCB,&err);  //恢复对话表情任务
				OSTaskSuspend((OS_TCB *)&AppTaskPowerTCB,&err);
				
				Light_Mode = 1;                                //亮度变化只提醒一遍，状态改变后再提醒
			}
			if (Light_ADC_Vol < 2.7&&Light_Mode == 1)
			{
				Light_Mode = 0;
			}
			
	        
		    OSTimeDlyHMSM(0,0,2,0,OS_OPT_TIME_DLY,&err); //每隔一段时间读取一次电压值
		    
	}
}

/*
*********************************************************************************************************
*                                          人检测 TASK
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
				OSTaskSuspend((OS_TCB *)&AppTasktalkTCB,&err);
				Mode = 1;
				if(Last_Mode == 0)
				{
					printf("@TextToSpeech#你好，有什么我可以帮你的吗？$");
				    OSTimeDlyHMSM(0,0,5,0,OS_OPT_TIME_DLY,&err); 
				}
		        Last_Mode = ~Last_Mode;
				Debug_printf("on\n");
		    }
			if(Mode == 1)
			{
				OSTaskResume((OS_TCB *)&AppTaskShowBQTCB,&err);  //恢复空闲显示表情任务
				OSTaskResume((OS_TCB *)&AppTasktalkTCB,&err);  //恢复对话表情任务
				Mode = 0 ;
				OSTimeDlyHMSM(0,0,1,0,OS_OPT_TIME_DLY,&err); //检测到人后一段时间内不再检测 延时
			}
		    
	}
	
}

/*
*********************************************************************************************************
*                                          画板 TASK
*********************************************************************************************************
*/

static void  AppTaskTouchScan (void *p_arg)
{
	OS_ERR err;
		u8 t=0;
	u8 i=0;	  	   
 	u16 lastpos[5][2];		//最后一次的数据
 
	
	(void)p_arg;
	while(DEF_TRUE)
	{
			tp_dev.scan(0);
		for(t=0;t<5;t++)
		{
			if((tp_dev.sta)&(1<<t))
			{
//                Debug_printf("X坐标:%d,Y坐标:%d\r\n",tp_dev.x[0],tp_dev.y[0]);
				if(tp_dev.x[t]<LCD_PIXEL_WIDTH&&tp_dev.y[t]<LCD_PIXEL_HEIGHT)
				{
					if(lastpos[t][0]==0XFFFF)
					{
						lastpos[t][0] = tp_dev.x[t];
						lastpos[t][1] = tp_dev.y[t];
					}
//					GUI_SelectLayer(1);
                    GUI_SetColor(GUI_BLUE);
					GUI_DrawLine(lastpos[t][0],lastpos[t][1],tp_dev.x[t],tp_dev.y[t]);//画线
					lastpos[t][0]=tp_dev.x[t];
					lastpos[t][1]=tp_dev.y[t];
					if(tp_dev.x[t]>(LCD_PIXEL_WIDTH-24)&&tp_dev.y[t]<40)
					{
//					  GUI_SelectLayer(1);
					  GUI_SetBkColor(GUI_BLACK);
					  GUI_Clear();
					  GUI_SetFont(&GUI_Font24_ASCII);
					  GUI_SetColor(GUI_RED);
					  GUI_DispStringHCenterAt("RST",456,10);
					}
				}
			}else lastpos[t][0]=0XFFFF;
		
		
		 OSTimeDlyHMSM(0,0,0,5,OS_OPT_TIME_DLY,&err);i++;
	}
		
	}	
}


/*
*********************************************************************************************************
*                                          摸摸 TASK
*********************************************************************************************************
*/

static void  AppTaskTouch (void *p_arg)
{
	OS_ERR err;
     u8 touch_time = 0;
	 u8 do_Mode = 1;
	(void)p_arg;
	while(DEF_TRUE)
	{
        if(Touch_TimeMode)
		{
			if(tp_dev.scan(0)==1)
			{
				Debug_printf("touch\n");
				touch_time ++;
	
			}
		    if(touch_time >=8&&do_Mode == 1)
			{
				printf("@TextToSpeech#好痒啊,别摸我了$");
				do_Mode = 0;
			}
			if(touch_time >=16&&do_Mode == 0)
			{
				printf("@TextToSpeech#再来，我要咬你了$");
				
				OSTaskSuspend((OS_TCB *)&AppTaskShowBQTCB,&err);
				OSTaskSuspend((OS_TCB *)&AppTasktalkTCB,&err);
				
//				GUI_SelectLayer(1);
				GUI_SetBkColor(GUI_BLACK);
				GUI_Clear();
				OSTimeDly ( 50, OS_OPT_TIME_DLY, & err );
				_ShowGIF2("0:yaoren.gif",1,2,3);
				touch_time =0;
				do_Mode = 1;
				
				OSTaskResume((OS_TCB *)&AppTaskShowBQTCB,&err);  //恢复空闲显示表情任务
				OSTaskResume((OS_TCB *)&AppTasktalkTCB,&err);  //恢复对话表情任务
			}
			
			OSTimeDlyHMSM(0,0,0,480,OS_OPT_TIME_DLY,&err);
		}
		else
        {
			GUI_TOUCH_Exec();                             // 刷新坐标
			OSTimeDlyHMSM(0,0,0,10,OS_OPT_TIME_DLY,&err);
		}
	}
			
}



/*
*********************************************************************************************************
*                                         窗口 TASK
*********************************************************************************************************
*/

static void  AppTaskWindow (void *p_arg)
{
	OS_ERR err;
	(void)p_arg;
	while(DEF_TRUE)
	{
           Debug_printf("hello\n");
			MainTask();
		    OSTimeDlyHMSM(0,0,0,450,OS_OPT_TIME_DLY,&err);
			
    }
}
/*
*********************************************************************************************************
*                                          检测任务堆栈使用量 TASK  
*********************************************************************************************************
*/
#if SystemData == 1
static void  SystemDatasBroadcast (void *p_arg)
{
	OS_ERR err;
	OS_TCB      *p_tcb;	        /* 定义一个任务控制块指针, TCB = TASK CONTROL BLOCK */
	float CPU = 0.0f;
    (void)p_arg;
	
	while(DEF_TRUE){
		
    CPU_SR_ALLOC();  //这条必须有，其展开内容同ucos-ii中
	CPU_CRITICAL_ENTER();
    p_tcb = OSTaskDbgListPtr;
    CPU_CRITICAL_EXIT();
	
	/* 打印标题 */
	Debug_printf("===============================================================\r\n");
	Debug_printf(" 优先级 使用栈 剩余栈 百分比 利用率   任务名\r\n");
	Debug_printf("  Prio   Used  Free   Per    CPU     Taskname\r\n");

	/* 遍历任务控制块列表(TCB list)，打印所有的任务的优先级和名称 */
	while (p_tcb != (OS_TCB *)0) 
		{
			CPU = (float)p_tcb->CPUUsage / 100;
	Debug_printf("   %d      %d    %d    %d%%   %f%%    %s\r\n", 
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
#endif



