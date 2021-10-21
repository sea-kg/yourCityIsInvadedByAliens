#pragma once

#include "yservices.h"
#include "render.h"

// ---------------------------------------------------------------------
// WindowYService

class WindowYService : public YServiceBase {
    public:
        WindowYService();
        static std::wstring name() { return L"WindowYService"; }
        virtual bool init() override;
        virtual bool deinit() override;
        RenderWindow *getRenderWindow();
        int getWidth();
        int getHeight();
        void toggleFullscreen();

    private:
        std::wstring TAG;

        std::wstring m_sWindowName;
        int m_nWindowWidth;
        int m_nWindowHeight;
        int m_nWindowWidthOrig;
        int m_nWindowHeightOrig;
        RenderWindow *m_pRenderWindow;

};
