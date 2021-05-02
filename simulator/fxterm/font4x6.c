#include "fonts.h"
#define FONT_HEIGHT 6
#define FONT_DATA_HEIGHT 5
#define FONT_WIDTH 4
#define FONT_CHAR_COUNT	128

#include "deffnt4.h"

static const FONTDATA gCursors4x6Buffer[FONT_HEIGHT * CURSOR_COUNT]=
{
//CURSOR_INDEX_NORM	
	XXXX,
	XXXX,
	XXXX,
	XXXX,
	XXXX,
	XXXX,
//CURSOR_INDEX_ALPHA
	XXXX,
	XXXX,
	XXXX,
	XXXX,
	XXXX,
	XXXX,
//CURSOR_INDEX_SHIFT
	XXXX,
	XXXX,
	XXXX,
	XXXX,
	XXXX,
	XXXX,
//CURSOR_INDEX_SHALP	
	XXXX,
	XXXX,
	XXXX,
	XXXX,
	XXXX,
	XXXX,
//CURSOR_INDEX_CTRL	
	XXXX,
	XXXX,
	XXXX,
	XXXX,
	XXXX,
	XXXX,
//CURSOR_INDEX_EXT
	XXXX,
	XXXX,
	XXXX,
	XXXX,
	XXXX,
	XXXX,
//CURSOR_INDEX_SHEXT
	XXXX,
	XXXX,
	XXXX,
	XXXX,
	XXXX,
	XXXX,
};
#define FONT_TODO ____,____,____,____,____,

static const FONTDATA gFont4x6Buffer[FONT_DATA_HEIGHT * (FONT_CHAR_COUNT)]=
{
//[0]= 
FONT_TODO
//[1]=
FONT_TODO
//[2]=
FONT_TODO
//[3]=
FONT_TODO
//[4]=
FONT_TODO
//[5]=
FONT_TODO
//[6]=
FONT_TODO
//[7]=
FONT_TODO
//[8]=
FONT_TODO
//[9]=	
FONT_TODO
//[10]=
FONT_TODO
//[11]=
FONT_TODO
//[12]=
FONT_TODO
//[13]=
FONT_TODO
//[14]=
FONT_TODO
//[15]=
FONT_TODO
//[16]=
FONT_TODO
//[17]=
FONT_TODO
//[18]=
FONT_TODO
//[19]=
FONT_TODO
//[20]=
FONT_TODO
//[21]=
FONT_TODO
//[22]=
FONT_TODO
//[23]=
FONT_TODO
//[24]=
FONT_TODO
//[25]=
FONT_TODO
//[26]=
FONT_TODO
//[27]=
FONT_TODO
//[28]=
FONT_TODO
//[29]=
FONT_TODO
//[30]=
FONT_TODO
//[31]=
FONT_TODO
//[32]= 
____,
____,
____,
____,
____,
//[33]=!
_X__,
_X__,
_X__,
____,
_X__,
//[34]="
X_X_,
X_X_,
____,
____,
____,
//[35]=#
X_X_,
XXX_,
X_X_,
XXX_,
X_X_,
//[36]=$
_XX_,
XX__,
XXX_,
_XX_,
XX__,
//[37]=%
X___,
__X_,
_X__,
X___,
__X_,
//[38]=&
_XX_,
X_X_,
_X__,
XXX_,
XX_X,
//[39]='
XX__,
_X__,
____,
____,
____,
//[40]=(
_X__,
X___,
X___,
X___,
_X__,
//[41]=)
_X__,
__X_,
__X_,
__X_,
_X__,
//[42]=*
X_X_,
_X__,
XXX_,
_X__,
X_X_,
//[43]=+
____,
_X__,
XXX_,
_X__,
____,
//[44]=,
____,
____,
____,
XX__,
_X__,
//[45]=-
____,
____,
XXX_,
____,
____,
//[46]=.
____,
____,
____,
XX__,
XX__,
//[47]=/
__X_,
__X_,
_X__,
X___,
X___,
//[48]=0
_XX_,
X_X_,
X_X_,
X_X_,
XX__,
//[49]=1
_X__,
XX__,
_X__,
_X__,
XXX_,
//[50]=2
XX__,
__X_,
_X__,
X___,
XXX_,
//[51]=3
XX__,
__X_,
_X__,
__X_,
XX__,
//[52]=4
__X_,
X_X_,
X_X_,
XXX_,
__X_,
//[53]=5
XXX_,
X___,
XX__,
__X_,
_X__,
//[54]=6
_X__,
X___,
XX__,
X_X_,
_X__,
//[55]=7
XXX_,
__X_,
_XX_,
_X__,
_X__,
//[56]=8
_X__,
X_X_,
_X__,
X_X_,
_X__,
//[57]=9
_X__,
X_X_,
XX__,
__X_,
_X__,
//[58]=:
____,
_X__,
____,
_X__,
____,
//[59]=;
____,
_X__,
____,
_X__,
X___,
//[60]=<
__X_,
_X__,
X___,
_X__,
__X_,
//[61]==
____,
____,
XXX_,
____,
XXX_,
//[62]=>
X___,
_X__,
__X_,
_X__,
X___,
//[63]=?
XX__,
__X_,
_X__,
____,
_X__,
//[64]=@
XXX_,
X_X_,
X_X_,
X___,
XXX_,
//[65]=A
_X__,
X_X_,
XXX_,
X_X_,
X_X_,
//[66]=B
XX__,
X_X_,
XX__,
X_X_,
XX__,
//[67]=C
_XX_,
X___,
X___,
X___,
_XX_,
//[68]=D
XX__,
X_X_,
X_X_,
X_X_,
XX__,
//[69]=E
XXX_,
X___,
XX__,
X___,
XXX_,
//[70]=F
XXX_,
X___,
XX__,
X___,
X___,
//[71]=G
_XX_,
X___,
X_X_,
X_X_,
_XX_,
//[72]=H
X_X_,
X_X_,
XXX_,
X_X_,
X_X_,
//[73]=I
XXX_,
_X__,
_X__,
_X__,
XXX_,
//[74]=J
__X_,
__X_,
__X_,
X_X_,
_X__,
//[75]=K
X_X_,
X_X_,
XX__,
X_X_,
X_X_,
//[76]=L
X___,
X___,
X___,
X___,
XXX_,
//[77]=M
X_X_,
XXX_,
XXX_,
X_X_,
X_X_,
//[78]=N
X_X_,
XXX_,
X_X_,
X_X_,
X_X_,
//[79]=O
_X__,
X_X_,
X_X_,
X_X_,
_X__,
//[80]=P
XX__,
X_X_,
XX__,
X___,
X___,
//[81]=Q
_X__,
X_X_,
X_X_,
XXX_,
_XX_,
//[82]=R
XX__,
X_X_,
XX__,
X_X_,
X_X_,
//[83]=S
_XX_,
X___,
XXX_,
__X_,
XX__,
//[84]=T
XXX_,
_X__,
_X__,
_X__,
_X__,
//[85]=U
X_X_,
X_X_,
X_X_,
X_X_,
XXX_,
//[86]=V
X_X_,
X_X_,
X_X_,
X_X_,
_X__,
//[87]=W
X_X_,
X_X_,
XXX_,
XXX_,
X_X_,
//[88]=X
X_X_,
X_X_,
_X__,
X_X_,
X_X_,
//[89]=Y
X_X_,
X_X_,
_X__,
_X__,
_X__,
//[90]=Z
XXX_,
__X_,
_X__,
X___,
XXX_,
//[91]=[
_XX_,
_X__,
_X__,
_X__,
_XX_,
//[92]=\ 
X___,
_X__,
__X_,
___X,
____,
//[93]=]
_XX_,
__X_,
__X_,
__X_,
_XX_,
//[94]=^
_X__,
X_X_,
____,
____,
____,
//[95]=_
____,
____,
____,
____,
XXXX,
//[96]=`
_XX_,
_X__,
____,
____,
____,
//[97]=a
____,
____,
_XX_,
X_X_,
XXX_,
//[98]=b
X___,
X___,
XX__,
X_X_,
XX__,
//[99]=c
____,
____,
_XX_,
X___,
_XX_,
//[100]=d
__X_,
__X_,
_XX_,
X_X_,
_XX_,
//[101]=e
____,
____,
XXX_,
XX__,
_XX_,
//[102]=f
__X_,
_X__,
XXX_,
_X__,
_X__,
//[103]=g
____,
XXX_,
X_X_,
__X_,
XXX_,
//[104]=h
X___,
X___,
XX__,
X_X_,
X_X_,
//[105]=i
_X__,
____,
_X__,
_X__,
_X__,
//[106]=j
__X_,
____,
__X_,
__X_,
XXX_,
//[107]=k
X___,
X___,
X_X_,
XX__,
X_X_,
//[108]=l
XX__,
_X__,
_X__,
_X__,
XXX_,
//[109]=m
____,
____,
XXX_,
XXX_,
X_X_,
//[110]=n
____,
____,
XX__,
X_X_,
X_X_,
//[111]=o
____,
____,
_X__,
X_X_,
_X__,
//[112]=p
____,
XX__,
X_X_,
XX__,
X___,
//[113]=q
____,
_XX_,
X_X_,
_XX_,
__X_,
//[114]=r
____,
____,
XX__,
X___,
X___,
//[115]=s
____,
____,
_XX_,
_X__,
XX__,
//[116]=t
X___,
XX__,
X___,
X_X_,
_X__,
//[117]=u
____,
____,
X_X_,
X_X_,
XXX_,
//[118]=v
____,
____,
X_X_,
X_X_,
_X__,
//[119]=w
____,
____,
X_X_,
XXX_,
XXX_,
//[120]=x
____,
____,
X_X_,
_X__,
X_X_,
//[121]=y
____,
____,
X_X_,
_X__,
X___,
//[122]=z
____,
____,
XX__,
_X__,
_XX_,
//[123]={
_XX_,
_X__,
X___,
_X__,
_XX_,
//[124]=|
_X__,
_X__,
_X__,
_X__,
_X__,
//[125]=}
XX__,
_X__,
__X_,
_X__,
XX__,
//[126]=~
____,
_X_X,
X_X_,
____,
____,
//[127]=
____,
_X__,
X_X_,
X_X_,
XXX_,

};

const FONT gFont4x6 = {FONT_WIDTH,FONT_HEIGHT,FONT_DATA_HEIGHT,FONT_CHAR_COUNT,gFont4x6Buffer,gCursors4x6Buffer};

#undef FONT_HEIGHT 
#undef FONT_DATA_HEIGHT
#undef FONT_WIDTH
#undef FONT_CHAR_COUNT
#undef FONT_TODO