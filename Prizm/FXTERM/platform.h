#ifndef PLATFORM_H
#define PLATFORM_H
typedef struct{
    int x;
    int y;
    int xofs;
    int yofs;
    int width;
    int height;
    char colormode;
    char zero4;
    char P20_1;
    char P20_2;
    int bitmap;
    char color_idx1;
    char color_idx2;
    char color_idx3;
    char P20_3;
    char writemodify;
    char writekind;
    char zero6;
    char one1;
    int transparency;
} TDispGraph;

#include <fxcg/display.h>
#include <fxcg/keyboard.h>
#include <fxcg/rtc.h>
#include <fxcg/serial.h>
#include <stdlib.h>
#include <string.h>



#define WIDTH 384
#define HEIGHT 216
#define dbgprint(a) 
#define dbgprint1(a,b)
#define dbgprint2(a,b,c)
#define dbgprint3(a,b,c,d)
#define dbgprint4(a,b,c,d,e)

#define KEY_PRGM_OPTN 68
#define KEY_PRGM_MENU 48
#define KEY_PRGM_POWER 57

typedef int (*TSysCallFuncPtr)( int R4, int R5, int R6, int R7, int FNo );
extern TSysCallFuncPtr fnSysCallFuncPtr;


/*
//syscall 0x1BB7: int Serial_Open( unsigned char *mode );
#define Serial_Open(mode) fnSysCallFuncPtr((int)mode,0,0,0,0x1BB7)
//syscall 0x1BBC: int Serial_BufferedTransmitOneByte( unsigned char byte_to_transmit );
#define Serial_BufferedTransmitOneByte(byte_to_transmit) fnSysCallFuncPtr((int)byte_to_transmit,0,0,0,0x1BBC)
//syscall 0x1BB8: int Serial_Close( int mode );
#define Serial_Close(mode) fnSysCallFuncPtr((int)mode,0,0,0,0x1BB8)
//syscall 0x1BB9: int Serial_ReadOneByte( unsigned char*result );
#define Serial_ReadOneByte(pResult) fnSysCallFuncPtr((int)pResult,0,0,0,0x1BB9)
*/

#define EmulHack_Sleep(x)


#endif //#ifndef PLATFORM_H
