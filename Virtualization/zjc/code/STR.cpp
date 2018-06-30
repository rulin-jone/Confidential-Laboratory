#include "stdafx.h"
bool STRDetect()
{
	printf("STRDetect......");
	unsigned char mem[4] = { 0 };
	_asm str mem; //调用str指令，将str表基址放入mem中
	printf("            Done.\n");
	if ((mem[0] == 0x00) && (mem[1] == 0x40)) return false;
	else return true;
}