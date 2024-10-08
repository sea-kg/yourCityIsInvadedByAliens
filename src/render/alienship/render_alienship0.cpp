#include "render_alienship0.h"
#include "ylog.h"

// ---------------------------------------------------------------------
// RenderAlienShip0

RenderAlienShip0::RenderAlienShip0(
    GameAlienShipState *pState,
    const YJson &jsonAlienShip,
    bool bShadow,
    SDL_Texture* tex,
    int nPositionZ
)
: RenderObject(nPositionZ) {
    TAG = L"RenderAlienShip0";
    m_bShadow = bShadow;
    m_pState = pState;
    m_pTexture = tex;
    m_nFrameWidth = jsonAlienShip[L"frame"][L"width-px"].getNumber();
    m_nFrameHeight = jsonAlienShip[L"frame"][L"height-px"].getNumber();
    m_nFramesCount = jsonAlienShip[L"frame"][L"count"].getNumber();

    std::wstring sMoveDirectiron = jsonAlienShip[L"frame"][L"move-direction"].getString();
    if (sMoveDirectiron == L"vertical") {
        m_bFrameMoveDirectionVertical = true;
    } else if (sMoveDirectiron == L"horizontal") {
        m_bFrameMoveDirectionVertical = false;
    } else {
        YLog::throw_err(TAG, L"frame.move-direction expected 'vertical' or 'horizontal', but got '" + sMoveDirectiron + L"'");
    }

    if (m_bShadow) {
        m_currentFrame.x = m_bFrameMoveDirectionVertical ? m_nFrameWidth : 0;
        m_currentFrame.y = m_bFrameMoveDirectionVertical ? 0 : m_nFrameHeight;
    } else {
        m_currentFrame.x = 0;
        m_currentFrame.y = 0;
    }
    m_currentFrame.w = m_nFrameWidth;
    m_currentFrame.h = m_nFrameHeight;
    m_nPrevPosition = 0;
    m_nSpeedAnimation = jsonAlienShip[L"animation"][L"speed-ms"].getNumber();
}

void RenderAlienShip0::modify(const GameState& state, IRenderWindow* pRenderWindow) {
    long position = state.getElapsedTime() / m_nSpeedAnimation;
    IShootingStrategy *m_pShootingStrategy = m_pState->getShootingStrategyLogic()->getCurrentShootingStrategy();

    if (m_nPrevPosition == position) {
        m_coordPositionRendering = m_pState->getPosition() - state.getCoordLeftTop();
        return; // skip - already desition done
    }
    m_nPrevPosition = position;

    if (m_bFrameMoveDirectionVertical) {
        m_currentFrame.y = (position % m_nFramesCount) * m_nFrameHeight;
    } else {
        m_currentFrame.x = (position % m_nFramesCount) * m_nFrameWidth;
    }

    if (m_pState->isShooting()) {
        std::cout << "Shooting!  ";
        m_pShootingStrategy->shoot(
            m_pState->getPosition(),
            m_pState->getMoveDirection()
        );
    }

    GameBioplastState *pBioplastState = m_pShootingStrategy->popBioplast();
    while (pBioplastState != nullptr) {
        pRenderWindow->addBioplast(pBioplastState);
        pBioplastState = m_pShootingStrategy->popBioplast();
    }
};

bool RenderAlienShip0::canDraw(const GameState& state) {
    return true;
}

void RenderAlienShip0::draw(SDL_Renderer* renderer) {
    RenderColor emptyColor(0, 0, 0, 0);
    emptyColor.changeRenderColor(renderer);

    SDL_Rect dst;

    if (m_bShadow) {
        dst.x = m_coordPositionRendering.getX() - m_nFrameWidth/4;
        dst.y = m_coordPositionRendering.getY() - m_nFrameHeight/4;
    } else {
        dst.x = m_coordPositionRendering.getX() - m_nFrameWidth/2;
        dst.y = m_coordPositionRendering.getY() - m_nFrameHeight/2;
    }

    dst.w = m_currentFrame.w;
    dst.h = m_currentFrame.h;

    SDL_RenderCopy(renderer, m_pTexture, &m_currentFrame, &dst);
};
