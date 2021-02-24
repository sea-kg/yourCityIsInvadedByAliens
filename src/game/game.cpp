#include "game.h"
#include "wsjcpp_core.h"
#include <SDL.h>


// ---------------------------------------------------------------------
// GameBuilding

GameBuilding::GameBuilding(nlohmann::json &jsonData) {
    m_sName = jsonData["name"];
    int nPoints = jsonData["points"];
    for (int i = 0; i < nPoints; i++) {
        int nX = jsonData["x" + std::to_string(i)];
        int nY = jsonData["y" + std::to_string(i)];
        m_vPoints.push_back(CoordXY(nX,nY));
    }
}

const std::string &GameBuilding::getName() {
    return m_sName;
}

const std::vector<CoordXY> &GameBuilding::getPoints() {
    return m_vPoints;
}

// ---------------------------------------------------------------------
// GameTank0State

GameTank0State::GameTank0State(const CoordXY &p0) 
: m_nDirection(MoveObjectDirection::UP) {
    m_p0 = p0;
}

MoveObjectDirection GameTank0State::getDirection() {
    return m_nDirection;
}

const CoordXY &GameTank0State::getPosition() {
    return m_p0;
}

void GameTank0State::turnLeft() {
    switch (m_nDirection) {
        case MoveObjectDirection::UP:
            m_nDirection = MoveObjectDirection::UP_LEFT;
            break;
        case MoveObjectDirection::UP_LEFT:
            m_nDirection = MoveObjectDirection::LEFT;
            break;
        case MoveObjectDirection::UP_RIGHT:
            m_nDirection = MoveObjectDirection::UP;
            break;
        case MoveObjectDirection::DOWN:
            m_nDirection = MoveObjectDirection::DOWN_RIGHT;
            break;
        case MoveObjectDirection::DOWN_LEFT:
            m_nDirection = MoveObjectDirection::DOWN;
            break;
        case MoveObjectDirection::DOWN_RIGHT:
            m_nDirection = MoveObjectDirection::RIGHT;
            break;
        case MoveObjectDirection::LEFT:
            m_nDirection = MoveObjectDirection::DOWN_LEFT;
            break;
        case MoveObjectDirection::RIGHT:
            m_nDirection = MoveObjectDirection::UP_RIGHT;
            break;
    }
}

void GameTank0State::turnRight() {
    switch (m_nDirection) {
        case MoveObjectDirection::UP:
            m_nDirection = MoveObjectDirection::UP_RIGHT;
            break;
        case MoveObjectDirection::UP_LEFT:
            m_nDirection = MoveObjectDirection::UP;
            break;
        case MoveObjectDirection::UP_RIGHT:
            m_nDirection = MoveObjectDirection::RIGHT;
            break;
        case MoveObjectDirection::DOWN:
            m_nDirection = MoveObjectDirection::DOWN_LEFT;
            break;
        case MoveObjectDirection::DOWN_LEFT:
            m_nDirection = MoveObjectDirection::LEFT;
            break;
        case MoveObjectDirection::DOWN_RIGHT:
            m_nDirection = MoveObjectDirection::DOWN;
            break;
        case MoveObjectDirection::LEFT:
            m_nDirection = MoveObjectDirection::UP_LEFT;
            break;
        case MoveObjectDirection::RIGHT:
            m_nDirection = MoveObjectDirection::DOWN_RIGHT;
            break;
    }
}

void GameTank0State::move() {
    int nStep = 10;
    if (m_nDirection == MoveObjectDirection::UP) {
        m_p0 += CoordXY(0,-1 * nStep);
    } else if (m_nDirection == MoveObjectDirection::UP_LEFT) {
        m_p0 += CoordXY(-1*nStep, -1*nStep);
    } else if (m_nDirection == MoveObjectDirection::UP_RIGHT) {
        m_p0 += CoordXY(nStep, -1*nStep);
    } else if (m_nDirection == MoveObjectDirection::DOWN) {
        m_p0 += CoordXY(0, nStep);
    } else if (m_nDirection == MoveObjectDirection::DOWN_LEFT) {
        m_p0 += CoordXY(-1*nStep, nStep);
    } else if (m_nDirection == MoveObjectDirection::DOWN_RIGHT) {
        m_p0 += CoordXY(nStep, nStep);
    } else if (m_nDirection == MoveObjectDirection::LEFT) {
        m_p0 += CoordXY(-1*nStep, 0);
    } else if (m_nDirection == MoveObjectDirection::RIGHT) {
        m_p0 += CoordXY(nStep, 0);
    }
}

void GameTank0State::shot() {
    // TODO
}

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

void GameState::setMouseCaptured(bool bMouseCaptured) {
    m_bMouseCaptured = bMouseCaptured;
    SDL_SetRelativeMouseMode(m_bMouseCaptured ? SDL_TRUE : SDL_FALSE);
}

bool GameState::isMouseCaptured() const {
    return m_bMouseCaptured;
}