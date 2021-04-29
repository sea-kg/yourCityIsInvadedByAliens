#include "utils_loader_screen.h"

UtilsLoaderScreen::UtilsLoaderScreen(
    const std::string &sResourceDir,
    RenderWindow *pRenderWindow,
    GameState *pGameState
) {
    m_sResourceDir = sResourceDir;
    m_pRenderWindow = pRenderWindow;
    m_pGameState = pGameState;
}

UtilsLoaderScreen::~UtilsLoaderScreen() {
    // TODO remove objects from render
    for (int i = 0; i < m_vObjects.size(); i++) {
        m_pRenderWindow->removeObject(m_vObjects[i]);
    }
}

void UtilsLoaderScreen::init() {
    m_pTextureLoaderBackground = m_pRenderWindow->loadTexture(m_sResourceDir + "/app/textures/loader-screen-background.png");
    int nBackW = 840;
    int nBackH = 840;
    // m_pTextureLogo = m_pRenderWindow->loadTexture(m_sResourceDir + "/app/textures/logo.png");
    int nWindowWidth, nWindowHeight;

    m_pRenderWindow->getWindowSize(&nWindowWidth, &nWindowHeight);

    for (int x = 0; x < nBackW*2; x += nBackW) {
        for (int y = 0; y < nBackH*2; y += nBackH) {
            this->addObject(
                new RenderRectTexture(
                    CoordXY(x,y),
                    m_pTextureLoaderBackground,
                    nBackW,
                    nBackH,
                    0
                )
            );
        }
    }

    RenderColor progressBarColor(255,255,255,255);

    this->addObject(new RenderLine(
        CoordXY(300, nWindowHeight/2 - 50),
        CoordXY(nWindowWidth - 300, nWindowHeight/2 - 50),
        progressBarColor
    ));

    this->addObject(new RenderLine(
        CoordXY(300, nWindowHeight/2 + 50),
        CoordXY(nWindowWidth - 300, nWindowHeight/2 + 50),
        progressBarColor
    ));

    m_pText = new RenderAbsoluteTextBlock(
        CoordXY(300, nWindowHeight/2 + 50),
        "Loading..."
    );
    this->addObject(m_pText);

    this->draw();
}

void UtilsLoaderScreen::updateText(const std::string &sNewText) {
    m_pText->updateText(sNewText);
    this->draw();
}

void UtilsLoaderScreen::addObject(RenderObject *pObject) {
    m_pRenderWindow->addPanelsObject(pObject);
    m_vObjects.push_back(pObject);
}

void UtilsLoaderScreen::draw() {
    m_pRenderWindow->clear();
    m_pRenderWindow->modifyObjects(*m_pGameState);
    m_pRenderWindow->drawObjects();
}