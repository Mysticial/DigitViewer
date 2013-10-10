#include <iostream>
using namespace std;

#include "DigitViewerUI.h"


int main(){

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
