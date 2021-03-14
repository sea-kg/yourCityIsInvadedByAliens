#include <SDL.h>
#include <SDL_image.h>
#include <iostream>

#include "render_window.h"
#include "render_rocket.h"
#include "render_bioplast.h"

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

    // if (SDL_GetDesktopDisplayMode(0, &m_displayMode)) {
    //     printf("Error getting desktop display mode\n");
    // }
}

RenderWindow::~RenderWindow() {
    if (m_pRenderer) {
        SDL_DestroyRenderer(m_pRenderer);
    }
}

void RenderWindow::addObject(RenderObject *pObject) {
    m_vObjects.push_back(pObject);
}

void RenderWindow::addRocket(GameRocketState *pState) {
    m_vObjects.push_back(new RenderRocket(pState, m_pTextureRocket, 3000));
}

void RenderWindow::addBioplast(GameBioplastState *pState) {
    m_vObjects.push_back(new RenderBioplast(pState, m_pTextureBioplast, 3000));
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
    auto it = std::find(m_vObjects.begin(), m_vObjects.end(), pObject);
    if (it != m_vObjects.end()) {
        m_vObjects.erase(it);
    }
}

void RenderWindow::sortObjectsByPositionZ() {
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

SDL_Texture* RenderWindow::loadTexture(const char* p_filePath) {
    SDL_Texture* texture = NULL;
    texture = IMG_LoadTexture(m_pRenderer, p_filePath);

    if (texture == NULL) {
        std::cout << "Failed to load texture. Error: " << SDL_GetError() << std::endl;
    }

    return texture;
}

void RenderWindow::loadTextureRocket(const char* p_filePath) {
    m_pTextureRocket = this->loadTexture(p_filePath);
}

void RenderWindow::loadTextureBioplast(const char* p_filePath) {
    m_pTextureBioplast = this->loadTexture(p_filePath);
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
    int nSize = m_vObjects.size();
    std::vector<RenderObject *> vDestroyedObjects;
    for (auto pObj: m_vObjects) {
        pObj->modify(state, this);
        if (pObj->isDestroyed()) {
            vDestroyedObjects.push_back(pObj);
        }
    }
    if (nSize > m_vObjects.size()) {
        for (int i = nSize; i < m_vObjects.size(); i++) {
            m_vObjects[i]->modify(state, this);
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

void RenderWindow::drawObjects() {
    
    for (auto pObj: m_vObjects) {
        pObj->draw(m_pRenderer);
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