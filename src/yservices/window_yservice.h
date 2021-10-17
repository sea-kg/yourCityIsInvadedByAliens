#pragma once

#include "yservices.h"
#include "render.h"

// ---------------------------------------------------------------------
// WindowYService

class WindowYService : public YServiceBase {
    public:
        WindowYService();
        static std::string name() { return "WindowYService"; }
        virtual bool init() override;
        virtual bool deinit() override;
        RenderWindow *getRenderWindow();
        int getWidth();
        int getHeight();
        void toggleFullscreen();

    private:
        std::string TAG;

        std::string m_sWindowName;
        int m_nWindowWidth;
        int m_nWindowHeight;
        RenderWindow *m_pRenderWindow;

};
