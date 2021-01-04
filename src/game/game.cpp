#include "game.h"
#include "wsjcpp_core.h"

// ---------------------------------------------------------------------
// CoordXY

CoordXY::CoordXY() {
    m_nX = 0;
    m_nY = 0;
}

CoordXY::CoordXY(int x, int y) {
    m_nX = x;
    m_nY = y;
}

int CoordXY::x() const {
    return m_nX;
}

int CoordXY::y() const {
    return m_nY;
}

void CoordXY::update(int x, int y) {
    m_nX = x;
    m_nY = y;
}

CoordXY& CoordXY::operator+=(const CoordXY& other) {
    m_nX = m_nX + other.x();
    m_nY = m_nY + other.y();
    return *this;
}

CoordXY& CoordXY::operator-=(const CoordXY& other) {
    m_nX = m_nX - other.x();
    m_nY = m_nY - other.y();
    return *this;
}

CoordXY CoordXY::operator+(const CoordXY& other) const {
    CoordXY c(m_nX, m_nY);
    c += other;
    return c;
}

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