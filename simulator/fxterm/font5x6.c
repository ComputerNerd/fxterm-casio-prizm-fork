#include "fonts.h"
#define FONT_HEIGHT 6
#define FONT_DATA_HEIGHT 5
#define FONT_WIDTH 5
#define FONT_CHAR_COUNT 128

#include "deffnt5.h"

static const FONTDATA gCursors5x6Buffer[FONT_HEIGHT * CURSOR_COUNT]=
{
//CURSOR_INDEX_NORM	
	XXXXX,
	XXXXX,
	XXXXX,
	XXXXX,
	XXXXX,
	XXXXX,
//CURSOR_INDEX_ALPHA
	X__XX,
	_XX_X,
	____X,
	_XX_X,
	_XX_X,
	XXXXX,
//CURSOR_INDEX_SHIFT
	XXXXX,
	XXXXX,
	XXXXX,
	XXXXX,
	XXXXX,
	XXXXX,
//CURSOR_INDEX_SHALP	
	XXXXX,
	XXXXX,
	XXXXX,
	XXXXX,
	XXXXX,
	XXXXX,
//CURSOR_INDEX_CTRL	
	X__XX,
	_XX_X,
	_XXXX,
	_XX_X,
	X__XX,
	XXXXX,
//CURSOR_INDEX_EXT
	X____,
	X_XXX,
	X___X,
	X_XXX,
	X____,
	XXXXX,
//CURSOR_INDEX_SHEXT
	XXXXX,
	XXXXX,
	XXXXX,
	XXXXX,
	XXXXX,
	XXXXX,
};

#define FONT_TODO _____,_____,_____,_____,_____,

static const FONTDATA gFont5x6Buffer[FONT_DATA_HEIGHT * (1*FONT_CHAR_COUNT+0)]=
{
	FONT_TODO//[0]=
	FONT_TODO//[1]=
	FONT_TODO//[2]=
	FONT_TODO//[3]=
	FONT_TODO//[4]=
	FONT_TODO//[5]=
	FONT_TODO//[6]=
	FONT_TODO//[7]=
	FONT_TODO//[8]=
	FONT_TODO//[9]=
	FONT_TODO//[10]=
	FONT_TODO//[11]=
	FONT_TODO//[12]=
	FONT_TODO//[13]=
	FONT_TODO//[14]=
	FONT_TODO//[15]=
	FONT_TODO//[16]=
	FONT_TODO//[17]=
	FONT_TODO//[18]=
	FONT_TODO//[19]=
	FONT_TODO//[20]=
	FONT_TODO//[21]=
	FONT_TODO//[22]=
	FONT_TODO//[23]=
	FONT_TODO//[24]=
	FONT_TODO//[25]=
	FONT_TODO//[26]=
	FONT_TODO//[27]=
	FONT_TODO//[28]=
	FONT_TODO//[29]=
	FONT_TODO//[30]=
	FONT_TODO//[31]=
	//[32]=
_____,
_____,
_____,
_____,
_____,
//[33]=!
__X__,
__X__,
__X__,
_____,
__X__,
//[34]="
_X_X_,
_X_X_,
_____,
_____,
_____,
//[35]=#
_X_X_,
XXXXX,
_X_X_,
XXXXX,
_X_X_,
//[36]=$
_XXXX,
X_X__,
_XXX_,
__X_X,
XXXX_,
//[37]=%
_X__X,
___X_,
__X__,
_X___,
X__X_,
//[38]=&
_XX__,
X_X__,
_X___,
X_X_X,
_X_X_,
//[39]='
_XX__,
__X__,
_____,
_____,
_____,
//[40]=(
__X__,
_X___,
_X___,
_X___,
__X__,
//[41]=)
_X___,
__X__,
__X__,
__X__,
_X___,
//[42]=*
__X__,
X_X_X,
_XXX_,
X_X_X,
__X__,
//[43]=+
_____,
__X__,
_XXX_,
__X__,
_____,
//[44]=,
_____,
_____,
_____,
_XX__,
__X__,
//[45]=-
_____,
_____,
XXXX_,
_____,
_____,
//[46]=.
_____,
_____,
_____,
_XX__,
_XX__,
//[47]=/
____X,
___X_,
__X__,
_X___,
X____,
//[48]=0
_XXX_,
X_XX_,
XX_X_,
X__X_,
_XX__,
//[49]=1
__X__,
_XX__,
__X__,
__X__,
_XXX_,
//[50]=2
_XX__,
X__X_,
__X__,
_X___,
XXXX_,
//[51]=3
XXX__,
__X__,
_X___,
__X__,
XXX__,
//[52]=4
__X__,
_XX__,
X_X__,
XXXX_,
__X__,
//[53]=5
XXXX_,
X____,
XXX__,
___X_,
_XX__,
//[54]=6
_XX__,
X____,
XXX__,
X__X_,
_XX__,
//[55]=7
XXXX_,
___X_,
__X__,
_X___,
_X___,
//[56]=8
_XX__,
X__X_,
_XX__,
X__X_,
_XX__,
//[57]=9
_XX__,
X__X_,
_XXX_,
___X_,
_XX__,
//[58]=:
_____,
_XX__,
_____,
_XX__,
_____,
//[59]=;
_____,
_XX__,
_____,
__X__,
_X___,
//[60]=<
__X__,
_X___,
X____,
_X___,
__X__,
//[61]==
_____,
XXXX_,
_____,
XXXX_,
_____,
//[62]=>
_X___,
__X__,
___X_,
__X__,
_X___,
//[63]=?
XXX__,
___X_,
__X__,
_____,
__X__,
//[64]=@
XXX__,
___X_,
XX_X_,
XX_X_,
XXX__,
//[65]=A
_XX__,
X__X_,
XXXX_,
X__X_,
X__X_,
//[66]=B
XXX__,
X__X_,
XXX__,
X__X_,
XXX__,
//[67]=C
_XX__,
X__X_,
X____,
X__X_,
_XX__,
//[68]=D
XX___,
X_X__,
X__X_,
X_X__,
XX___,
//[69]=E
XXXX_,
X____,
XXX__,
X____,
XXXX_,
//[70]=F
XXXX_,
X____,
XXX__,
X____,
X____,
//[71]=G
_XX__,
X____,
X_XX_,
X__X_,
_XXX_,
//[72]=H
X__X_,
X__X_,
XXXX_,
X__X_,
X__X_,
//[73]=I
_XXX_,
__X__,
__X__,
__X__,
_XXX_,
//[74]=J
_XXX_,
___X_,
___X_,
X__X_,
_XX__,
//[75]=K
X__X_,
X_X__,
XX___,
X_X__,
X__X_,
//[76]=L
X____,
X____,
X____,
X____,
XXXX_,
//[77]=M
X___X,
XX_XX,
X_X_X,
X___X,
X___X,
//[78]=N
X__X_,
XX_X_,
X_XX_,
X__X_,
X__X_,
//[79]=O
_XX__,
X__X_,
X__X_,
X__X_,
_XX__,
//[80]=P
XXX__,
X__X_,
XXX__,
X____,
X____,
//[81]=Q
_XX__,
X__X_,
X__X_,
X_X__,
_X_X_,
//[82]=R
XXX__,
X__X_,
XXX__,
X_X__,
X__X_,
//[83]=S
_XXX_,
X____,
_XX__,
___X_,
XXX__,
//[84]=T
_XXX_,
__X__,
__X__,
__X__,
__X__,
//[85]=U
X__X_,
X__X_,
X__X_,
X__X_,
XXXX_,
//[86]=V
X__X_,
X__X_,
X__X_,
_XX__,
__X__,
//[87]=W
X___X,
X___X,
X_X_X,
X_X_X,
_X_X_,
//[88]=X
_X_X_,
_X_X_,
__X__,
_X_X_,
_X_X_,
//[89]=Y
_X_X_,
_X_X_,
__X__,
__X__,
__X__,
//[90]=Z
XXXX_,
__X__,
_X___,
X____,
XXXX_,
//[91]=[
_XX__,
_X___,
_X___,
_X___,
_XX__,
//[92]=\ 
X____,
_X___,
__X__,
___X_,
_____,
//[93]=]
_XX__,
__X__,
__X__,
__X__,
_XX__,
//[94]=^
__X__,
_X_X_,
_____,
_____,
_____,
//[95]=_
_____,
_____,
_____,
_____,
XXXXX,
//[96]=`
_XX__,
_X___,
_____,
_____,
_____,
//[97]=a
_XX__,
___X_,
_XXX_,
X__X_,
_XXX_,
//[98]=b
X____,
X____,
XXX__,
X__X_,
XXX__,
//[99]=c
_____,
_____,
_XX__,
X____,
_XX__,
//[100]=d
___X_,
___X_,
_XXX_,
X__X_,
_XXX_,
//[101]=e
_XX__,
X__X_,
XXXX_,
X____,
_XX__,
//[102]=f
__XX_,
_X___,
XXX__,
_X___,
_X___,
//[103]=g
_XXX_,
X__X_,
_XXX_,
___X_,
_XX__,
//[104]=h
X____,
X____,
XXX__,
X__X_,
X__X_,
//[105]=i
__X__,
_____,
_XX__,
__X__,
_XXX_,
//[106]=j
___X_,
_____,
__XX_,
X__X_,
_XX__,
//[107]=k
_X___,
_X___,
_X_X_,
_XX__,
_X_X_,
//[108]=l
_XX__,
__X__,
__X__,
__X__,
_XXX_,
//[109]=m
_____,
_____,
XX_X_,
X_X_X,
X_X_X,
//[110]=n
_____,
_____,
X_XX_,
XX_X_,
X__X_,
//[111]=o
_____,
_____,
_XX__,
X__X_,
_XX__,
//[112]=p
_____,
XXX__,
X__X_,
XXX__,
X____,
//[113]=q
_____,
_XXX_,
X__X_,
_XXX_,
___X_,
//[114]=r
_____,
X_X__,
XX_X_,
X____,
X____,
//[115]=s
_____,
_XXX_,
_XX__,
___X_,
XXX__,
//[116]=t
_X___,
XXX__,
_X___,
_X_X_,
__X__,
//[117]=u
_____,
_____,
X__X_,
X__X_,
_XXX_,
//[118]=v
_____,
_____,
X__X_,
_XX__,
__X__,
//[119]=w
_____,
_____,
X___X,
X_X_X,
_X_X_,
//[120]=x
_____,
_____,
XX_X_,
__X__,
X_XX_,
//[121]=y
_____,
X__X_,
_XX__,
_X___,
X____,
//[122]=z
_____,
XXXX_,
__X__,
_X___,
XXXX_,
//[123]={
_XX__,
_X___,
X____,
_X___,
_XX__,
//[124]=|
__X__,
__X__,
__X__,
__X__,
__X__,
//[125]=}
_XX__,
__X__,
___X_,
__X__,
_XX__,
//[126]=~
_____,
_X__X,
X_X_X,
X__X_,
_____,
//[127]=
_____,
__X__,
_X_X_,
_X_X_,
_XXX_,
};

const FONT gFont5x6 = {FONT_WIDTH,FONT_HEIGHT,FONT_DATA_HEIGHT,FONT_CHAR_COUNT,gFont5x6Buffer,gCursors5x6Buffer};

#undef FONT_HEIGHT 
#undef FONT_DATA_HEIGHT
#undef FONT_WIDTH
#undef FONT_CHAR_COUNT
#undef FONT_TODO