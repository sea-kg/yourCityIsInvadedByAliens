#include "ykeyboard.h"
#include "ylog.h"


// ---------------------------------------------------------------------
// YKeyboard

YKeyboard::YKeyboard() {
    pollState();
}

void YKeyboard::pollState() {
    m_pKeyboardStateArray = SDL_GetKeyboardState(NULL);
}

void YKeyboard::toLogPressedButtons() {
    std::wstring sLog = L"\n---\n";
    sLog += std::wstring(L"Arrow Space: ") + (m_pKeyboardStateArray[SDL_SCANCODE_DOWN] ? L"Pressed" : L"No") + L"\n";
    sLog += std::wstring(L"Arrow Up: ") + (m_pKeyboardStateArray[SDL_SCANCODE_UP] ? L"Pressed" : L"No") + L"\n";
    sLog += std::wstring(L"Arrow Left: ") + (m_pKeyboardStateArray[SDL_SCANCODE_LEFT] ? L"Pressed" : L"No") + L"\n";
    sLog += std::wstring(L"Arrow Right: ") + (m_pKeyboardStateArray[SDL_SCANCODE_RIGHT] ? L"Pressed" : L"No") + L"\n";
    sLog += std::wstring(L"Arrow Down: ") + (m_pKeyboardStateArray[SDL_SCANCODE_DOWN] ? L"Pressed" : L"No") + L"\n";
    sLog += L"---\n";
    YLog::info(L"updateStateByKeyboard", sLog);
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
    bool bArrowDownRight =
        !m_pKeyboardStateArray[SDL_SCANCODE_UP]
        && !m_pKeyboardStateArray[SDL_SCANCODE_LEFT]
        && m_pKeyboardStateArray[SDL_SCANCODE_RIGHT]
        && m_pKeyboardStateArray[SDL_SCANCODE_DOWN];
    bool bWasdDownRight =
        !m_pKeyboardStateArray[SDL_SCANCODE_W]
        && !m_pKeyboardStateArray[SDL_SCANCODE_A]
        && m_pKeyboardStateArray[SDL_SCANCODE_D]
        && m_pKeyboardStateArray[SDL_SCANCODE_S];
    return bArrowDownRight || bWasdDownRight;
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

bool YKeyboard::isB() {
    return m_pKeyboardStateArray[SDL_SCANCODE_B];
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