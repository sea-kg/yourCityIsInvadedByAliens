#include "yasset_minimap.h"
#include <render.h>

// ---------------------------------------------------------------------
// YAssetMinimap

YAssetMinimap::YAssetMinimap(
    YAssetsService *pAssetsService,
    SDL_Texture *pTextureBackground,
    int nWidth,
    int nHeight
)
: YAsset(pAssetsService), RenderObject(5100) {
    TAG = L"YAssetMinimap";
    m_pTextureBackground = pTextureBackground;
    m_nWidth = nWidth;
    m_nHeight = nHeight;
    m_nWindowWidth = 0;
    m_nWindowHeight = 0;

    m_nX = 0;
    m_nY = 0;

    m_currentFrame.x = 0;
    m_currentFrame.y = 0;
    m_currentFrame.w = m_nWidth;
    m_currentFrame.h = m_nHeight;

    m_pMapService = findYService<MapYService>();
    m_nWidthK = float(m_nWidth) / float(m_pMapService->getMapWidth());
    m_nHeightK = float(m_nHeight) / float(m_pMapService->getMapHeight());

    m_pTexturePlayer = SDL_CreateTexture(
        m_pAssetsService->getRenderWindow()->getRenderer(),
        SDL_PIXELFORMAT_ARGB8888,
        SDL_TEXTUREACCESS_STATIC,
        m_nWidth,
        m_nHeight
    );

    m_nPrevPlayerPositionX = -1;
    m_nPrevPlayerPositionY = -1;
    m_nBufferPixelsSize = m_nWidth * m_nHeight; // * sizeof(Uint32);
    m_pDynamicObjectsPixels = new Uint32[m_nBufferPixelsSize];
    m_pBackgroundMinimapPixels = new Uint32[m_nBufferPixelsSize];
    m_nShiftToLeftByX = 7;

    if (true) {
        // copy pixels from background texture
        SDL_RenderCopy(
            m_pAssetsService->getRenderWindow()->getRenderer(),
            m_pTextureBackground,
            &m_currentFrame,
            &m_currentFrame
        );
        SDL_Surface *pSshot = SDL_CreateRGBSurface(
            0, m_nWidth, m_nHeight, 32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000
        );
        SDL_RenderReadPixels(
            m_pAssetsService->getRenderWindow()->getRenderer(),
            &m_currentFrame,
            SDL_PIXELFORMAT_ARGB8888,
            pSshot->pixels,
            pSshot->pitch
        );
        memcpy(m_pBackgroundMinimapPixels, pSshot->pixels, m_nBufferPixelsSize * sizeof(Uint32));
        SDL_FreeSurface(pSshot);
    } else {
        for (int i = 0; i < m_nBufferPixelsSize; i++) {
            m_pBackgroundMinimapPixels[i] = 0xFF009933;
        }
    }

    redrawBackground();
    redrawPlayerPosition();
}

void YAssetMinimap::setAbsolutePosition(bool bAbsolutePosition) {
    m_bAbsolutePosition = bAbsolutePosition;
}

void YAssetMinimap::setPosition(int nX, int nY) {
    m_nX = nX;
    m_nY = nY;
}

void YAssetMinimap::setOrderZ(int nOrder) {
    m_nPositionZ = nOrder;
}

void YAssetMinimap::modify(const GameState& state, IRenderWindow* pRenderWindow) {
    // if (m_needUpdate.isAndDoReset()) {
    //     // TODO
    // }
    if (m_nWindowWidth != state.getWindowWidth()) {
        m_nWindowWidth = state.getWindowWidth();
        m_nX = m_nWindowWidth - m_nWidth - 50;
    }

    if (m_nWindowHeight != state.getWindowWidth()) {
        m_nWindowHeight = state.getWindowHeight();
        m_nY = m_nWindowHeight - m_nHeight - 50;
    }

    const YPos &pos = state.getPlayerPosition();
    int nPlayerPositionX = (pos.getX() * m_nWidthK);
    int nPlayerPositionY = (pos.getY() * m_nHeightK);

    if (nPlayerPositionX != m_nPrevPlayerPositionX || m_nPrevPlayerPositionY != nPlayerPositionY) {
        m_nPrevPlayerPositionX = nPlayerPositionX;
        m_nPrevPlayerPositionY = nPlayerPositionY;
        redrawPlayerPosition();
    }
}

bool YAssetMinimap::canDraw(const GameState& state) {
    return true;
}

void YAssetMinimap::draw(SDL_Renderer* renderer) {
    SDL_Rect dst;
    dst.x = m_nX;
    dst.y = m_nY;
    dst.w = m_nWidth;
    dst.h = m_nHeight;
    SDL_RenderCopy(renderer, m_pTexturePlayer, &m_currentFrame, &dst);
}

void YAssetMinimap::redrawBackground() {
    for (const auto &road : m_pMapService->getRoads()) {
        int x0 = road.getMinX() * m_nWidthK;
        int y0 = road.getMinY() * m_nHeightK;
        int x1 = road.getMaxX() * m_nWidthK;
        int y1 = road.getMaxY() * m_nHeightK;

        for (int x = x0 - m_nShiftToLeftByX; x < x1 - m_nShiftToLeftByX; x++) {
            for (int y = y0; y < y1; y++) {
                int p = m_nWidth * y + x;
                m_pBackgroundMinimapPixels[p] = 0xFF565656;
            }
        }
    }
}

void YAssetMinimap::redrawPlayerPosition() {
    // https://dzone.com/articles/sdl2-pixel-drawing

    memcpy(m_pDynamicObjectsPixels, m_pBackgroundMinimapPixels, m_nBufferPixelsSize * sizeof(Uint32));

    if (m_nPrevPlayerPositionX >= 0 && m_nPrevPlayerPositionY >= 0) {
        int p = m_nWidth * m_nPrevPlayerPositionY + (m_nPrevPlayerPositionX - m_nShiftToLeftByX);
        // YLog::info(TAG, L"New player position " + std::to_wstring(i));
        // argb
        m_pDynamicObjectsPixels[p - m_nWidth - 1] = 0xFFFFFFFF;
        m_pDynamicObjectsPixels[p - m_nWidth] = 0xFFFFFFFF;
        m_pDynamicObjectsPixels[p - m_nWidth + 1] = 0xFFFFFFFF;
        m_pDynamicObjectsPixels[p - 1] = 0xFFFFFFFF;
        m_pDynamicObjectsPixels[p] = 0xFFFFFFFF;
        m_pDynamicObjectsPixels[p + 1] = 0xFFFFFFFF;
        m_pDynamicObjectsPixels[p + m_nWidth - 1] = 0xFFFFFFFF;
        m_pDynamicObjectsPixels[p + m_nWidth] = 0xFFFFFFFF;
        m_pDynamicObjectsPixels[p + m_nWidth + 1] = 0xFFFFFFFF;
    }

    const std::vector<GameAlienBerryState *> &vBerries = m_pMapService->getAlienBerries();
    for (int i = 0; i < vBerries.size(); i++) {
        int x0 = vBerries[i]->getPosition().getX() * m_nWidthK;
        int y0 = vBerries[i]->getPosition().getY() * m_nHeightK;

        int p = m_nWidth * y0 + (x0 - m_nShiftToLeftByX);
        const int color = 0xFF36d9ff;
        m_pDynamicObjectsPixels[p - 2*m_nWidth - 2] = color;
        m_pDynamicObjectsPixels[p - 2*m_nWidth - 1] = color;
        m_pDynamicObjectsPixels[p - 2*m_nWidth] = color;
        m_pDynamicObjectsPixels[p - 2*m_nWidth + 1] = color;
        m_pDynamicObjectsPixels[p - 2*m_nWidth + 2] = color;
        m_pDynamicObjectsPixels[p - m_nWidth - 2] = color;
        m_pDynamicObjectsPixels[p - 2] = color;

        m_pDynamicObjectsPixels[p + 2] = color;
        m_pDynamicObjectsPixels[p + m_nWidth + 2] = color;
        m_pDynamicObjectsPixels[p + 2*m_nWidth - 1] = color;
        m_pDynamicObjectsPixels[p + 2*m_nWidth - 2] = color;
        m_pDynamicObjectsPixels[p + 2*m_nWidth] = color;
        m_pDynamicObjectsPixels[p + 2*m_nWidth + 1] = color;
        m_pDynamicObjectsPixels[p + 2*m_nWidth + 2] = color;
    }

    int nPitch = m_nWidth * sizeof(Uint32);
    SDL_UpdateTexture(m_pTexturePlayer, nullptr, m_pDynamicObjectsPixels, nPitch);
}
