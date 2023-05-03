#include "yasset_alien_berry.h"
#include <render.h>

// ---------------------------------------------------------------------
// YAssetAlienBerry

YAssetAlienBerry::YAssetAlienBerry(
    YAssetsService *pAssetsService,
    SDL_Texture *pTextureAlienBerry,
    int nFrameWidth,
    int nFrameHeight,
    int nFrameNumber
)
: YAsset(pAssetsService), RenderObject(1000) {
    // m_pAlienBerry0State = pAlienBerry0State;
    m_pTexture = pTextureAlienBerry;
    m_nFrameWidth = nFrameWidth;
    m_nFrameHeight = nFrameHeight;
    m_nFrameNumber = nFrameNumber;

    m_currentFrame.x = 0;
    m_currentFrame.y = 0;
    m_currentFrame.w = m_nFrameWidth;
    m_currentFrame.h = m_nFrameHeight;
    m_nPrevPosition = 0;
}

void YAssetAlienBerry::setState(GameAlienBerryState *pState) {
    m_pState = pState;
}

int YAssetAlienBerry::getFrameWidth() {
    return m_nFrameWidth;
}

int YAssetAlienBerry::getFrameHeight() {
    return m_nFrameHeight;
}

void YAssetAlienBerry::setOrderZ(int nOrder) {
    m_nPositionZ = nOrder;
}

void YAssetAlienBerry::modify(const GameState& state, IRenderWindow* pRenderWindow) {
    const long m_nSpeedAnimation = 200;

    m_coordRender = CoordXY(m_pState->getPosition().getX(), m_pState->getPosition().getY()) - state.getCoordLeftTop();

    long nFrameNumber = (state.getElapsedTime() / m_nSpeedAnimation) % m_nFrameNumber;

    if (m_nPrevPosition == nFrameNumber) {
        return; // skip - already desition done
    }
    m_currentFrame.y = nFrameNumber * m_nFrameHeight;
};

bool YAssetAlienBerry::canDraw(const GameState& state) {
    return
        m_coordRender.isInsideRect(state.getWindowRect())
        || m_coordRenderEnd.isInsideRect(state.getWindowRect())
    ;
}

void YAssetAlienBerry::draw(SDL_Renderer* renderer) {
    // RenderColor emptyColor(0, 0, 0, 0);
    // emptyColor.changeRenderColor(renderer);

    SDL_Rect dst;
    dst.x = m_coordRender.x(); // - m_nTextureTileWidth/2;
    dst.y = m_coordRender.y(); // - m_nTextureTileHeight/2;
    dst.w = m_currentFrame.w;
    dst.h = m_currentFrame.h;

    SDL_RenderCopy(renderer, m_pTexture, &m_currentFrame, &dst);
};




// ---------------------------------------------------------------------
// RenderAlienBerry0Rocket

RenderAlienBerry0Rocket::RenderAlienBerry0Rocket(
    GameRocketState *pRocketState,
    SDL_Texture* tex,
    int nTextureTileWidth,
    int nTextureTileHeight,
    int nPositionZ
) 
: RenderObject(nPositionZ) {
    m_pRocketState = pRocketState;
    m_pTexture = tex;
    m_nTextureTileWidth = nTextureTileWidth;
    m_nTextureTileHeight = nTextureTileHeight;
    m_size = YPos(m_nTextureTileWidth,m_nTextureTileHeight);
    m_currentFrame.x = 0;
    m_currentFrame.y = 0;
    m_currentFrame.w = m_size.getX();
    m_currentFrame.h = m_size.getY();
    m_nPrevPosition = 0;
    m_nLifeTime = 0;
    m_nMaxLifeTime = 50;
    m_nSpeedAnimation = 50;
}

RenderAlienBerry0Rocket::~RenderAlienBerry0Rocket() {
    // delete m_pRocketState;
}

void RenderAlienBerry0Rocket::modify(const GameState& state, IRenderWindow* pRenderWindow) {
    long position = state.getElapsedTime() / m_nSpeedAnimation;

    if (m_nPrevPosition == position) {
        m_coordRender = m_pRocketState->getPosition() - state.getCoordLeftTop().toYPos();
        m_coordRenderEnd = m_pRocketState->getPosition() - state.getCoordLeftTop().toYPos() + m_size;
        return; // skip - already desition done
    }

    if (m_nLifeTime < m_nMaxLifeTime && m_pRocketState->isExploded()) {
        m_nLifeTime = m_nMaxLifeTime;
    }

    m_nPrevPosition = position;
    
    if (m_nLifeTime == m_nMaxLifeTime) {
        m_pRocketState->destroy();
        m_nLifeTime++;
        m_currentFrame.x = m_nTextureTileWidth;
        m_currentFrame.y = m_nTextureTileHeight;
        return;
    }

    if (m_nLifeTime > m_nMaxLifeTime) {
        m_nLifeTime++;
        if (m_nLifeTime > m_nMaxLifeTime + 2) {
            // remove object from rendering
            this->destroy();
        }
        return;
    }

    m_pRocketState->move();
    m_nLifeTime++;

    m_coordRender = m_pRocketState->getPosition() - state.getCoordLeftTop().toYPos();
    m_coordRenderEnd = m_pRocketState->getPosition() - state.getCoordLeftTop().toYPos() + m_size;
    MoveObjectDirection dr = m_pRocketState->getDirection();

    int nWidth = 50;

    switch(dr) {
        case MoveObjectDirection::UP:
            m_currentFrame.x = m_nTextureTileWidth;
            m_currentFrame.y = 0;
            break;
        case MoveObjectDirection::UP_LEFT:
            m_currentFrame.x = 0;
            m_currentFrame.y = 0;
            break;
        case MoveObjectDirection::UP_RIGHT:
            m_currentFrame.x = m_nTextureTileWidth*2;
            m_currentFrame.y = 0;
            break;
        case MoveObjectDirection::DOWN:
            m_currentFrame.x = m_nTextureTileWidth;
            m_currentFrame.y = m_nTextureTileHeight*2;
            break;
        case MoveObjectDirection::DOWN_LEFT:
            m_currentFrame.x = 0;
            m_currentFrame.y = m_nTextureTileHeight*2;
            break;
        case MoveObjectDirection::DOWN_RIGHT:
            m_currentFrame.x = m_nTextureTileWidth*2;
            m_currentFrame.y = m_nTextureTileHeight*2;
            break;
        case MoveObjectDirection::LEFT:
            m_currentFrame.x = 0;
            m_currentFrame.y = m_nTextureTileHeight;
            break;
        case MoveObjectDirection::RIGHT:
            m_currentFrame.x = m_nTextureTileWidth*2;
            m_currentFrame.y = m_nTextureTileHeight;
            break;
    }
};

bool RenderAlienBerry0Rocket::canDraw(const GameState& state) {
    return
        state.getWindowRect().hasPos(m_coordRender)
        || state.getWindowRect().hasPos(m_coordRenderEnd)
    ;
}

void RenderAlienBerry0Rocket::draw(SDL_Renderer* renderer) {
    RenderColor emptyColor(0, 0, 0, 0);
    emptyColor.changeRenderColor(renderer);

    SDL_Rect dst;
    dst.x = m_coordRender.getX() - m_nTextureTileWidth/2;
    dst.y = m_coordRender.getY() - m_nTextureTileHeight/2;
    dst.w = m_currentFrame.w;
    dst.h = m_currentFrame.h;

    SDL_RenderCopy(renderer, m_pTexture, &m_currentFrame, &dst);
};