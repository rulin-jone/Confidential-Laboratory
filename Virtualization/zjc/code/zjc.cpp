#include "Main.h"


int main()
{
	/* enable functions */
	BOOL	ENABLE_VBOX_CHECKS = TRUE;
	BOOL	ENABLE_VMWARE_CHECKS = TRUE;
	BOOL	ENABLE_VPC_CHECKS = TRUE;
	BOOL	ENABLE_XEN_CHECKS = TRUE;
	BOOL    ENABLE_General_CHECKS = TRUE;

	/* Resize the console window for better visibility */
	resize_console_window();

	/* Display general informations */
	print_os();

	if (IsWoW64())
		_tprintf(_T("Process is running under WOW64\n\n"));

	/* General detection */
	if (ENABLE_General_CHECKS) {
		print_category(TEXT("General VM Detection"));
		loaded_dlls();
		exec_check(&idt_trick, TEXT("Checking Interupt Descriptor Table location: "));
		exec_check(&ldt_trick, TEXT("Checking Local Descriptor Table location: "));
		exec_check(&gdt_trick, TEXT("Checking Global Descriptor Table location: "));
		exec_check(&str_trick, TEXT("Checking Store Task Register location: "));
		exec_check(&Time, TEXT("Checking Time : "));
		exec_check(&cpuid_is_hypervisor, TEXT("Checking if CPU hypervisor field is set using cpuid(0x1)"));
		exec_check(&VMDriverServices, TEXT("VM Driver Services : "));
		exec_check(&serial_number_bios_wmi, TEXT("Checking SerialNumber from BIOS using WMI: "));
	}

	/* VirtualBox Detection */
	if (ENABLE_VBOX_CHECKS) {
		print_category(TEXT("VirtualBox Detection"));
		vbox_reg_key_value();
		exec_check(&vbox_dir, TEXT("Checking dir oracle\\virtualbox guest additions\\: "));
		vbox_files();
		vbox_reg_keys();
		exec_check(&vbox_check_mac, TEXT("Checking Mac Address start with 08:00:27: "));
		vbox_devices();
		exec_check(&vbox_window_class, TEXT("Checking VBoxTrayToolWndClass / VBoxTrayToolWnd: "));
		exec_check(&vbox_network_share, TEXT("Checking VirtualBox Shared Folders network provider: "));
		vbox_processes();
		exec_check(&vbox_eventlogfile_wmi, TEXT("Checking NTEventLog from WMI: "));
		exec_check(&vbox_firmware_SMBIOS, TEXT("Checking SMBIOS firmware : "));
		exec_check(&vbox_firmware_ACPI, TEXT("Checking ACPI tables : "));
	}

	/* VMWare Detection */
	if (ENABLE_VMWARE_CHECKS) {
		print_category(TEXT("VMWare Detection"));
		exec_check(&In, TEXT("Checking In Instruction : "));
		vmware_reg_key_value();
		vmware_reg_keys();
		vmware_files();
		vmware_mac();
		exec_check(&vmware_adapter_name, TEXT("Checking VMWare network adapter name: "));
		vmware_devices();
		exec_check(&vmware_dir, TEXT("Checking VMWare directory: "));
		vmware_processes();
		exec_check(&vmware_firmware_SMBIOS, TEXT("Checking SMBIOS firmware : "));
		exec_check(&vmware_firmware_ACPI, TEXT("Checking ACPI tables : "));
	}

	/* Virtual PC Detection */
	if (ENABLE_VPC_CHECKS) {
		print_category(TEXT("Virtual PC Detection"));
		virtual_pc_process();
		virtual_pc_reg_keys();
		exec_check(&ISA, TEXT("Checking Instruction Set Architecture : "));
	}

	/* Xen Detection */
	if (ENABLE_XEN_CHECKS) {
		print_category(TEXT("Xen Detection"));
		xen_process();
		exec_check(&xen_check_mac, TEXT("Checking Mac Address start with 08:16:3E: "));

	}

	_tprintf(_T("\n\nAnalysis done."));

	getchar();
	return 0;
}

