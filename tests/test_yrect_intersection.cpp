#include "yrect.h"

#include <vector>
#include <iostream>

int main() {
    struct DataTest {
        DataTest(const YRect &rect, bool bHasIntersection) : rect(rect), bHasIntersection(bHasIntersection) {};
        YRect rect;
        bool bHasIntersection;
    };

    YRect br(100, 100, 200, 200);

    std::vector<DataTest> vTests;
    vTests.push_back(DataTest(YRect(100, 100, 200, 200), true));
    vTests.push_back(DataTest(YRect(200, 100, 300, 200), true));
    vTests.push_back(DataTest(YRect(0, 100, 100, 200), true));
    vTests.push_back(DataTest(YRect(100, 200, 200, 300), true));
    vTests.push_back(DataTest(YRect(100, 0, 200, 100), true));
    vTests.push_back(DataTest(YRect(150, 150, 250, 250), true));
    vTests.push_back(DataTest(YRect(300, 300, 400, 400), false));
    vTests.push_back(DataTest(YRect(50, 125, 250, 175), true));
    vTests.push_back(DataTest(YRect(125, 50, 175, 250), true));
    vTests.push_back(DataTest(YRect(125, 50, 175, 75), false));


    int nRes = 0;
    int nCounter = 0;
    int nAlltests = vTests.size()*2;
    for (int i = 0; i < vTests.size(); ++i) {
        nCounter++;
        YRect r = vTests[i].rect;
        bool bIntersect = br.hasIntersection(r);
        if (bIntersect != vTests[i].bHasIntersection) {
            std::cerr
                << "(" << nCounter << "/" << nAlltests << ") "
                << "Expeceted " << (vTests[i].bHasIntersection ? "has" : "hasn't") << " intersection "
                << " br(" << br.getMinX() << "," << br.getMinY() << "," << br.getMaxX() << "," << br.getMaxY() << ") "
                << " and rect" << i << "(" << r.getMinX() << "," << r.getMinY() << "," << r.getMaxX() << "," << r.getMaxY() << ") "
                << ", but " << (bIntersect ? "has" : "hasn't") << " intersection "
                << std::endl;
            nRes++;
        } else {
            std::cout
                << "(" << nCounter << "/" << nAlltests << ") "
                << "Successfull " << (vTests[i].bHasIntersection ? "has" : "hasn't") << " intersection "
                << " br(" << br.getMinX() << "," << br.getMinY() << "," << br.getMaxX() << "," << br.getMaxY() << ") "
                << " and rect" << i << "(" << r.getMinX() << "," << r.getMinY() << "," << r.getMaxX() << "," << r.getMaxY() << ") "
                << std::endl;
        }
        nCounter++;
        bIntersect = r.hasIntersection(br);
        if (bIntersect != vTests[i].bHasIntersection) {
            std::cerr
                << "(" << nCounter << "/" << nAlltests << ") "
                << "Expeceted " << (vTests[i].bHasIntersection ? "has" : "hasn't") << " intersection "
                << " rect" << i << "(" << r.getMinX() << "," << r.getMinY() << "," << r.getMaxX() << "," << r.getMaxY() << ") "
                << " and br(" << br.getMinX() << "," << br.getMinY() << "," << br.getMaxX() << "," << br.getMaxY() << ") "
                << ", but " << (bIntersect ? "has" : "hasn't") << " intersection "
                << std::endl;
            nRes++;
        } else {
            std::cout
                << "(" << nCounter << "/" << nAlltests << ") "
                << "Successfull " << (vTests[i].bHasIntersection ? "has" : "hasn't") << " intersection "
                << " rect" << i << "(" << r.getMinX() << "," << r.getMinY() << "," << r.getMaxX() << "," << r.getMaxY() << ") "
                << " and br(" << br.getMinX() << "," << br.getMinY() << "," << br.getMaxX() << "," << br.getMaxY() << ") "
                << std::endl;
        }
    };

    return nRes;
}