#include <yservices.h>
#include "main_controller.h"

int main(int argc, char* args[]) {

    YLog::info(L"main", L"Init yservices");
    if (!YServices::init({})) {
        YLog::err(L"main", L"Could not init services");
        return -1;
    }

    MainController *pMainController = new MainController();

    if (!pMainController->init()) {
        return -1;
    }

    return pMainController->startUI();
}