#include <iostream>
using namespace std;

#include "../Public/Exception.h"
#include "DigitViewerUI.h"

int main(){
    try{

        //  Launch the main menu.
        DigitViewer::Menu_Main();

    }catch (ym_exception &e){
        e.print();
    }catch (std::exception &e){
        Console::print(e.what());
    }

    Console::println("\n");
    Console::Pause('w');
}
