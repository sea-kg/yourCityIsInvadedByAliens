#include "ykeyboard.h"



// ---------------------------------------------------------------------
// YKeyboard

YKeyboard::YKeyboard() {
    pollState();
}

void YKeyboard::pollState() {
    m_pKeyboardStateArray = SDL_GetKeyboardState(NULL);
}

bool YKeyboard::isUp() {
    bool bArrowUp =
        m_pKeyboardStateArray[SDL_SCANCODE_UP]
        && !m_pKeyboardStateArray[SDL_SCANCODE_LEFT]
        && !m_pKeyboardStateArray[SDL_SCANCODE_RIGHT]
        && !m_pKeyboardStateArray[SDL_SCANCODE_DOWN];
    bool bWasdUp = 
        m_pKeyboardStateArray[SDL_SCANCODE_W]
        && !m_pKeyboardStateArray[SDL_SCANCODE_A]
        && !m_pKeyboardStateArray[SDL_SCANCODE_D]
        && !m_pKeyboardStateArray[SDL_SCANCODE_S];
    return bArrowUp || bWasdUp;
}

bool YKeyboard::isUpLeft() {
    bool bArrowUp =
        m_pKeyboardStateArray[SDL_SCANCODE_UP]
        && m_pKeyboardStateArray[SDL_SCANCODE_LEFT]
        && !m_pKeyboardStateArray[SDL_SCANCODE_RIGHT]
        && !m_pKeyboardStateArray[SDL_SCANCODE_DOWN];
    bool bWasdUp = 
        m_pKeyboardStateArray[SDL_SCANCODE_W]
        && m_pKeyboardStateArray[SDL_SCANCODE_A]
        && !m_pKeyboardStateArray[SDL_SCANCODE_D]
        && !m_pKeyboardStateArray[SDL_SCANCODE_S];
    return bArrowUp || bWasdUp;
}

bool YKeyboard::isUpRight() {
    bool bArrowUp =
        m_pKeyboardStateArray[SDL_SCANCODE_UP]
        && !m_pKeyboardStateArray[SDL_SCANCODE_LEFT]
        && m_pKeyboardStateArray[SDL_SCANCODE_RIGHT]
        && !m_pKeyboardStateArray[SDL_SCANCODE_DOWN];
    bool bWasdUp = 
        m_pKeyboardStateArray[SDL_SCANCODE_W]
        && !m_pKeyboardStateArray[SDL_SCANCODE_A]
        && m_pKeyboardStateArray[SDL_SCANCODE_D]
        && !m_pKeyboardStateArray[SDL_SCANCODE_S];
    return bArrowUp || bWasdUp;
}

bool YKeyboard::isDown() {
    bool bArrowUp =
        !m_pKeyboardStateArray[SDL_SCANCODE_UP]
        && !m_pKeyboardStateArray[SDL_SCANCODE_LEFT]
        && !m_pKeyboardStateArray[SDL_SCANCODE_RIGHT]
        && m_pKeyboardStateArray[SDL_SCANCODE_DOWN];
    bool bWasdUp = 
        !m_pKeyboardStateArray[SDL_SCANCODE_W]
        && !m_pKeyboardStateArray[SDL_SCANCODE_A]
        && !m_pKeyboardStateArray[SDL_SCANCODE_D]
        && m_pKeyboardStateArray[SDL_SCANCODE_S];
    return bArrowUp || bWasdUp;
}

bool YKeyboard::isDownLeft() {
    bool bArrowUp =
        !m_pKeyboardStateArray[SDL_SCANCODE_UP]
        && m_pKeyboardStateArray[SDL_SCANCODE_LEFT]
        && !m_pKeyboardStateArray[SDL_SCANCODE_RIGHT]
        && m_pKeyboardStateArray[SDL_SCANCODE_DOWN];
    bool bWasdUp = 
        !m_pKeyboardStateArray[SDL_SCANCODE_W]
        && m_pKeyboardStateArray[SDL_SCANCODE_A]
        && !m_pKeyboardStateArray[SDL_SCANCODE_D]
        && m_pKeyboardStateArray[SDL_SCANCODE_S];
    return bArrowUp || bWasdUp;
}

bool YKeyboard::isDownRight() {
    bool bArrowUp =
        !m_pKeyboardStateArray[SDL_SCANCODE_UP]
        && !m_pKeyboardStateArray[SDL_SCANCODE_LEFT]
        && m_pKeyboardStateArray[SDL_SCANCODE_RIGHT]
        && m_pKeyboardStateArray[SDL_SCANCODE_DOWN];
    bool bWasdUp = 
        !m_pKeyboardStateArray[SDL_SCANCODE_W]
        && !m_pKeyboardStateArray[SDL_SCANCODE_A]
        && m_pKeyboardStateArray[SDL_SCANCODE_D]
        && m_pKeyboardStateArray[SDL_SCANCODE_S];
    return bArrowUp || bWasdUp;
}

bool YKeyboard::isLeft() {
    bool bArrowUp =
        !m_pKeyboardStateArray[SDL_SCANCODE_UP]
        && m_pKeyboardStateArray[SDL_SCANCODE_LEFT]
        && !m_pKeyboardStateArray[SDL_SCANCODE_RIGHT]
        && !m_pKeyboardStateArray[SDL_SCANCODE_DOWN];
    bool bWasdUp = 
        !m_pKeyboardStateArray[SDL_SCANCODE_W]
        && m_pKeyboardStateArray[SDL_SCANCODE_A]
        && !m_pKeyboardStateArray[SDL_SCANCODE_D]
        && !m_pKeyboardStateArray[SDL_SCANCODE_S];
    return bArrowUp || bWasdUp;
}

bool YKeyboard::isRight() {
    bool bArrowUp =
        !m_pKeyboardStateArray[SDL_SCANCODE_UP]
        && !m_pKeyboardStateArray[SDL_SCANCODE_LEFT]
        && m_pKeyboardStateArray[SDL_SCANCODE_RIGHT]
        && !m_pKeyboardStateArray[SDL_SCANCODE_DOWN];
    bool bWasdUp = 
        !m_pKeyboardStateArray[SDL_SCANCODE_W]
        && !m_pKeyboardStateArray[SDL_SCANCODE_A]
        && m_pKeyboardStateArray[SDL_SCANCODE_D]
        && !m_pKeyboardStateArray[SDL_SCANCODE_S];
    return bArrowUp || bWasdUp;
}

bool YKeyboard::isF1() {
    return m_pKeyboardStateArray[SDL_SCANCODE_F1];
}

bool YKeyboard::isF4() {
    return m_pKeyboardStateArray[SDL_SCANCODE_F4];
}

bool YKeyboard::isF11() {
    return m_pKeyboardStateArray[SDL_SCANCODE_F11];
}

bool YKeyboard::isSpace() {
    return m_pKeyboardStateArray[SDL_SCANCODE_SPACE];
}

bool YKeyboard::isEscape() {
    return m_pKeyboardStateArray[SDL_SCANCODE_ESCAPE];
}

bool YKeyboard::isK() {
    return m_pKeyboardStateArray[SDL_SCANCODE_K];
}

bool YKeyboard::isL() {
    return m_pKeyboardStateArray[SDL_SCANCODE_L];
}

bool YKeyboard::isG() {
    return m_pKeyboardStateArray[SDL_SCANCODE_G];
}

bool YKeyboard::isH() {
    return m_pKeyboardStateArray[SDL_SCANCODE_H];
}

bool YKeyboard::isEnter() {
    return m_pKeyboardStateArray[SDL_SCANCODE_RETURN];
}

std::wstring YKeyboard::toString() {
    return L"TODO";
}