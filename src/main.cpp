#include <iostream>
#include <vector>
#include <fstream>
#include <chrono>
#include <wsjcpp_core.h>
#include "main_controller.h"

int main(int argc, char* args[]) {

    MainController *pMainController = new MainController("Your City Is Invaded Aliens (v0.0.1)");

    if (!pMainController->init()) {
        return -1;
    }

    return pMainController->startUI();
}