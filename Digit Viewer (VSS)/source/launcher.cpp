//#define DIGITVIEWER_STANDALONE
#include "y-cruncher/y-cruncher.h"

#if WIN32
#include <windows.h>
#include "y-cruncher/ConsoleIO/ymo_APIs_cio_Windows.h"
#elif defined __linux
#include "y-cruncher/ConsoleIO/ymo_APIs_cio_ColorCodes.h"
#else
#include "y-cruncher/ConsoleIO/ymo_APIs_cio_Default.h"
#endif
#include "y-cruncher/ConsoleIO/ymo_APIs_cio_Shared.h"

#include "Dispatcher.h"
#include "y-cruncher/ProcessorCapability/cpu_x86_internals.h"

int global_argc;
char **global_argv;

#ifdef _WIN32
#include "Launcher_Windows.h"
#else
#include "Launcher_Linux.h"
#endif



int main(int argc,char *argv[]){
    global_argc = argc;
    global_argv = argv;

    ymo_set_color_G(0);
    ymo_print_astr(PROGRAM_NAME);
    ymo_set_color_W();

    ymo_print_astr("\n\n");

    ymo_print_astr("Detecting Environment...");
    ymo_print_astr("\n\n");

    cpu_x86 features;
    features.detect_host();
    features.print();

#ifdef _WIN32
    if (can_run_x64_SSE41(features))
        launch("Digit Viewer - x64 SSE4.1");
    else if (can_run_x64_SSE2(features))
        launch("Digit Viewer - x64 SSE2");
    else if (can_run_x86_SSE2(features))
        launch("Digit Viewer - x86 SSE2");
    else
        launch("Digit Viewer - x86");
#else
    if (can_run_x64_SSE41(features))
        launch("Digit Viewer - x64 SSE4.1");
    else
        launch("Digit Viewer - x64 SSE2");
#endif
}
