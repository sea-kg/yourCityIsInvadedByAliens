
#include "Roads2DGenerator.h"
#include <iostream>
#include <math.h>
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

int Roads2DGeneratorPoint::getX() {
    return m_nX;
}

int Roads2DGeneratorPoint::getY() {
    return m_nY;
}

// ---------------------------------------------------------------------
// Roads2DGenerator

Roads2DGenerator::Roads2DGenerator(int nWidthPixels, int nHeightPixels) {
    m_nWidthPixels = nWidthPixels;
    m_nHeightPixels = nHeightPixels;
}

void Roads2DGenerator::generate(float nDensity) {
    if (nDensity > 1.0) {
        nDensity = 1.0;
    }
    if (nDensity < 0.0) {
        nDensity = 0.0;
    }
    int pixels = m_nWidthPixels*m_nHeightPixels;
    m_nMaxMainPoints = nDensity * (pixels / 2);
    random.setInitSeed(std::time(0));
    // random.setInitSeed(1684782685);


    // std::cout << "m_nWidthPixels = " << m_nWidthPixels << "; m_nHeightPixels = " << m_nHeightPixels << std::endl;

    reset_map();
    random_main_points();
    // print_map();

    move_diagonal_tails_loop();
    // print_map();

    bool bAgain = true;
    while (bAgain) {
        std::vector<Roads2DGeneratorPoint> vPoints = find_single_points();
        if (vPoints.size() <= 1) {
            bAgain = false;
            break;
        }
        Roads2DGeneratorPoint p0 = vPoints[random.getNextRandom() % vPoints.size()];
        Roads2DGeneratorPoint p1 = vPoints[random.getNextRandom() % vPoints.size()];
        connect_points(p0, p1);
        move_diagonal_tails_loop();
    }

    // remove last single point
    remove_single_points();
    remove_rames();
    connect_all_close_points();

    // print_map();
    // remove_all_short_cicles
    remove_all_short_cicles_loop();
    remove_rames();
    move_diagonal_tails_loop();

    // print_map();

    tryConnectDeadlocksLoop();
    // commented: move_diagonal_tails_loop()

    remove_all_short_cicles_loop();
    remove_rames();
    move_diagonal_tails_loop();
    remove_deadlocks_loop();
    remove_single_points();
    remove_rames();

    // print_map();

    // std::cout << "------- done -------" << std::endl;
    // write_map_to_image()
    // print_map();
}

void Roads2DGenerator::print_map() {
    // std::cout << "map: " << std::endl;
    for (int y = 0; y < m_nHeightPixels; y++) {
        std::string sLine = "";
        for (int x = 0; x < m_nWidthPixels; x++) {
            std::string sFormat = "";
            if(m_vPixelMap[x][y]) {
                sFormat = "0;30;47";
            } else {
                sFormat = "2;31;40";
            }
            sLine += "\x1b[" + sFormat + "m  \x1b[0m";
        }
        std::cout << sLine << std::endl;
    }
}

std::vector<std::vector<std::string>> Roads2DGenerator::exportToTable() {
    std::vector<std::vector<std::string>> vTextures;
    for (int y = 0; y < m_nHeightPixels; y++) {
        std::vector<std::string> vLine;
        for (int x = 0; x < m_nWidthPixels; x++) {
            if (m_vPixelMap[x][y]) {
                vLine.push_back(get_road_part(x,y));
            } else {
                vLine.push_back("");
            }
        }
    }
    return vTextures;
}

void Roads2DGenerator::reset_map() {
    m_vPixelMap.clear();
    for (int x = 0; x < m_nWidthPixels; x++) {
        std::vector<bool> line;
        for (int y = 0; y < m_nHeightPixels; y++) {
            line.push_back(false);
        }
        m_vPixelMap.push_back(line);
    }
}

bool Roads2DGenerator::is_border(int x, int y) {
    if (x == 0 || x == m_nWidthPixels - 1) {
        return true;
    }
    if (y == 0 || y == m_nHeightPixels - 1) {
        return true;
    }
    return false;
}

bool Roads2DGenerator::is_allowed(int x, int y) {
    if (is_border(x, y)) {
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

bool Roads2DGenerator::try_change_to_true(int x, int y) {
    m_vPixelMap[x][y] = true;
    // write_map_to_image()
    if (!is_allowed(x,y)) {
        m_vPixelMap[x][y] = false;
        // write_map_to_image()
        return false;
    }
    return true;
}

void Roads2DGenerator::random_main_points() {
    int immp = 0;
    Roads2DGeneratorSafeLoop safeLoop(1000);
    while (immp < m_nMaxMainPoints) {
        safeLoop.doIncrement();
        int x = (random.getNextRandom() % (m_nWidthPixels - 2)) + 1;
        int y = (random.getNextRandom() % (m_nHeightPixels - 2)) + 1;
        if (try_change_to_true(x,y)) {
            immp += 1;
        }
        if (safeLoop.isOverMax()) {
            print_map();
            std::cout << "Roads2DGenerator::move_diagonal_tails_loop(), nSafeWhile = " << safeLoop.getLoopNumber() << std::endl;
            exit(1);
        }
    }
}

int Roads2DGenerator::move_diagonal_tails() {
    int x = 0;
    int ret = 0;
    for (int x = 0; x < m_vPixelMap.size(); x++) {
        std::vector<bool> line = m_vPixelMap[x];
        for (int y = 0; y < line.size(); y++) {
            ret += check_and_random_move(x, y);
        }
    }
    return ret;
}

void Roads2DGenerator::move_diagonal_tails_loop() {
    int mdt = move_diagonal_tails();
    Roads2DGeneratorSafeLoop safeLoop(1000);
    while (mdt > 0) {
        safeLoop.doIncrement();
        mdt = move_diagonal_tails();
        if (safeLoop.isOverMax()) {
            print_map();
            std::cout << "Roads2DGenerator::move_diagonal_tails_loop(), nSafeWhile = " << safeLoop.getLoopNumber() << std::endl;
            exit(1);
        }
    }
}

bool Roads2DGenerator::check_and_random_move(int x, int y) {
    if (is_border(x, y)) {
        return 0;
    }
    int ret = 0;
    if (m_vPixelMap[x][y] && m_vPixelMap[x+1][y+1] && !m_vPixelMap[x][y+1] && !m_vPixelMap[x+1][y]) {
        ret += 1;
        m_vPixelMap[x+1][y+1] = false;
        if (random.getNextRandom() % 2 == 0) {
            try_change_to_true(x,y+1);
        } else {
            try_change_to_true(x+1,y);
        }
    }
    if (!m_vPixelMap[x][y] && !m_vPixelMap[x+1][y+1] && m_vPixelMap[x][y+1] && m_vPixelMap[x+1][y]) {
        ret += 1;
        m_vPixelMap[x][y+1] = false;
        if (random.getNextRandom() % 2 == 0) {
            try_change_to_true(x,y);
        } else {
            try_change_to_true(x+1,y+1);
        }
    }
    return ret;
}

int Roads2DGenerator::get_around_count(int x, int y) {
    if (is_border(x, y)) {
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

bool Roads2DGenerator::is_single_point(int x, int y) {
    if (is_border(x, y)) {
        return false;
    }
    if (!m_vPixelMap[x][y]) {
        return false;
    }
    if (get_around_count(x,y) == 0) {
        return true;
    }
    return false;
}

std::vector<Roads2DGeneratorPoint> Roads2DGenerator::find_single_points() {
    std::vector<Roads2DGeneratorPoint> vSinglePoints;
    for (int x = 0; x < m_vPixelMap.size(); x++) {
        std::vector<bool> line = m_vPixelMap[x];
        for (int y = 0; y < line.size(); y++) {
            if (is_single_point(x, y)) {
                vSinglePoints.push_back(Roads2DGeneratorPoint(x, y));
            }
        }
    }
    return vSinglePoints;
}

int Roads2DGenerator::drawline_by_y(int x0, int x1, int y) {
    int ret = 0;
    int ix = std::min(x0,x1);
    int mx = std::max(x0,x1);
    for (int i = ix; i <= mx; i++) {
        if (!m_vPixelMap[i][y]) {
            if (try_change_to_true(i,y)) {
                ret += 1;
            }
        }
    }
    return ret;
}

int Roads2DGenerator::drawline_by_x(int y0, int y1, int x) {
    int ret = 0;
    int iy = std::min(y0,y1);
    int my = std::max(y0,y1);
    for (int i = iy; i <= my; i++) {
        if (!m_vPixelMap[x][i]) {
            if (try_change_to_true(x,i)) {
                ret += 1;
            }
        }
    }
    return ret;
}

int Roads2DGenerator::connect_points(Roads2DGeneratorPoint p0, Roads2DGeneratorPoint p1) {
    int ret = 0;
    int x0 = p0.getX();
    int y0 = p0.getY();
    int x1 = p1.getX();
    int y1 = p1.getY();
    if (random.getNextRandom() % 2 == 0) {
        ret += drawline_by_y(x0, x1, y0);
        ret += drawline_by_x(y0, y1, x1);
    } else {
        ret += drawline_by_x(y0, y1, x0);
        ret += drawline_by_y(x0, x1, y1);
    }
    return ret;
}

void Roads2DGenerator::remove_single_points() {
    std::vector<Roads2DGeneratorPoint> vPoints = find_single_points();
    for (int i = 0; i < vPoints.size(); i++) {
        int x = vPoints[i].getX();
        int y = vPoints[i].getY();
        m_vPixelMap[x][y] = false;
        // write_map_to_image()
    }
}

bool Roads2DGenerator::is_rame(int x, int y) {
    if (is_border(x, y)) {
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

void Roads2DGenerator::remove_rames() {
    for (int x = 0; x < m_vPixelMap.size(); x++) {
        std::vector<bool> line = m_vPixelMap[x];
        for (int y = 0; y < line.size(); y++) {
            if (is_rame(x, y)) {
                m_vPixelMap[x][y] = false;
                // write_map_to_image()
            }
        }
    }
}

bool Roads2DGenerator::can_connect_close_points(int x, int y) {
    if (is_border(x, y)) {
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

void Roads2DGenerator::connect_all_close_points() {
    for (int x = 0; x < m_vPixelMap.size(); x++) {
        std::vector<bool> line = m_vPixelMap[x];
        for (int y = 0; y < line.size(); y++) {
            int _around_n = get_around_count(x, y);
            if (can_connect_close_points(x, y) && _around_n < 6) {
                try_change_to_true(x,y);
            }
        }
    }
}

int Roads2DGenerator::remove_all_short_cicles() {
    int ret = 0;
    for (int x = 0; x < m_vPixelMap.size(); x++) {
        std::vector<bool> line = m_vPixelMap[x];
        for (int y = 0; y < line.size(); y++) {
            if (get_around_count(x, y) == 8 && !m_vPixelMap[x][y]) {
                int n = random.getNextRandom() % 4;
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

void Roads2DGenerator::remove_all_short_cicles_loop() {
    Roads2DGeneratorSafeLoop safeLoop(1000);
    while (remove_all_short_cicles() > 0) {
        safeLoop.doIncrement();
        if (safeLoop.isOverMax()) {
            print_map();
            std::cout << "Roads2DGenerator::remove_all_short_cicles_loop(), nSafeWhile = " << safeLoop.getLoopNumber() << std::endl;
            exit(1);
        }
        continue;
    }
}

bool Roads2DGenerator::is_deadlock_point(int x, int y) {
    if (is_border(x, y)) {
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
            if (is_deadlock_point(x, y)) {
                vDeadlockPoints.push_back(Roads2DGeneratorPoint(x,y));
            }
        }
    }
    return vDeadlockPoints;
}

Roads2DGeneratorPoint Roads2DGenerator::find_short_point_from(Roads2DGeneratorPoint p0, std::vector<Roads2DGeneratorPoint> points) {
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

void Roads2DGenerator::tryConnectDeadlocksLoop() {
    std::vector<Roads2DGeneratorPoint> vDeadlocks = findDeadlockPoints();
    Roads2DGeneratorSafeLoop safeLoop(100);
    while (vDeadlocks.size() > 0) {
        safeLoop.doIncrement();
        int pn0 = random.getNextRandom() % vDeadlocks.size();
        Roads2DGeneratorPoint p0 = vDeadlocks[pn0];
        Roads2DGeneratorPoint p1 = find_short_point_from(p0, vDeadlocks);
        // # print(p0, p1)
        int connected = connect_points(p0, p1);
        if (connected == 0) {
            int x = p0.getX();
            int y = p0.getY();
            m_vPixelMap[x][y] = false;
            // write_map_to_image()
        }

        // fix infinity looop
        std::vector<Roads2DGeneratorPoint> vTmpDeadlocks = findDeadlockPoints();
        if (isEqual(vTmpDeadlocks, vDeadlocks)) {
            remove_deadlocks_loop();
        }

        // next interation
        remove_single_points();
        vDeadlocks = findDeadlockPoints();

        // if (safeLoop.getLoopNumber() > 90) {
        //     std::cout << safeLoop.getLoopNumber() << std::endl;
        //     print_map();
        // }
        if (safeLoop.isOverMax()) {
            // fix and break from cicle
            remove_deadlocks_loop();
            break;
            // std::cout << "max" << std::endl;
            // print_map();
            // std::cout
            //     << "Roads2DGenerator::tryConnectDeadlocksLoop(), "
            //     << "nSafeWhile = " << safeLoop.getLoopNumber()
            //     << ", getInitSeed random = " << random.getInitSeed()
            //     << ", vDeadlocks.size() = " << vDeadlocks.size()
            //     << std::endl;
            // exit(1);
        }
    }
}

void Roads2DGenerator::remove_deadlocks_loop() {
    std::vector<Roads2DGeneratorPoint> vDeadlocks = findDeadlockPoints();
    while (vDeadlocks.size() > 0) {
        int x = vDeadlocks[0].getX();
        int y = vDeadlocks[0].getY();
        m_vPixelMap[x][y] = false;
        // write_map_to_image();
        vDeadlocks = findDeadlockPoints();
    }
}

std::string Roads2DGenerator::get_road_part(int x, int y) {
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

