/*
MIT License

Copyright (c) 2021-2023 Evgenii Sopov (mrseakg@gmail.com)

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

// original source-code: https://github.com/sea-kg/Roads2DGenerator

#include "Roads2DGenerator.h"
#include <iostream>
#include <cmath>
#include <ctime>

// ---------------------------------------------------------------------
// Roads2DGeneratorSafeLoop

Roads2DGeneratorSafeLoop::Roads2DGeneratorSafeLoop(int nMaxLoop) {
    m_nMaxLoop = nMaxLoop;
    m_nCurrentLoop = 0;
}

void Roads2DGeneratorSafeLoop::doIncrement() {
    m_nCurrentLoop++;
}

bool Roads2DGeneratorSafeLoop::isOverMax() {
    return m_nCurrentLoop >= m_nMaxLoop;
}

int Roads2DGeneratorSafeLoop::getLoopNumber() {
    return m_nCurrentLoop;
}

// ---------------------------------------------------------------------
// Roads2DGeneratorPseudoRandom

Roads2DGeneratorPseudoRandom::Roads2DGeneratorPseudoRandom() {
    m_nSeed = 1;
}

void Roads2DGeneratorPseudoRandom::setInitSeed(unsigned int nSeed) {
    m_nSeed = nSeed;
    m_nInitSeed = nSeed;
}

unsigned int Roads2DGeneratorPseudoRandom::getNextRandom() {
    m_nSeed = std::sin(m_nSeed + 1) * float(m_nSeed + 1103515245) + 123;
    return m_nSeed;
}

unsigned int Roads2DGeneratorPseudoRandom::getInitSeed() {
    return m_nInitSeed;
}

unsigned int Roads2DGeneratorPseudoRandom::getSeed() {
    return m_nSeed;
}

// ---------------------------------------------------------------------
// Roads2DGeneratorPoint

Roads2DGeneratorPoint::Roads2DGeneratorPoint() {
    m_nX = 0;
    m_nY = 0;
}

Roads2DGeneratorPoint::Roads2DGeneratorPoint(int nX, int nY) {
    m_nX = nX;
    m_nY = nY;
}

int Roads2DGeneratorPoint::getX() const {
    return m_nX;
}

int Roads2DGeneratorPoint::getY() const {
    return m_nY;
}

// ---------------------------------------------------------------------
// Roads2DGeneratorGraph

Roads2DGeneratorGraph::Roads2DGeneratorGraph() {

}

int Roads2DGeneratorGraph::findOrAddPointGetIndex(Roads2DGeneratorPoint point) {
    for (int i = 0; i < m_vPoints.size(); i++) {
        if (m_vPoints[i].getX() == point.getX() && m_vPoints[i].getY() == point.getY()) {
            return i;
        }
    }
    m_vPoints.push_back(point);
    return m_vPoints.size() - 1;
}

void Roads2DGeneratorGraph::addConnection(int index1, int index2) {
    int _min = std::min(index1, index2);
    int _max = std::max(index1, index2);
    for (int i = 0; i < m_vConnections.size(); i++) {
        if (m_vConnections[i].first == _min && m_vConnections[i].second == _max) {
            return;
        }
    }
    m_vConnections.push_back(std::pair<int,int>(_min, _max));
}

const std::vector<Roads2DGeneratorPoint> &Roads2DGeneratorGraph::getPoints() const {
    return m_vPoints;
}

const std::vector<std::pair<int,int>> &Roads2DGeneratorGraph::getConnections() const {
    return m_vConnections;
}

// ---------------------------------------------------------------------
// Roads2DGeneratorConnectedComponent

Roads2DGeneratorConnectedComponent::Roads2DGeneratorConnectedComponent() {

}

bool Roads2DGeneratorConnectedComponent::hasPoint(Roads2DGeneratorPoint point) {
    for (int i = 0; i < m_vPoints.size(); i++) {
        if (m_vPoints[i].getX() == point.getX() && m_vPoints[i].getY() == point.getY()) {
            return true;
        }
    }
    return false;
}

void Roads2DGeneratorConnectedComponent::addPoint(Roads2DGeneratorPoint point) {
    if (!hasPoint(point)) {
        m_vPoints.push_back(point);
    }
}

const std::vector<Roads2DGeneratorPoint> &Roads2DGeneratorConnectedComponent::getPoints() const {
    return m_vPoints;
}

bool Roads2DGeneratorConnectedComponent::hasIntersection(
    const Roads2DGeneratorConnectedComponent &component
) {
    const std::vector<Roads2DGeneratorPoint> &vPoints = component.getPoints();
    for (int i = 0; i < vPoints.size(); i++) {
        if (this->hasPoint(vPoints[i])) {
            return true;
        }
    }
    return false;
}

void Roads2DGeneratorConnectedComponent::merge(
    const Roads2DGeneratorConnectedComponent &component
) {
    const std::vector<Roads2DGeneratorPoint> &vPoints = component.getPoints();
    for (int i = 0; i < vPoints.size(); i++) {
        this->addPoint(vPoints[i]);
    }
}

// ---------------------------------------------------------------------
// Roads2DGeneratorConnectedComponents

Roads2DGeneratorConnectedComponents::Roads2DGeneratorConnectedComponents() {

}

void Roads2DGeneratorConnectedComponents::addConnectedPoints(
    Roads2DGeneratorPoint p1, Roads2DGeneratorPoint p2
) {
    bool bAdded = false;
    for (int i = 0; i < m_vComponents.size(); i++) {
        if (m_vComponents[i].hasPoint(p1)) {
            m_vComponents[i].addPoint(p2);
            bAdded = true;
        }
        if (m_vComponents[i].hasPoint(p2)) {
            m_vComponents[i].addPoint(p1);
            bAdded = true;
        }
        // if (m_vComponents[i].hasPoint(p1) || m_vComponents[i].hasPoint(p2)) {
        //     m_vComponents[i].addPoint(p1);
        //     m_vComponents[i].addPoint(p2);
        //     bAdded = true;
        // }
    }
    if (!bAdded) {
        Roads2DGeneratorConnectedComponent component;
        component.addPoint(p1);
        component.addPoint(p2);
        m_vComponents.push_back(component);
    }
    mergeComponents();
}

std::vector<Roads2DGeneratorConnectedComponent> Roads2DGeneratorConnectedComponents::getComponents() {
    return m_vComponents;
}

void Roads2DGeneratorConnectedComponents::mergeComponents() {

    // copy vector
    std::vector<Roads2DGeneratorConnectedComponent> vMergedComponents = m_vComponents;
    bool bMerged = true;
    while (bMerged) {
        bMerged = false;
        for (int i = vMergedComponents.size() - 1; i >= 1; i--) {
            for (int t = i - 1; t >= 0; t--) {
                if (vMergedComponents[i].hasIntersection(vMergedComponents[t])) {
                    vMergedComponents[i].merge(vMergedComponents[t]);
                    // remove
                    vMergedComponents.erase(vMergedComponents.begin() + t);
                    bMerged = true;
                    break;
                }
            }
            if (bMerged) {
                break;
            }
        }
    }

    // for (int i0 = 0; i0 < m_vComponents.size() - 1; i0++) {
    //     bool bMerged = false;
    //     for (int i1 = i0 + 1; i1 < m_vComponents.size(); i1++) {
    //         if (m_vComponents[i1].hasIntersection(m_vComponents[i0])) {
    //             m_vComponents[i1].merge(m_vComponents[i0]);
    //             bMerged = true;
    //         }
    //     }
    //     if (!bMerged) {
    //         vMergedComponents.push_back(m_vComponents[i0]);
    //     }
    // }
    m_vComponents = vMergedComponents;
}

// ---------------------------------------------------------------------
// Roads2DGenerator

Roads2DGenerator::Roads2DGenerator(int nWidthPixels, int nHeightPixels) {
    m_nWidthPixels = nWidthPixels;
    m_nHeightPixels = nHeightPixels;
}

void Roads2DGenerator::generate(float nDensity) {
    // https://en.wikipedia.org/wiki/Wave_function_collapse
    if (nDensity > 1.0) {
        nDensity = 1.0;
    }
    if (nDensity < 0.0) {
        nDensity = 0.0;
    }
    int pixels = m_nWidthPixels*m_nHeightPixels;
    m_nMaxMainPoints = nDensity * (pixels / 2);
    m_random.setInitSeed(std::time(0));
    // m_random.setInitSeed(1686154273);


    // std::cout << "m_nWidthPixels = " << m_nWidthPixels << "; m_nHeightPixels = " << m_nHeightPixels << std::endl;

    resetMap();
    randomInitPoints();
    // printMap();

    moveDiagonalTailsLoop();
    // printMap();

    bool bAgain = true;
    while (bAgain) {
        std::vector<Roads2DGeneratorPoint> vPoints = findSinglePoints();
        if (vPoints.size() <= 1) {
            bAgain = false;
            break;
        }
        Roads2DGeneratorPoint p0 = vPoints[m_random.getNextRandom() % vPoints.size()];
        Roads2DGeneratorPoint p1 = vPoints[m_random.getNextRandom() % vPoints.size()];
        connectPoints(p0, p1);
        moveDiagonalTailsLoop();
    }

    // remove last single point
    removeSinglePoints();
    removeRames();
    connectAllClosePoints();

    // printMap();
    // removeAllShortCicles
    removeAllShortCiclesLoop();
    removeRames();
    moveDiagonalTailsLoop();

    // printMap();

    tryConnectDeadlocksLoop();
    // commented: moveDiagonalTailsLoop()

    removeAllShortCiclesLoop();
    removeRames();
    moveDiagonalTailsLoop();
    removeDeadlocksLoop();
    removeSinglePoints();
    removeRames();

    connectUnunionRoads();
    removeDeadlocksLoop();
    removeSinglePoints();
    removeRames();

    // printMap();

    // std::cout << "------- done -------" << std::endl;
    // std::cout << "init m_random: " << m_random.getInitSeed() << std::endl;

    // write_map_to_image()
    // printMap();
}

void Roads2DGenerator::printMap() {
    // std::cout << "map: " << std::endl;
    for (int y = 0; y < m_nHeightPixels; y++) {
        std::string sLine = "";
        for (int x = 0; x < m_nWidthPixels; x++) {
            #ifdef _WIN32
                if (m_vPixelMap[x][y]) {
                    sLine += char(219);
                    sLine += char(219);
                } else {
                    sLine += "  ";
                }
            #else
                std::string sFormat = "";
                if(m_vPixelMap[x][y]) {
                    sFormat = "0;30;47";
                } else {
                    sFormat = "2;31;40";
                }
                sLine += "\x1b[" + sFormat + "m  \x1b[0m";
            #endif
        }
        std::cout << sLine << std::endl;
    }
}

std::vector<std::vector<std::string>> Roads2DGenerator::exportToTable() {
    std::vector<std::vector<std::string>> vResult;
    for (int y = 0; y < m_nHeightPixels; y++) {
        std::vector<std::string> vLine;
        for (int x = 0; x < m_nWidthPixels; x++) {
            if (m_vPixelMap[x][y]) {
                vLine.push_back(getRoadPart(x,y));
            } else {
                vLine.push_back("");
            }
        }
        vResult.push_back(vLine);
    }
    return vResult;
}

std::vector<std::vector<bool>> Roads2DGenerator::exportToPixelMap() {
    return m_vPixelMap;
}

Roads2DGeneratorGraph Roads2DGenerator::exportToGraph() {
    Roads2DGeneratorGraph graph;
    for (int x = 0; x < m_nWidthPixels - 1; x++) {
        for (int y = 0; y < m_nHeightPixels - 1; y++) {
            if (m_vPixelMap[x][y]) {
                int indexXY = graph.findOrAddPointGetIndex(Roads2DGeneratorPoint(x,y));
                if (m_vPixelMap[x+1][y]) {
                    int indexX1Y = graph.findOrAddPointGetIndex(Roads2DGeneratorPoint(x+1,y));
                    graph.addConnection(indexXY, indexX1Y);
                }
                if (m_vPixelMap[x][y+1]) {
                    int indexXY1 = graph.findOrAddPointGetIndex(Roads2DGeneratorPoint(x,y+1));
                    graph.addConnection(indexXY, indexXY1);
                }
            }
        }
    }
    return graph;
}

void Roads2DGenerator::resetMap() {
    m_vPixelMap.clear();
    for (int x = 0; x < m_nWidthPixels; x++) {
        std::vector<bool> line;
        for (int y = 0; y < m_nHeightPixels; y++) {
            line.push_back(false);
        }
        m_vPixelMap.push_back(line);
    }
}

bool Roads2DGenerator::isBorder(int x, int y) {
    if (x == 0 || x == m_nWidthPixels - 1) {
        return true;
    }
    if (y == 0 || y == m_nHeightPixels - 1) {
        return true;
    }
    return false;
}


bool Roads2DGenerator::isRame(int x, int y) {
    if (isBorder(x, y)) {
        return false;
    }
    if (!m_vPixelMap[x][y]) {
        return false;
    }
    bool b00 = m_vPixelMap[x-1][y-1];
    bool b01 = m_vPixelMap[x-1][y];
    bool b02 = m_vPixelMap[x-1][y+1];
    bool b10 = m_vPixelMap[x  ][y-1];
    // bool b11 = m_vPixelMap[x  ][y]
    bool b12 = m_vPixelMap[x  ][y+1];
    bool b20 = m_vPixelMap[x+1][y-1];
    bool b21 = m_vPixelMap[x+1][y];
    bool b22 = m_vPixelMap[x+1][y+1];

    if (b00 && b01 && b02 && !b10 && !b12 && !b20 && !b21 && !b22) {
        return true;
    }
    if (b20 && b21 && b22 && !b00 && !b01 && !b02 && !b10 && !b12) {
        return true;
    }
    if (b00 && b10 && b20 && !b02 && !b12 && !b22 && !b01 && !b21) {
        return true;
    }
    if (b02 && b12 && b22 && !b00 && !b10 && !b20 && !b01 && !b21) {
        return true;
    }
    return false;
}

bool Roads2DGenerator::isEqual(std::vector<Roads2DGeneratorPoint> vLeft, std::vector<Roads2DGeneratorPoint> vRight) {
    if (vLeft.size() != vRight.size()) {
        return false;
    }
    for (int i = 0; i < vLeft.size(); i++) {
        if (vLeft[i].getX() != vRight[i].getX() && vLeft[i].getY() != vRight[i].getY()) {
            return false;
        }
    }
    return true;
}

bool Roads2DGenerator::isAllowed(int x, int y) {
    if (isBorder(x, y)) {
        return false;
    }
    x = x - 1;
    y = y - 1;
    for (int x0 = 0; x0 < 2; x0++) {
        for (int y0 = 0; y0 < 2; y0++) {
            bool b1 = m_vPixelMap[x + x0    ][y + y0    ];
            bool b2 = m_vPixelMap[x + x0 + 1][y + y0    ];
            bool b3 = m_vPixelMap[x + x0 + 1][y + y0 + 1];
            bool b4 = m_vPixelMap[x + x0    ][y + y0 + 1];
            if (b1 && b2 && b3 && b4) {
                return false;
            }
        }
    }
    return true;
}

bool Roads2DGenerator::isSinglePoint(int x, int y) {
    if (isBorder(x, y)) {
        return false;
    }
    if (!m_vPixelMap[x][y]) {
        return false;
    }
    if (getAroundCount(x,y) == 0) {
        return true;
    }
    return false;
}

bool Roads2DGenerator::tryChangeToTrue(int x, int y) {
    m_vPixelMap[x][y] = true;
    // write_map_to_image()
    if (!isAllowed(x,y)) {
        m_vPixelMap[x][y] = false;
        // write_map_to_image()
        return false;
    }
    return true;
}

void Roads2DGenerator::randomInitPoints() {
    int immp = 0;
    Roads2DGeneratorSafeLoop safeLoop(1000);
    while (immp < m_nMaxMainPoints) {
        safeLoop.doIncrement();
        int x = (m_random.getNextRandom() % (m_nWidthPixels - 2)) + 1;
        int y = (m_random.getNextRandom() % (m_nHeightPixels - 2)) + 1;
        if (tryChangeToTrue(x,y)) {
            immp += 1;
        }
        if (safeLoop.isOverMax()) {
            printMap();
            std::cout << "Roads2DGenerator::moveDiagonalTailsLoop(), nSafeWhile = " << safeLoop.getLoopNumber() << std::endl;
            exit(1);
        }
    }
}

int Roads2DGenerator::moveDiagonalTails() {
    int x = 0;
    int ret = 0;
    for (int x = 0; x < m_vPixelMap.size(); x++) {
        std::vector<bool> line = m_vPixelMap[x];
        for (int y = 0; y < line.size(); y++) {
            ret += checkAndRandomMove(x, y);
        }
    }
    return ret;
}

void Roads2DGenerator::moveDiagonalTailsLoop() {
    int mdt = moveDiagonalTails();
    Roads2DGeneratorSafeLoop safeLoop(1000);
    while (mdt > 0) {
        safeLoop.doIncrement();
        mdt = moveDiagonalTails();
        if (safeLoop.isOverMax()) {
            printMap();
            std::cout << "Roads2DGenerator::moveDiagonalTailsLoop(), nSafeWhile = " << safeLoop.getLoopNumber() << std::endl;
            exit(1);
        }
    }
}

bool Roads2DGenerator::checkAndRandomMove(int x, int y) {
    if (isBorder(x, y)) {
        return 0;
    }
    int ret = 0;
    if (m_vPixelMap[x][y] && m_vPixelMap[x+1][y+1] && !m_vPixelMap[x][y+1] && !m_vPixelMap[x+1][y]) {
        ret += 1;
        m_vPixelMap[x+1][y+1] = false;
        if (m_random.getNextRandom() % 2 == 0) {
            tryChangeToTrue(x,y+1);
        } else {
            tryChangeToTrue(x+1,y);
        }
    }
    if (!m_vPixelMap[x][y] && !m_vPixelMap[x+1][y+1] && m_vPixelMap[x][y+1] && m_vPixelMap[x+1][y]) {
        ret += 1;
        m_vPixelMap[x][y+1] = false;
        if (m_random.getNextRandom() % 2 == 0) {
            tryChangeToTrue(x,y);
        } else {
            tryChangeToTrue(x+1,y+1);
        }
    }
    return ret;
}

int Roads2DGenerator::getAroundCount(int x, int y) {
    if (isBorder(x, y)) {
        return 4;
    }
    int ret = 0;
    for (int dx = 0; dx < 3; dx++) {
        for (int dy = 0; dy < 3; dy++) {
            int x0 = x + dx - 1;
            int y0 = y + dy - 1;
            if (x0 == x && y0 == y) {
                continue;
            }
            if (m_vPixelMap[x0][y0]) {
                ret += 1;
            }
        }
    }
    return ret;
}



std::vector<Roads2DGeneratorPoint> Roads2DGenerator::findSinglePoints() {
    std::vector<Roads2DGeneratorPoint> vSinglePoints;
    for (int x = 0; x < m_vPixelMap.size(); x++) {
        std::vector<bool> line = m_vPixelMap[x];
        for (int y = 0; y < line.size(); y++) {
            if (isSinglePoint(x, y)) {
                vSinglePoints.push_back(Roads2DGeneratorPoint(x, y));
            }
        }
    }
    return vSinglePoints;
}

int Roads2DGenerator::drawLineByY(int x0, int x1, int y) {
    int ret = 0;
    int ix = std::min(x0,x1);
    int mx = std::max(x0,x1);
    for (int i = ix; i <= mx; i++) {
        if (!m_vPixelMap[i][y]) {
            if (tryChangeToTrue(i,y)) {
                ret += 1;
            }
        }
    }
    return ret;
}

int Roads2DGenerator::drawLineByX(int y0, int y1, int x) {
    int ret = 0;
    int iy = std::min(y0,y1);
    int my = std::max(y0,y1);
    for (int i = iy; i <= my; i++) {
        if (!m_vPixelMap[x][i]) {
            if (tryChangeToTrue(x,i)) {
                ret += 1;
            }
        }
    }
    return ret;
}

int Roads2DGenerator::connectPoints(Roads2DGeneratorPoint p0, Roads2DGeneratorPoint p1) {
    int ret = 0;
    int x0 = p0.getX();
    int y0 = p0.getY();
    int x1 = p1.getX();
    int y1 = p1.getY();
    if (m_random.getNextRandom() % 2 == 0) {
        ret += drawLineByY(x0, x1, y0);
        ret += drawLineByX(y0, y1, x1);
    } else {
        ret += drawLineByX(y0, y1, x0);
        ret += drawLineByY(x0, x1, y1);
    }
    return ret;
}

void Roads2DGenerator::removeSinglePoints() {
    std::vector<Roads2DGeneratorPoint> vPoints = findSinglePoints();
    for (int i = 0; i < vPoints.size(); i++) {
        int x = vPoints[i].getX();
        int y = vPoints[i].getY();
        m_vPixelMap[x][y] = false;
        // write_map_to_image()
    }
}

void Roads2DGenerator::removeRames() {
    for (int x = 0; x < m_vPixelMap.size(); x++) {
        std::vector<bool> line = m_vPixelMap[x];
        for (int y = 0; y < line.size(); y++) {
            if (isRame(x, y)) {
                m_vPixelMap[x][y] = false;
                // write_map_to_image()
            }
        }
    }
}

bool Roads2DGenerator::canConnectClosePoints(int x, int y) {
    if (isBorder(x, y)) {
        return false;
    }
    if (m_vPixelMap[x][y]) {
        return false;
    }
    if (m_vPixelMap[x][y+1] && m_vPixelMap[x][y-1]) {
        return true;
    }
    if (m_vPixelMap[x+1][y] && m_vPixelMap[x-1][y]) {
        return true;
    }
    return false;
}

void Roads2DGenerator::connectAllClosePoints() {
    for (int x = 0; x < m_vPixelMap.size(); x++) {
        std::vector<bool> line = m_vPixelMap[x];
        for (int y = 0; y < line.size(); y++) {
            int _around_n = getAroundCount(x, y);
            if (canConnectClosePoints(x, y) && _around_n < 6) {
                tryChangeToTrue(x,y);
            }
        }
    }
}

int Roads2DGenerator::removeAllShortCicles() {
    int ret = 0;
    for (int x = 0; x < m_vPixelMap.size(); x++) {
        std::vector<bool> line = m_vPixelMap[x];
        for (int y = 0; y < line.size(); y++) {
            if (getAroundCount(x, y) == 8 && !m_vPixelMap[x][y]) {
                int n = m_random.getNextRandom() % 4;
                if (n == 0) {
                    m_vPixelMap[x][y+1] = false;
                } else if (n == 1) {
                    m_vPixelMap[x][y-1] = false;
                } else if (n == 2) {
                    m_vPixelMap[x+1][y] = false;
                } else if (n == 2) {
                    m_vPixelMap[x-1][y] = false;
                }
                ret += 1;
                // write_map_to_image()
            }
        }
    }
    return ret;
}

void Roads2DGenerator::removeAllShortCiclesLoop() {
    Roads2DGeneratorSafeLoop safeLoop(1000);
    while (removeAllShortCicles() > 0) {
        safeLoop.doIncrement();
        if (safeLoop.isOverMax()) {
            printMap();
            std::cout << "Roads2DGenerator::removeAllShortCiclesLoop(), nSafeWhile = " << safeLoop.getLoopNumber() << std::endl;
            exit(1);
        }
        continue;
    }
}

bool Roads2DGenerator::isDeadlockPoint(int x, int y) {
    if (isBorder(x, y)) {
        return false;
    }
    if (!m_vPixelMap[x][y]) {
        return false;
    }
    int count = 0;
    if (m_vPixelMap[x-1][y]) count += 1;
    if (m_vPixelMap[x+1][y]) count += 1;
    if (m_vPixelMap[x][y+1]) count += 1;
    if (m_vPixelMap[x][y-1]) count += 1;
    return count == 1;
}

std::vector<Roads2DGeneratorPoint> Roads2DGenerator::findDeadlockPoints() {
    std::vector<Roads2DGeneratorPoint> vDeadlockPoints;
    for (int x = 0; x < m_vPixelMap.size(); x++) {
        std::vector<bool> line = m_vPixelMap[x];
        for (int y = 0; y < line.size(); y++) {
            if (isDeadlockPoint(x, y)) {
                vDeadlockPoints.push_back(Roads2DGeneratorPoint(x,y));
            }
        }
    }
    return vDeadlockPoints;
}

Roads2DGeneratorPoint Roads2DGenerator::findShortPointFrom(Roads2DGeneratorPoint p0, std::vector<Roads2DGeneratorPoint> points) {
    int x0 = p0.getX();
    int y0 = p0.getY();
    int found_x1 = x0;
    int found_y1 = y0;
    int dist = m_vPixelMap.size() + m_vPixelMap[0].size() + 1; // max dist
    for (int i = 0; i < points.size(); i++) {
        int x1 = points[i].getX();
        int y1 = points[i].getY();
        if (x1 == x0 && y1 == y0) {
            continue;
        }
        int x_max = std::max(x0, x1);
        int x_min = std::min(x0, x1);
        int y_max = std::max(y0, y1);
        int y_min = std::min(y0, y1);
        int new_dist = (x_max - x_min) + (y_max - y_min);
        if (new_dist < dist) {
            dist = new_dist;
            found_x1 = x1;
            found_y1 = y1;
        }
    }
    return Roads2DGeneratorPoint(found_x1, found_y1);
}

void Roads2DGenerator::tryConnectDeadlocksLoop() {
    std::vector<Roads2DGeneratorPoint> vDeadlocks = findDeadlockPoints();
    Roads2DGeneratorSafeLoop safeLoop(100);
    while (vDeadlocks.size() > 0) {
        safeLoop.doIncrement();
        int pn0 = m_random.getNextRandom() % vDeadlocks.size();
        Roads2DGeneratorPoint p0 = vDeadlocks[pn0];
        Roads2DGeneratorPoint p1 = findShortPointFrom(p0, vDeadlocks);
        // # print(p0, p1)
        int connected = connectPoints(p0, p1);
        if (connected == 0) {
            int x = p0.getX();
            int y = p0.getY();
            m_vPixelMap[x][y] = false;
            // write_map_to_image()
        }

        // fix infinity looop
        std::vector<Roads2DGeneratorPoint> vTmpDeadlocks = findDeadlockPoints();
        if (isEqual(vTmpDeadlocks, vDeadlocks)) {
            removeDeadlocksLoop();
        }

        // next interation
        removeSinglePoints();
        vDeadlocks = findDeadlockPoints();

        // if (safeLoop.getLoopNumber() > 90) {
        //     std::cout << safeLoop.getLoopNumber() << std::endl;
        //     printMap();
        // }
        if (safeLoop.isOverMax()) {
            // fix and break from cicle
            removeDeadlocksLoop();
            break;
            // std::cout << "max" << std::endl;
            // printMap();
            // std::cout
            //     << "Roads2DGenerator::tryConnectDeadlocksLoop(), "
            //     << "nSafeWhile = " << safeLoop.getLoopNumber()
            //     << ", getInitSeed m_random = " << m_random.getInitSeed()
            //     << ", vDeadlocks.size() = " << vDeadlocks.size()
            //     << std::endl;
            // exit(1);
        }
    }
}

void Roads2DGenerator::removeDeadlocksLoop() {
    std::vector<Roads2DGeneratorPoint> vDeadlocks = findDeadlockPoints();
    while (vDeadlocks.size() > 0) {
        int x = vDeadlocks[0].getX();
        int y = vDeadlocks[0].getY();
        m_vPixelMap[x][y] = false;
        // write_map_to_image();
        vDeadlocks = findDeadlockPoints();
    }
}

void Roads2DGenerator::connectUnunionRoads() {
    std::vector<Roads2DGeneratorConnectedComponent> comps = findConnectedComponents();
    Roads2DGeneratorSafeLoop safeLoop(100);
    while (comps.size() > 1) {
        Roads2DGeneratorPoint p0 = comps[0].getPoints()[m_random.getNextRandom() % comps[0].getPoints().size()];
        Roads2DGeneratorPoint p1 = comps[1].getPoints()[m_random.getNextRandom() % comps[1].getPoints().size()];
        connectPoints(p0, p1);
        moveDiagonalTailsLoop();
        comps = findConnectedComponents();

        safeLoop.doIncrement();
        if (safeLoop.isOverMax()) {
            printMap();
            std::cout << "Roads2DGenerator::connectUnunionRoads(), nSafeWhile = " << safeLoop.getLoopNumber() << std::endl;
            exit(1);
        }
    }
    // std::cout << "comps.size() = " << comps.size() << std::endl;
}

std::string Roads2DGenerator::getRoadPart(int x, int y) {
    if (x < 0 || x >= m_nWidthPixels || y < 0 || y >= m_nHeightPixels) {
        return "error";
    }
    if (!m_vPixelMap[x][y]) {
        return "";
    }

    bool left = m_vPixelMap[x][y-1];
    bool right = m_vPixelMap[x][y+1];
    bool top = m_vPixelMap[x-1][y];
    bool bottom = m_vPixelMap[x+1][y];

    if (left && right && top && bottom) {
        return "cross";
    }
    if (left && right && ! top && ! bottom) {
        return "horizontal";
    }
    if (! left && ! right && top && bottom) {
        return "vertical";
    }
    if (! left && right && ! top && bottom) {
        return "right-down";
    }
    if (left && ! right && ! top && bottom) {
        return "left-down";
    }
    if (! left && right && top && ! bottom) {
        return "right-up";
    }
    if (left && ! right && top && ! bottom) {
        return "left-up";
    }
    if (left && ! right && top && bottom) {
        return "left-up-down";
    }
    if (! left && right && top && bottom) {
        return "right-up-down";
    }
    if (left && right && ! top && bottom) {
        return "left-right-down";
    }
    if (left && right && top && ! bottom) {
        return "left-right-up";
    }
    return "unknown";
}

std::vector<Roads2DGeneratorConnectedComponent> Roads2DGenerator::findConnectedComponents() {
    Roads2DGeneratorConnectedComponents components;
    int nPoints = 0;
    for (int x = 0; x < m_nWidthPixels-1; x++) {
        for (int y = 0; y < m_nHeightPixels-1; y++) {
            Roads2DGeneratorPoint p0(x,y);
            Roads2DGeneratorPoint p1(x+1,y);
            Roads2DGeneratorPoint p2(x,y+1);
            if (m_vPixelMap[p0.getX()][p0.getY()]) {
                nPoints++;
            }
            if (m_vPixelMap[p0.getX()][p0.getY()] && m_vPixelMap[p1.getX()][p1.getY()]) {
                components.addConnectedPoints(p0, p1);
            }
            if (m_vPixelMap[p0.getX()][p0.getY()] && m_vPixelMap[p2.getX()][p2.getY()]) {
                components.addConnectedPoints(p0, p2);
            }
        }
    }
    // std::cout << "nPoints = " << nPoints << std::endl;
    std::vector<Roads2DGeneratorConnectedComponent> vComponents = components.getComponents();
    // for (int i = 0; i < vComponents.size(); i++) {
    //     std::cout << "vComponents[i].size() = " << vComponents[i].getPoints().size() << std::endl;
    // }
    return vComponents;
}
