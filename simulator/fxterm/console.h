#include "fonts.h"

int putcharFxterm(char c);

int putsFxterm(const char *pszText);

#define CK_UP		'\20'
#define CK_DOWN		'\16'
#define CK_LEFT		'\2'
#define CK_RIGHT	'\6'
#define	CK_ENTER	'\n'
#define	CK_ESC		'\33'

char TranslateChar(char cRawChar);

extern unsigned short *VRAM;
void InitScreen(const FONT *pFont);
void EnableCursor(int iShow);

