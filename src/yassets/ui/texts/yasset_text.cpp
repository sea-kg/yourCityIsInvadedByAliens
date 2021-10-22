#include "yasset_text.h"
#include <render.h>
#include <codecvt>

// ---------------------------------------------------------------------
// YAssetText

YAssetText::YAssetText(
    YAssetsService *pAssetsService,
    SDL_Texture *pTexture,
    const std::vector<std::wstring> &vAlphabets
)
: YAsset(pAssetsService), RenderObject(1000) {
    m_pTexture = pTexture;
    m_vAlphabets = vAlphabets;
    m_bUpdatedText = false;
    m_nFontSize = 25;
}

void YAssetText::setAbsolutePosition(bool bAbsolutePosition) {
    m_bAbsolutePosition = bAbsolutePosition;
}

void YAssetText::setPosition(int nX, int nY) {
    m_nX = nX;
    m_nY = nY;
}

void YAssetText::setText(const std::wstring& sText) {
    m_sUpdateText = sText;
    m_bUpdatedText = true;
    // YLog::info(TAG, L"m_sUpdateText = " + m_sUpdateText);
}

void YAssetText::setOrderZ(int nOrder) {
    m_nPositionZ = nOrder;
}

void YAssetText::modify(const GameState& state, IRenderWindow* pRenderWindow) {
    // nothing i think
    if (m_bUpdatedText) {
        m_sText = m_sUpdateText;
        m_bUpdatedText = false;
    }
};

bool YAssetText::canDraw(const GameState& state) {
    return true;
}

void YAssetText::draw(SDL_Renderer* renderer) {
    SDL_Rect currentFrame;
    currentFrame.x = 0;
    currentFrame.y = 0;
    currentFrame.w = 50;
    currentFrame.h = 50;

    SDL_Rect dst;
    dst.x = 0;
    dst.y = m_nY;
    dst.w = m_nFontSize;
    dst.h = m_nFontSize;

    int nPos = 0;
    // TODO optimaze here - move find positions on step setText
    for (int i = 0; i < m_sText.size(); i++) {
        wchar_t c = m_sText[i];
        if (c == L'\n') {
            dst.y += m_nFontSize;
            nPos = 0;
            continue;
        }
        findPosition(currentFrame, c);
        dst.x = nPos*m_nFontSize + m_nX;
        SDL_RenderCopy(renderer, m_pTexture, &currentFrame, &dst);
        nPos++;
    }
};

void YAssetText::findPosition(SDL_Rect &frame, wchar_t c) {
    frame.x = 0;
    frame.y = 0;
    for (int y = 0; y < m_vAlphabets.size(); y++) {
        const std::wstring &sAlphabet = m_vAlphabets[y];
        // std::wcout << sAlphabet << std::endl;
        for (int x = 0; x < sAlphabet.size(); x++) {
            if (sAlphabet[x] == c) {
                frame.x = x*50;
                frame.y = (y+1)*50; // ignore first line because it must has only one wrong character
                return;
            }
        }
    }
}

