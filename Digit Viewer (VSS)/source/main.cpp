#include <iostream>
using namespace std;

#include "Dispatcher.h"
#include "DigitViewerUI.h"


void ycr_check_instruction_set(){
    ymo_print_astr("\nChecking processor/OS features...\n\n");
    ymo_print_astr("Required Features:\n");
    ymo_set_color_T(1);
    ymo_print_astr(ymc_ISA_required);
    ymo_print_astr("\n\n");

    cpu_x86 features;
    features.detect_host();
    if (!ymc_ISA_check(features)){
        ymo_set_color_R(1);
        ymo_print_astr("\nWarning:\n\n");

        ymo_print_astr("This computer does not have all the processor or operating system\n");
        ymo_print_astr("features that are necessary to run this binary.\n\n");
        ymo_print_astr("Proceeding may cause the program to crash.\n\n");

        ymo_set_color_W();
        ym_pause();
        ymo_print_astr("\n\n");
    }
    ymo_print_astr("\n\n");
}

int main(){
    ycr_check_instruction_set();

    try{

        //  Launch the main menu.
        Menu_Main();

    }catch (ym_exception &e){
        e.print();
    }catch (std::exception &e){
        ymo_print_astr(e.what());
    }

    printf("\n\n");
    ym_pause();
}
