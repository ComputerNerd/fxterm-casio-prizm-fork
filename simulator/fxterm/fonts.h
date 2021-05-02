#ifndef FONTS_H
#define FONTS_H

typedef unsigned char FONTDATA;

#define CURSOR_INDEX_NORM	0
#define CURSOR_INDEX_ALPHA	1
#define CURSOR_INDEX_SHIFT	2
#define CURSOR_INDEX_SHALP	3
#define CURSOR_INDEX_CTRL	4
#define CURSOR_INDEX_EXT	5
#define CURSOR_INDEX_SHEXT	6
#define CURSOR_COUNT		7


typedef struct
{
	char mcWidth;
	char mcHeight;
	char mcDataHeight;
	unsigned short mwCharacterCount;
	const FONTDATA *mpcFontData;
	const FONTDATA *mpcCursorData;
}FONT;


extern const FONT gFont8x8;
extern const FONT gFont6x8;
extern const FONT gFont5x6;
extern const FONT gFont4x6;

#endif //#ifndef FONTS_H
