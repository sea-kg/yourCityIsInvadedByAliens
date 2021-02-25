#include "game_state.h"
#include "wsjcpp_core.h"
#include <SDL.h>

// ---------------------------------------------------------------------
// GameState

GameState::GameState(int windowWidth, int windowHeight) {
    m_nStartTime = 0;
    m_nElapsedTime = 0;
    m_nWindowWidth = windowWidth;
    m_nWindowHeight = windowHeight;
}

void GameState::init() {
    m_nStartTime = WsjcppCore::getCurrentTimeInMilliseconds();
}

void GameState::updateElapsedTime() {
    m_nElapsedTime = WsjcppCore::getCurrentTimeInMilliseconds() - m_nStartTime;
}

void GameState::addBuilding(GameBuilding *pBuilding) {
    m_vBuildings.push_back(pBuilding);
}

long GameState::getElapsedTime() const {
    return m_nElapsedTime;
}

const CoordXY &GameState::getCoordLeftTop() const {
    return m_coordLeftTop;
}

void GameState::incrementCoordLeftTopX(int nX) {
    m_coordLeftTop += CoordXY(nX, 0);
}

void GameState::incrementCoordLeftTopY(int nY) {
    m_coordLeftTop += CoordXY(0, nY);
}

const int GameState::windowWidth() const {
    return m_nWindowWidth;
}

const int GameState::windowHeight() const {
    return m_nWindowHeight;
}

// TODO move to MainController
void GameState::setMouseCaptured(bool bMouseCaptured) {
    m_bMouseCaptured = bMouseCaptured;
    SDL_SetRelativeMouseMode(m_bMouseCaptured ? SDL_TRUE : SDL_FALSE);
}

bool GameState::isMouseCaptured() const {
    return m_bMouseCaptured;
}