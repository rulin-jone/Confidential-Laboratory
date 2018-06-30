#include "General.h"

/*
Check if the DLL is loaded in the context of the process
*/
VOID loaded_dlls()
{
	/* Some vars */
	HMODULE hDll = NULL;

	/* Array of strings of blacklisted dlls */
	TCHAR* szDlls[] = {
		_T("sbiedll.dll"),		// Sandboxie
		_T("pstorec.dll"),		// SunBelt Sandbox
		_T("vmcheck.dll"),		// Virtual PC

	};

	WORD dwlength = sizeof(szDlls) / sizeof(szDlls[0]);
	for (int i = 0; i < dwlength; i++)
	{
		TCHAR msg[256] = _T("");
		_stprintf_s(msg, sizeof(msg) / sizeof(TCHAR), _T("Checking if process loaded modules contains: %s "), szDlls[i]);

		/* Check if process loaded modules contains the blacklisted dll */
		hDll = GetModuleHandle(szDlls[i]);
		if (hDll == NULL)
			print_results(FALSE, msg);
		else
		{
			print_results(TRUE, msg);
			hDll = NULL;
		}
	}
}

/*
check the idt address base
*/
BOOL idt_trick()
{
	UINT idt_base = get_idt_base();
	if ((idt_base >> 24) == 0xff)
		return TRUE; //VMWare detected

	else
		return FALSE; 
}

/*
check the ldt address base
*/
BOOL ldt_trick()
{
	UINT ldt_base = get_ldt_base();

	if (ldt_base == 0xdead0000)
		return FALSE;
	else
		return TRUE; // VMWare detected	
}

/*
check the gdt address base
*/
BOOL gdt_trick()
{
	UINT gdt_base = get_gdt_base();

	if ((gdt_base >> 24) == 0xff)
		return TRUE; // VMWare detected	

	else
		return FALSE;
}

/*
check the str address base
*/
BOOL str_trick()
{
	UCHAR *mem = get_str_base();

	if ((mem[0] == 0x00) && (mem[1] == 0x40))
		return TRUE; // VMWare detected	
	else
		return FALSE;
}

/*
check the cpu id
*/
BOOL cpuid_is_hypervisor()
{
	INT CPUInfo[4] = { -1 };

	/* Query hypervisor precense using CPUID (EAX=1), BIT 31 in ECX */
	__cpuid(CPUInfo, 1);
	if ((CPUInfo[2] >> 31) & 1)
		return TRUE;
	else
		return FALSE;
}


/*
Check SerialNumber devices using WMI
*/
BOOL serial_number_bios_wmi()
{
	IWbemServices *pSvc = NULL;
	IWbemLocator *pLoc = NULL;
	IEnumWbemClassObject* pEnumerator = NULL;
	BOOL bStatus = FALSE;
	HRESULT hRes;
	BOOL bFound = FALSE;

	// Init WMI
	bStatus = InitWMI(&pSvc, &pLoc, _T("ROOT\\CIMV2"));

	if (bStatus)
	{
		// If success, execute the desired query
		bStatus = ExecWMIQuery(&pSvc, &pLoc, &pEnumerator, _T("SELECT * FROM Win32_BIOS"));
		if (bStatus)
		{
			// Get the data from the query
			IWbemClassObject *pclsObj = NULL;
			ULONG uReturn = 0;
			VARIANT vtProp;

			while (pEnumerator)
			{
				hRes = pEnumerator->Next(WBEM_INFINITE, 1, &pclsObj, &uReturn);
				if (0 == uReturn)
					break;

				// Get the value of the Name property
				hRes = pclsObj->Get(_T("SerialNumber"), 0, &vtProp, 0, 0);

				// Do our comparaison
				if (
					(StrStrI(vtProp.bstrVal, _T("VMWare")) != 0) ||
					(StrStrI(vtProp.bstrVal, _T("Xen")) != 0) ||
					(StrStrI(vtProp.bstrVal, _T("Virtual")) != 0)
					)
				{
					bFound = TRUE;
					break;
				}

				// release the current result object
				VariantClear(&vtProp);
				pclsObj->Release();
			}

			// Cleanup
			pSvc->Release();
			pLoc->Release();
			pEnumerator->Release();
			CoUninitialize();
		}
	}

	return bFound;
}

/*
check instruction execution time using rdtsc
*/
BOOL Time()
{
	__asm
	{
		rdtsc
		xchg ebx, eax
		rdtsc
		sub eax, ebx
		cmp eax, 0xff
		jg detected
	}
	return FALSE;
detected:
	return TRUE;
}
