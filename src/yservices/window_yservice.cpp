
#include "window_yservice.h"
#include <settings_yservice.h>
#include <ycore.h>


// ---------------------------------------------------------------------
// WindowYService

REGISTRY_YSERVICE(WindowYService)

WindowYService::WindowYService()
    : YServiceBase(WindowYService::name(), {
        SettingsYService::name()
    }) {
    TAG = WindowYService::name();
    m_sWindowName = L"Your City Is Invaded Aliens (v0.0.0)";
    m_nWindowWidth = 1280;
    m_nWindowHeight = 720;
    m_nWindowWidthOrig = 1280;
    m_nWindowHeightOrig = 720;
    m_pRenderWindow = nullptr;
}

bool WindowYService::init() {
    if (SDL_Init(SDL_INIT_VIDEO) > 0) {
        std::cerr << "HEY.. SDL_Init HAS FAILED. SDL_ERROR: " << SDL_GetError() << std::endl;
        return false;
    }

    if (!(IMG_Init(IMG_INIT_PNG))) {
        std::cerr << "IMG_init has failed. Error: " << SDL_GetError() << std::endl;
        return false;
    }

    if (TTF_Init() == -1) {
        printf("TTF_Init: %s\n", TTF_GetError());
        return false;
    }

    // TODO move to sound service    
    if (SDL_Init(SDL_INIT_AUDIO) < 0) {
        printf("Failed to init SDL\n");
        exit(1);
    }
    int result = 0;
    int flags = MIX_INIT_OGG;
    if (flags != (result = Mix_Init(flags))) {
        printf("Could not initialize mixer (result: %d).\n", result);
        printf("Mix_Init: %s\n", Mix_GetError());
        exit(1);
    }

    // init
    m_pRenderWindow = new RenderWindow(
        m_sWindowName,
        m_nWindowWidth,
        m_nWindowHeight
    );
    return true;
}

bool WindowYService::deinit() {
    // checking settings
    return true;
}

RenderWindow *WindowYService::getRenderWindow() {
    return m_pRenderWindow;
}

int WindowYService::getWidth() {
    return m_nWindowWidth;
}

int WindowYService::getHeight() {
    return m_nWindowHeight;
}

void WindowYService::toggleFullscreen() {
    if (!m_pRenderWindow->toggleFullscreen()) {
        // window mode
        m_nWindowWidth = m_nWindowWidthOrig;
        m_nWindowHeight = m_nWindowHeightOrig;
    } else {
        // full screen mode
        int w,h;
        m_pRenderWindow->getWindowSize(&w,&h);
        m_nWindowWidth = w;
        m_nWindowHeight = h;
    }
    std::cout << m_nWindowWidth << L":" << m_nWindowHeight << std::endl;
}