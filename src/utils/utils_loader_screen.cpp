#include "utils_loader_screen.h"

UtilsLoaderScreen::UtilsLoaderScreen(
    const std::string &sResourceDir,
    RenderWindow *pRenderWindow,
    GameState *pGameState
) {
    m_sResourceDir = sResourceDir;
    m_pRenderWindow = pRenderWindow;
    m_pGameState = pGameState;
    m_nProgressCurrent = 0;
    m_nProgressMax = 100;
}

UtilsLoaderScreen::~UtilsLoaderScreen() {
    // TODO remove objects from render
    for (int i = 0; i < m_vObjects.size(); i++) {
        m_pRenderWindow->removeObject(m_vObjects[i]);
    }
}

void UtilsLoaderScreen::init() {
    m_pTextureLoaderBackground = m_pRenderWindow->loadTexture(m_sResourceDir + "/app/textures/loader-screen-background.png");
    m_pTextureLogoBig = m_pRenderWindow->loadTexture(m_sResourceDir + "/app/textures/logo-big-500x500.png");
    m_pTextureProgressBar = m_pRenderWindow->loadTexture(m_sResourceDir + "/app/textures/bootscreen-progressbar.png");

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

    
    this->addObject(new RenderRectTexture(
        CoordXY((nWindowWidth - 500)/2, 10),
        m_pTextureLogoBig,
        500, 500,
        1 // z-position
    ));

    m_pProgressBar = new RenderBootScreenProgressBar(
        m_pTextureProgressBar,
        CoordXY((nWindowWidth - 500)/2, 520)
    );
    m_pProgressBar->updateProgress(m_nProgressMax, m_nProgressCurrent);
    this->addObject(m_pProgressBar);

    m_pText = new RenderAbsoluteTextBlock(
        CoordXY((nWindowWidth - 500)/2, 580),
        "Loading..."
    );
    this->addObject(m_pText);

    this->draw();
}

void UtilsLoaderScreen::updateText(const std::string &sNewText) {
    m_pText->updateText(sNewText);
    this->draw();
}

void UtilsLoaderScreen::setProgressMax(int nVal) {
    m_nProgressMax = nVal;
    m_pProgressBar->updateProgress(m_nProgressMax, m_nProgressCurrent);
    this->draw();
};

void UtilsLoaderScreen::setProgressCurrent(int nVal) {
    m_nProgressCurrent = nVal;
    m_pProgressBar->updateProgress(m_nProgressMax, m_nProgressCurrent);
    this->draw();
};

void UtilsLoaderScreen::addToProgressMax(int nVal) {
    m_nProgressMax += nVal;
    m_pProgressBar->updateProgress(m_nProgressMax, m_nProgressCurrent);
    this->draw();
};

void UtilsLoaderScreen::addToProgressCurrent(int nVal) {
    m_nProgressCurrent += nVal;
    m_pProgressBar->updateProgress(m_nProgressMax, m_nProgressCurrent);
    this->draw();
};

void UtilsLoaderScreen::addObject(RenderObject *pObject) {
    m_pRenderWindow->addPanelsObject(pObject);
    m_vObjects.push_back(pObject);
}

void UtilsLoaderScreen::draw() {
    m_pRenderWindow->clear();
    m_pRenderWindow->modifyObjects(*m_pGameState);
    m_pRenderWindow->drawObjects();
}