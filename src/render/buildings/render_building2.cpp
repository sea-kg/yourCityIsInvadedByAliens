#include "render_building2.h"
#include "sea5kg_triangulation.h"


// ---------------------------------------------------------------------
// RenderBuilding2

RenderBuilding2::RenderBuilding2(GameBuilding *pBuilding, SDL_Texture* pTexture) 
: RenderObject(600) {
    m_pBuilding = pBuilding;
    m_pTexture = pTexture;
    m_currentFrame.x = 0;
    m_currentFrame.y = 0;
    m_currentFrame.w = 50;
    m_currentFrame.h = 50;

    const std::vector<CoordXY> &vPoints = m_pBuilding->getPoints();
    int nMaxX = vPoints[0].x();
    int nMinX = vPoints[0].x();
    int nMaxY = vPoints[0].y();
    int nMinY = vPoints[0].y();

    RenderColor buildingColor(255, 57, 57, 255);

    for (int i = 0; i < vPoints.size(); i++) {
        CoordXY p0 = vPoints[i];
        CoordXY p1 = vPoints[(i+1) % vPoints.size()];
        
        RenderLine *pLine = new RenderLine(p0, p1, buildingColor, getPositionZ());
        m_vBorderLines.push_back(pLine);
        nMaxX = std::max(p0.x(), nMaxX);
        nMinX = std::min(p0.x(), nMinX);
        nMaxY = std::max(p0.y(), nMaxY);
        nMinY = std::min(p0.y(), nMinY);
    }

    Sea5kgTriangulationTriangulator *pTriangulator = new Sea5kgTriangulationTriangulator();
    Sea5kgTriangulationArea area("some");
    area.setCountTriangles(25);
    for (int i = 0; i < vPoints.size(); i++) {
        area.addPoint(vPoints[i].x(), vPoints[i].y());
    }
    pTriangulator->addArea(area);
    pTriangulator->triangulate();
    const std::vector<Sea5kgTriangulationTriangle> &vTriangles = pTriangulator->getTriangles();
    if (vTriangles.size() > 0) {
        std::cout << "vTriangles.size() : " <<  vTriangles.size() << std::endl;
    }
    for (int i = 0; i < vTriangles.size(); i++) {
        Sea5kgTriangulationTriangle tr = vTriangles[i];
        // calculate center of triangle
        int x = (tr.p1.getXint() + tr.p2.getXint() + tr.p3.getXint()) / 3;
        int y = (tr.p1.getYint() + tr.p2.getYint() + tr.p3.getYint()) / 3;
        m_vFillPointsAbsolute.push_back(CoordXY(x,y));
        m_vFillPoints.push_back(CoordXY(x,y));

        m_vLines.push_back(
            new RenderLine(
                CoordXY(tr.p1.getXint(),tr.p1.getYint()),
                CoordXY(tr.p2.getXint(),tr.p2.getYint()),
                buildingColor
            )
        );
        m_vLines.push_back(
            new RenderLine(
                CoordXY(tr.p2.getXint(),tr.p2.getYint()),
                CoordXY(tr.p3.getXint(),tr.p3.getYint()),
                buildingColor
            )
        );
        m_vLines.push_back(
            new RenderLine(
                CoordXY(tr.p3.getXint(),tr.p3.getYint()),
                CoordXY(tr.p1.getXint(),tr.p1.getYint()),
                buildingColor
            )
        );
    }

    // pTriangulator->getAreas();
    // vAreas[i].getCountTriangles();

    /*int step = 50;
    for (int x = m_nMinX; x < m_nMaxX; x += step) {
        for (int y = m_nMinY; y < m_nMaxY; y += step) {
            CoordXY p(x,y);
           
            if (containsPoint(vPoints, p)) {
                m_vFillPointsAbsolute.push_back(CoordXY(x,y));
                m_vFillPoints.push_back(CoordXY(x,y));
            }
        }
    }*/
    m_minPos = CoordXY(nMinX, nMinY);
    m_maxPos = CoordXY(nMaxX, nMaxY);
}

void RenderBuilding2::modify(const GameState& state, IRenderWindow* pRenderWindow) {
    for (int i = 0; i < m_vBorderLines.size(); i++) {
        m_vBorderLines[i]->modify(state, pRenderWindow);
    }

    for (int i = 0; i < m_vFillPointsAbsolute.size(); i++) {
        m_vFillPoints[i] = m_vFillPointsAbsolute[i] - state.getCoordLeftTop();
    }

    for (int i = 0; i < m_vLines.size(); i++) {
        m_vLines[i]->modify(state, pRenderWindow);
    }
}

bool RenderBuilding2::canDraw(const GameState& state) {
    return
        m_minPos.isInsideRect(state.getWindowRect())
        || m_maxPos.isInsideRect(state.getWindowRect())
    ;
}

void RenderBuilding2::draw(SDL_Renderer* renderer) {

    for (int i = 0; i < m_vBorderLines.size(); i++) {
        m_vBorderLines[i]->draw(renderer);
    }

    for (int i = 0; i < m_vLines.size(); i++) {
        m_vLines[i]->draw(renderer);
    }

    SDL_Rect dst;
    dst.w = m_currentFrame.w;
    dst.h = m_currentFrame.h;

    for (int i = 0; i < m_vFillPoints.size(); i++) {
        dst.x = m_vFillPoints[i].x() - 18;
        dst.y = m_vFillPoints[i].y() - 32;
        SDL_RenderCopy(renderer, m_pTexture, &m_currentFrame, &dst);
    }
}

CoordXY RenderBuilding2::getMinPoint() {
    return m_minPos;
}

CoordXY RenderBuilding2::getMaxPoint() {
    return m_maxPos;
}

bool RenderBuilding2::containsPoint(const std::vector<CoordXY> &vPoints, const CoordXY &point) {
    // https://ru.stackoverflow.com/questions/464787/%D0%A2%D0%BE%D1%87%D0%BA%D0%B0-%D0%B2%D0%BD%D1%83%D1%82%D1%80%D0%B8-%D0%BC%D0%BD%D0%BE%D0%B3%D0%BE%D1%83%D0%B3%D0%BE%D0%BB%D1%8C%D0%BD%D0%B8%D0%BA%D0%B0

    bool bResult = false;
    int size = vPoints.size();
    int j = size - 1;
    for (int i = 0; i < size; i++) {
        if ( (vPoints[i].y() < point.y() && vPoints[j].y() >= point.y() || vPoints[j].y() < point.y() && vPoints[i].y() >= point.y()) &&
            (vPoints[i].x() + (point.y() - vPoints[i].y()) / (vPoints[j].y() - vPoints[i].y()) * (vPoints[j].x() - vPoints[i].x()) < point.x()) ) {
            bResult = !bResult;
        }
        j = i;
    }
    return bResult;
}