#include <yasset_text.h>
#include <render.h>
#include <codecvt>

// ---------------------------------------------------------------------
// YAssetText

YAssetText::YAssetText(
    YAssetsService *pAssetsService,
    SDL_Texture *pTexture,
    const std::wstring &sAlphabet
)
: YAsset(pAssetsService), RenderObject(1000) {
    m_pTexture = pTexture;
    // TODO redesign to wstring
    m_sAlphabet = std::wstring(sAlphabet.begin(), sAlphabet.end());
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
    YLog::info(TAG, L"m_sUpdateText = " + m_sUpdateText);
}

RenderObject *YAssetText::getRenderObject() {
    // TOOD redesign
    return new RenderAbsoluteTextBlock(
        CoordXY(m_nX, m_nY),
        m_sText
    );
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

    for (int i = 0; i < m_sText.size(); i++) {
        wchar_t c = m_sText[i];
        currentFrame.x = 0;
        for (int n = 0; n < m_sAlphabet.size(); n++) {
            if (m_sAlphabet[n] == c) {
                currentFrame.x = (n + 1)*50;
            }
        }
        dst.x = i*m_nFontSize + m_nX;
        SDL_RenderCopy(renderer, m_pTexture, &currentFrame, &dst);
    }
};

