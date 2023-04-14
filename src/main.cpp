#include <yservices.h>
#include "main_controller.h"
#include <filesystem>
#include <string>

#ifdef _WIN32
int wmain(int argc, wchar_t *argv[])
#else
int main(int argc, char *argv[])
#endif
{
    // TODO set to settings service
    auto dir = std::filesystem::weakly_canonical(std::filesystem::path(argv[0])).parent_path();

    YLog::info(L"main", L"Exectutable path: " + YCore::s2ws(dir.string()));

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