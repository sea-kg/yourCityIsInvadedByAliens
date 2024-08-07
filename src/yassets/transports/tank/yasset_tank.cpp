#include "yasset_tank.h"
#include <render.h>

// ---------------------------------------------------------------------
// YAssetTank

YAssetTank::YAssetTank(
    YAssetsService *pAssetsService,
    SDL_Texture *pTextureTank,
    SDL_Texture *pTextureRocket,
    int nFrameWidth,
    int nFrameHeight
)
: YAsset(pAssetsService) {
    // m_pTank0State = pTank0State;
    m_pTexture = pTextureTank;
    m_pTextureRocket = pTextureRocket;
    m_nTextureTileWidth = nFrameWidth;
    m_nTextureTileHeight = nFrameHeight;

    m_currentFrame.x = 0;
    m_currentFrame.y = 0;
    m_currentFrame.w = m_nTextureTileWidth;
    m_currentFrame.h = m_nTextureTileHeight;
    m_nPrevPosition = 0;
}

void YAssetTank::setGameStateTank(GameTank0State *pTank0State) {
    m_pTank0State = pTank0State;
}

void YAssetTank::setOrderZ(int nOrder) {
    m_nPositionZ = nOrder;
}

void YAssetTank::modify(const GameState& state, IRenderWindow* pRenderWindow) {
    long m_nSpeedAnimation = 200;

    long position = state.getElapsedTime() / m_nSpeedAnimation;

    if (m_nPrevPosition == position) {
        m_coordRender = m_pTank0State->getPosition() - state.getCoordLeftTop();
        m_coordRenderEnd = YPos(
            m_pTank0State->getPosition().getX() + m_nTextureTileWidth,
            m_pTank0State->getPosition().getY() + m_nTextureTileHeight
        );
        return; // skip - already desition done
    }

    m_nPrevPosition = position;

    m_coordRender = m_pTank0State->getPosition() - state.getCoordLeftTop();
    m_coordRenderEnd = YPos(
        m_pTank0State->getPosition().getX() + m_nTextureTileWidth,
        m_pTank0State->getPosition().getY() + m_nTextureTileHeight
    );

    MoveObjectDirection dr = m_pTank0State->getDirection();

    GameRocketState *pRocketState = m_pTank0State->popRocket();
    if (pRocketState != nullptr) {
        pRenderWindow->addRocket(
            pRocketState,
            new RenderTank0Rocket(
                pRocketState,
                m_pTextureRocket,
                m_nTextureTileWidth,
                m_nTextureTileHeight
            )
        );
    }

    if (!m_pTank0State->hasRocket()) {
        switch(dr) {
            case MoveObjectDirection::UP:
                m_currentFrame.x = m_nTextureTileWidth*2;
                m_currentFrame.y = m_nTextureTileHeight;
                break;
            case MoveObjectDirection::UP_LEFT:
                m_currentFrame.x = m_nTextureTileWidth;
                m_currentFrame.y = m_nTextureTileHeight;
                break;
            case MoveObjectDirection::UP_RIGHT:
                m_currentFrame.x = m_nTextureTileWidth*3;
                m_currentFrame.y = m_nTextureTileHeight;
                break;
            case MoveObjectDirection::DOWN:
                m_currentFrame.x = m_nTextureTileWidth*2;
                m_currentFrame.y = m_nTextureTileHeight*3;
                break;
            case MoveObjectDirection::DOWN_LEFT:
                m_currentFrame.x = m_nTextureTileWidth;
                m_currentFrame.y = m_nTextureTileHeight*3;
                break;
            case MoveObjectDirection::DOWN_RIGHT:
                m_currentFrame.x = m_nTextureTileWidth*3;
                m_currentFrame.y = m_nTextureTileHeight*3;
                break;
            case MoveObjectDirection::LEFT:
                m_currentFrame.x = m_nTextureTileWidth;
                m_currentFrame.y = m_nTextureTileHeight*2;
                break;
            case MoveObjectDirection::RIGHT:
                m_currentFrame.x = m_nTextureTileWidth*3;
                m_currentFrame.y = m_nTextureTileHeight*2;
                break;
        }
    } else {
        switch(dr) {
            case MoveObjectDirection::UP:
                m_currentFrame.x = m_nTextureTileWidth*2;
                m_currentFrame.y = 0;
                break;
            case MoveObjectDirection::UP_LEFT:
                m_currentFrame.x = 0;
                m_currentFrame.y = 0;
                break;
            case MoveObjectDirection::UP_RIGHT:
                m_currentFrame.x = m_nTextureTileWidth*4;
                m_currentFrame.y = 0;
                break;
            case MoveObjectDirection::DOWN:
                m_currentFrame.x = m_nTextureTileWidth*2;
                m_currentFrame.y = m_nTextureTileHeight*4;
                break;
            case MoveObjectDirection::DOWN_LEFT:
                m_currentFrame.x = 0;
                m_currentFrame.y = m_nTextureTileHeight*4;
                break;
            case MoveObjectDirection::DOWN_RIGHT:
                m_currentFrame.x = m_nTextureTileWidth*4;
                m_currentFrame.y = m_nTextureTileHeight*4;
                break;
            case MoveObjectDirection::LEFT:
                m_currentFrame.x = 0;
                m_currentFrame.y = m_nTextureTileWidth*2;
                break;
            case MoveObjectDirection::RIGHT:
                m_currentFrame.x = m_nTextureTileWidth*4;
                m_currentFrame.y = m_nTextureTileHeight*2;
                break;
        }
    }
};

bool YAssetTank::canDraw(const GameState& state) {
    return
        m_coordRender.isInsideRect(state.getWindowRect())
        || m_coordRenderEnd.isInsideRect(state.getWindowRect())
    ;
}

void YAssetTank::draw(SDL_Renderer* renderer) {
    RenderColor emptyColor(0, 0, 0, 0);
    emptyColor.changeRenderColor(renderer);

    SDL_Rect dst;
    dst.x = m_coordRender.getX() - m_nTextureTileWidth/2;
    dst.y = m_coordRender.getY() - m_nTextureTileHeight/2;
    dst.w = m_currentFrame.w;
    dst.h = m_currentFrame.h;

    SDL_RenderCopy(renderer, m_pTexture, &m_currentFrame, &dst);
};




// ---------------------------------------------------------------------
// RenderTank0Rocket

RenderTank0Rocket::RenderTank0Rocket(
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

RenderTank0Rocket::~RenderTank0Rocket() {
    // delete m_pRocketState;
}

// isDestroyed
void RenderTank0Rocket::modify(const GameState& state, IRenderWindow* pRenderWindow) {
    if (state.isPauseGame()) {
        return;
    }

    long position = state.getElapsedTime() / m_nSpeedAnimation;

    if (m_nPrevPosition == position) {
        m_coordRender = m_pRocketState->getPosition() - state.getCoordLeftTop();
        m_coordRenderEnd = m_pRocketState->getPosition() - state.getCoordLeftTop() + m_size;
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
            // TODO also need remove from states
            this->removeFromRenderLater();
            m_pRocketState->removeLater();
            m_pRocketState = nullptr;
        }
        return;
    }

    m_pRocketState->move();
    m_nLifeTime++;

    m_coordRender = m_pRocketState->getPosition() - state.getCoordLeftTop();
    m_coordRenderEnd = m_pRocketState->getPosition() - state.getCoordLeftTop() + m_size;
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

bool RenderTank0Rocket::canDraw(const GameState& state) {
    return
        state.getWindowRect().hasPos(m_coordRender)
        || state.getWindowRect().hasPos(m_coordRenderEnd)
    ;
}

void RenderTank0Rocket::draw(SDL_Renderer* renderer) {
    RenderColor emptyColor(0, 0, 0, 0);
    emptyColor.changeRenderColor(renderer);

    SDL_Rect dst;
    dst.x = m_coordRender.getX() - m_nTextureTileWidth/2;
    dst.y = m_coordRender.getY() - m_nTextureTileHeight/2;
    dst.w = m_currentFrame.w;
    dst.h = m_currentFrame.h;

    SDL_RenderCopy(renderer, m_pTexture, &m_currentFrame, &dst);
};