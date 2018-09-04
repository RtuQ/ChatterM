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
FRESULT res_sd;                /* 文件操作结果 */
//UINT fnum;            					  /* 文件成功读写数量 */
//BYTE ReadBuffer[1024]={0};        /* 读缓冲区 */
//BYTE WriteBuffer[] =              /* 写缓冲区*/
//"欢迎使用野火STM32 F429开发板 今天是个好日子，新建文件系统测试文件\r\n";  


OS_MEM  mem;                    //声明内存管理对象
uint8_t ucArray [ 70 ] [ 4 ];   //声明内存分区大小

uint8_t Voice_Mode = 0;


// ADC1换的电压值通过MDA方式传到SRAM
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
	
   //在外部SPI Flash挂载文件系统，文件系统挂载时会对SPI设备初始化
	res_sd = f_mount(&fs,"0:",1);

#if OS_CFG_STAT_TASK_EN > 0u
    OSStatTaskCPUUsageInit(&err);                               /* Compute CPU capacity with no task running            */
#endif

#ifdef CPU_CFG_INT_DIS_MEAS_EN
    CPU_IntDisMeasMaxCurReset();
#endif
	
	/* 创建内存管理对象 mem */
		OSMemCreate ((OS_MEM      *)&mem,             //指向内存管理对象
								 (CPU_CHAR    *)"Mem For Test",   //命名内存管理对象
								 (void        *)ucArray,          //内存分区的首地址
								 (OS_MEM_QTY   )70,               //内存分区中内存块数目
								 (OS_MEM_SIZE  )4,                //内存块的字节数目
								 (OS_ERR      *)&err);            //返回错误类型
								 
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
*                                          表情显示 TASK
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
*                                          系统运行指示 TASK
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
		
		
		
		/* 阻塞任务，等待任务消息 */
		pMsg = (u16 *)OSTaskQPend ((OS_TICK        )0,                    //无期限等待
											  (OS_OPT         )OS_OPT_PEND_BLOCKING, //没有消息就阻塞任务
											  (OS_MSG_SIZE   *)&msg_size,            //返回消息长度
											  (CPU_TS        *)0,                    //返回消息被发布的时间戳
											  (OS_ERR        *)&err);                //返回错误类型

	

     	print ( "ReceiveData = %d\n", * pMsg );                                   //打印消息内容
		Voice_Mode = 1;
		switch(* pMsg)
		{
			case 0x01:{
						print("你好\n");
						OSTimeDly(2000,OS_OPT_TIME_DLY,&err);
						printf("@TextToSpeech#有什么我能帮你的$");
				
			          }break;
			case 0x02:
                      {
					    print("名字\n");
					  }break;
			case 0x04:
						{
							print("性别\n");
							OSTimeDly(1000,OS_OPT_TIME_DLY,&err);
						}break;
						
	        case 0x06:
						{
							print("温度\n");
							if(DHT22_Read_Data(&tem,&hu) == 0)
								{
									printf("@TextToSpeech#现在的温度是[n2]%d，湿度是百分之[n2]%d$",tem/10,hu/10);
									OSTaskSuspend((OS_TCB *)&AppTaskShowBQTCB,&err);
									_ShowJPEG2("0:xiao.jpg",0,0);
									if(tem >=28)
									{
										printf("@TextToSpeech#这真的太热了！$");
										_ShowJPEG2("0:ku.jpg",0,0);
									}
									OSTimeDly(20000,OS_OPT_TIME_DLY,&err);
								}
							else printf("@TextToSpeech#啊呀我的温度传感器坏了$");
						}break;
			case 0x0f:
						{
							print("生气\n");
							OSTaskSuspend((OS_TCB *)&AppTaskShowBQTCB,&err);  //挂起显示表情任务
							_ShowJPEG2("0:fahuo.jpg",0,0);
						  
						}break;
			case 0x0c:
			            {
							print("笑话\n");
							OSTaskSuspend((OS_TCB *)&AppTaskShowBQTCB,&err);
									_ShowJPEG2("0:hanxiao.jpg",0,0);
							if(i==0)printf("@TextToSpeech#有一学渣，暑假作业全部做完，在最后一页上面写了一句话:我的作业有17道错题，74个错别字，麻烦老师认真的逐一找出来，请不要用一个“阅”，换我们一个月！$");
						    if(i==1)printf("@TextToSpeech#乌龟受伤.让蜗牛去买药。过了2个小时.蜗牛还没回来。乌龟急了骂道:再不回来我就死了!这时门外传来了蜗牛的声音:你再说我不去了!$");
						    OSTimeDlyHMSM(0,0,25,0,OS_OPT_TIME_DLY,&err);
							i++; 
							if(i>=2)i = 0;
							
						}break;
			case 0x14:
						{
							print("笨蛋\n");
							OSTaskSuspend((OS_TCB *)&AppTaskShowBQTCB,&err);
							
							GUI_SelectLayer(1);
							GUI_SetBkColor(GUI_BLACK);
							GUI_Clear();
//							WM_MULTIBUF_Enable(1);
							_ShowGIF2("0:renzhu.gif",2,1);
													
						}break;
				      
				
			
		}

         OSTaskResume((OS_TCB *)&AppTaskShowBQTCB,&err);  //恢复空闲显示表情任务
		
		Voice_Mode = 0;
		
		/* 退还内存块 */
		OSMemPut ((OS_MEM  *)&mem,                                 //指向内存管理对象
							(void    *)pMsg,                                 //内存块的首地址
							(OS_ERR  *)&err);		                             //返回错误类型

		
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
*                                          电量检测 TASK
*********************************************************************************************************
*/
static void  AppTaskPower  ( void * p_arg )
{
	OS_ERR err;
	 	 
    float ADC_Vol; // 用于保存转换计算后的电压值
	
	(void)p_arg;
	while (DEF_TRUE){
      	    
			ADC_Vol =(float) ADC_ConvertedValue/4096*(float)13.3; // 读取转换的AD值
	        print("\r\n The current AD value = %f V \r\n",ADC_Vol); 
			if (ADC_Vol <= 10&&Voice_Mode == 0)
			{
				_ShowJPEG2("0:meidian.jpg",0,0);
				printf("@TextToSpeech#快没电了，要给我换电池咯！$");
			}
		    OSTimeDlyHMSM(0,5,0,0,OS_OPT_TIME_DLY,&err); //每隔5分钟检测一次电压值
		    
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
				OSTaskSuspend((OS_TCB *)&AppTasktalk,&err);
				Mode = 1;
				if(Last_Mode == 0)
				{
					printf("@TextToSpeech#你好，有什么我可以帮你的吗？$");
				    OSTimeDlyHMSM(0,0,5,0,OS_OPT_TIME_DLY,&err); 
				}
		        Last_Mode = ~Last_Mode;
				print("on\n");
		    }
			if(Mode == 1)
			{
				OSTaskResume((OS_TCB *)&AppTaskShowBQTCB,&err);  //恢复空闲显示表情任务
				OSTaskResume((OS_TCB *)&AppTasktalk,&err);  //恢复对话表情任务
				Mode = 0 ;
			}
		    OSTimeDlyHMSM(0,0,0,250,OS_OPT_TIME_DLY,&err); //每隔250ms检测一次
	}
	
}
/*
*********************************************************************************************************
*                                          检测任务堆栈使用量 TASK  
*********************************************************************************************************
*/

static void  SystemDatasBroadcast (void *p_arg)
{
	OS_ERR err;
//  CPU_STK_SIZE free,used;
//  (void)p_arg;
//  while(DEF_TRUE)
//  {
//	OSTaskStkChk (&SystemDatasBroadcast_TCB,&free,&used,&err);//  把统计任务本身的堆栈使用量也打印出来
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


	OS_TCB      *p_tcb;	        /* 定义一个任务控制块指针, TCB = TASK CONTROL BLOCK */
	float CPU = 0.0f;
    (void)p_arg;
	
	while(DEF_TRUE){
		
    CPU_SR_ALLOC();  //这条必须有，其展开内容同ucos-ii中
	CPU_CRITICAL_ENTER();
    p_tcb = OSTaskDbgListPtr;
    CPU_CRITICAL_EXIT();
	
	/* 打印标题 */
	print("===============================================================\r\n");
	print(" 优先级 使用栈 剩余栈 百分比 利用率   任务名\r\n");
	print("  Prio   Used  Free   Per    CPU     Taskname\r\n");

	/* 遍历任务控制块列表(TCB list)，打印所有的任务的优先级和名称 */
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




