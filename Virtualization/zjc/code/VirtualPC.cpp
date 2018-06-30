#include "VirtualPC.h"

/*
Check for process list
*/

VOID virtual_pc_process()
{
	TCHAR *szProcesses[] = {
		_T("VMSrvc.exe"),
		_T("VMUSrvc.exe"),
	};

	WORD iLength = sizeof(szProcesses) / sizeof(szProcesses[0]);
	for (int i = 0; i < iLength; i++)
	{
		TCHAR msg[256] = _T("");
		_stprintf_s(msg, sizeof(msg) / sizeof(TCHAR), _T("Checking Virtual PC processes %s: "), szProcesses[i]);
		if (GetProcessIdFromName(szProcesses[i]))
			print_results(TRUE, msg);
		else
			print_results(FALSE, msg);
	}
}



VOID virtual_pc_reg_keys()
{
	/* Array of strings of blacklisted registry keys */
	TCHAR* szKeys[] = {
		_T("SOFTWARE\\Microsoft\\Virtual Machine\\Guest\\Parameters"),
	};

	WORD dwlength = sizeof(szKeys) / sizeof(szKeys[0]);

	/* Check one by one */
	for (int i = 0; i < dwlength; i++)
	{
		TCHAR msg[256] = _T("");
		_stprintf_s(msg, sizeof(msg) / sizeof(TCHAR), _T("Checking reg key %s: "), szKeys[i]);
		if (Is_RegKeyExists(HKEY_LOCAL_MACHINE, szKeys[i]))
			print_results(TRUE, msg);
		else
			print_results(FALSE, msg);
	}
}

DWORD IsInsideVPC_exceptionFilter(LPEXCEPTION_POINTERS ep)
{
	PCONTEXT ctx = ep->ContextRecord;
	ctx->Ebx = -1;
	ctx->Eip += 4;
	return EXCEPTION_CONTINUE_EXECUTION;
}
BOOL ISA()
{
	bool rc = TRUE;
	__try
	{
		__asm
		{
			push ebx
			mov ebx, 0
			mov eax, 1
			__emit 0fh
			__emit 3fh
			__emit 07h
			__emit 0bh
			test ebx, ebx
			setz[rc]
			pop ebx
		}
	}
	__except (IsInsideVPC_exceptionFilter(GetExceptionInformation()))
	{
		rc = FALSE;
	}
	return rc;
}