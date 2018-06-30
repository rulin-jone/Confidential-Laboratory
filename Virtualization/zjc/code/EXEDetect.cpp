#include "stdafx.h"
#include <Tlhelp32.h>
bool CheckVMWare_exe()
{
	DWORD ret = 0; 
	PROCESSENTRY32 pe32; //定义用来存放快照进程信息的结构体
	pe32.dwSize = sizeof(pe32); 
	HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0); //获取系统中所有当前运行进程的快照
	if (hProcessSnap == INVALID_HANDLE_VALUE) return true; //获取进程失败
	bool bMore = Process32First(hProcessSnap, &pe32); 
	while (bMore)
	{
		if (_tcscmp(pe32.szExeFile, L"vmware.exe") == 0) return false;
		bMore = Process32Next(hProcessSnap, &pe32);
	} //遍历快照中所有进程信息，检测是否含有vmware.exe的进程
	CloseHandle(hProcessSnap);
	return true;
}
bool CheckVB_exe()
{
	DWORD ret = 0;
	PROCESSENTRY32 pe32;
	pe32.dwSize = sizeof(pe32);
	HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hProcessSnap == INVALID_HANDLE_VALUE) return true;
	bool bMore = Process32First(hProcessSnap, &pe32);
	while (bMore)
	{
		if (_tcscmp(pe32.szExeFile, L"VBoxService.exe") == 0) return false;
		bMore = Process32Next(hProcessSnap, &pe32);
	}
	CloseHandle(hProcessSnap);
}
bool EXEDetect()
{
	printf("EXEDetect......");
	bool answer1_exe = CheckVMWare_exe();
	bool answer2_exe = CheckVB_exe;
	printf("            Done.\n");
	if (answer1_exe == false || answer2_exe == false) return false;
	else return true;
}