
#pragma once
#include "SDL.h"

class YKeyboard {

    public:
        YKeyboard();
        void pollState();
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
        bool isF12();
        bool isSpace();
    
    private:
        const Uint8 *m_pKeyboardStateArray;
};
