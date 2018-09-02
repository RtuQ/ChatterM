/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   ��1.5.1�汾�⽨�Ĺ���ģ��
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:����  STM32 F429 ������ 
  * ��̳    :http://www.firebbs.cn
  * �Ա�    :https://fire-stm32.taobao.com
  *
  ******************************************************************************
  */
/*
*********************************************************************************************************
*                                            EXAMPLE CODE
*
*               This file is provided as an example on how to use Micrium products.
*
*               Please feel free to use any application code labeled as 'EXAMPLE CODE' in
*               your application products.  Example code may be used as is, in whole or in
*               part, or may be used as a reference only. This file can be modified as
*               required to meet the end-product requirements.
*
*               Please help us continue to provide the Embedded community with the finest
*               software available.  Your honesty is greatly appreciated.
*
*               You can find our product's user manual, API reference, release notes and
*               more information at https://doc.micrium.com.
*               You can contact us at www.micrium.com.
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*
*                                            EXAMPLE CODE
*
*                                     ST Microelectronics STM32
*                                              on the
*
*                                           STM3240G-EVAL
*                                         Evaluation Board
*
* Filename      : includes.h
* Version       : V1.00
* Programmer(s) : DC
*********************************************************************************************************
*/
#ifndef  INCLUDES_PRESENT
#define  INCLUDES_PRESENT

/*
*********************************************************************************************************
*                                         STANDARD LIBRARIES
*********************************************************************************************************
*/

#include  <stdarg.h>
#include  <stdio.h>
#include  <stdlib.h>
#include  <math.h>


/*
*********************************************************************************************************
*                                              LIBRARIES
*********************************************************************************************************
*/

#include  <cpu.h>
#include  <lib_def.h>
#include  <lib_ascii.h>
#include  <lib_math.h>
#include  <lib_mem.h>
#include  <lib_str.h>

/*
*********************************************************************************************************
*                                                 ST
*********************************************************************************************************
*/

#include  "stm32f4xx.h"

/*
*********************************************************************************************************
*                                              APP / BSP
*********************************************************************************************************
*/
#include "bsp.h"
#include "bsp_led.h" 
#include "bsp_debug_usart.h"
#include "bsp_lcd.h"
#include "bsp_sdram.h"
//#include "ff.h"
#include "app.h"
//#include "common.h"
/*
*********************************************************************************************************
*                                                 OS
*********************************************************************************************************
*/

#include  <os.h>	
#include "os_type.h"
#include "os_cfg_app.h"
#include  <os_app_hooks.h>
#include  <app_cfg.h>

/*
*********************************************************************************************************
*                                               emWin
*********************************************************************************************************
*/
#include "GUI.h"
#include "DIALOG.h"

/*
*********************************************************************************************************
*                                              FUNCTION
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                            INCLUDES END
*********************************************************************************************************
*/
#define DISABLE_IRQ()   __set_PRIMASK(1)
#define ENABLE_IRQ()    __set_PRIMASK(0)

#endif

