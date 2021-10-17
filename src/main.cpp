#include <yservices.h>
#include "main_controller.h"

int main(int argc, char* args[]) {

    YLog::info("main", "Init yservices");
    if (!YServices::init({})) {
        YLog::err("main", "Could not init services");
        return -1;
    }

    MainController *pMainController = new MainController();

    if (!pMainController->init()) {
        return -1;
    }

    return pMainController->startUI();
}