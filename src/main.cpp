#include <yservices.h>
#include "main_controller.h"

int main(int argc, char* args[]) {

    if (!YServices::init({})) {
        YLog::err("main", "Could not init services");
        return -1;
    }

    MainController *pMainController = new MainController("Your City Is Invaded Aliens (v0.0.1)");

    if (!pMainController->init()) {
        return -1;
    }

    return pMainController->startUI();
}