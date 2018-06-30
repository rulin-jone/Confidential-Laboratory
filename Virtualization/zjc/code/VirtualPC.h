#include <Windows.h>
#include "Utils.h"

VOID virtual_pc_process();
VOID virtual_pc_reg_keys();
BOOL ISA();
DWORD IsInsideVPC_exceptionFilter(LPEXCEPTION_POINTERS ep);