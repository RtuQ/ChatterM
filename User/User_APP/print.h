#ifndef _print_H
#define _print_H  
#include "includes.h"



#define print_USART USART6
#define console_print   Usart_SendByte


void    print(char* fmt, ...);

#endif

