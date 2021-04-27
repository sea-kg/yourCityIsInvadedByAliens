#include "render.h"
#include "sea5kg_triangulation.h"
#include <cmath>
#define PI 3.14159265

// ---------------------------------------------------------------------
// RenderLine

RenderLine::RenderLine(const CoordXY &p1, const CoordXY &p2, const RenderColor &color, int nPositionZ) 
: RenderObject(nPositionZ), m_color(color) {
    m_coord1 = p1;
    m_coord2 = p2;
    m_startCoord1 = p1;
    m_startCoord2 = p2;
}

void RenderLine::modify(const GameState& state, IRenderWindow* pRenderWindow) {
    m_coord1 = m_startCoord1 - state.getCoordLeftTop();
    m_coord2 = m_startCoord2 - state.getCoordLeftTop() ;
}

void RenderLine::draw(SDL_Renderer* renderer) {
    m_color.changeRenderColor(renderer);
    SDL_RenderDrawLine(renderer, m_coord1.x(), m_coord1.y(), m_coord2.x(), m_coord2.y());
}

const CoordXY &RenderLine::getAbsoluteCoord1() {
    return m_startCoord1;
}

const CoordXY &RenderLine::getAbsoluteCoord2() {
    return m_startCoord2;
}

const CoordXY &RenderLine::getCoord1() {
    return m_coord1;
}

const CoordXY &RenderLine::getCoord2() {
    return m_coord2;
}

void RenderLine::updateAbsoluteCoords(const CoordXY &p1, const CoordXY &p2) {
    m_startCoord1 = p1;
    m_startCoord2 = p2;
}

// ---------------------------------------------------------------------
// RenderTriangle

RenderTriangle::RenderTriangle(
    const CoordXY &p1,
    const CoordXY &p2,
    const CoordXY &p3,
    int nPositionZ
) : RenderObject(nPositionZ),
    m_color(255,255,255,255),
    m_line1(p1,p2, m_color),
    m_line2(p2,p3, m_color),
    m_line3(p3,p1, m_color)
{
    // nothing
}

void RenderTriangle::modify(const GameState& state, IRenderWindow* pRenderWindow) {
    m_line1.modify(state, pRenderWindow);
    m_line2.modify(state, pRenderWindow);
    m_line3.modify(state, pRenderWindow);
}

void RenderTriangle::draw(SDL_Renderer* renderer) {
    m_line1.draw(renderer);
    m_line2.draw(renderer);
    m_line3.draw(renderer);
}

// ---------------------------------------------------------------------
// RenderRectTexture

RenderRectTexture::RenderRectTexture(
    const CoordXY &p0, 
    SDL_Texture* tex,
    int nTextureWidth, 
    int nTextureHeight,
    int nPositionZ
) : RenderObject(nPositionZ) {
    m_pTexture = tex;
    m_coordCenter = p0;
    currentFrame.x = 0;
    currentFrame.y = 0;
    currentFrame.w = nTextureWidth;
    currentFrame.h = nTextureHeight;
}

void RenderRectTexture::modify(const GameState& state, IRenderWindow* pRenderWindow) {
    m_coordReal = m_coordCenter - state.getCoordLeftTop();

};

void RenderRectTexture::draw(SDL_Renderer* renderer) {
    SDL_Rect dst;
    // 4 is scale
    dst.x = m_coordReal.x();
    dst.y = m_coordReal.y();
    dst.w = currentFrame.w;
    dst.h = currentFrame.h;

    SDL_RenderCopy(renderer, m_pTexture, &currentFrame, &dst);
};


// ---------------------------------------------------------------------
// RenderBackground

RenderBackground::RenderBackground(const CoordXY &p0, SDL_Texture* tex, int nPositionZ) 
: RenderObject(nPositionZ) {
    m_pTexture = tex;
    m_coordReal = p0;
    m_currentFrame.x = 0;
    m_currentFrame.y = 0;
    m_currentFrame.w = 500;
    m_currentFrame.h = 500; // HARD code aiyayai
}

void RenderBackground::modify(const GameState& state, IRenderWindow* pRenderWindow) {
    m_coordRender = m_coordReal - state.getCoordLeftTop();

};

void RenderBackground::draw(SDL_Renderer* renderer) {
    if (m_coordRender.x() < -500 || m_coordRender.x() > 2000) {
        return;
    }

    if (m_coordRender.y() < -500 || m_coordRender.x() > 2000) {
        return;
    }

    SDL_Rect dst;
    dst.x = m_coordRender.x();
    dst.y = m_coordRender.y();
    dst.w = m_currentFrame.w;
    dst.h = m_currentFrame.h;

    SDL_RenderCopy(renderer, m_pTexture, &m_currentFrame, &dst);
};

// ---------------------------------------------------------------------
// RenderTextBlock

RenderAbsoluteTextBlock::RenderAbsoluteTextBlock(const CoordXY &p0, const std::string &sText, int nPositionZ)
: RenderObject(nPositionZ) {
    m_coordCenter = p0;
    m_sText = sText;
    m_sUpdateText = m_sText;
    // m_pFont = TTF_OpenFont("./res/fonts/amatic/AmaticSC-Regular.ttf", 42);
    m_pFont = TTF_OpenFont("./res/fonts/roboto/Roboto-Black.ttf", 16);
    m_color = {255, 255, 255};
    if (!m_pFont) {
        printf("TTF_OpenFont: %s\n", TTF_GetError());
        // handle error
    }

    // SDL_Rect currentFrame;
}

void RenderAbsoluteTextBlock::modify(const GameState& state, IRenderWindow* pRenderWindow) {
    // nothing i think
    if (m_sUpdateText != m_sText) {
        m_sText = m_sUpdateText;
    }
};

void RenderAbsoluteTextBlock::draw(SDL_Renderer* renderer) {
    // int w,h;
    // if (TTF_SizeText(font,"Hello World!",&w,&h)) {
        // perhaps print the current TTF_GetError(), the string can't be rendered...
    // } else {
    //    printf("width=%d height=%d\n",w,h);
    // }

    SDL_Surface* surfaceMessage = TTF_RenderText_Solid(m_pFont, m_sText.c_str(), m_color);
     // as TTF_RenderText_Solid could only be used on SDL_Surface then you have to create the surface first

    SDL_Texture* Message = SDL_CreateTextureFromSurface(renderer, surfaceMessage); //now you can convert it into a texture
    
    SDL_Rect Message_rect; //create a rect
    Message_rect.x = m_coordCenter.x();  //controls the rect's x coordinate 
    Message_rect.y = m_coordCenter.y(); // controls the rect's y coordinte
    Message_rect.w = surfaceMessage->w; // controls the width of the rect
    Message_rect.h = surfaceMessage->h; // controls the height of the rect

    // Mind you that (0,0) is on the top left of the window/screen, think a rect as the text's box, that way it would be very simple to understand

    // Now since it's a texture, you have to put RenderCopy in your game loop area, the area where the whole code executes

    SDL_RenderCopy(renderer, Message, NULL, &Message_rect); //you put the renderer's name first, the Message, the crop size(you can ignore this if you don't want to dabble with cropping), and the rect which is the size and coordinate of your texture

    // Don't forget to free your surface and texture
    SDL_FreeSurface(surfaceMessage);
    SDL_DestroyTexture(Message);
};

void RenderAbsoluteTextBlock::updateText(const std::string &sNewText) {
    m_sUpdateText = sNewText;
}

// ---------------------------------------------------------------------
// RenderBuilding

RenderBuilding::RenderBuilding(GameBuilding *pBuilding) 
: RenderObject(600) {
    m_pBuilding = pBuilding;
    const std::vector<CoordXY> &vPoints = m_pBuilding->getPoints();
    int nMaxX = vPoints[0].x();
    int nMinX = vPoints[0].x();
    int nMaxY = vPoints[0].y();
    int nMinY = vPoints[0].y();

    for (int i = 0; i < vPoints.size(); i++) {
        CoordXY p0 = vPoints[i];
        CoordXY p1 = vPoints[(i+1) % vPoints.size()];
        RenderColor color(122, 17, 17, 255);
        RenderLine *pLine = new RenderLine(p0, p1, color);
        m_vBorderLines.push_back(pLine);
        nMaxX = std::max(p0.x(), nMaxX);
        nMinX = std::min(p0.x(), nMinX);
        nMaxY = std::max(p0.y(), nMaxY);
        nMinY = std::min(p0.y(), nMinY);
    }
    int nStepWidth = 10;
    for (int x = nMinX + nStepWidth; x <= nMaxX; x = x + nStepWidth) {
        int nMinCrossY = nMaxY;
        int nMaxCrossY = nMinY;
        findMinMaxYCross(x, nMinCrossY, nMaxCrossY);
        CoordXY p0(x, nMinCrossY);
        CoordXY p1(x, nMaxCrossY);

        // find min cross with Y
        RenderColor color(255, 60, 70, 255);
        RenderLine *pLine = new RenderLine(p0, p1, color);
        m_vFillLines.push_back(pLine);
    }
}

void RenderBuilding::modify(const GameState& state, IRenderWindow* pRenderWindow) {
    for (int i = 0; i < m_vBorderLines.size(); i++) {
        m_vBorderLines[i]->modify(state, pRenderWindow);
    }
    for (int i = 0; i < m_vFillLines.size(); i++) {
        m_vFillLines[i]->modify(state, pRenderWindow);
    }
}

void RenderBuilding::draw(SDL_Renderer* renderer) {
    for (int i = 0; i < m_vBorderLines.size(); i++) {
        m_vBorderLines[i]->draw(renderer);
    }
    for (int i = 0; i < m_vFillLines.size(); i++) {
        m_vFillLines[i]->draw(renderer);
    }
}

void RenderBuilding::findMinMaxYCross(int nX, int &nMinY, int &nMaxY) {
    for (int i = 0; i < m_vBorderLines.size(); i++) {
        CoordXY p0 = m_vBorderLines[i]->getAbsoluteCoord1();
        CoordXY p1 = m_vBorderLines[i]->getAbsoluteCoord2();
        int nMinX = std::min(p0.x(), p1.x());
        int nMaxX = std::max(p0.x(), p1.x());
        if (nMinX == nMaxX && nMaxX == nX) {
            nMinY = std::min(p0.y(), p1.y());
            nMaxY = std::max(p0.y(), p1.y());
            return;
        }

        if (nMinX <= nX && nX <= nMaxX) {
            if (p0.y() == p1.y()) {
                nMinY = std::min(nMinY, p0.y());
                nMaxY = std::max(nMaxY, p0.y());
            } else {
                double a1 = p0.y() - p1.y();
                double b1 = p1.x() - p0.x();
                double c1 = p0.x() * p1.y() - p1.x() * p0.y();
                double a2 = p0.y() - p1.y();
                double c2 = nX * p1.y() - nX * p0.y();

                double det = - a2 * b1;
                int nFoundY = double(a2 * c1 - a1 * c2) / det;
                
                nMinY = std::min(nMinY, nFoundY);
                nMaxY = std::max(nMaxY, nFoundY);
            }
        }
    }
}


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
    m_nMaxX = vPoints[0].x();
    m_nMinX = vPoints[0].x();
    m_nMaxY = vPoints[0].y();
    m_nMinY = vPoints[0].y();

    RenderColor buildingColor(255, 57, 57, 255);

    for (int i = 0; i < vPoints.size(); i++) {
        CoordXY p0 = vPoints[i];
        CoordXY p1 = vPoints[(i+1) % vPoints.size()];
        
        RenderLine *pLine = new RenderLine(p0, p1, buildingColor, getPositionZ());
        m_vBorderLines.push_back(pLine);
        m_nMaxX = std::max(p0.x(), m_nMaxX);
        m_nMinX = std::min(p0.x(), m_nMinX);
        m_nMaxY = std::max(p0.y(), m_nMaxY);
        m_nMinY = std::min(p0.y(), m_nMinY);
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

void RenderBuilding2::draw(SDL_Renderer* renderer) {
    if (m_nMinX < -500 || m_nMaxX > 2000) {
        return;
    }
    if (m_nMinY < -500 || m_nMaxY > 2000) {
        return;
    }

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
    return CoordXY(m_nMinX, m_nMinY);
}

CoordXY RenderBuilding2::getMaxPoint() {
    return CoordXY(m_nMaxX, m_nMaxY);
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

// ---------------------------------------------------------------------
// RenderMouse

RenderMouse::RenderMouse(
    const CoordXY &p1,
    SDL_Texture* pTextureCursorTarget,
    int nPositionZ
) : RenderObject(nPositionZ),
    m_p1(p1),
    m_nCursorType(0)
{
    m_pTextureCursorTarget = pTextureCursorTarget;

    m_currentFrame.x = 0;
    m_currentFrame.y = 0;
    m_currentFrame.w = 24;
    m_currentFrame.h = 24;

    this->updateCoord(p1);
}

void RenderMouse::modify(const GameState& state, IRenderWindow* pRenderWindow) {
    // m_pLineMoveble1->modify(state);
    // m_pLineMoveble2->modify(state);
}

void RenderMouse::draw(SDL_Renderer* renderer) {
    SDL_Rect dst;
    dst.x = m_p1.x() - 12;
    dst.y = m_p1.y() - 12;
    dst.w = m_currentFrame.w;
    dst.h = m_currentFrame.h;
    SDL_RenderCopy(renderer, m_pTextureCursorTarget, &m_currentFrame, &dst);
}

void RenderMouse::updateCoord(const CoordXY &p0) {
    m_p1 = p0;
}
