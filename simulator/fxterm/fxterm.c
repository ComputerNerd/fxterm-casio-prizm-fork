#include <zlib.h>

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
static unsigned char gcFontIndex = 3;//gFont8x8
static char gcLocalEcho = 0;
static char gcCursorAlwaysVisible = 0;
static char gcMemoryDump = 0;


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

	status = MenuAddCheckBox(pContext,"Memory dump mode",&gcMemoryDump);
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
	putsFxterm(pszMessage);
	putsFxterm("\n\nPress EXE to continue");
	while(TranslateChar(PRGM_GetKey()) != CK_ENTER);
}


#define REC_BUF_SIZE 16384

static void CheckSerialWrite(int ret) {
	if (ret != 0) {
		if (ret == 2) {
			putsFxterm("Serial write no space.\n");
		} else if (ret == 3) {
			putsFxterm("Serial write not open.\n");
		} else {
			putsFxterm("Serial write unk err.\n");
		}
	}
}

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

		if (!gcMemoryDump) {
			if(c)
			{
				if(c=='\n'){
					Serial_WriteSingle('\r');
					Serial_WriteSingle('\n');
				}else
					Serial_WriteSingle(c);
				if(gcLocalEcho)
				{
					putcharFxterm(c);//local echo
				}
				dbgprint1("<%c>",c);
			}
		}

		if(0 == Serial_ReadSingle(aRecBuffer))
		{
			if (gcMemoryDump) {
				// We have the first byte. Now get seven more for the starting address and the length.

				const unsigned char * bufPtr = aRecBuffer + 1;
				for (unsigned iRead = 0; iRead < 7; ++iRead) {
					while(Serial_ReadSingle(bufPtr) != 0) {
						// Do nothing while waiting.
						c = PRGM_GetKey();
						if(c == KEY_PRGM_MENU) {
							break;
						}
					}
					++bufPtr;
				}

				if(c == KEY_PRGM_MENU) {
					break;
				}
				const unsigned int * bufAsInt = (const unsigned int*)aRecBuffer;
				unsigned char* startAddress = *bufAsInt++;
				unsigned int length = *bufAsInt;
				sprintf(aRecBuffer, "SA: %p len: %d\n", startAddress, length);
				putsFxterm(aRecBuffer);
				// At this point we will need to send a byte so make sure we have enough room in the send buffer.
				while (Serial_PollTX() <= 1) {
					// Keep waiting until we have room for a byte.
				}
				if ((length > REC_BUF_SIZE) || (length < 1)) {
					CheckSerialWrite(Serial_WriteSingle(0)); // Declined because the length is too big or small.
				} else {
					CheckSerialWrite(Serial_WriteSingle(1)); // Accepted. The host now needs the checksum.
					// At this point we can overwrite aRecBuffer because we have made a copy of the start address and length.
					memmove(aRecBuffer, startAddress, length); // The purpose of moving it into a buffer is if the memory is changed during the transfer process. The checksum and the data won't match if this happens. However now that it's in our buffer it's far more likely to be safe. Also I used memmove instead of memcpy to hopefully help with cases where aRecBuffer and startAddress overlap.
					unsigned int crc = crc32(0L, Z_NULL, 0); // Get the starting value.
					crc = crc32(crc, aRecBuffer, length);
					// Send the checksum to the host.
					while (Serial_PollTX() <= sizeof(unsigned int)) {
						// Ensure that we can put the four bytes in the buffer.
					}
					CheckSerialWrite(Serial_Write(&crc, sizeof(unsigned int)));
					
					for (unsigned i = 0; i < length; ++i) {
						while (Serial_PollTX() <= 1) {
							// As usual wait for room in the buffer.
						}
						CheckSerialWrite(Serial_WriteSingle(*startAddress++));
					}

				}
			} else {
				int iRead = 1;
				int iWrote = 0;
				while(iWrote < iRead)
				{
					while((iRead < REC_BUF_SIZE) && (0 == Serial_ReadSingle(aRecBuffer+iRead)))iRead++;
					putcharFxterm(aRecBuffer[iWrote]);iWrote++;
				}
			}
		}
	}
	Serial_Close(1);
}

void main( void )
{
	MenuContext aMenuContext = {0};
	Bdisp_EnableColor(1);
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
