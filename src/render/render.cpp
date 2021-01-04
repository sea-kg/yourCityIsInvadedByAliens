#include "render.h"
#include "wsjcpp_core.h"
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
// RenderLine

RenderLine::RenderLine(const CoordXY &p1, const CoordXY &p2, int nPositionZ) 
: RenderObject(nPositionZ) {
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
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
    SDL_RenderDrawLine(renderer, m_coord1.x(), m_coord1.y(), m_coord2.x(), m_coord2.y());
}

const CoordXY &RenderLine::getAbsoluteCoord1() {
    return m_startCoord1;
}

const CoordXY &RenderLine::getAbsoluteCoord2() {
    return m_startCoord2;
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
) : RenderObject(nPositionZ), m_line1(p1,p2), m_line2(p2,p3), m_line3(p3,p1) {
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
) : RenderObject(nPositionZ), m_line1(p1,p2), m_line2(p2,p3), m_line3(p3,p1) {
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
// RenderPlayer0

RenderPlayer0::RenderPlayer0(
    const CoordXY &p0,
    float nSpeedAnimation
) : RenderObject(1000) {


    m_nSpeedAnimation = nSpeedAnimation;
    m_nSize = 20;
    m_coordCenter = p0;
    CoordXY p1 = p0;
    p1 += CoordXY(-m_nSize,m_nSize);
    CoordXY p2 = p0;
    p2 += CoordXY(m_nSize,m_nSize);
    CoordXY p3 = p0;
    p3 += CoordXY(0,-m_nSize);

    m_pLine1 = new RenderLine(p1,p2);
    m_pLine2 = new RenderLine(p2,p3);
    m_pLine3 = new RenderLine(p3,p1);

    m_rectFrame1.x = m_coordCenter.x() - m_nSize/4;
    m_rectFrame1.y = m_coordCenter.y() - m_nSize/4;
    m_rectFrame1.w = m_nSize/2;
    m_rectFrame1.h = m_nSize/2;
}

void RenderPlayer0::modify(const GameState& state) {
    float nTimePosition = float(state.getElapsedTime() / m_nSpeedAnimation);
    // nTimePosition = nTimePosition / 1000;
    int diffX1 = sin(nTimePosition) * m_nSize;
    int diffY1 = cos(nTimePosition) * m_nSize;
    
    int diffX2 = sin(nTimePosition + PI/3) * m_nSize;
    int diffY2 = cos(nTimePosition + PI/3) * m_nSize;

    int diffX3 = sin(nTimePosition + 2*PI/3) * m_nSize;
    int diffY3 = cos(nTimePosition + 2*PI/3) * m_nSize;

    CoordXY p1 = m_coordCenter;
    p1 += CoordXY(diffX1, diffY1);
    p1 -= state.getCoordLeftTop();
    CoordXY p2 = m_coordCenter;
    p2 += CoordXY(diffX2, diffY2);
    p2 -= state.getCoordLeftTop();
    CoordXY p3 = m_coordCenter;
    p3 += CoordXY(diffX3, diffY3);
    p3 -= state.getCoordLeftTop();

    // std::cout << diffX1 << " " << diffY1 << std::endl;
    m_pLine1->updateAbsoluteCoords(p1, p2);
    m_pLine2->updateAbsoluteCoords(p2, p3);
    m_pLine3->updateAbsoluteCoords(p3, p1);

    m_pLine1->modify(state);
    m_pLine2->modify(state);
    m_pLine3->modify(state);

    // rect pulsation
    float nHartPulsation = float(state.getElapsedTime()/m_nSpeedAnimation*0.5);

    int nBorderLength = m_nSize/3;
    { 
        int nBorderLength1 = sin(nHartPulsation)*nBorderLength; 
        m_rectFrame1.x = m_coordCenter.x() - nBorderLength1/2 + nBorderLength/3;
        m_rectFrame1.y = m_coordCenter.y() - nBorderLength1/2 + nBorderLength/3;
        m_rectFrame1.w = nBorderLength1;
        m_rectFrame1.h = nBorderLength1;
    }
    { 
        int nBorderLength2 = sin(nHartPulsation + PI/3)*nBorderLength; 
        m_rectFrame2.x = m_coordCenter.x() - nBorderLength2/2 + nBorderLength/2;
        m_rectFrame2.y = m_coordCenter.y() - nBorderLength2/2 + nBorderLength/2;
        m_rectFrame2.w = nBorderLength2;
        m_rectFrame2.h = nBorderLength2;
    }
}

void RenderPlayer0::draw(SDL_Renderer* renderer) {
    m_pLine1->draw(renderer);
    m_pLine2->draw(renderer);
    m_pLine3->draw(renderer);
    SDL_RenderFillRect( renderer, &m_rectFrame1);
    SDL_RenderFillRect( renderer, &m_rectFrame2);
    // SDL_RenderDrawRect(renderer, &m_rectFrame)
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
// RenderTextBlock

RenderAbsoluteTextBlock::RenderAbsoluteTextBlock(const CoordXY &p0, const std::string &sText, int nPositionZ)
: RenderObject(nPositionZ) {
    m_coordCenter = p0;
    m_sText = sText;
    m_sUpdateText = m_sText;
    // m_pFont = TTF_OpenFont("./res/fonts/amatic/AmaticSC-Regular.ttf", 42);
    m_pFont = TTF_OpenFont("./res/fonts/fff/FFF_Tusj.ttf", 24);
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
    for (int i = 0; i < vPoints.size(); i++) {
        m_vLines.push_back(new RenderLine(vPoints[i], vPoints[(i+1) % vPoints.size()]));
    }
}

void RenderBuilding::modify(const GameState& state) {
    for (int i = 0; i < m_vLines.size(); i++) {
        m_vLines[i]->modify(state);
    }
}

void RenderBuilding::draw(SDL_Renderer* renderer) {
    for (int i = 0; i < m_vLines.size(); i++) {
        m_vLines[i]->draw(renderer);
    }
}
