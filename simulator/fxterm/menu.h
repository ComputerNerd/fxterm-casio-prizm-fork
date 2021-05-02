
#ifndef MENU_H
#define MENU_H


#define MAX_NAME_LENGTH 21

#define MENU_TYPE_EXE		0

typedef void * MenuID;
#define ID_EXIT (MenuID)0
typedef struct 
{
	void *mpCtx;
}MenuContextExe;

#define MENU_TYPE_CHECK_BOX	1
typedef struct 
{
	char *mpcChecked;
}MenuContextCheckBox;
#define MENU_TYPE_VALUES	2


typedef struct 
{
	const char  **mppszValues;//NULL terminated list
	unsigned char *mpcSelectedIndex;
}MenuContextValues;


typedef struct 
{
	const char  *mpszName;
	int miType;
	union
	{
		MenuContextExe mExe;
		MenuContextCheckBox mCheckBox;
		MenuContextValues mValues;
	}mData;
}MenuItem;

typedef struct
{
	MenuItem **mppList;
	int miMenuItems;
	int miListCapa;
}MenuContext;

typedef int MENU_STATUS;
#define MENU_STATUS_OK	0
#define MENU_STATUS_MEM	1
#define MENU_STATUS_ABORT	2

//typedef MENU_STATUS(*MenuInfoCB)(const char *pszName,void *pContext);


MENU_STATUS MenuInit(MenuContext *pContext,int iMenuItemCountHint);
MENU_STATUS MenuAddExe(MenuContext *pContext,const char *pszName,void *pCtx);
MENU_STATUS MenuAddCheckBox(MenuContext *pContext,const char *pszName,char *pcChecked);
MENU_STATUS MenuAddValues(MenuContext *pContext,const char *pszName,const char *apszValues[],unsigned char *pcSelected);
MENU_STATUS MenuRun(MenuContext *pContext,char *pszTitle,void **ppContext);
MENU_STATUS MenuTerm(MenuContext *pContext);



#endif //#ifndef MENU_H
