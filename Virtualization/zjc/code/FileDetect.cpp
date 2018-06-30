#include "stdafx.h"
#include <Shlwapi.h>
#pragma comment(lib,"shlwapi.lib")
bool CheckVM_file()
{
	if (PathIsDirectory((LPCTSTR)"C:\\Program Files\\VMware\\VMware Tools\\") == 0) return true; //检查通常虚拟机软件存放的文件目录是否存在
	else return false;
}
bool CheckVB_file()
{
	if (PathIsDirectory((LPCTSTR)"C:\\Program Files\\Oracle\\VirtualBox Guest Additions\\") == 0) return true;
	else return false;
}
bool FileDetect()
{
	printf("FileDetect......");
	bool answer1_file = CheckVM_file();
	bool answer2_file = CheckVB_file();
	printf("           Done.\n");
	if (answer1_file == false || answer2_file == false) return false;
	else return true;
}