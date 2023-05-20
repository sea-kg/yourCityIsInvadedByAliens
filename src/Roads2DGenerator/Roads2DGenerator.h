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

#ifndef __ROADS_2D_GENERATOR_H__
#define __ROADS_2D_GENERATOR_H__

#include <vector>
#include <string>

class Roads2DGeneratorSafeLoop {
    public:
        Roads2DGeneratorSafeLoop(int nMaxLoop);
        void doIncrement();
        bool isOverMax();
        int getLoopNumber();
    private:
        int m_nMaxLoop;
        int m_nCurrentLoop;
};

class Roads2DGeneratorPseudoRandom {
    public:
        Roads2DGeneratorPseudoRandom();
        void setInitSeed(unsigned int nSeed);
        unsigned int getNextRandom();
        unsigned int getInitSeed();
        unsigned int getSeed();
    private:
        unsigned int m_nSeed;
        unsigned int m_nInitSeed;
};

class Roads2DGeneratorPoint {
    public:
        Roads2DGeneratorPoint();
        Roads2DGeneratorPoint(int x, int y);
        int getX() const;
        int getY() const;
    private:
        int m_nX;
        int m_nY;
};

class Roads2DGeneratorGraph {
    public:
        Roads2DGeneratorGraph();
        int findOrAddPointGetIndex(Roads2DGeneratorPoint point);
        void addConnection(int index1, int index2);
        const std::vector<Roads2DGeneratorPoint> &getPoints() const;
        const std::vector<std::pair<int,int>> &getConnections() const;

    private:
        std::vector<Roads2DGeneratorPoint> m_vPoints;
        std::vector<std::pair<int,int>> m_vConnections;
};

class Roads2DGeneratorConnectedComponent {
    public:
        Roads2DGeneratorConnectedComponent();
        bool hasPoint(Roads2DGeneratorPoint point);
        void addPoint(Roads2DGeneratorPoint point);
        const std::vector<Roads2DGeneratorPoint> &getPoints() const;
        bool hasIntersection(const Roads2DGeneratorConnectedComponent &component);
        void merge(const Roads2DGeneratorConnectedComponent &component);

    private:
        std::vector<Roads2DGeneratorPoint> m_vPoints;
};

class Roads2DGeneratorConnectedComponents {
    public:
        Roads2DGeneratorConnectedComponents();
        void addConnectedPoints(Roads2DGeneratorPoint point1, Roads2DGeneratorPoint point2);
        std::vector<Roads2DGeneratorConnectedComponent> getComponents();

    private:
        void mergeComponents();

        std::vector<Roads2DGeneratorConnectedComponent> m_vComponents;
};

class Roads2DGenerator {
    public:
        Roads2DGenerator(int nWidthPixels, int nHeightPixels);
        void generate(float nDensity);
        void printMap();
        std::vector<std::vector<std::string>> exportToTable();
        std::vector<std::vector<bool>> exportToPixelMap();
        Roads2DGeneratorGraph exportToGraph();

    private:
        void resetMap();
        bool isBorder(int x, int y);
        bool isAllowed(int x, int y);
        bool isRame(int x, int y);
        bool isSinglePoint(int x, int y);
        bool isEqual(std::vector<Roads2DGeneratorPoint> vLeft, std::vector<Roads2DGeneratorPoint> vRight);
        bool tryChangeToTrue(int x, int y);
        void randomInitPoints();
        int moveDiagonalTails();
        void moveDiagonalTailsLoop();
        bool checkAndRandomMove(int x, int y);
        int getAroundCount(int x, int y);
        std::vector<Roads2DGeneratorPoint> findSinglePoints();
        int drawLineByY(int x0, int x1, int y);
        int drawLineByX(int y0, int y1, int x);
        int connectPoints(Roads2DGeneratorPoint p0, Roads2DGeneratorPoint p1);
        void removeSinglePoints();
        void removeRames();
        bool canConnectClosePoints(int x, int y);
        void connectAllClosePoints();
        int removeAllShortCicles();
        void removeAllShortCiclesLoop();
        bool isDeadlockPoint(int x, int y);
        std::vector<Roads2DGeneratorPoint> findDeadlockPoints();
        Roads2DGeneratorPoint findShortPointFrom(Roads2DGeneratorPoint p0, std::vector<Roads2DGeneratorPoint> points);
        void tryConnectDeadlocksLoop();
        void removeDeadlocksLoop();
        void connectUnunionRoads();
        std::string getRoadPart(int x, int y);
        std::vector<Roads2DGeneratorConnectedComponent> findConnectedComponents();

        int m_nWidthPixels;
        int m_nHeightPixels;
        int m_nMaxMainPoints;
        std::vector<std::vector<bool>> m_vPixelMap;

        Roads2DGeneratorPseudoRandom m_random;
};


#endif // __ROADS_2D_GENERATOR_H__
