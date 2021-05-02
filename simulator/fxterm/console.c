#include "platform.h"
#include "fonts.h"
#include "vtparse.h"
#include "keyboard.hpp"
#include "console.h"
//stty rows 27 columns 64


#define SCR_FLAG_BRIGHT			  1
#define SCR_FLAG_FAINT			  2
#define SCR_FLAG_ITALIC			  4
#define SCR_FLAG_UNDERLINE1		  8
#define SCR_FLAG_BLINKSLOW		 16
#define SCR_FLAG_BLINKFAST		 32
#define SCR_FLAG_INVERSE		 64
#define SCR_FLAG_CONCEAL		128
#define SCR_FLAG_CROSSEDOUT		256
#define SCR_FLAG_FRAMED			512
#define SCR_FLAG_ENCIRCLED		1024
#define SCR_FLAG_OVERLINED		2048

#define CURSOR_STATE_DISABLED 0
#define CURSOR_STATE_HIDDEN 1
#define CURSOR_STATE_VISIBLE 2
#define CURSOR_BG_BUFFER_LEN	64

#define CURSOR_HALF_PERIOD 500 //ms

typedef unsigned short WORD;
typedef struct
{
	const FONTDATA *mpFontData;
	const FONTDATA *mpCursorData;
	WORD mwCursorBackground[CURSOR_BG_BUFFER_LEN];
	WORD mwWidth;
	WORD mwHeight;
	WORD mwWinLeft;
	WORD mwWinRight;
	WORD mwWinTop;
	WORD mwWinBottom;
	WORD mwCursorX;
	WORD mwCursorY;
	WORD mwColorTextIndex;
	WORD mwColorBGIndex;
	WORD mwFlags;
	WORD mwLastLinePxlOffset;
	WORD mwFontWidth;
	WORD mwFontHeight;
	WORD mwFontDataHeight;
	WORD mwFontCharCount;
	WORD mwSaveCursorX;
	WORD mwSaveCursorY;
	WORD mwCursorType;
	WORD mwCursorState;
	int miCursorTimer;
	char mcDirtyFlag;
}SCREEN;

#define RGB(r,g,b) ((r >> 3)<<11) | ((g >> 2) << 5) | (b >> 3)
static const WORD gwColorPalette[16] = 
{
	RGB(0,0,0),
	RGB(160,0,0),
	RGB(0,160,0),
	RGB(160,160,0),
	RGB(0,0,160),
	RGB(160,0,160),
	RGB(0,160,160),
	RGB(160,160,160),
	RGB(127,127,127),
	RGB(255,0,0),
	RGB(0,255,0),
	RGB(255,255,0),
	RGB(0,0,255),
	RGB(255,0,255),
	RGB(0,255,255),
	RGB(255,255,255)
};

typedef struct
{
	int miPRGMKeyCode;
	const char mcKeys[CURSOR_COUNT];
}TKeyMeaning;

/*
#define CURSOR_INDEX_NORM	0
#define CURSOR_INDEX_ALPHA	1
#define CURSOR_INDEX_SHIFT	2
#define CURSOR_INDEX_SHALP	3
#define CURSOR_INDEX_CTRL	4
#define CURSOR_INDEX_EXT	5
#define CURSOR_INDEX_SHEXT	6
#define CURSOR_COUNT		7



*/

//Console keys
#define KEY_COUNT 47
static const TKeyMeaning gKeyTable[KEY_COUNT]=
{
	{79,'~'      ,0      ,0      ,0      ,0      ,0      ,0      },//F1
	{69,'`'      ,0      ,0      ,0      ,0      ,0      ,0      },//F2
	{59,'|'      ,0      ,0      ,0      ,0      ,0      ,0      },//F3
	{49,'<'      ,0      ,0      ,0      ,0      ,0      ,0      },//F4
	{39,'>'      ,0      ,0      ,0      ,0      ,0      ,0      },//F5
	{29,0      ,0      ,0      ,0      ,0      ,0      ,0      },//F6
	
//	{78,0      ,0      ,0      ,0      ,0      ,0      ,0      },//shift
//	{68,0      ,0      ,0      ,0      ,0      ,0      ,0      },//optn
	{58,0      ,0      ,0      ,0      ,0      ,0      ,0      },//vars
	{48,0      ,0      ,0      ,0      ,0      ,0      ,0      },//menu
	{38,'\2'   ,'\2'   ,'\2'   ,'\2'   ,'\2'   ,'\2'   ,'\2'   },//left
	{28,'\20'  ,'\20'  ,'\20'  ,'\20'  ,'\20'  ,'\20'  ,'\20'  },//up
	
//	{77,0      ,0      ,0      ,0      ,0      ,0      ,0      },//alpha
	{67,0      ,0      ,0      ,0      ,0      ,0      ,0      },//x^2
//	{57,0      ,0      ,0      ,0      ,0      ,0      ,0      },//^
	{47,'\33'  ,'\33'  ,'\33'  ,'\33'  ,'\33'  ,'\33'  ,'\33'  },//exit
	{37,'\16'  ,'\16'  ,'\16'  ,'\16'  ,'\16'  ,'\16'  ,'\16'  },//down
	{27,'\6'   ,'\6'   ,'\6'   ,'\6'   ,'\6'   ,'\6'   ,'\6'   },//right
	
	{76,0      ,'A'    ,0      ,'a'    ,'\1'   ,0      ,0      },//x,t
	{66,0      ,'B'    ,0      ,'b'    ,'\2'   ,0      ,0      },//log
	{56,0      ,'C'    ,0      ,'c'    ,'\3'   ,0      ,0      },//ln
	{46,0      ,'D'    ,0      ,'d'    ,'\4'   ,0      ,0      },//sin 
	{36,0      ,'E'    ,0      ,'e'    ,'\5'   ,0      ,0      },//cos 
	{26,0      ,'F'    ,0      ,'f'    ,'\6'   ,0      ,0      },//tan

	{75,0      ,'G'    ,0      ,'g'    ,'\7'   ,0      ,0      },//a/b/c
	{65,'\t'   ,'H'    ,0      ,'h'    ,'\10'  ,0      ,0      },//F-D 
	{55,'('    ,'I'    ,0      ,'i'    ,'\11'  ,0      ,0      },//(   
	{45,')'    ,'J'    ,0      ,'j'    ,'\12'  ,0      ,0      },//)   
	{35,','    ,'K'    ,0      ,'k'    ,'\13'  ,0      ,0      },//,   
	{25,0      ,'L'    ,0      ,'l'    ,'\14'  ,0      ,0      },//->

	{74,'7'    ,'M'    ,'&'    ,'m'    ,'\15'  ,0      ,0      },//7
	{64,'8'    ,'N'    ,0      ,'n'    ,'\16'  ,0      ,0      },//8
	{54,'9'    ,'O'    ,0      ,'o'    ,'\17'  ,0      ,0      },//9
	{44,'\b'   ,'\b'   ,'\b'   ,'\b'   ,'\b'   ,'\b'   ,'\b'   },//del
	{10,'\3'   ,'\3'   ,'\3'   ,'\3'   ,'\3'   ,'\3'   ,'\3'   },//ac

	{73,'4'    ,'P'    ,'$'    ,'p'    ,'\20'  ,0      ,0      },//4
	{63,'5'    ,'Q'    ,'%'    ,'q'    ,'\21'  ,0      ,0      },//5
	{53,'6'    ,'R'    ,'^'    ,'r'    ,'\22'  ,0      ,0      },//6
	{43,'*'    ,'S'    ,'{'    ,'s'    ,'\23'  ,0      ,0      },//*
	{33,'/'    ,'T'    ,'}'    ,'t'    ,'\24'  ,0      ,0      },///

	{72,'1'    ,'U'    ,'!'    ,'u'    ,'\25'  ,0      ,0      },//1
	{62,'2'    ,'V'    ,'@'    ,'v'    ,'\26'  ,0      ,0      },//2
	{52,'3'    ,'W'    ,'#'    ,'w'    ,'\27'  ,0      ,0      },//3
	{42,'+'    ,'X'    ,'['    ,'x'    ,'\30'  ,0      ,0      },//+
	{32,'-'    ,'Y'    ,']'    ,'y'    ,'\31'  ,0      ,0      },//-

	{71,'0'    ,'Z'    ,0      ,'z'    ,'\32'  ,0      ,0      },//0
	{61,'.'    ,' '    ,'='    ,' '    ,0      ,0      ,0      },//.
	{51,'?'    ,'\"'   ,0      ,0      ,0      ,0      ,0      },//exp
	{41,'_'    ,';'    ,':'    ,'-'    ,0      ,0      ,0      },//(-)
	{31,'\n'   ,'\n'   ,'\n'   ,'\n'   ,'\n'   ,'\n'   ,'\n'   },//exe

};


#define DEFAULT_TEXT	15
#define DEFAULT_BG		0
#define DEFAULT_FLAGS	0


static SCREEN gScreen;

static void assertX(int i,int c)
{
	dbgprint2("assert %i,%i\n",i,c);
	i = i;// <<-- put breakpoint here
}

int ch = -1;
#define assert() assertX(__LINE__,ch)

static void DrawChar(int ch)
{
	int mask,j,k;
	int x = gScreen.mwCursorX * (gScreen.mwFontWidth);
	int y = gScreen.mwCursorY * (gScreen.mwFontHeight);
	WORD wColorText = gwColorPalette[gScreen.mwColorTextIndex | (((gScreen.mwFlags & SCR_FLAG_BRIGHT) && gScreen.mwColorTextIndex)?8:0)];
	WORD wColorBG = gwColorPalette[gScreen.mwColorBGIndex];
	if(ch >= gScreen.mwFontCharCount)
	{
		ch = ' ';//unknown char
	}
	ch = ch * gScreen.mwFontDataHeight;
	if(gScreen.mwFlags & (SCR_FLAG_ITALIC | SCR_FLAG_INVERSE))
	{
		WORD w = wColorText;
		wColorText = wColorBG;
		wColorBG = w;
	}
	for(k=0;k<gScreen.mwFontDataHeight;k++)
	{
		mask = 1 << (gScreen.mwFontWidth -1);
		for(j=0;j<gScreen.mwFontWidth;j++)
		{
			VRAM[x+j+WIDTH*(y+k)]=gScreen.mpFontData[ch+k]&mask?wColorText:wColorBG;
			mask >>= 1;
		}
	}
	
	if(gScreen.mwFontDataHeight < gScreen.mwFontHeight)
	{
		y += gScreen.mwFontDataHeight;
		y *= WIDTH;
		y += x;
		if(gScreen.mwFlags & SCR_FLAG_UNDERLINE1)
		{
			wColorBG = wColorText;
		}
		for(j=0;j<gScreen.mwFontWidth;j++)
		{
			VRAM[j+y]=wColorBG;
		}
	}
}
///////////////////////////////////////////////////////////////////////////////////////////////////
const char gcCursorColors[CURSOR_COUNT]={2,9,11,11,10,10,11};

static void ShowCursor()
{
	if(gScreen.mwCursorState == CURSOR_STATE_HIDDEN)
	{
		int i,j;
		int x = gScreen.mwCursorX * gScreen.mwFontWidth;
		int y = gScreen.mwCursorY * gScreen.mwFontHeight;
		int ch = gScreen.mwCursorType * gScreen.mwFontHeight;
		WORD *pBG = gScreen.mwCursorBackground;
		int mask;
		WORD wColor = gwColorPalette[gcCursorColors[gScreen.mwCursorType]];

		for(j=0;j<gScreen.mwFontHeight;j++)
		{
			for(i=0;i<gScreen.mwFontWidth;i++)
			{
				*pBG = VRAM[x+i+WIDTH*(y+j)];
				pBG++;
			}
		}


		for(j=0;j<gScreen.mwFontHeight;j++)
		{
			mask = 1 << (gScreen.mwFontWidth-1);
			for(i=0;i<gScreen.mwFontWidth;i++)
			{
				VRAM[x+i+WIDTH*(y+j)]=gScreen.mpCursorData[ch+j]&mask?wColor:0;
				mask >>= 1;
			}
		}
		gScreen.mwCursorState = CURSOR_STATE_VISIBLE;
	}
	gScreen.miCursorTimer = RTC_GetTicks();
	gScreen.mcDirtyFlag = 1;
}

static void HideCursor()
{
	if(gScreen.mwCursorState == CURSOR_STATE_VISIBLE)
	{
		int i,j;
		WORD *pBG = gScreen.mwCursorBackground;
		int x = gScreen.mwCursorX * gScreen.mwFontWidth;
		int y = gScreen.mwCursorY * gScreen.mwFontHeight;

		for(j=0;j<gScreen.mwFontHeight;j++)
		{
			for(i=0;i<gScreen.mwFontWidth;i++)
			{
				VRAM[x+i+WIDTH*(y+j)] = *pBG;
				pBG++;
			}
		}
		gScreen.mwCursorState = CURSOR_STATE_HIDDEN;
	}
	gScreen.miCursorTimer = RTC_GetTicks();
	gScreen.mcDirtyFlag = 1;
}

static void FlushVRAM()
{
	gScreen.mcDirtyFlag = 0;
	Bdisp_PutDisp_DD();
}

static void CursorFlasher()
{
	if((gScreen.mwCursorState != CURSOR_STATE_DISABLED) && RTC_Elapsed_ms(gScreen.miCursorTimer,CURSOR_HALF_PERIOD))
	{
		switch(gScreen.mwCursorState)
		{
		case CURSOR_STATE_VISIBLE:HideCursor();break;
		case CURSOR_STATE_HIDDEN:ShowCursor();break;
		}
	}
	else
	{
		EmulHack_Sleep(50);//windows hack
	}
	if(gScreen.mcDirtyFlag)
	{
		FlushVRAM();
	}
}
///////////////////////////////////////////////////////////////////////////////////////////////////
static void memmoveint(int *pTgt,int *pSrc,int iLenInInts)
{
	int i = 0;
	while(i<iLenInInts)
	{
		pTgt[0]=pSrc[0];
		pTgt++;
		pSrc++;
		i++;
	}
}

static void Scroll()
{
	memmoveint((int*)VRAM,(int*)(VRAM+gScreen.mwFontHeight*WIDTH),(gScreen.mwLastLinePxlOffset*WIDTH/2));
	memset(VRAM+gScreen.mwLastLinePxlOffset*WIDTH,0,gScreen.mwFontHeight*WIDTH*2);
	FlushVRAM();
}


static void CursorDown()
{
	if(gScreen.mwCursorY < gScreen.mwHeight-1)
	{
		gScreen.mwCursorY++;
	}
	else
	{
		Scroll();
	}
}

static void CursorUp()
{
	if(gScreen.mwCursorY)
	{
		gScreen.mwCursorY--;
	}
}


static void CursorHome()
{
	gScreen.mwCursorX = 0;
}

static void CursorRight()
{
	if(gScreen.mwCursorX < gScreen.mwWidth-1)
	{
		gScreen.mwCursorX++;
	}
	else
	{
		CursorHome();
		CursorDown();
	}
}

static void CursorRightTab()
{
	CursorRight();
	while(gScreen.mwCursorX & 3)
	{
		CursorRight();
	}
}

static void CursorLeft()
{
	if(gScreen.mwCursorX)
	{
		gScreen.mwCursorX--;
	}
}

static void CursorGoto(int x,int y)
{
	if(y < 0)
	{
		gScreen.mwCursorY = 0;
	}
	else if(y < gScreen.mwHeight)
	{
		gScreen.mwCursorY = y;
	}
	if(x < 0)
	{
		gScreen.mwCursorX = 0;
	}
	else if(x < gScreen.mwWidth)
	{
		gScreen.mwCursorX = x;
	}
}

static void ClrVRAM( void )
{
	memset(VRAM,0,WIDTH*HEIGHT*2);
}

unsigned short *VRAM = NULL;
void InitScreen(const FONT *pFont)
{
	if(!VRAM)
	{
		VRAM = (unsigned short *) GetVRAMAddress();
		vtparse_init();
		EnableColor(1);
	}
	gScreen.mpFontData = pFont->mpcFontData;
	gScreen.mpCursorData = pFont->mpcCursorData;
	gScreen.mwFontWidth = pFont->mcWidth;
	gScreen.mwFontHeight = pFont->mcHeight;
	gScreen.mwFontDataHeight = pFont->mcDataHeight;
	gScreen.mwFontCharCount = pFont->mwCharacterCount;
	gScreen.mwCursorX = 0;
	gScreen.mwCursorY = 0;
	gScreen.mwColorTextIndex = DEFAULT_TEXT;
	gScreen.mwColorBGIndex = DEFAULT_BG;
	gScreen.mwFlags = DEFAULT_FLAGS;
	gScreen.mwWidth = (WORD)(WIDTH / (gScreen.mwFontWidth));
	gScreen.mwHeight = (WORD)(HEIGHT / (gScreen.mwFontHeight));
	gScreen.mwWinLeft = 0;
	gScreen.mwWinRight = gScreen.mwWidth;
	gScreen.mwWinTop = 0;
	gScreen.mwWinBottom = gScreen.mwHeight;

	gScreen.mwLastLinePxlOffset = (gScreen.mwHeight - 1)*(gScreen.mwFontHeight);
	gScreen.mwSaveCursorX = 0;
	gScreen.mwSaveCursorY = 0;
	if(CURSOR_BG_BUFFER_LEN < gScreen.mwFontWidth * gScreen.mwFontHeight)
	{
		assert();
	}
	gScreen.mwCursorState = CURSOR_STATE_HIDDEN;
	ClrVRAM();
	gScreen.miCursorTimer = RTC_GetTicks();
}

static void SaveCursor()
{
	gScreen.mwSaveCursorX = gScreen.mwCursorX;
	gScreen.mwSaveCursorY = gScreen.mwCursorY;
}

static void UnsaveCursor()
{
	gScreen.mwCursorX = gScreen.mwSaveCursorX;
	gScreen.mwCursorY = gScreen.mwSaveCursorY;
}

static void SGRParser(int iCode)
{
	if(iCode >= 30 && iCode <=37)
	{
		gScreen.mwColorTextIndex = iCode - 30;
		dbgprint1("[text color %u]",gScreen.mwColorTextIndex);
		return;
	}
	if(iCode >= 40 && iCode <=47)
	{
		gScreen.mwColorBGIndex = iCode - 40;
		dbgprint1("[bg color %u]",gScreen.mwColorBGIndex);
		return;
	}
	switch(iCode)
	{
	case 0:
		gScreen.mwFlags = DEFAULT_FLAGS;
		gScreen.mwColorBGIndex = DEFAULT_BG;
		dbgprint1("[reset attr]",gScreen.mwColorBGIndex);
	case 39:
		gScreen.mwColorTextIndex = DEFAULT_TEXT;
		dbgprint("[reset text color]");
		break;
	case 1:gScreen.mwFlags = SCR_FLAG_BRIGHT;dbgprint("[bright+]");break;
	case 2:gScreen.mwFlags = SCR_FLAG_FAINT;dbgprint("[faint+]");break;
	case 3:gScreen.mwFlags = SCR_FLAG_ITALIC;dbgprint("[italic+]");break;
	case 4:gScreen.mwFlags |= SCR_FLAG_UNDERLINE1;dbgprint("[underline1+]");break;
	case 5:gScreen.mwFlags = SCR_FLAG_BLINKSLOW;dbgprint("[blinksl+]");break;
	case 6:gScreen.mwFlags = SCR_FLAG_BLINKFAST;dbgprint("[blinkfa+]");break;
	case 7:gScreen.mwFlags = SCR_FLAG_INVERSE;dbgprint("[inverse+]");break;
	case 8:gScreen.mwFlags = SCR_FLAG_CONCEAL;dbgprint("[conceal+]");break;
	case 9:gScreen.mwFlags = SCR_FLAG_CROSSEDOUT;dbgprint("[crossedout+]");break;
	case 22:gScreen.mwFlags &= ~SCR_FLAG_BRIGHT;dbgprint("[bright-]");break;
	case 23:gScreen.mwFlags &= ~SCR_FLAG_ITALIC;dbgprint("[italic-]");break;
	case 24:gScreen.mwFlags &= ~SCR_FLAG_UNDERLINE1;dbgprint("[underline1-]");break;
	case 25:gScreen.mwFlags &= ~(SCR_FLAG_BLINKSLOW | SCR_FLAG_BLINKFAST);dbgprint("[blink-]");break;
	case 27:gScreen.mwFlags &= ~SCR_FLAG_INVERSE;dbgprint("[inverse-]");break;
	case 28:gScreen.mwFlags &= ~SCR_FLAG_CONCEAL;dbgprint("[conceal-]");break;
	case 49:gScreen.mwColorBGIndex = DEFAULT_BG;dbgprint("[reset bg color]");break;
	case 51:gScreen.mwFlags = SCR_FLAG_FRAMED;dbgprint("[framed+]");break;
	case 52:gScreen.mwFlags = SCR_FLAG_ENCIRCLED;dbgprint("[encircled+]");break;
	case 53:gScreen.mwFlags = SCR_FLAG_OVERLINED;dbgprint("[overlined+]");break;
	case 54:gScreen.mwFlags &= ~(SCR_FLAG_FRAMED | SCR_FLAG_ENCIRCLED);dbgprint("[framed-encircled-]");break;
	case 55:gScreen.mwFlags &= ~SCR_FLAG_OVERLINED;dbgprint("[overlined-]");break;
	default:assert();
	}
}


static int GetParam(int iIndex,int iDefault)
{
	if(gParser.num_params > iIndex)
	{
		iDefault = gParser.params[iIndex];
	}
	return iDefault;
}

void parser_callback(vtparse_action_t action, unsigned char ch)
{
    int i,k;
	switch(action)
	{
	case VTPARSE_ACTION_PRINT:
		DrawChar(ch);
		dbgprint1("%c",ch);
		CursorRight();
		break;
	case VTPARSE_ACTION_EXECUTE:
		switch(ch)
		{
		case 10:CursorDown();CursorHome();dbgprint("[#10]");break;
		case 13:CursorHome();dbgprint("[#13]\n");gScreen.mwFlags = DEFAULT_FLAGS;break;
		case 14:dbgprint("[altfont]");break;//Switch to an alternate character set.
		case 15:dbgprint("[normfont]");break;//Return to regular character set after Shift Out.
		case  8:CursorLeft();dbgprint("[#8]");break;
		case  9:CursorRightTab();dbgprint("[tab]");break;
		case  7:dbgprint("[bell]");break;//todo
		default:assert();
		}
		break;
	case VTPARSE_ACTION_CSI_DISPATCH:
		switch(ch)
		{
		case 'F':
//CSI n F CPL – Cursor Previous Line Moves cursor to beginning of the line n (default 1) lines up 
			k = GetParam(0,1)-1;
			dbgprint1("[home,line %i]",k);
			CursorGoto(0,-k);
		case 'A':
//Cursor Up \e[{COUNT}A Moves the cursor up by COUNT rows; the default count is 1. 
			k = GetParam(0,1);
			dbgprint1("[up %i]",k);
			if (gScreen.mwCursorY > k) gScreen.mwCursorY -= k;else gScreen.mwCursorY = 0;
			break;
		case 'E':
//CSI n E CNL – Cursor Next Line Moves cursor to beginning of the line n (default 1) lines down. 
			dbgprint("[home]");
			gScreen.mwCursorX = 0;
		case 'B':
//Cursor Down \e[{COUNT}B Moves the cursor down by COUNT rows; the default count is 1. 
			k = GetParam(0,1);
			dbgprint1("[down %i]",k);
			if (gScreen.mwCursorY < gScreen.mwHeight - k) gScreen.mwCursorY += k;else gScreen.mwCursorY = gScreen.mwHeight-1;
			break;
		case 'C':
//Cursor Forward \e[{COUNT}C Moves the cursor forward by COUNT columns; the default count is 1. 
			k = GetParam(0,1);
			dbgprint1("[right %i]",k);
			while(k && (gScreen.mwCursorX < gScreen.mwWidth))
			{
				DrawChar(32);
				gScreen.mwCursorX++;
				k--;
			}
			break;
		case 'D':
//Cursor Backward \e[{COUNT}D Moves the cursor backward by COUNT columns; the default count is 1. 
			k = GetParam(0,1);
			dbgprint1("[left %i]",k);
			if (gScreen.mwCursorX > k) gScreen.mwCursorX -= k;else gScreen.mwCursorX = 0;
			break;
		case 'G':
//CSI n G CHA – Cursor Horizontal Absolute Moves the cursor to column n. 
			k = GetParam(0,1)-1;
			dbgprint1("[GotoX %i]",k);
			if (k  < gScreen.mwHeight ) gScreen.mwCursorX = k;
			break;
		case 'f':
//Force Cursor Position \e[{ROW};{COLUMN}f Identical to Cursor Home. 
		case 'H':
//Cursor Home \e[{ROW};{COLUMN}H Sets the cursor position. If no row/column parameters are provided (i.e., \eH) the cursor will move to the home position (the upper left of the screen). 
			k = GetParam(0,1)-1;
			dbgprint1("[GotoXY %i",k);
			if (k  < gScreen.mwHeight ) gScreen.mwCursorY = k;else gScreen.mwCursorY = gScreen.mwHeight - 1;
			k = GetParam(1,1)-1;
			dbgprint1(",%i]",k);
			if (k  < gScreen.mwWidth ) gScreen.mwCursorX = k;else gScreen.mwCursorX = gScreen.mwWidth - 1;
			//gScreen.mwFlags = DEFAULT_FLAGS;
			break;
			//Erases part of the line. If n is zero (or missing), clear from cursor to the end of the line. If n is one, clear from cursor to beginning of the line. If n is two, clear entire line. Cursor position does not change.
		case 'J':
//Erase Down \e[J Erases the screen from the current line down to the bottom of the screen. 
//Erase Up \e[1J Erases the screen from the current line up to the top of the screen. 
//Erase Screen \e[2J Erases the screen with the background colour and moves the cursor to home. 
			k = GetParam(0,0);
			dbgprint1("[Clear Scr %i]",k);
			ClrVRAM();//todo:cursor home
			break;
		case 'K':
//Erase to End of Line \e[K Erases from the current cursor position to the end of the current line. 
//Erase to Start of Line \e[1K Erases from the current cursor position to the start of the current line. 
//Erase Line \e[2K Erases the entire current line. 
			k = GetParam(0,0);
			dbgprint1("[Clear Line %i]",k);
			break;
		case 'S':
//CSI n S SU – Scroll Up Scroll whole page up by n (default 1) lines. New lines are added at the bottom. (not ANSI.SYS) 
			break;
		case 'T':
//CSI n T SD – Scroll Down Scroll whole page down by n (default 1) lines. New lines are added at the top. (not ANSI.SYS) 
			break;
		case 'c':
//Query Device Code \e[c  Requests a Report Device Code response from the device.  
//Report Device Code \e[{code}0c  Generated by the device in response to Query Device Code request.  
//Esc[c  Identify what terminal type  DA  
//Esc[0c  Identify what terminal type (another)  DA  
//Esc[?1;Value0c  Response: terminal type code n  DA  

		break;
		case 'g':
//Set Tab \e[#;#;#g
//Clear Tab <ESC>[g Clears tab at the current position. 
//Clear All Tabs <ESC>[3g Clears all tabs. 
			break;
		case 'h':
//Enable Line Wrap \e[7h Text wraps to next line if longer than the length of the display area.  
//CSI ?25h DECTCEM Shows the cursor. 
/*
Esc[20h  Set new line mode  LMN  
Esc[?1h  Set cursor key to application  DECCKM  
none  Set ANSI (versus VT52)  DECANM  
Esc[?3h  Set number of columns to 132  DECCOLM  
Esc[?4h  Set smooth scrolling  DECSCLM  
Esc[?5h  Set reverse video on screen  DECSCNM  
Esc[?6h  Set origin to relative  DECOM  
Esc[?7h  Set auto-wrap mode  DECAWM  
Esc[?8h  Set auto-repeat mode  DECARM  
Esc[?9h  Set interlacing mode  DECINLM  
*/
//		gScreen.mwCursorState = CURSOR_STATE_DISABLED; //hide  cursor 
			break;
		case 'i':
//Print Screen \e[i Print the current screen. 
//Print Line \e[1i Print the current line. 
//Stop Print Log \e[4i Disable log. 
//Start Print Log \e[5i Start log; all received text is echoed to a printer. 
			break;
		case 'l':
//Disable Line Wrap \e[7l Disables line wrapping. 
//CSI ?25l DECTCEM Hides the cursor. 
/*
Esc[20l  Set line feed mode  LMN  
Esc[?1l  Set cursor key to cursor  DECCKM  
Esc[?2l  Set VT52 (versus ANSI)  DECANM  
Esc[?3l  Set number of columns to 80  DECCOLM  
Esc[?4l  Set jump scrolling  DECSCLM  
Esc[?5l  Set normal video on screen  DECSCNM  
Esc[?6l  Set origin to absolute  DECOM  
Esc[?7l  Reset auto-wrap mode  DECAWM  
Esc[?8l  Reset auto-repeat mode  DECARM  
Esc[?9l  Reset interlacing mode  DECINLM  
*/
			gScreen.mwCursorState = CURSOR_STATE_HIDDEN;//show cursor 
			break;
		case 'm':
/*
Esc[m  Turn off character attributes  SGR0  
Esc[0m  Turn off character attributes  SGR0  
Esc[1m  Turn bold mode on  SGR1  
Esc[2m  Turn low intensity mode on  SGR2  
Esc[4m  Turn underline mode on  SGR4  
Esc[5m  Turn blinking mode on  SGR5  
Esc[7m  Turn reverse video on  SGR7  
Esc[8m  Turn invisible text mode on  SGR8  
*/
			for(i = 0; i < gParser.num_params; i++)
			{
				SGRParser(gParser.params[i]);
			}
			break;
		case 'n':
//Query Device Status \e[5n  Requests a Report Device Status response from the device.  
	//Report Device OK \e[0n  Generated by the device in response to a Query Device Status request; indicates that device is functioning correctly.  
	//Report Device Failure \e[3n  Generated by the device in response to a Query Device Status request; indicates that device is functioning improperly.  
//Query Cursor Position \e[6n  Requests a Report Cursor Position response from the device.  
	//Report Cursor Position \e[{ROW};{COLUMN}R  Generated by the device in response to a Query Cursor Position request; reports current cursor position.  
			break;

		case 'p':
//Set Key Definition \e[{key};"{string}"p Associates a string of text to a keyboard key. {key} indicates the key by its ASCII value in decimal. 
		case 'r':
//Scroll Screen \e[r Enable scrolling for entire display. 
//Scroll Screen \e[{start};{end}r Enable scrolling from row {start} to row {end}. 
			{
				int a = GetParam(0,0);
				int b = GetParam(1,0);
				dbgprint1("%i",a);
			}
			/*assert();*/ //Esc[Line;Liner  Set top and bottom lines of a window  DECSTBM  
			break;
		case 's':
//Save Cursor \e[s Save current cursor position. 
			SaveCursor();
			break;
		case 'u':
//Unsave Cursor \e[u Restores cursor position after a Save Cursor. 
			UnsaveCursor();
			break;
		case 'y':
//Esc[2;1y  Confidence power up test  DECTST  
//Esc[2;2y  Confidence loopback test  DECTST  
//Esc[2;9y  Repeat power up test  DECTST  
//Esc[2;10y  Repeat loopback test  DECTST  
			break;
		default:assert();
		}
		break;

	case VTPARSE_ACTION_ESC_DISPATCH:
		switch(ch)
		{
		case '>':
//Exit alternate keypad mode 			
//Esc>  Set numeric keypad mode  DECKPNM  
			break;
		case '=':
//Enter alternate keypad mode 			
//Esc=  Set alternate keypad mode  DECKPAM  
			break;
		case '<':
//Esc<  Enter/exit ANSI mode (VT52)  setansi  
			break;
/*
Esc(A  Set United Kingdom G0 character set  setukg0  
Esc)A  Set United Kingdom G1 character set  setukg1  
Esc(B  Set United States G0 character set  setusg0  
Esc)B  Set United States G1 character set  setusg1  
Esc(0  Set G0 special chars. & line set  setspecg0  
Esc)0  Set G1 special chars. & line set  setspecg1  
Esc(1  Set G0 alternate character ROM  setaltg0  
Esc)1  Set G1 alternate character ROM  setaltg1  
Esc(2  Set G0 alt char ROM and spec. graphics  setaltspecg0  
Esc)2  Set G1 alt char ROM and spec. graphics  setaltspecg1  
*/
		case '(':
//Font Set G0 \e( Set default font. 
			break;
		case ')':
//Font Set G1 \e) Set alternate font. 
			break;
		case '#':
//Esc#3  Double-height letters, top half  DECDHL  
//Esc#4  Double-height letters, bottom half  DECDHL  
//Esc#5  Single width, single height letters  DECSWL  
//Esc#6  Double width, single height letters  DECDWL  
//Esc#8  Screen alignment display  DECALN  
			break;
		case '7':
//Save Cursor & Attrs \e7 Save current cursor position. 
			SaveCursor();
			break;
//Restore Cursor & Attrs \e8 Restores cursor position after a Save Cursor. 
		case '8':
			UnsaveCursor();
			break;
		case 'A':
//EscA  Move cursor up one line  cursorup  
			if (gScreen.mwCursorY) 
			{
				gScreen.mwCursorY --;
			}
			break;
		case 'B':
//EscB  Move cursor down one line  cursordn  
			if (gScreen.mwCursorY < gScreen.mwHeight - 1) 
			{
				gScreen.mwCursorY ++;
			}
			else 
			{
				gScreen.mwCursorY = gScreen.mwHeight-1;
			}
			break;
		case 'C':
//EscC  Move cursor right one char  cursorrt  
			CursorRight();
			break;
		case 'D':
//EscD  Move/scroll window up one line  IND  
//EscD  Move cursor left one char  cursorlf  
			CursorLeft();
			break;
		case 'E':
//EscE  Move to next line  NEL  
			CursorDown();
			break;
		case 'F':
//EscF  Use special graphics character set  setgr  
			break;
		case 'G':
//EscG  Use normal US/UK character set  resetgr  
			break;
		case 'H':
//SetTab();
			break;
		case 'I':
//EscI  Generate a reverse line-feed  revindex  
			break;
		case 'J':
//Erase to end of screen 			
//EscJ  Erase to end of screen  cleareos  
			break;
		case 'K':
//EscK  Erase to end of current line  cleareol  
			break;
		case 'M':
//EscM  Move/scroll window down one line  RI  
			CursorDown();
			break;
		case 'N':
//EscN  Set single shift 2  SS2  
			break;
		case '0':
//EscO  Set single shift 3  SS3  
			break;
		case 'Z':
//EscZ  Identify what the terminal is  ident  
//Esc/Z  Correct response to ident  identresp  
			break;

		case 'c':
//Reset Device \ec Reset all terminal settings to default. On my PC this causes the screen to clear and cursor to go to the home position, but I don't know that this would always be the case.  
//Escc  Reset terminal to initial state  RIS  
			break;

		default:assert();
		}
		break;
	default:
		dbgprint3("Received action %s, char=0x%02x (%c)\n", ACTION_NAMES[action],ch,ch);
		dbgprint1("Intermediate chars: '%s'\n", gParser.intermediate_chars);
		dbgprint1("%d Parameters:\n", gParser.num_params);
		for(i = 0; i < gParser.num_params; i++)
		{
			dbgprint1("\t%d\n", gParser.params[i]);
		}
		dbgprint("\n");

	}
}

char TranslateChar(char cRawChar)
{
	static char cOldRawChar = 0;
	int i;
	if(cRawChar == cOldRawChar)
	{
		cRawChar = 0;
	}
	else
	{
		cOldRawChar = cRawChar;
	}
	if(!cRawChar)
	{
		CursorFlasher();
		return 0;
	}
	gScreen.mcDirtyFlag = 1;
	switch(cRawChar)
	{
	case KEY_PRGM_SHIFT:
		switch(gScreen.mwCursorType)
		{
		case CURSOR_INDEX_ALPHA:gScreen.mwCursorType = CURSOR_INDEX_SHALP;break;
		case CURSOR_INDEX_SHALP:gScreen.mwCursorType = CURSOR_INDEX_ALPHA;break;
		case CURSOR_INDEX_EXT  :gScreen.mwCursorType = CURSOR_INDEX_SHEXT;break;
		case CURSOR_INDEX_SHEXT:gScreen.mwCursorType = CURSOR_INDEX_EXT;break;
		case CURSOR_INDEX_SHIFT:gScreen.mwCursorType = CURSOR_INDEX_NORM;break;
		default:gScreen.mwCursorType = CURSOR_INDEX_SHIFT;break;
		}
		return 0;
	case KEY_PRGM_ALPHA:
		switch(gScreen.mwCursorType)
		{
		case CURSOR_INDEX_ALPHA:gScreen.mwCursorType = CURSOR_INDEX_NORM;break;
		case CURSOR_INDEX_SHIFT:gScreen.mwCursorType = CURSOR_INDEX_SHALP;break;
		default:gScreen.mwCursorType = CURSOR_INDEX_ALPHA;break;
		}
		return 0;
	case KEY_PRGM_OPTN:
		switch(gScreen.mwCursorType)
		{
		case CURSOR_INDEX_CTRL:gScreen.mwCursorType = CURSOR_INDEX_NORM;break;
		default:gScreen.mwCursorType = CURSOR_INDEX_CTRL;break;
		}
		return 0;
	case KEY_PRGM_POWER:
		switch(gScreen.mwCursorType)
		{
		case CURSOR_INDEX_SHIFT:gScreen.mwCursorType = CURSOR_INDEX_SHEXT;break;
		case CURSOR_INDEX_EXT:gScreen.mwCursorType = CURSOR_INDEX_NORM;break;
		default:gScreen.mwCursorType = CURSOR_INDEX_EXT;break;
		}
		return 0;
	}
	i = 0;
	while(i<KEY_COUNT)
	{
		if(gKeyTable[i].miPRGMKeyCode == cRawChar)
		{
			return gKeyTable[i].mcKeys[gScreen.mwCursorType];
		}
		i++;
	}
	assert();
	return 0;
}

int putchar(char c)
{
	HideCursor();
	vtparse((unsigned char)c);
	gScreen.mcDirtyFlag = 1;
	return (unsigned char)c;
}

int puts(const char *pszText)
{
	HideCursor();
	while(pszText[0])
	{
		vtparse((unsigned char)pszText[0]);
		pszText++;
	}
	gScreen.mcDirtyFlag = 1;
	return 1;
}


void EnableCursor(int iShow)
{
	if(iShow)
	{
		gScreen.mwCursorState = CURSOR_STATE_HIDDEN;
		ShowCursor();
	}
	else
	{
		HideCursor();
		gScreen.mwCursorState = CURSOR_STATE_DISABLED;
	}
}
