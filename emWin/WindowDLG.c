/*********************************************************************
*                                                                    *
*                SEGGER Microcontroller GmbH & Co. KG                *
*        Solutions for real time microcontroller applications        *
*                                                                    *
**********************************************************************
*                                                                    *
* C-file generated by:                                               *
*                                                                    *
*        GUI_Builder for emWin version 5.26                          *
*        Compiled Aug 18 2014, 17:12:05                              *
*        (c) 2014 Segger Microcontroller GmbH & Co. KG               *
*                                                                    *
**********************************************************************
*                                                                    *
*        Internet: www.segger.com  Support: support@segger.com       *
*                                                                    *
**********************************************************************
*/

// USER START (Optionally insert additional includes)
// USER END

#include "DIALOG.h"
#include "WindowDLG.h"


WM_HWIN hWin2;

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/

extern OS_TCB   AppTaskWindowTCB;
extern OS_TCB   AppTaskShowBQTCB;

extern u8 Touch_TimeMode;

#define ID_WINDOW_0   (GUI_ID_USER + 0x00)
#define ID_DROPDOWN_0   (GUI_ID_USER + 0x01)
#define ID_DROPDOWN_1   (GUI_ID_USER + 0x02)
#define ID_DROPDOWN_2   (GUI_ID_USER + 0x03)
#define ID_DROPDOWN_3   (GUI_ID_USER + 0x04)
#define ID_DROPDOWN_4   (GUI_ID_USER + 0x05)
#define ID_BUTTON_0   (GUI_ID_USER + 0x06)
#define ID_BUTTON_1   (GUI_ID_USER + 0x07)


// USER START (Optionally insert additional defines)
// USER END

/*********************************************************************
*
*       Static data
*
**********************************************************************
*/

// USER START (Optionally insert additional static data)
// USER END

/*********************************************************************
*
*       _aDialogCreate
*/
static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
  { WINDOW_CreateIndirect, "Window", ID_WINDOW_0, 0,0, 480, 272, 0, 0x0, 0 },
  { DROPDOWN_CreateIndirect, "Dropdown", ID_DROPDOWN_0, 112, 68, 85, 19, 0, 0x0, 0 },
  { DROPDOWN_CreateIndirect, "Dropdown", ID_DROPDOWN_1, 222, 68, 60, 19, 0, 0x0, 0 },
  { DROPDOWN_CreateIndirect, "Dropdown", ID_DROPDOWN_2, 307, 68, 60, 19, 0, 0x0, 0 },
  { DROPDOWN_CreateIndirect, "Dropdown", ID_DROPDOWN_3, 152, 140, 60, 19, 0, 0x0, 0 },
  { DROPDOWN_CreateIndirect, "Dropdown", ID_DROPDOWN_4, 268, 140, 60, 19, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "Button", ID_BUTTON_0, 118, 207, 80, 20, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "Button", ID_BUTTON_1, 253, 209, 80, 20, 0, 0x0, 0 },
  // USER START (Optionally insert additional widgets)
  // USER END
};

/*********************************************************************
*
*       Static code
*
**********************************************************************
*/

// USER START (Optionally insert additional static code)
// USER END

/*********************************************************************
*
*       _cbDialog
*/
static void _cbDialog(WM_MESSAGE * pMsg) {
  WM_HWIN hItem;
  int     NCode;
  int     Id;
 OS_ERR      err;
  // USER START (Optionally insert additional variables)
  // USER END

  switch (pMsg->MsgId) {
  case WM_INIT_DIALOG:
    //
    // Initialization of 'Window'
    //
    hItem = pMsg->hWin;
    WINDOW_SetBkColor(hItem, 0x00FF4646);

	hItem = WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_0);
	DROPDOWN_SetFont(hItem, GUI_FONT_24B_ASCII);
	DROPDOWN_AddString(hItem, "2016");
	DROPDOWN_AddString(hItem, "2017");
	DROPDOWN_AddString(hItem, "2018");
	DROPDOWN_AddString(hItem, "2019");
	DROPDOWN_AddString(hItem, "2020");

	/* ÏÂÃæÕâ¸öÈý¸öº¯ÊýÊÇÐÂÔöµÄ */
	DROPDOWN_SetAutoScroll(hItem, 1);
	DROPDOWN_SetListHeight(hItem, 100);
	DROPDOWN_SetScrollbarWidth(hItem, 16);
	//
	// Initialization of 'Dropdown'
	//
	hItem = WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_1);
	DROPDOWN_SetFont(hItem, GUI_FONT_24B_ASCII);
	DROPDOWN_AddString(hItem, "1");
	DROPDOWN_AddString(hItem, "2");
	DROPDOWN_AddString(hItem, "3");
	DROPDOWN_AddString(hItem, "4");
	DROPDOWN_AddString(hItem, "5");
	DROPDOWN_AddString(hItem, "6");
	DROPDOWN_AddString(hItem, "7");
	DROPDOWN_AddString(hItem, "8");
	DROPDOWN_AddString(hItem, "9");
	DROPDOWN_AddString(hItem, "10");
	DROPDOWN_AddString(hItem, "11");
	DROPDOWN_AddString(hItem, "12");

	/* ÏÂÃæÕâ¸öÈý¸öº¯ÊýÊÇÐÂÔöµÄ */
	DROPDOWN_SetAutoScroll(hItem, 1);
	DROPDOWN_SetListHeight(hItem, 100);
	DROPDOWN_SetScrollbarWidth(hItem, 16);

	hItem = WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_2);
	DROPDOWN_SetFont(hItem, GUI_FONT_24B_ASCII);
	DROPDOWN_AddString(hItem, "1");
	DROPDOWN_AddString(hItem, "2");
	DROPDOWN_AddString(hItem, "3");
	DROPDOWN_AddString(hItem, "4");
	DROPDOWN_AddString(hItem, "5");
	DROPDOWN_AddString(hItem, "6");
	DROPDOWN_AddString(hItem, "7");
	DROPDOWN_AddString(hItem, "8");
	DROPDOWN_AddString(hItem, "9");
	DROPDOWN_AddString(hItem, "10");
	DROPDOWN_AddString(hItem, "11");
	DROPDOWN_AddString(hItem, "12");
	DROPDOWN_AddString(hItem, "14");
	DROPDOWN_AddString(hItem, "15");
	DROPDOWN_AddString(hItem, "16");
	DROPDOWN_AddString(hItem, "17");
	DROPDOWN_AddString(hItem, "18");
	DROPDOWN_AddString(hItem, "19");
	DROPDOWN_AddString(hItem, "20");
	DROPDOWN_AddString(hItem, "21");
	DROPDOWN_AddString(hItem, "22");
	DROPDOWN_AddString(hItem, "23");
	DROPDOWN_AddString(hItem, "24");
	DROPDOWN_AddString(hItem, "25");
	DROPDOWN_AddString(hItem, "26");
	DROPDOWN_AddString(hItem, "27");
	DROPDOWN_AddString(hItem, "28");
	DROPDOWN_AddString(hItem, "29");
	DROPDOWN_AddString(hItem, "30");
	DROPDOWN_AddString(hItem, "31");

	/* ÏÂÃæÕâ¸öÈý¸öº¯ÊýÊÇÐÂÔöµÄ */
	DROPDOWN_SetAutoScroll(hItem, 1);
	DROPDOWN_SetListHeight(hItem, 100);
	DROPDOWN_SetScrollbarWidth(hItem, 16);

	hItem = WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_3);
	DROPDOWN_SetFont(hItem, GUI_FONT_24B_ASCII);
	DROPDOWN_AddString(hItem, "1");
	DROPDOWN_AddString(hItem, "2");
	DROPDOWN_AddString(hItem, "3");
	DROPDOWN_AddString(hItem, "4");
	DROPDOWN_AddString(hItem, "5");
	DROPDOWN_AddString(hItem, "6");
	DROPDOWN_AddString(hItem, "7");
	DROPDOWN_AddString(hItem, "8");
	DROPDOWN_AddString(hItem, "9");
	DROPDOWN_AddString(hItem, "10");
	DROPDOWN_AddString(hItem, "11");
	DROPDOWN_AddString(hItem, "12");

	/* ÏÂÃæÕâ¸öÈý¸öº¯ÊýÊÇÐÂÔöµÄ */
	DROPDOWN_SetAutoScroll(hItem, 1);
	DROPDOWN_SetListHeight(hItem, 100);
	DROPDOWN_SetScrollbarWidth(hItem, 16);

	hItem = WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_4);
	DROPDOWN_SetFont(hItem, GUI_FONT_24B_ASCII);
	DROPDOWN_AddString(hItem, "1");
	DROPDOWN_AddString(hItem, "2");
	DROPDOWN_AddString(hItem, "3");
	DROPDOWN_AddString(hItem, "4");
	DROPDOWN_AddString(hItem, "5");
	DROPDOWN_AddString(hItem, "6");
	DROPDOWN_AddString(hItem, "7");
	DROPDOWN_AddString(hItem, "8");
	DROPDOWN_AddString(hItem, "9");
	DROPDOWN_AddString(hItem, "10");
	DROPDOWN_AddString(hItem, "11");
	DROPDOWN_AddString(hItem, "12");
	DROPDOWN_AddString(hItem, "14");
	DROPDOWN_AddString(hItem, "15");
	DROPDOWN_AddString(hItem, "16");
	DROPDOWN_AddString(hItem, "17");
	DROPDOWN_AddString(hItem, "18");
	DROPDOWN_AddString(hItem, "19");
	DROPDOWN_AddString(hItem, "20");
	DROPDOWN_AddString(hItem, "21");
	DROPDOWN_AddString(hItem, "22");
	DROPDOWN_AddString(hItem, "23");
	DROPDOWN_AddString(hItem, "24");
	DROPDOWN_AddString(hItem, "25");
	DROPDOWN_AddString(hItem, "26");
	DROPDOWN_AddString(hItem, "27");
	DROPDOWN_AddString(hItem, "28");
	DROPDOWN_AddString(hItem, "29");
	DROPDOWN_AddString(hItem, "30");
	DROPDOWN_AddString(hItem, "31");
	DROPDOWN_AddString(hItem, "32");
	DROPDOWN_AddString(hItem, "33");
	DROPDOWN_AddString(hItem, "34");
	DROPDOWN_AddString(hItem, "35");
	DROPDOWN_AddString(hItem, "36");
	DROPDOWN_AddString(hItem, "37");
	DROPDOWN_AddString(hItem, "38");
	DROPDOWN_AddString(hItem, "39");
	DROPDOWN_AddString(hItem, "40");
	DROPDOWN_AddString(hItem, "41");
	DROPDOWN_AddString(hItem, "42");
	DROPDOWN_AddString(hItem, "43");
	DROPDOWN_AddString(hItem, "44");
	DROPDOWN_AddString(hItem, "45");
	DROPDOWN_AddString(hItem, "46");
	DROPDOWN_AddString(hItem, "47");
	DROPDOWN_AddString(hItem, "48");
	DROPDOWN_AddString(hItem, "49");
	DROPDOWN_AddString(hItem, "50");
	DROPDOWN_AddString(hItem, "51");
	DROPDOWN_AddString(hItem, "52");
	DROPDOWN_AddString(hItem, "53");
	DROPDOWN_AddString(hItem, "54");
	DROPDOWN_AddString(hItem, "55");
	DROPDOWN_AddString(hItem, "56");
	DROPDOWN_AddString(hItem, "57");
	DROPDOWN_AddString(hItem, "58");
	DROPDOWN_AddString(hItem, "59");

	/* ÏÂÃæÕâ¸öÈý¸öº¯ÊýÊÇÐÂÔöµÄ */
	DROPDOWN_SetAutoScroll(hItem, 1);
	DROPDOWN_SetListHeight(hItem, 100);
	DROPDOWN_SetScrollbarWidth(hItem, 16);


    // USER START (Optionally insert additional code for further widget initialization)
    // USER END
    break;
  case WM_NOTIFY_PARENT:
    Id    = WM_GetId(pMsg->hWinSrc);
    NCode = pMsg->Data.v;
    switch(Id) {
    case ID_DROPDOWN_0: // Notifications sent by 'Dropdown'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_RELEASED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_SEL_CHANGED:
									  {
										  DROPDOWN_GetSel(ID_DROPDOWN_0);
									  }
										break;
      // USER START (Optionally insert additional code for further notification handling)
      // USER END
      }
      break;
    case ID_DROPDOWN_1: // Notifications sent by 'Dropdown'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_RELEASED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_SEL_CHANGED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      // USER START (Optionally insert additional code for further notification handling)
      // USER END
      }
      break;
    case ID_DROPDOWN_2: // Notifications sent by 'Dropdown'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_RELEASED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_SEL_CHANGED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      // USER START (Optionally insert additional code for further notification handling)
      // USER END
      }
      break;
    case ID_DROPDOWN_3: // Notifications sent by 'Dropdown'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_RELEASED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_SEL_CHANGED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      // USER START (Optionally insert additional code for further notification handling)
      // USER END
      }
      break;
    case ID_DROPDOWN_4: // Notifications sent by 'Dropdown'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_RELEASED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_SEL_CHANGED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      // USER START (Optionally insert additional code for further notification handling)
      // USER END
      }
      break;
    case ID_BUTTON_0: // Notifications sent by 'Button'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_RELEASED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      // USER START (Optionally insert additional code for further notification handling)
      // USER END
      }
      break;
    case ID_BUTTON_1: // Notifications sent by 'Button'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_RELEASED:
								  {GUI_EndDialog(hWin2,0);
							//	  OSTaskSuspend((OS_TCB *)&AppTaskWindowTCB,&err);
								   Touch_TimeMode = 1 ;
								  OSTaskResume((OS_TCB *)&AppTaskShowBQTCB,&err);
								  }	break;
      }
      break;
    // USER START (Optionally insert additional code for further Ids)
    // USER END
    }
    break;
  // USER START (Optionally insert additional message handling)
  // USER END
  default:
    WM_DefaultProc(pMsg);
    break;
  }
}

/*********************************************************************
*
*       Public code
*
**********************************************************************
*/
/*********************************************************************
*
*       CreateWindow
*/
WM_HWIN CreateWindow(void);
WM_HWIN CreateWindow(void) {
  WM_HWIN hWin;

  hWin = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, WM_HBKWIN, 0, 0);
  return hWin;
}

// USER START (Optionally insert additional public code)
// USER END
void MainTask(void) {
	//
	// Check if recommended memory for the sample is available
	//
	hWin2 = CreateWindow();
	while (1) {
		Debug_printf("stop?\n");
		GUI_Delay(10);
	}
}

/*************************** End of file ****************************/
