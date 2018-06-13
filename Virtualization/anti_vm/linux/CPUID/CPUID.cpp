#include <stdio.h>
#include <stdint.h>
#include <string.h>

#define TRUE  1
#define FALSE 0

typedef char* string;

static inline int cpuid_hv_bit()//内联函数 
  { 
    int ecx;
    __asm__ volatile("cpuid" \  
            : "=c"(ecx) \  
            : "a"(0x01)  );    // gcc汇编指令 
    return (ecx >> 31) & 0x1;
}
 
static inline void cpuid_hv_vendor_00(char * vendor) {
    int ebx = 0, ecx = 0, edx = 0;

    __asm__ volatile("cpuid" \
            : "=b"(ebx), \
            "=c"(ecx), \
            "=d"(edx) \
            : "a"(0x40000000));
    sprintf(vendor  , "%c%c%c%c", ebx, (ebx >> 8), (ebx >> 16), (ebx >> 24));// 格式化数据写入字符串 
    sprintf(vendor+4, "%c%c%c%c", ecx, (ecx >> 8), (ecx >> 16), (ecx >> 24));
    sprintf(vendor+8, "%c%c%c%c", edx, (edx >> 8), (edx >> 16), (edx >> 24));
    vendor[12] = 0x00;
}
//是否成功获取CPUID 
int cpu_hv() {
    return cpuid_hv_bit() ? TRUE : FALSE;
}
 
void cpu_write_hv_vendor(char * vendor) {
    cpuid_hv_vendor_00(vendor);
}
//通过cpuid与字符串的匹配，判断是何种虚拟环境 
int cpu_known_vm_vendors() {
    const int count = 6;
    int i;
    char cpu_hv_vendor[13];
    string strs[count];
    strs[0] = "KVMKVMKVM\0\0\0"; /* KVM */
    strs[1] = "Microsoft Hv"; /* Microsoft Hyper-V or Windows Virtual PC */
    strs[2] = "VMwareVMware"; /* VMware */
    strs[3] = "XenVMMXenVMM"; /* Xen */
    strs[4] = "prl hyperv  "; /* Parallels */
    strs[5] = "VBoxVBoxVBox"; /* VirtualBox */
    cpu_write_hv_vendor(cpu_hv_vendor);
    for (i = 0; i < count; i++) {
        if (!memcmp(cpu_hv_vendor, strs[i], 12)) return TRUE;
    }
    return FALSE;
}

int main()
{
    char cpu_hv_vendor[13];
    cpu_write_hv_vendor(cpu_hv_vendor);

    printf("CPU Hypervisor Bit: %d\n", cpu_hv());
    printf("Hypervisor: %s, is known vm vendor: %s\n", cpu_hv_vendor, cpu_known_vm_vendors() ? "true" : "false");

    return 0;
}
