
#pragma once
#include "SDL.h"
#include <string>

class YKeyboard {

    public:
        YKeyboard();
        void pollState();
        void toLogPressedButtons();
        bool isUp();
        bool isUpLeft();
        bool isUpRight();
        bool isDown();
        bool isDownLeft();
        bool isDownRight();
        bool isLeft();
        bool isRight();
        bool isF1();
        bool isF4();
        bool isF11();
        bool isSpace();
        bool isEscape();
        bool isK();
        bool isL();
        bool isG();
        bool isH();
        bool isEnter();
        std::wstring toString();
    private:
        const Uint8 *m_pKeyboardStateArray;
        std::wstring TAG;
};
