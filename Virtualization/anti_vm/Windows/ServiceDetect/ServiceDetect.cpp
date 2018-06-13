#include <stdio.h>
#include <Windows.h>
#include <iostream>
using namespace std;
bool CheckVM_service()
{
	int menu = 0;
	SC_HANDLE SCMan = OpenSCManager(NULL, NULL, SC_MANAGER_ENUMERATE_SERVICE); //打开指定的服务控制管理器数据库
	if (SCMan == NULL)
	{
		cout << GetLastError() << endl;
		cout << "OpenSCManager Error" << endl;
		return -1;
	}
	//保存系统服务的结构
	LPENUM_SERVICE_STATUSA service_status;
	DWORD cbBytesNeeded = NULL;
	DWORD ServicesReturned = NULL;
	DWORD ResumeHandle = NULL;
	service_status = (LPENUM_SERVICE_STATUSA)LocalAlloc(LPTR, 1024 * 64);
	//获取系统服务的简单信息
	bool ESS = EnumServicesStatusA(SCMan, //系统服务句柄
		SERVICE_WIN32,//服务类型
		SERVICE_STATE_ALL, //服务的状态
		(LPENUM_SERVICE_STATUSA)service_status, //输出参数，系统服务的结构
		1024 * 64, //结构的大小
		&cbBytesNeeded, //输出参数，接受返回所需的服务
		&ServicesReturned, //输出参数，接受返回服务的数量
		&ResumeHandle); //输入输出参数，第一次调用必须为0，返回为0代表成功
	if (ESS == NULL)
	{
		cout << "EnumServicesStatus Error" << endl;
		return -1;
	}
	for (int i = 0; i < ServicesReturned; i++) //查找返回的服务中是否有虚拟机标志服务
	{
		if (strstr(service_status[i].lpDisplayName, "VMware Tools") != NULL ||
			strstr(service_status[i].lpDisplayName, "VMware 物理磁盘助手服务") != NULL)
			return false;
	}
	//关闭服务管理器的句柄
	CloseServiceHandle(SCMan);
	return true;
}
bool CheckVirtualPC_service()
{
	int menu = 0;
	//打开系统服务控制器
	SC_HANDLE SCMan = OpenSCManager(NULL, NULL, SC_MANAGER_ENUMERATE_SERVICE);
	if (SCMan == NULL)
	{
		cout << GetLastError() << endl
			<< "OpenSCManager Error" << endl;
		return -1;
	}
	//保存系统服务的结构
	LPENUM_SERVICE_STATUSA service_status;
	DWORD cbBytesNeeded = NULL;
	DWORD ServicesReturned = NULL;
	DWORD ResumeHandle = NULL;
	service_status = (LPENUM_SERVICE_STATUSA)LocalAlloc(LPTR, 1024 * 64);
	//获取系统服务的简单信息
	bool ESS = EnumServicesStatusA(SCMan, //系统服务句柄
		SERVICE_WIN32, //服务的类型
		SERVICE_STATE_ALL, //服务的状态
		(LPENUM_SERVICE_STATUSA)service_status, //输出参数，系统服务的结构
		1024 * 64, //结构的大小
		&cbBytesNeeded, //输出参数，接受返回所需的服务
		&ServicesReturned, //输出参数，接受返回服务的数量
		&ResumeHandle); //输入输出参数，第一次调用必须为0，返回为0代表成功
	if (ESS == NULL)
	{
		cout << "EnumServicesStatus" << endl;
		return -1;
	}
	for (int i = 0; i < ServicesReturned; i++)
	{
		if (strstr(service_status[i].lpDisplayName, "Virtual Machine") != NULL) return false;
	}
	//关闭服务管理器的句柄
	CloseServiceHandle(SCMan);
	return true;
}
bool CheckVB_service()
{
	int menu = 0;
	//打开系统服务控制器
	SC_HANDLE SCMan = OpenSCManager(NULL, NULL, SC_MANAGER_ENUMERATE_SERVICE);
	if (SCMan == NULL)
	{
		cout << GetLastError() << endl
			<< "OpenSCManager Error" << endl;
		return -1;
	}
	//保存系统服务的结构
	LPENUM_SERVICE_STATUSA service_status;
	DWORD cbBytesNeeded = NULL;
	DWORD ServicesReturned = NULL;
	DWORD ResumeHandle = NULL;
	service_status = (LPENUM_SERVICE_STATUSA)LocalAlloc(LPTR, 1024 * 64);
	//获取系统服务的简单信息
	bool ESS = EnumServicesStatusA(SCMan, //系统服务句柄
		SERVICE_WIN32, //服务的类型
		SERVICE_STATE_ALL, //服务的状态
		(LPENUM_SERVICE_STATUSA)service_status, //输出参数，系统服务的结构
		1024 * 64, //结构的大小
		&cbBytesNeeded, //输出参数，接受返回所需的服务
		&ServicesReturned, //输出参数，接受返回服务的数量
		&ResumeHandle); //输入输出参数，第一次调用必须为0，返回为0代表成功
	if (ESS == NULL)
	{
		cout << "EnumServicesStatus Error" << endl;
		return -1;
	}
	for (int i = 0; i < ServicesReturned; i++)
	{
		if (strstr(service_status[i].lpDisplayName, "VirtualBox Guest") != NULL) return false;
	}
	//关闭服务管理器的句柄
	CloseServiceHandle(SCMan);
	return true;
}
bool ServiceDetect()
{
	printf("Service Detect......");
	bool answer1_service = CheckVM_service();
	bool answer2_service = CheckVirtualPC_service();
	bool answer3_service = CheckVB_service();
	printf("       Done.\n");
	if (answer1_service == false || answer2_service == false || answer3_service == false) return false;
	else return true;
}