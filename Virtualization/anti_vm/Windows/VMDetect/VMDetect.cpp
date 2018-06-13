#include <stdio.h>
#include <Windows.h>
bool IsInsideVMWare()
{
	bool rc = true;

	__try
	{
		__asm
		{
			push   edx
			push   ecx
			push   ebx

			mov    eax, 'VMXh'
			mov    ebx, 0
			mov    ecx, 10 //指定功能号，用于获取VMWare版本
			mov    edx, 'VX' //端口号
			in     eax, dx //从端口dx读取版本到eax

			cmp    ebx, 'VMXh' //判断ebx中是否包含VMWare版本
			setz[rc]

			pop    ebx
			pop    ecx
			pop    edx
		}
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		rc = false;
	}

	return rc;
}
bool VMDetect()
{
	printf("VMware Detect......");
	bool rc;
	rc = IsInsideVMWare();
	printf("        Done.\n");
	if (rc == false) return true;
	else return false;
}