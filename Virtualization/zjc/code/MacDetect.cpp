#include "stdafx.h"
#include<wtypes.h>
#include<nb30.h>
#include<wincon.h>
#pragma comment(lib,"netapi32.lib")
using namespace std;
typedef struct _ASTAT_ //定义存放网卡信息的结构体
{
	ADAPTER_STATUS adapt; //网络适配器状态
	NAME_BUFFER NameBuff[30]; //名字表
} ASTAT, *PASTAT;
void get_3part_mac(string &mac)
{
	NCB Ncb; //定义一个NCB（网络控制块）类型的结构体变量Ncb
	ASTAT Adapter;
	UCHAR uRetCode; //执行Netbios()函数的返回值
	LANA_ENUM lenum; //定义结构体，存放mac地址
	memset(&Ncb, 0, sizeof(Ncb));
	Ncb.ncb_command = NCBENUM; //统计系统中网卡的数量
	Ncb.ncb_buffer = (UCHAR *)&lenum; //ncb_buffer指向由LANA_ENUM结构填充的缓冲区
	Ncb.ncb_length = sizeof(lenum);
	uRetCode = Netbios(&Ncb); //获取网卡序列号
	for (int i = 0; i < lenum.length; i++) //对每一个网卡，以网卡编号为输入编号，获取mac地址
	{
		memset(&Ncb, 0, sizeof(Ncb));
		Ncb.ncb_command = NCBRESET; //对网卡发送NCBRESET命令，进行初始化
		Ncb.ncb_lana_num = lenum.lana[i]; //指定网卡号
		uRetCode = Netbios(&Ncb);
		memset(&Ncb, 0, sizeof(Ncb));
		Ncb.ncb_command = NCBASTAT; //对网卡发送NCBSTAT命令，获取网卡信息
		Ncb.ncb_lana_num = lenum.lana[i];
		strcpy_s((char *)Ncb.ncb_callname,10, "*"); //远程系统名赋值为*
		Ncb.ncb_buffer = (unsigned char *)&Adapter; //指定返回的信息存放的变量
		Ncb.ncb_length = sizeof(Adapter);
		uRetCode = Netbios(&Ncb); //接着发送NCBSTAT命令，以获取网卡信息
		//将网卡MAC地址格式转化为常用16进制，输出到字符串mac中
		if (uRetCode == 0)
		{
			char tmp[128]; 
			sprintf_s(tmp,128, "%02x-%02x-%02x",
				Adapter.adapt.adapter_address[0],
				Adapter.adapt.adapter_address[1],
				Adapter.adapt.adapter_address[2]
			);
			mac = tmp;
		}
	}
}
bool CheckVMWare()
{
	string mac;
	get_3part_mac(mac);
	if (mac == "00-05-69" || mac == "00-0c-29" || mac == "00-50-56")
	{
		return false;
	}
	else
	{
		return true;
	}
}
bool CheckVirtualPC()
{
	string mac;
	get_3part_mac(mac);
	if (mac == "00-03-ff")
	{
		return false;
	}
	else
	{
		return true;
	}
}
bool CheckVirtualBox()
{
	string mac;
	get_3part_mac(mac);
	if (mac == "08-00-27")
	{
		return false;
	}
	else
	{
		return true;
	}
}
bool CheckVirtualMachine()
{
	printf("MacDetect......");
	bool answer1 = CheckVirtualPC();
	bool answer2 = CheckVMWare();
	bool answer3 = CheckVirtualBox();
	printf("            Done.\n");
	if (answer1 == false || answer2 == false || answer3 == false) return false;
	else return true;
}