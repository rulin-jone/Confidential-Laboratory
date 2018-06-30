#include <windows.h>
#include <tchar.h>
#include <Winternl.h>
#include <devguid.h>   //Device guids  
#include <winioctl.h>  //IOCTL
#include <intrin.h>    //cpuid()

#include <SetupAPI.h>
#pragma comment(lib, "setupapi.lib")

#include "Utils.h"
#include "VersionHelpers.h"

VOID loaded_dlls();
BOOL idt_trick();
BOOL ldt_trick();
BOOL gdt_trick();
BOOL str_trick();
BOOL cpuid_is_hypervisor();
BOOL serial_number_bios_wmi();
BOOL Time();