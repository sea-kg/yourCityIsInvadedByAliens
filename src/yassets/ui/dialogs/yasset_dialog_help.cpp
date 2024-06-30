#include "yasset_dialog_help.h"
#include <render.h>

// ---------------------------------------------------------------------
// YAssetDialogHelp

YAssetDialogHelp::YAssetDialogHelp(
    YAssetsService *pAssetsService,
    SDL_Texture *pTextureHelp,
    int nHelpWidth,
    int nHelpHeight,
    SDL_Texture *pTextureDialogBackground,
    int nDialogBackgroundWidth,
    int nDialogBackgroundHeight
)
: YAsset(pAssetsService) {
    m_bShow = false;
    m_pTextureHelp = pTextureHelp;
    m_nHelpWidth = nHelpWidth;
    m_nHelpHeight = nHelpHeight;

    m_pTextureDialogBackground = pTextureDialogBackground;
    m_nDialogBackgroundWidth = nDialogBackgroundWidth;
    m_nDialogBackgroundHeight = nDialogBackgroundHeight;

    m_nWindowWidth = 0;
    m_nWindowHeight = 0;

    m_currentFrame.x = 0;
    m_currentFrame.y = 0;
    m_currentFrame.w = m_nHelpWidth;
    m_currentFrame.h = m_nHelpHeight;
}

void YAssetDialogHelp::setShow(bool bShow) {
    m_bShow = bShow;
}

void YAssetDialogHelp::setOrderZ(int nOrder) {
    m_nPositionZ = nOrder;
}

void YAssetDialogHelp::modify(const GameState& state, IRenderWindow* pRenderWindow) {
    if (m_nWindowWidth != state.getWindowWidth()) {
        m_nWindowWidth = state.getWindowWidth();
        // center
        m_nX = m_nWindowWidth/2 - m_nHelpWidth/2;
    }

    if (m_nWindowHeight != state.getWindowWidth()) {
        m_nWindowHeight = state.getWindowHeight();
        // center
        m_nY = m_nWindowHeight/2 - m_nHelpHeight/2;
    }

    if (m_needUpdate.isAndDoReset()) {
        // TODO
        // state
    }
};

bool YAssetDialogHelp::canDraw(const GameState& state) {
    return m_bShow;
}

void YAssetDialogHelp::draw(SDL_Renderer* renderer) {
    SDL_Rect dst;
    dst.x = m_nX;
    dst.y = m_nY;
    dst.w = m_nHelpWidth;
    dst.h = m_nHelpHeight;
    SDL_RenderCopy(renderer, m_pTextureHelp, &m_currentFrame, &dst);
};
