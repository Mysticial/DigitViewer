/* cpu_x86_internals.h
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 04/12/2014
 * Last Modified    : 04/12/2014
 * 
 */

#pragma once
#ifndef _cpu_x86_internals_H
#define _cpu_x86_internals_H
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
//#include "../../Modules_v1.0/APIs/ConsoleIO/ymo_APIs_cio_headers.h"
#include <string.h>
#include "cpu_x86_headers.h"
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Helpers
void print_bool(int x){
    if (x){
        ymo_set_color_G(1);
        ymo_print_wstr(L"Yes");
    }else{
        ymo_set_color_R(1);
        ymo_print_wstr(L"No");
    }
    ymo_set_color_W();
    ymo_print_wstr(L"\n");
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Windows
#if _WIN32
#include <intrin.h>
void cpu_x86::cpuid(int32_t out[4],int32_t x){
    __cpuidex(out,x,0);
}
__int64 xgetbv(unsigned int x){
    return _xgetbv(x);
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Detect 64-bit - Note that this snippet of code for detecting 64-bit has been copied from MSDN.
typedef BOOL (WINAPI *LPFN_ISWOW64PROCESS) (HANDLE, PBOOL);
LPFN_ISWOW64PROCESS fnIsWow64Process;
BOOL IsWow64()
{
    BOOL bIsWow64 = FALSE;

    fnIsWow64Process = (LPFN_ISWOW64PROCESS) GetProcAddress(
        GetModuleHandle(TEXT("kernel32")),"IsWow64Process");

    if (NULL != fnIsWow64Process)
    {
        if (!fnIsWow64Process(GetCurrentProcess(),&bIsWow64))
        {
            printf("Error Detecting Operating System.\n");
            printf("Defaulting to 32-bit OS.\n\n");
            bIsWow64 = FALSE;
        }
    }
    return bIsWow64;
}
bool cpu_x86::detect_OS_x64(){
#ifdef _M_X64
    return true;
#else
    return IsWow64() != 0;
#endif
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Linux GCC
#elif (defined __linux) && (defined __GNUC__)
void cpu_x86::cpuid(int32_t out[4],int32_t x){
    __asm__ __volatile__ (
        "cpuid":
        "=a" (out[0]),
        "=b" (out[1]),
        "=c" (out[2]),
        "=d" (out[3])
        : "a" (x), "c" (0)
    );
}
uint64_t xgetbv(unsigned int index){
    uint32_t eax, edx;
    __asm__ __volatile__("xgetbv" : "=a"(eax), "=d"(edx) : "c"(index));
    return ((uint64_t)edx << 32) | eax;
}
#define _XCR_XFEATURE_ENABLED_MASK  0
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Detect 64-bit
bool cpu_x86::detect_OS_x64(){
    //  We only support x64 on Linux.
    return true;
}
#else
#error "No cpuid intrinsic defined."
#endif
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
cpu_x86::cpu_x86(){
    memset(this,0,sizeof(*this));
}
bool cpu_x86::detect_OS_AVX(){
    //  Copied from: http://stackoverflow.com/a/22521619/922184

    bool avxSupported = false;

    int cpuInfo[4];
    cpuid(cpuInfo,1);

    bool osUsesXSAVE_XRSTORE = (cpuInfo[2] & (1 << 27)) != 0;
    bool cpuAVXSuport = (cpuInfo[2] & (1 << 28)) != 0;

    if (osUsesXSAVE_XRSTORE && cpuAVXSuport)
    {
        uint64_t xcrFeatureMask = xgetbv(_XCR_XFEATURE_ENABLED_MASK);
        avxSupported = (xcrFeatureMask & 0x6) != 0;
    }

    return avxSupported;
}
std::string cpu_x86::get_vendor_string(){
    int32_t CPUInfo[4];
    char name[13];

    cpuid(CPUInfo,0);
    memcpy(name + 0,&CPUInfo[1],4);
    memcpy(name + 4,&CPUInfo[3],4);
    memcpy(name + 8,&CPUInfo[2],4);
    name[12] = '\0';

    return name;
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void cpu_x86::detect_host(){
    //  OS Features
    OS_x64 = detect_OS_x64();
    OS_AVX = detect_OS_AVX();

    //  Vendor
    std::string vendor(get_vendor_string());
    if (vendor == "GenuineIntel"){
        Vendor_Intel = true;
    }else if (vendor == "AuthenticAMD"){
        Vendor_AMD = true;
    }

    int info[4];
    cpuid(info, 0);
    int nIds = info[0];

    cpuid(info, 0x80000000);
    uint32_t nExIds = info[0];

    //  Detect Features
    if (nIds >= 0x00000001){
        cpuid(info,0x00000001);
        HW_MMX    = (info[3] & ((int)1 << 23)) != 0;
        HW_SSE    = (info[3] & ((int)1 << 25)) != 0;
        HW_SSE2   = (info[3] & ((int)1 << 26)) != 0;
        HW_SSE3   = (info[2] & ((int)1 <<  0)) != 0;

        HW_SSSE3  = (info[2] & ((int)1 <<  9)) != 0;
        HW_SSE41  = (info[2] & ((int)1 << 19)) != 0;
        HW_SSE42  = (info[2] & ((int)1 << 20)) != 0;
        HW_AES    = (info[2] & ((int)1 << 25)) != 0;

        HW_AVX    = (info[2] & ((int)1 << 28)) != 0;
        HW_FMA3   = (info[2] & ((int)1 << 12)) != 0;

        HW_RDRAND = (info[2] & ((int)1 << 30)) != 0;
    }
    if (nIds >= 0x00000007){
        cpuid(info,0x00000007);
        HW_AVX2   = (info[1] & ((int)1 <<  5)) != 0;

        HW_BMI1   = (info[1] & ((int)1 <<  3)) != 0;
        HW_BMI2   = (info[1] & ((int)1 <<  8)) != 0;
        HW_ADX    = (info[1] & ((int)1 << 19)) != 0;

        HW_AVX512F  = (info[1] & ((int)1 << 16)) != 0;
        HW_AVX512PF = (info[1] & ((int)1 << 26)) != 0;
        HW_AVX512ER = (info[1] & ((int)1 << 27)) != 0;
        HW_AVX512CD = (info[1] & ((int)1 << 28)) != 0;
        HW_SHA      = (info[1] & ((int)1 << 29)) != 0;
    }
    if (nExIds >= 0x80000001){
        cpuid(info,0x80000001);
        HW_x64   = (info[3] & ((int)1 << 29)) != 0;
        HW_ABM   = (info[2] & ((int)1 <<  5)) != 0;
        HW_SSE4a = (info[2] & ((int)1 <<  6)) != 0;
        HW_FMA4  = (info[2] & ((int)1 << 16)) != 0;
        HW_XOP   = (info[2] & ((int)1 << 11)) != 0;
    }
}
void cpu_x86::print() const{
    ymo_print_astr("CPU Vendor:\n");
    ymo_print_astr("    AMD         = "); print_bool(Vendor_AMD);
    ymo_print_astr("    Intel       = "); print_bool(Vendor_Intel);
    ymo_print_astr("\n");

    ymo_print_astr("OS Features:\n");
    ymo_print_astr("  * 64-bit      = "); print_bool(OS_x64);
    ymo_print_astr("  * AVX + XSAVE = "); print_bool(OS_AVX);
    ymo_print_astr("\n");

    ymo_print_astr("Hardware Features:\n");
    ymo_print_astr("    MMX         = "); print_bool(HW_MMX);
    ymo_print_astr("  * x64         = "); print_bool(HW_x64);
    ymo_print_astr("  * ABM         = "); print_bool(HW_ABM);
    ymo_print_astr("    RDRAND      = "); print_bool(HW_RDRAND);
    ymo_print_astr("    BMI1        = "); print_bool(HW_BMI1);
    ymo_print_astr("  * BMI2        = "); print_bool(HW_BMI2);
    ymo_print_astr("    ADX         = "); print_bool(HW_ADX);
    ymo_print_astr("\n");

    ymo_print_astr("SIMD: 128-bit\n");
    ymo_print_astr("  * SSE         = "); print_bool(HW_SSE);
    ymo_print_astr("  * SSE2        = "); print_bool(HW_SSE2);
    ymo_print_astr("  * SSE3        = "); print_bool(HW_SSE3);
    ymo_print_astr("  * SSSE3       = "); print_bool(HW_SSSE3);
    ymo_print_astr("    SSE4a       = "); print_bool(HW_SSE4a);
    ymo_print_astr("  * SSE4.1      = "); print_bool(HW_SSE41);
    ymo_print_astr("    SSE4.2      = "); print_bool(HW_SSE42);
    ymo_print_astr("    AES-NI      = "); print_bool(HW_AES);
#ifdef YCC_ENABLE_AVX512_DETECTION_PRINTS
    ymo_print_astr("    SHA         = "); print_bool(HW_SHA);
#endif
    ymo_print_astr("\n");

    ymo_print_astr("SIMD: 256-bit\n");
    ymo_print_astr("  * AVX         = "); print_bool(HW_AVX);
    ymo_print_astr("  * XOP         = "); print_bool(HW_XOP);
    ymo_print_astr("  * FMA3        = "); print_bool(HW_FMA3);
    ymo_print_astr("  * FMA4        = "); print_bool(HW_FMA4);
    ymo_print_astr("  * AVX2        = "); print_bool(HW_AVX2);
    ymo_print_astr("\n");

#ifdef YCC_ENABLE_AVX512_DETECTION_PRINTS
    ymo_print_astr("SIMD: 512-bit\n");
    ymo_print_astr("    AVX512-F    = "); print_bool(HW_AVX512F);
    ymo_print_astr("    AVX512-PF   = "); print_bool(HW_AVX512PF);
    ymo_print_astr("    AVX512-ER   = "); print_bool(HW_AVX512ER);
    ymo_print_astr("    AVX512-CD   = "); print_bool(HW_AVX512CD);
    ymo_print_astr("\n");
#endif

//    ymo_print_astr("* Indicates features that y-cruncher may try to use.\n");
}
void cpu_x86::print_host(){
    cpu_x86 features;
    features.detect_host();
    features.print();
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#endif
