#include <yservices.h>
#include "main_controller.h"
#include <filesystem>
#include <string>

#include <cstdlib>
#include <iostream>
#include <ctime>

#include "Roads2DGenerator.h"

#ifdef _WIN32
int wmain(int argc, wchar_t *argv[])
#else
int main(int argc, char *argv[])
#endif
{
    Roads2DGenerator road2gen(82, 41);
    road2gen.generate(0.5);
    road2gen.print_map();
    return 0;

    YLog::info(L"main", YCore::s2ws("hello"));

    std::srand(std::time(nullptr)); // use current time as seed for random generator

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