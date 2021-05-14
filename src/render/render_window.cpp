#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <algorithm>

#include "render_window.h"
#include "render_rocket.h"
#include "render_bioplast.h"

// ---------------------------------------------------------------------
// RanderWindowLayer

RanderWindowLayer::RanderWindowLayer(const std::string &sName) {
    m_sName = sName;
}

void RanderWindowLayer::addObject(RenderObject *pObject) {
    m_vObjects.push_back(pObject);
}

void RanderWindowLayer::removeObject(RenderObject *pObject) {
    auto it = std::find(m_vObjects.begin(), m_vObjects.end(), pObject);
    if (it != m_vObjects.end()) {
        m_vObjects.erase(it);
    }
}

void RanderWindowLayer::drawObjects(const GameState& state, SDL_Renderer *pRenderer) {
    for (auto pObj: m_vObjects) {
        if (pObj->canDraw(state)) {
            pObj->draw(pRenderer);
        }
    }
}

void RanderWindowLayer::modifyObjects(const GameState& state, IRenderWindow *pWindow) {
    int nSize = m_vObjects.size();
    std::vector<RenderObject *> vDestroyedObjects;
    for (auto pObj: m_vObjects) {
        pObj->modify(state, pWindow);
        if (pObj->isDestroyed()) {
            vDestroyedObjects.push_back(pObj);
        }
    }
    if (nSize > m_vObjects.size()) {
        for (int i = nSize; i < m_vObjects.size(); i++) {
            m_vObjects[i]->modify(state, pWindow);
            if (m_vObjects[i]->isDestroyed()) {
                vDestroyedObjects.push_back(m_vObjects[i]);
            }
        }
    }
    for (auto pObj : vDestroyedObjects) {
        this->removeObject(pObj);
        delete pObj;
    }
}

void RanderWindowLayer::sortObjectsByPositionZ() {
    int nSize = m_vObjects.size();
    if (nSize < 2) {
        return; // nothing to sort
    }
    int nPermutations = 1; // 1 for start
    while (nPermutations > 0) {
        nPermutations = 0;
        for (int i = 0; i < nSize - 1; i++) {
            if (m_vObjects[i]->getPositionZ() > m_vObjects[i+1]->getPositionZ()) {
                RenderObject *pObject = m_vObjects[i+1];
                m_vObjects[i+1] = m_vObjects[i];
                m_vObjects[i] = pObject;
                nPermutations++;
            }
        }
        // std::cout << "nPermutations:" << nPermutations << std::endl;
    }
}

// ---------------------------------------------------------------------
// RenderWindow

RenderWindow::RenderWindow(const char* title, int w, int h) {
    m_pWindow = NULL;
    m_bFullsreeen = false;
    m_pWindow = SDL_CreateWindow(
        title,
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        w, h,
        SDL_WINDOW_SHOWN // | SDL_WINDOW_FULLSCREEN_DESKTOP
    );

    if (m_pWindow == NULL) {
        std::cout << "Window failed to init. Error: " << SDL_GetError() << std::endl;
    }

    m_pRenderer = NULL;
    m_pTextureRocket = nullptr;
    m_pRenderer = SDL_CreateRenderer(m_pWindow, -1, SDL_RENDERER_ACCELERATED);

    // order is important
    m_nLayerGround = createRenderWindowLayer("ground");
    m_nLayerRoads = createRenderWindowLayer("roads");
    m_nLayerVegetation = createRenderWindowLayer("vegetation");
    m_nLayerBuildings = createRenderWindowLayer("buildings");
    m_nLayerTransport = createRenderWindowLayer("transport");
    m_nLayerRockets = createRenderWindowLayer("rockets");
    m_nLayerFlying = createRenderWindowLayer("flying");
    m_nLayerClouds = createRenderWindowLayer("clouds");
    m_nLayerPanels = createRenderWindowLayer("panels");

    // if (SDL_GetDesktopDisplayMode(0, &m_displayMode)) {
    //     printf("Error getting desktop display mode\n");
    // }
}

RenderWindow::~RenderWindow() {
    if (m_pRenderer) {
        SDL_DestroyRenderer(m_pRenderer);
    }
}

void RenderWindow::addGroundObject(RenderObject *pObject) {
    m_vLayers[m_nLayerGround]->addObject(pObject);
}

void RenderWindow::addRoadsObject(RenderObject *pObject) {
    m_vLayers[m_nLayerRoads]->addObject(pObject);
}

void RenderWindow::addVegetationObject(RenderObject *pObject) {
    m_vLayers[m_nLayerVegetation]->addObject(pObject);
}

void RenderWindow::addBuildingsObject(RenderObject *pObject) {
    m_vLayers[m_nLayerBuildings]->addObject(pObject);
}

void RenderWindow::addTransportsObject(RenderObject *pObject) {
    m_vLayers[m_nLayerTransport]->addObject(pObject);
}

void RenderWindow::addRocketsObject(RenderObject *pObject) {
    m_vLayers[m_nLayerRockets]->addObject(pObject);
}

void RenderWindow::addFlyingObject(RenderObject *pObject) {
    m_vLayers[m_nLayerFlying]->addObject(pObject);
}

void RenderWindow::addCloudsObject(RenderObject *pObject) {
    m_vLayers[m_nLayerClouds]->addObject(pObject);
}

void RenderWindow::addPanelsObject(RenderObject *pObject) {
    m_vLayers[m_nLayerPanels]->addObject(pObject);
}

void RenderWindow::addRocket(GameRocketState *pState) {
    m_vRockets.push_back(pState);
    addRocketsObject(new RenderRocket(pState, m_pTextureRocket, 3000));
}

void RenderWindow::addBioplast(GameBioplastState *pState) {
    m_vBioplasts.push_back(pState);
    addRocketsObject(new RenderBioplast(pState, m_pTextureBioplast, 3000));
}

void RenderWindow::toggleFullscreen() {
    m_bFullsreeen = !m_bFullsreeen;
    if (m_bFullsreeen) {
        SDL_SetWindowFullscreen(m_pWindow, SDL_WINDOW_FULLSCREEN_DESKTOP);
    } else {
        SDL_SetWindowFullscreen(m_pWindow, 0);
    }
}

bool RenderWindow::isFullscreen() {
    return m_bFullsreeen;
}

void RenderWindow::removeObject(RenderObject *pObject) {
    for (int i = 0; i < m_vLayers.size(); i++) {
        m_vLayers[i]->removeObject(pObject);
    }
}

void RenderWindow::sortObjectsByPositionZ() {
    for (int i = 0; i < m_vLayers.size(); i++) {
        m_vLayers[i]->sortObjectsByPositionZ();
    }
}

SDL_Texture* RenderWindow::loadTexture(const std::string &sFilePath) {
    SDL_Texture* texture = NULL;
    texture = IMG_LoadTexture(m_pRenderer, sFilePath.c_str());

    if (texture == NULL) {
        std::cout << "Failed to load texture by path " << sFilePath << ". Error: " << SDL_GetError() << std::endl;
    }

    return texture;
}

void RenderWindow::loadTextureRocket(const std::string &sFilePath) {
    m_pTextureRocket = this->loadTexture(sFilePath.c_str());
}

void RenderWindow::loadTextureBioplast(const std::string &sFilePath) {
    m_pTextureBioplast = this->loadTexture(sFilePath.c_str());
}

void RenderWindow::cleanUp() {
    SDL_DestroyWindow(m_pWindow);
}

void RenderWindow::clear() {
    // SDL_SetRenderDrawColor(m_pRenderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    // background color
    SDL_SetRenderDrawColor(m_pRenderer, 0, 75, 92, 195);
    SDL_RenderClear(m_pRenderer);
}

void RenderWindow::modifyObjects(const GameState& state) {
    for (int i = 0; i < m_vLayers.size(); i++) {
        m_vLayers[i]->modifyObjects(state, this);
    }
}

void RenderWindow::drawObjects(const GameState& state) {
    for (int i = 0; i < m_vLayers.size(); i++) {
        m_vLayers[i]->drawObjects(state, m_pRenderer);
    }

    // finish
    SDL_RenderPresent(m_pRenderer);
}

SDL_Renderer* RenderWindow::getRenderer() {
    return m_pRenderer;
}

void RenderWindow::getWindowSize(int* w, int* h) {
    SDL_GetWindowSize(m_pWindow, w, h);
}

int RenderWindow::createRenderWindowLayer(const std::string &sName) {
    auto pLayer = new RanderWindowLayer(sName);
    m_vLayers.push_back(pLayer);
    return m_vLayers.size() - 1;
}

RanderWindowLayer *RenderWindow::getLayer(int nLayer) {
    return m_vLayers[nLayer];
}