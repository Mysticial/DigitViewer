
#include "y-cruncher/y-cruncher.h"

#ifdef _WIN32
#include <intrin.h>
#endif

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
int main(){
    ymo_set_color_G(0);
    ymo_print_astr(PROGRAM_NAME);
    ymo_set_color_W();

    ymo_print_astr("\n\n");

    ymo_print_astr("Detecting Environment...");
    ymo_print_astr("\n\n");

    //  Hardware Features
    int x64     = false;
    int MMX     = false;
    int SSE     = false;
    int SSE2    = false;
    int SSE3    = false;
    int SSSE3   = false;
    int SSE41   = false;
    int SSE42   = false;
    int SSE4a   = false;
    int AVX     = false;
    int XOP     = false;
    int FMA3    = false;
    int FMA4    = false;


    int info[4];
    __cpuid(info, 0);
    int nIds = info[0];

    __cpuid(info, 0x80000000);
    int nExIds = info[0];

    //  Detect Instruction Set
    if (nIds >= 1){
        __cpuid(info,0x00000001);
        MMX   = (info[3] & ((int)1 << 23)) != 0;
        SSE   = (info[3] & ((int)1 << 25)) != 0;
        SSE2  = (info[3] & ((int)1 << 26)) != 0;
        SSE3  = (info[2] & ((int)1 <<  0)) != 0;

        SSSE3 = (info[2] & ((int)1 <<  9)) != 0;
        SSE41 = (info[2] & ((int)1 << 19)) != 0;
        SSE42 = (info[2] & ((int)1 << 20)) != 0;

        AVX   = (info[2] & ((int)1 << 28)) != 0;
        FMA3  = (info[2] & ((int)1 << 12)) != 0;
    }

    if (nExIds >= 0x80000001){
        __cpuid(info,0x80000001);
        x64   = (info[3] & ((int)1 << 29)) != 0;
        SSE4a = (info[2] & ((int)1 <<  6)) != 0;
        FMA4  = (info[2] & ((int)1 << 16)) != 0;
        XOP   = (info[2] & ((int)1 << 11)) != 0;
    }

    ymo_print_astr("Hardware Features:\n");
    ymo_print_astr("x64        = "); print_bool(x64);
    ymo_print_astr("MMX        = "); print_bool(MMX);
    ymo_print_astr("SSE        = "); print_bool(SSE);
    ymo_print_astr("SSE2       = "); print_bool(SSE2);
    ymo_print_astr("SSE3       = "); print_bool(SSE3);
    ymo_print_astr("SSSE3      = "); print_bool(SSSE3);
    ymo_print_astr("SSE4a      = "); print_bool(SSE4a);
    ymo_print_astr("SSE4.1     = "); print_bool(SSE41);
    ymo_print_astr("SSE4.2     = "); print_bool(SSE42);
    ymo_print_astr("AVX        = "); print_bool(AVX);
    ymo_print_astr("FMA3       = "); print_bool(FMA3);
    ymo_print_astr("FMA4       = "); print_bool(FMA4);
    ymo_print_astr("XOP        = "); print_bool(XOP);
    ymo_print_astr("\n");

    if (x64 && SSE41){
        system("\"\"Binaries/Digit Viewer - x64 SSE4.1.exe\"\"");
    }
    else if (x64 && SSE2){
        system("\"\"Binaries/Digit Viewer - x64 SSE2.exe\"\"");
    }
    else if (SSE2){
        system("\"\"Binaries/Digit Viewer - x86 SSE2.exe\"\"");
    }else{
        system("\"\"Binaries/Digit Viewer - x86.exe\"\"");
    }

}
