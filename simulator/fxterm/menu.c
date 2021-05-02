#include "platform.h"
#include "fonts.h"
#include "vtparse.h"
#include "keyboard.hpp"
#include "menu.h"
#include "console.h"

#define MENU_LIST_INCREMENT 8


MENU_STATUS MenuInit(MenuContext *pContext,int iMenuItemCountHint)
{
	pContext->mppList = NULL;	
	pContext->miMenuItems = 0;
	pContext->miListCapa  = iMenuItemCountHint;
	if(iMenuItemCountHint)
	{
		pContext->mppList = (MenuItem**)malloc(iMenuItemCountHint * sizeof(void *));
		if(pContext->mppList == 0)
		{
			return MENU_STATUS_MEM;
		}
	}
	return MENU_STATUS_OK;
}

static MENU_STATUS MenuAddItem(MenuContext *pContext)
{
    MenuItem *pItem;
	if(pContext->miMenuItems == pContext->miListCapa)
	{
		void *p = realloc((void*)(pContext->mppList),(pContext->miListCapa + MENU_LIST_INCREMENT) * sizeof(void *));
	    if(p == 0)
		{
			return MENU_STATUS_MEM;
		}
		pContext->mppList = (MenuItem**) p;
		pContext->miListCapa += MENU_LIST_INCREMENT;
	}

	pItem = (MenuItem*) malloc(sizeof(MenuItem));
    if(pItem == 0)
	{
		return MENU_STATUS_MEM;
	}
	pContext->mppList[pContext->miMenuItems] = pItem;
	return MENU_STATUS_OK;
}

MENU_STATUS MenuAddExe(MenuContext *pContext,const char *pszName,void *pCtx)
{
	MENU_STATUS status = MenuAddItem(pContext);
	MenuItem *pItem;
	if(status)
	{
		return status;
	}
	pItem = pContext->mppList[pContext->miMenuItems];
	pItem->mpszName = pszName;
	pItem->miType = MENU_TYPE_EXE;
	pItem->mData.mExe.mpCtx = pCtx;
	pContext->miMenuItems++;
	return MENU_STATUS_OK;
}

MENU_STATUS MenuAddCheckBox(MenuContext *pContext,const char *pszName,char *pcChecked)
{
	MENU_STATUS status = MenuAddItem(pContext);
	MenuItem *pItem;
	if(status)
	{
		return status;
	}
	pItem = pContext->mppList[pContext->miMenuItems];
	pItem->mpszName = pszName;
	pItem->miType = MENU_TYPE_CHECK_BOX;
	pItem->mData.mCheckBox.mpcChecked = pcChecked;
	pContext->miMenuItems++;
	return MENU_STATUS_OK;
}

MENU_STATUS MenuAddValues(MenuContext *pContext,const char *pszName,const char *apszValues[],unsigned char *pcSelected)
{
	MENU_STATUS status = MenuAddItem(pContext);
	MenuItem *pItem;
	unsigned char i;
	if(status)
	{
		return status;
	}
	pItem = pContext->mppList[pContext->miMenuItems];
	pItem->mpszName = pszName;
	pItem->miType = MENU_TYPE_VALUES;
	pItem->mData.mValues.mppszValues = apszValues;
	pItem->mData.mValues.mpcSelectedIndex = pcSelected;
	pContext->miMenuItems++;
	for(i=1;i<*pcSelected;i++)
	{
		if(apszValues[i] == NULL)
		{
			*pcSelected = i-1;
		}
	}
	return MENU_STATUS_OK;
}

MENU_STATUS MenuTerm(MenuContext *pContext)
{
	int i;
	for(i = 0;i < pContext->miMenuItems;i++)
	{
		free(pContext->mppList[i]);
	}
	free(pContext->mppList);
	return MENU_STATUS_OK;
}



MENU_STATUS MenuRun(MenuContext *pContext,char *pszTitle,void **ppContext)
{
	int iSelected = 0;
	for(;;)
	{
		int i;
		char key;
		MenuItem *pCurentItem;
		InitScreen(&gFont8x8);
		EnableCursor(0);

		putsFxterm(pszTitle);
		putcharFxterm('\n');
		for(i=0;i<pContext->miMenuItems;i++)
		{
			pCurentItem = pContext->mppList[i];
			putcharFxterm('\n');
			if(i == iSelected)
			{
				putsFxterm("\33[7m");
			}
			putsFxterm(pCurentItem->mpszName);
			putcharFxterm(' ');
			switch(pCurentItem->miType)
			{
			case MENU_TYPE_EXE:break;
			case MENU_TYPE_CHECK_BOX:
				putcharFxterm('[');
				putcharFxterm(pCurentItem->mData.mCheckBox.mpcChecked[0]?'X':' ');
				putcharFxterm(']');
				break;
			case MENU_TYPE_VALUES:
				putcharFxterm(':');
				putsFxterm(pCurentItem->mData.mValues.mppszValues[pCurentItem->mData.mValues.mpcSelectedIndex[0]]);
				break;
			}
			if(i == iSelected)
			{
				putsFxterm("\33[27m");
			}
		}
		pCurentItem = pContext->mppList[iSelected];
		while((key = TranslateChar(PRGM_GetKey())) == 0);
		switch(key)
		{
		case CK_UP:
			if(!iSelected)
			{
				iSelected = pContext->miMenuItems;
			}
			iSelected--;
			break;
		case CK_DOWN:
			iSelected++;
			if(iSelected == pContext->miMenuItems)
			{
				iSelected = 0;
			}
			break;
		case CK_ESC:
			ppContext[0] = ID_EXIT;
			goto lbExit;
			break;

		case CK_ENTER:
			switch(pCurentItem->miType)
			{
			case MENU_TYPE_EXE:
				ppContext[0] = pCurentItem->mData.mExe.mpCtx;
				goto lbExit;
				break;
			case MENU_TYPE_CHECK_BOX:
				pCurentItem->mData.mCheckBox.mpcChecked[0] = !pCurentItem->mData.mCheckBox.mpcChecked[0];
				break;
			case MENU_TYPE_VALUES:
				pCurentItem->mData.mValues.mpcSelectedIndex[0]++;
				if(pCurentItem->mData.mValues.mppszValues[pCurentItem->mData.mValues.mpcSelectedIndex[0]] == NULL)
				{
					pCurentItem->mData.mValues.mpcSelectedIndex[0] = 0;
				}
				break;
			}
			break;
		case CK_LEFT:
			switch(pCurentItem->miType)
			{
			case MENU_TYPE_CHECK_BOX:
				pCurentItem->mData.mCheckBox.mpcChecked[0] = !pCurentItem->mData.mCheckBox.mpcChecked[0];
				break;
			case MENU_TYPE_VALUES:
				if(pCurentItem->mData.mValues.mpcSelectedIndex[0])
				{
					pCurentItem->mData.mValues.mpcSelectedIndex[0]--;
				}
				break;
			}
			break;
		case CK_RIGHT:
			switch(pCurentItem->miType)
			{
			case MENU_TYPE_CHECK_BOX:
				pCurentItem->mData.mCheckBox.mpcChecked[0] = !pCurentItem->mData.mCheckBox.mpcChecked[0];
				break;
			case MENU_TYPE_VALUES:
				if(pCurentItem->mData.mValues.mppszValues[pCurentItem->mData.mValues.mpcSelectedIndex[0]+1])
				{
					pCurentItem->mData.mValues.mpcSelectedIndex[0]++;
				}
				break;
			}
			break;
		}
	}
lbExit:
	
	return MENU_STATUS_OK;
}

