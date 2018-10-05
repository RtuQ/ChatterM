#ifndef _WindowDLG_H
#define _WindowDLG_H

#include "includes.h"

typedef struct
{
	int year;
	int month;
	int day;
	int hour;
	int mint;
	int sec;
}Rtime;

WM_HWIN CreateWindow(void);
void MainTask(void);
#endif
