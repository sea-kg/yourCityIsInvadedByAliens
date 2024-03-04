#include "loader_controller.h"
#include "primitives/render_rect_texture.h"
#include <chrono>
#include <thread>


LoaderController::LoaderController(
    RenderWindow *pRenderWindow,
    GameState *pGameState
) {
    m_pRenderWindow = pRenderWindow;
    m_pGameState = pGameState;
    m_nProgressCurrent = 0;
    m_nProgressMax = 100;
    TAG = L"LoaderController";
}

void LoaderController::init() {
    auto *pAssets = findYService<YAssetsService>();
    
    auto *pAssetBackground = pAssets->createAsset<YAssetBackground>(L"bootscreen-background1");
    this->addObject(pAssetBackground);
    
    auto *pAssetLogo1 = pAssets->createAsset<YAssetImage>(L"logo1");
    pAssetLogo1->setAbsolutePosition(true);

    // TODO redesign 
    int nBackW = 1280;
    int nBackH = 720;
    int nWindowWidth, nWindowHeight;
    m_pRenderWindow->getWindowSize(&nWindowWidth, &nWindowHeight);
    int nLogoW = 500;
    int nLogoH = 500;
    int nProgressBarH = 50;
    int nPaddingTop = 10;
    int nTop = (nWindowHeight - nLogoH - nProgressBarH - 2*nPaddingTop)/2;

    pAssetLogo1->setPosition((nWindowWidth - nLogoW)/2, nTop);
    this->addObject(pAssetLogo1);

    nTop += nLogoH + nPaddingTop;

    m_pAssetProgressBar = pAssets->createAsset<YAssetProgressBar>(L"progressbar1");
    m_pAssetProgressBar->setPosition((nWindowWidth - nLogoW)/2, nTop);
    m_pAssetProgressBar->setProgressMax(m_nProgressMax);
    m_pAssetProgressBar->setProgressCurrent(m_nProgressCurrent);
    this->addObject(m_pAssetProgressBar);

    nTop += nProgressBarH + nPaddingTop;

    m_pAssetText = pAssets->createAsset<YAssetText>(L"text1");
    m_pAssetText->setAbsolutePosition(true);
    m_pAssetText->setPosition((nWindowWidth - nLogoW)/2, nTop);
    m_pAssetText->setText(L"Loading...");
    this->addObject(m_pAssetText);
    this->draw();
}

void LoaderController::deinit() {
    // TODO remove objects from render
    for (int i = 0; i < m_vObjects.size(); i++) {
        m_pRenderWindow->removeObject(m_vObjects[i]);
    }
}

void LoaderController::updateText(const std::wstring &sNewText) {
    m_pAssetText->setText(sNewText);
    this->draw();
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
}

void LoaderController::setProgressMax(int nVal) {
    m_nProgressMax = nVal;
    m_pAssetProgressBar->setProgressMax(nVal);
    this->draw();
};

void LoaderController::setProgressCurrent(int nVal) {
    m_nProgressCurrent = nVal;
    m_pAssetProgressBar->setProgressCurrent(m_nProgressCurrent);
    this->draw();
};

void LoaderController::addToProgressCurrent(int nVal) {
    m_nProgressCurrent += nVal;
    if (m_nProgressCurrent > m_nProgressMax) {
        YLog::err(TAG, L"addToProgressCurrent added more then progress max: " + std::to_wstring(m_nProgressCurrent) + L", max: " + std::to_wstring(m_nProgressMax));
    }
    m_pAssetProgressBar->setProgressCurrent(m_nProgressCurrent);
    this->draw();
};

void LoaderController::addObject(RenderObject *pObject) {
    m_pRenderWindow->addLoaderObject(pObject);
    m_vObjects.push_back(pObject);
}

void LoaderController::draw() {
    m_pRenderWindow->clear();
    m_pRenderWindow->modifyObjects(*m_pGameState);
    m_pRenderWindow->drawObjects(*m_pGameState);
}