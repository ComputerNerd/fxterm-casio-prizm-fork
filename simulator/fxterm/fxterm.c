#include "platform.h"
#include "console.h"
#include "menu.h"
#include "7305.h"
//stty rows 27 columns 64

static unsigned char gCommSettings[5]={0,5,0,0,0};
static const char *gCommSpeedNames[]={"300","600","1200","2400","4800","9600","19200","38400","57600","115200","230400","460800",NULL};
static const char *gCommParityNames[]={"none","odd","even",NULL};
static const char *gCommDataLenNames[]={"8 bit","7 bit",NULL};
static const char *gCommStopBitsNames[]={"1","2",NULL};



static const char *gFontNames[]={"4x6","5x6","6x8","8x8",NULL};
static const FONT *gFonts[]={&gFont4x6,&gFont5x6,&gFont6x8,&gFont8x8};
static unsigned char gcFontIndex = 2;//gFont6x8
static char gcLocalEcho = 0;
static char gcCursorAlwaysVisible = 0;


#define ID_ABOUT (MenuID)1
#define ID_QUIT  (MenuID)2
static MENU_STATUS CreateMenu(MenuContext *pContext)
{
	MENU_STATUS status = MenuInit(pContext,10);
	if(status)
	{
		goto lbExit;
	}
	status = MenuAddValues(pContext,"Comm speed",gCommSpeedNames,gCommSettings+1);
	if(status)
	{
		goto lbExit;
	}
	status = MenuAddValues(pContext,"Comm parity",gCommParityNames,gCommSettings+2);
	if(status)
	{
		goto lbExit;
	}
	status = MenuAddValues(pContext,"Comm data length",gCommDataLenNames,gCommSettings+3);
	if(status)
	{
		goto lbExit;
	}
	status = MenuAddValues(pContext,"Comm stop bits",gCommStopBitsNames,gCommSettings+4);
	if(status)
	{
		goto lbExit;
	}

	status = MenuAddValues(pContext,"Screen font",gFontNames,&gcFontIndex);
	if(status)
	{
		goto lbExit;
	}

	status = MenuAddCheckBox(pContext,"Local echo",&gcLocalEcho);
	if(status)
	{
		goto lbExit;
	}

	status = MenuAddCheckBox(pContext,"Cursor always visible",&gcCursorAlwaysVisible);
	if(status)
	{
		goto lbExit;
	}

	status = MenuAddExe(pContext,"Use setting",ID_EXIT);
	if(status)
	{
		goto lbExit;
	}

	status = MenuAddExe(pContext,"About",ID_ABOUT);
	if(status)
	{
		goto lbExit;
	}

	status = MenuAddExe(pContext,"Quit",ID_QUIT);
	if(status)
	{
		goto lbExit;
	}

lbExit:
	return status;
}



//23456789A123456789B123456789C123456789D123456789E123456789F1234
static const char *pszWelcomeText="\33[1;37m"
"FXTerm - VT 100 compatible terminal emulator\n"
"Version 0.02 alpha\n"
"Martin Poupe (mpoupe@gmail.com, http://martin.poupe.org/casio)\n"
"\n\n"
"Keys:\n"
"[Menu] - exit from terminal to menu\n"
"[Alpha] - use letters\n"
"[Shift] - case for letters, yellow symbols otherwise\n"
"[EXIT] - Esc\n"
"[OPTN] - Ctrl + letter\n"
"[DEL] - Back space\n"
"[AC] - ^C (Control-C)";


static void Message(const char *pszMessage)
{
	InitScreen(&gFont6x8);
	puts(pszMessage);
	puts("\n\nPress EXE to continue");
	while(TranslateChar(PRGM_GetKey()) != CK_ENTER);
}


#define REC_BUF_SIZE 16384

static void Terminal()
{
	unsigned char aRecBuffer[REC_BUF_SIZE];
	unsigned hsMode=0;
	if(gCommSettings[1]>=10){
		hsMode=gCommSettings[1];
		gCommSettings[1]=9;
	}
	if(Serial_Open(gCommSettings))
	{
		Message("\33[1;31mError while opening COM port\nCheck parameters");
		return;
	}
	if(hsMode)
		SCIF0.SCBRR = hsMode==10?1:0;
	InitScreen(gFonts[gcFontIndex]);
	for(;;)
	{
		unsigned char c = PRGM_GetKey();
		if(c == KEY_PRGM_MENU)
		{
			break;
		}

		c = TranslateChar(c);
		if(c)
		{
			if(c=='\n'){
				Serial_WriteSingle('\r');
				Serial_WriteSingle('\n');
			}else
				Serial_WriteSingle(c);
			if(gcLocalEcho)
			{
				putchar(c);//local echo
			}
			dbgprint1("<%c>",c);
		}

		if(0 == Serial_ReadSingle(aRecBuffer))
		{
			int iRead = 1;
			int iWrote = 0;
			while(iWrote < iRead)
			{
				while((iRead < REC_BUF_SIZE) && (0 == Serial_ReadSingle(aRecBuffer+iRead)))iRead++;
				putchar(aRecBuffer[iWrote]);iWrote++;
			}
		}
	}
	Serial_Close(1);
}

void main( void )
{
	MenuContext aMenuContext = {0};
	EnableColor(1);
	if(CreateMenu(&aMenuContext))
	{
		Message("\33[1;31mError while creating menu");
		return;
	}
	for(;;)
	{
		MenuID id;
		if(MenuRun(&aMenuContext,"FXterm main menu",&id))
		{
			Message("\33[1;31mError while running menu");
			return;
		}
		if(id == ID_EXIT)
		{
			Terminal();
		}
		else if(id == ID_ABOUT)
		{
			Message(pszWelcomeText);
		}
		else if(id == ID_QUIT)
		{
			break;
		}
	}
	MenuTerm(&aMenuContext);
}
