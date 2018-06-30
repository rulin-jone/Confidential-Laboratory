#include "stdafx.h"
bool TimeDetect()
{
	printf("TimeDetect......");
	//通过rdtsc指令测算执行一个指令所消耗的时间
	__asm
	{
		rdtsc
		xchg ebx,eax
		rdtsc
		sub eax,ebx
		cmp eax,0xff
		jg detected
	}
	printf("           Done.\n");
	return true;
detected:
	return false;
}