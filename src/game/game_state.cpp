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
    m_bIsChangedWindowSize = true;
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

void GameState::setCoordLeftTop(const CoordXY &coordLeftTop) {
    m_coordLeftTop = coordLeftTop;
}

bool GameState::isChangedWindowSize() const {
    return m_bIsChangedWindowSize;
}

const int GameState::getWindowWidth() const {
    return m_nWindowWidth;
}

const int GameState::getWindowHeight() const {
    return m_nWindowHeight;
}

void GameState::updateWindowSize(int w, int h) {
    m_bIsChangedWindowSize = true;
    m_nWindowWidth = w;
    m_nWindowHeight = h;
}

// TODO move to MainController
void GameState::setMouseCaptured(bool bMouseCaptured) {
    m_bMouseCaptured = bMouseCaptured;
    SDL_SetRelativeMouseMode(m_bMouseCaptured ? SDL_TRUE : SDL_FALSE);
}

bool GameState::isMouseCaptured() const {
    return m_bMouseCaptured;
}

void GameState::setMinPoint(const CoordXY &p) {
    m_minPoint = p;
}

const CoordXY &GameState::getMinPoint() {
    return m_minPoint;
}

void GameState::setMaxPoint(const CoordXY &p) {
    m_maxPoint = p;
}

const CoordXY &GameState::getMaxPoint() {
    return m_maxPoint;
}