#include "render.h"
#include "wsjcpp_core.h"
#include "sea5kg_triangulation.h"
#include <cmath>
#define PI 3.14159265

// ---------------------------------------------------------------------
// RenderObject

RenderObject::RenderObject(int nPositionZ) {
    m_nPositionZ = nPositionZ;
}

int RenderObject::getPositionZ() {
    return m_nPositionZ;
}

void RenderObject::modify(const GameState& state) {
    // nothing modify in base object
}

// ---------------------------------------------------------------------
// RenderColor

RenderColor::RenderColor(int nR, int nG, int nB, int nA) {
    m_nR = nR;
    m_nG = nG;
    m_nB = nB;
    m_nA = nA;
}

void RenderColor::changeRenderColor(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, m_nR, m_nG, m_nB, m_nA);
}

// ---------------------------------------------------------------------
// RenderLine

RenderLine::RenderLine(const CoordXY &p1, const CoordXY &p2, const RenderColor &color, int nPositionZ) 
: RenderObject(nPositionZ), m_color(color) {
    m_coord1 = p1;
    m_coord2 = p2;
    m_startCoord1 = p1;
    m_startCoord2 = p2;
}

void RenderLine::modify(const GameState& state) {
    m_coord1 = state.getCoordLeftTop() + m_startCoord1;
    m_coord2 = state.getCoordLeftTop() + m_startCoord2;
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

void RenderTriangle::modify(const GameState& state) {
    m_line1.modify(state);
    m_line2.modify(state);
    m_line3.modify(state);
}

void RenderTriangle::draw(SDL_Renderer* renderer) {
    m_line1.draw(renderer);
    m_line2.draw(renderer);
    m_line3.draw(renderer);
}


// ---------------------------------------------------------------------
// RenderTriangleAnimated1

RenderTriangleAnimated1::RenderTriangleAnimated1(
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
    m_coordDirection = CoordXY(10, 10);
}

void RenderTriangleAnimated1::modify(const GameState& state) {
    if (state.getElapsedTime() > 2000) {
        CoordXY p1 = m_line1.getAbsoluteCoord1();
        CoordXY p2 = m_line1.getAbsoluteCoord2();
        CoordXY p3 = m_line2.getAbsoluteCoord2();
        p1.update(p1.x() + m_coordDirection.x(), p1.y() + m_coordDirection.y());
        p2.update(p2.x() + m_coordDirection.x(), p2.y() + m_coordDirection.y());
        p3.update(p3.x() + m_coordDirection.x(), p3.y() + m_coordDirection.y());

        if (
            p1.x() < 0
            || p2.x() < 0
            || p3.x() < 0
            || p1.x() > state.windowWidth() 
            || p2.x() > state.windowWidth()
            || p3.x() > state.windowWidth()
            || p1.y() < 0
            || p2.y() < 0
            || p3.y() < 0
            || p1.y() > state.windowHeight() 
            || p2.y() > state.windowHeight()
            || p3.y() > state.windowHeight()
        ) {
            m_coordDirection.update(m_coordDirection.x()*-1, m_coordDirection.y()*-1);
        }

        m_line1.updateAbsoluteCoords(p1, p2);
        m_line2.updateAbsoluteCoords(p2, p3);
        m_line3.updateAbsoluteCoords(p3, p1);
    }

    m_line1.modify(state);
    m_line2.modify(state);
    m_line3.modify(state);
}

void RenderTriangleAnimated1::draw(SDL_Renderer* renderer) {
    m_line1.draw(renderer);
    m_line2.draw(renderer);
    m_line3.draw(renderer);
}

// ---------------------------------------------------------------------
// RenderRectTexture

RenderRectTexture::RenderRectTexture(const CoordXY &p0, SDL_Texture* tex, int nPositionZ) 
: RenderObject(nPositionZ) {
    m_pTexture = tex;
    m_coordCenter = p0;
    currentFrame.x = 0;
    currentFrame.y = 0;
    currentFrame.w = 32;
    currentFrame.h = 32; // HARD code aiyayai
}

void RenderRectTexture::modify(const GameState& state) {
    m_coordReal = m_coordCenter + state.getCoordLeftTop();

};

void RenderRectTexture::draw(SDL_Renderer* renderer) {
    SDL_Rect dst;
    // 4 is scale
    dst.x = m_coordReal.x();
    dst.y = m_coordReal.y();
    dst.w = currentFrame.w * 4;
    dst.h = currentFrame.h * 4;

    SDL_RenderCopy(renderer, m_pTexture, &currentFrame, &dst);
};


// ---------------------------------------------------------------------
// RenderBackground

RenderBackground::RenderBackground(const CoordXY &p0, SDL_Texture* tex, int nPositionZ) 
: RenderObject(nPositionZ) {
    m_pTexture = tex;
    m_coordCenter = p0;
    m_currentFrame.x = 0;
    m_currentFrame.y = 0;
    m_currentFrame.w = 500;
    m_currentFrame.h = 500; // HARD code aiyayai
}

void RenderBackground::modify(const GameState& state) {
    m_coordReal = m_coordCenter + state.getCoordLeftTop();

};

void RenderBackground::draw(SDL_Renderer* renderer) {
    SDL_Rect dst;
    dst.x = m_coordReal.x();
    dst.y = m_coordReal.y();
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
    m_pFont = TTF_OpenFont("./res/fonts/roboto/Roboto-Black.ttf", 24);
    m_color = {255, 255, 255};
    if (!m_pFont) {
        printf("TTF_OpenFont: %s\n", TTF_GetError());
        // handle error
    }

    // SDL_Rect currentFrame;
}

void RenderAbsoluteTextBlock::modify(const GameState& state) {
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

    //Mind you that (0,0) is on the top left of the window/screen, think a rect as the text's box, that way it would be very simple to understand

    //Now since it's a texture, you have to put RenderCopy in your game loop area, the area where the whole code executes

    SDL_RenderCopy(renderer, Message, NULL, &Message_rect); //you put the renderer's name first, the Message, the crop size(you can ignore this if you don't want to dabble with cropping), and the rect which is the size and coordinate of your texture

    //Don't forget to free your surface and texture
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

void RenderBuilding::modify(const GameState& state) {
    for (int i = 0; i < m_vBorderLines.size(); i++) {
        m_vBorderLines[i]->modify(state);
    }
    for (int i = 0; i < m_vFillLines.size(); i++) {
        m_vFillLines[i]->modify(state);
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

void RenderBuilding2::modify(const GameState& state) {
    for (int i = 0; i < m_vBorderLines.size(); i++) {
        m_vBorderLines[i]->modify(state);
    }

    for (int i = 0; i < m_vFillPointsAbsolute.size(); i++) {
        m_vFillPoints[i] = state.getCoordLeftTop() + m_vFillPointsAbsolute[i];
    }

    for (int i = 0; i < m_vLines.size(); i++) {
        m_vLines[i]->modify(state);
    }
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
// RenderPlayerAlienShip1

RenderPlayerAlienShip1::RenderPlayerAlienShip1(const CoordXY &p0, SDL_Texture* tex, int nPositionZ) 
: RenderObject(nPositionZ) {
    m_pTexture = tex;
    m_coordCenter = p0;
    currentFrame.x = 0;
    currentFrame.y = 0;
    currentFrame.w = 100;
    currentFrame.h = 100;
}

void RenderPlayerAlienShip1::modify(const GameState& state) {
    // m_coordReal = m_coordCenter + state.getCoordLeftTop();
    
    long m_nSpeedAnimation = 100;

    long position = state.getElapsedTime() / m_nSpeedAnimation;

    currentFrame.y = (position % 25) * 100;
};

void RenderPlayerAlienShip1::draw(SDL_Renderer* renderer) {
    RenderColor emptyColor(0, 0, 0, 0);
    emptyColor.changeRenderColor(renderer);

    SDL_Rect dst;
    dst.x = m_coordCenter.x();
    dst.y = m_coordCenter.y();
    dst.w = currentFrame.w;
    dst.h = currentFrame.h;

    SDL_RenderCopy(renderer, m_pTexture, &currentFrame, &dst);
};


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

void RenderMouse::modify(const GameState& state) {
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

// ---------------------------------------------------------------------
// RenderTank0

RenderTank0::RenderTank0(GameTank0State *pTank0State, SDL_Texture* tex,  int nPositionZ) 
: RenderObject(nPositionZ) {
    m_pTank0State = pTank0State;
    m_pAiTank0 = new AiTank0();
    m_pTexture = tex;
    m_coordCenter = pTank0State->getPosition();
    m_currentFrame.x = 0;
    m_currentFrame.y = 0;
    m_currentFrame.w = 50;
    m_currentFrame.h = 50;
    m_nPrevPosition = 0;
}

void RenderTank0::modify(const GameState& state) {
    long m_nSpeedAnimation = 200;

    long position = state.getElapsedTime() / m_nSpeedAnimation;

    if (m_nPrevPosition == position) {
        m_coordReal = m_coordCenter + state.getCoordLeftTop();
        return; // skip - already desition done
    }

    m_nPrevPosition = position;

    m_pAiTank0->makeStep(*m_pTank0State);

    m_coordCenter = m_pTank0State->getPosition();
    m_coordReal = m_coordCenter + state.getCoordLeftTop();

    MoveObjectDirection dr = m_pTank0State->getDirection();
    int nWidth = 50;
    switch(dr) {
        case MoveObjectDirection::UP:
            m_currentFrame.x = nWidth;
            m_currentFrame.y = 0;
            break;
        case MoveObjectDirection::UP_LEFT:
            m_currentFrame.x = 0;
            m_currentFrame.y = 0;
            break;
        case MoveObjectDirection::UP_RIGHT:
            m_currentFrame.x = nWidth*2;
            m_currentFrame.y = 0;
            break;
        case MoveObjectDirection::DOWN:
            m_currentFrame.x = nWidth;
            m_currentFrame.y = nWidth*2;
            break;
        case MoveObjectDirection::DOWN_LEFT:
            m_currentFrame.x = 0;
            m_currentFrame.y = nWidth*2;
            break;
        case MoveObjectDirection::DOWN_RIGHT:
            m_currentFrame.x = nWidth*2;
            m_currentFrame.y = nWidth*2;
            break;
        case MoveObjectDirection::LEFT:
            m_currentFrame.x = 0;
            m_currentFrame.y = nWidth;
            break;
        case MoveObjectDirection::RIGHT:
            m_currentFrame.x = nWidth*2;
            m_currentFrame.y = nWidth;
            break;
    }
};

void RenderTank0::draw(SDL_Renderer* renderer) {
    RenderColor emptyColor(0, 0, 0, 0);
    emptyColor.changeRenderColor(renderer);

    SDL_Rect dst;
    dst.x = m_coordReal.x() - 25;
    dst.y = m_coordReal.y() - 25;
    dst.w = m_currentFrame.w;
    dst.h = m_currentFrame.h;

    SDL_RenderCopy(renderer, m_pTexture, &m_currentFrame, &dst);
};