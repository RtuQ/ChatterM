/*
*********************************************************************************************************
*                                              EXAMPLE CODE
*
*                             (c) Copyright 2013; Micrium, Inc.; Weston, FL
*
*               All rights reserved.  Protected by international copyright laws.
*               Knowledge of the source code may NOT be used to develop a similar product.
*               Please help us continue to provide the Embedded community with the finest
*               software available.  Your honesty is greatly appreciated.
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*
*                                           MASTER INCLUDES
*
*                                       IAR Development Kits
*                                              on the
*
*                                    STM32F429II-SK KICKSTART KIT
*
* Filename      : includes.h
* Version       : V1.00
* Programmer(s) : FT
*********************************************************************************************************
*/

#ifndef  INCLUDES_MODULES_PRESENT
#define  INCLUDES_MODULES_PRESENT


/*
*********************************************************************************************************
*                                         STANDARD LIBRARIES
*********************************************************************************************************
*/


#include  <stdio.h>
#include  <string.h>
#include  <ctype.h>
#include  <stdlib.h>
#include  <stdarg.h>
#include  <math.h>


/*
*********************************************************************************************************
*                                                 OS
*********************************************************************************************************
*/

#include  "os.h"

/*
*********************************************************************************************************
*                                               emWin
*********************************************************************************************************
*/
#include "GUI.h"
#include "DIALOG.h"

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
*                                              APP / BSP
*********************************************************************************************************
*/
#include "app.h"
#include  <app_cfg.h>
#include "show_jpg.h"
#include "show_BMP.h"
#include "show_GIF.h"
#include "print.h"

/*
***********************************************BSP*****************************************************/

#include  <bsp.h>
#include "bsp_lcd.h"
#include "ff.h"
#include "diskio.h"
#include "bsp_debug_usart.h"
#include "bsp_usart3.h"
#include "bsp_DHT22.h"
#include "bsp_usart6.h"
#include "bsp_adc.h"

/*
*********************************************************************************************************
*                                               位操作定义函数
*********************************************************************************************************
*/
// 这里只定义了 GPIO ODR和IDR这两个寄存器的位带别名区地址，其他寄存器的没有定义

//SRAM 位带区:    0X2000 0000~0X200F 0000
//SRAM 位带别名区:0X2200 0000~0X23FF FFFF

//外设 位带区:    0X4000 0000~0X400F FFFF
//外设 位带别名区:0X4200 0000~0X43FF FFFF

// 把“位带地址+位序号”转换成别名地址的宏
#define BITBAND(addr, bitnum) ((addr & 0xF0000000)+0x02000000+((addr & 0x000FFFFF)<<5)+(bitnum<<2)) 

// 把一个地址转换成一个指针
#define MEM_ADDR(addr)  *((volatile unsigned long  *)(addr))

// 把位带别名区地址转换成指针
#define BIT_ADDR(addr, bitnum)   MEM_ADDR(BITBAND(addr, bitnum)) 


// GPIO ODR 和 IDR 寄存器地址映射 
#define GPIOA_ODR_Addr    (GPIOA_BASE+20) 
#define GPIOB_ODR_Addr    (GPIOB_BASE+20)   
#define GPIOC_ODR_Addr    (GPIOC_BASE+20)  
#define GPIOD_ODR_Addr    (GPIOD_BASE+20) 
#define GPIOE_ODR_Addr    (GPIOE_BASE+20) 
#define GPIOF_ODR_Addr    (GPIOF_BASE+20)      
#define GPIOG_ODR_Addr    (GPIOG_BASE+20)
#define GPIOH_ODR_Addr    (GPIOH_BASE+20)      
#define GPIOI_ODR_Addr    (GPIOI_BASE+20)
#define GPIOJ_ODR_Addr    (GPIOJ_BASE+20)      
#define GPIOK_ODR_Addr    (GPIOK_BASE+20)

#define GPIOA_IDR_Addr    (GPIOA_BASE+16)  
#define GPIOB_IDR_Addr    (GPIOB_BASE+16)  
#define GPIOC_IDR_Addr    (GPIOC_BASE+16)   
#define GPIOD_IDR_Addr    (GPIOD_BASE+16)  
#define GPIOE_IDR_Addr    (GPIOE_BASE+16)    
#define GPIOF_IDR_Addr    (GPIOF_BASE+16)    
#define GPIOG_IDR_Addr    (GPIOG_BASE+16)  
#define GPIOH_IDR_Addr    (GPIOH_BASE+16)
#define GPIOI_IDR_Addr    (GPIOI_BASE+16)
#define GPIOJ_IDR_Addr    (GPIOJ_BASE+16)
#define GPIOK_IDR_Addr    (GPIOK_BASE+16)


// 单独操作 GPIO的某一个IO口，n(0,1,2...16),n表示具体是哪一个IO口
#define PAout(n)   BIT_ADDR(GPIOA_ODR_Addr,n)  //输出   
#define PAin(n)    BIT_ADDR(GPIOA_IDR_Addr,n)  //输入   
  
#define PBout(n)   BIT_ADDR(GPIOB_ODR_Addr,n)  //输出   
#define PBin(n)    BIT_ADDR(GPIOB_IDR_Addr,n)  //输入   
  
#define PCout(n)   BIT_ADDR(GPIOC_ODR_Addr,n)  //输出   
#define PCin(n)    BIT_ADDR(GPIOC_IDR_Addr,n)  //输入   
  
#define PDout(n)   BIT_ADDR(GPIOD_ODR_Addr,n)  //输出   
#define PDin(n)    BIT_ADDR(GPIOD_IDR_Addr,n)  //输入   
  
#define PEout(n)   BIT_ADDR(GPIOE_ODR_Addr,n)  //输出   
#define PEin(n)    BIT_ADDR(GPIOE_IDR_Addr,n)  //输入  
  
#define PFout(n)   BIT_ADDR(GPIOF_ODR_Addr,n)  //输出   
#define PFin(n)    BIT_ADDR(GPIOF_IDR_Addr,n)  //输入  
  
#define PGout(n)   BIT_ADDR(GPIOG_ODR_Addr,n)  //输出   
#define PGin(n)    BIT_ADDR(GPIOG_IDR_Addr,n)  //输入  

#define PHout(n)   BIT_ADDR(GPIOH_ODR_Addr,n)  //输出   
#define PHin(n)    BIT_ADDR(GPIOH_IDR_Addr,n)  //输入  

#define PIout(n)   BIT_ADDR(GPIOI_ODR_Addr,n)  //输出   
#define PIin(n)    BIT_ADDR(GPIOI_IDR_Addr,n)  //输入 

#define PJout(n)   BIT_ADDR(GPIOJ_ODR_Addr,n)  //输出   
#define PJin(n)    BIT_ADDR(GPIOJ_IDR_Addr,n)  //输入  

#define PKout(n)   BIT_ADDR(GPIOK_ODR_Addr,n)  //输出   
#define PKin(n)    BIT_ADDR(GPIOK_IDR_Addr,n)  //输入  

/*
*********************************************************************************************************
*                                               SERIAL
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                            INCLUDES END
*********************************************************************************************************
*/


#endif
