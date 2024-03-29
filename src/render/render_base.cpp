
#include "render_base.h"

// ---------------------------------------------------------------------
// RenderColor

RenderColor::RenderColor(int nR, int nG, int nB, int nA) {
    m_nR = nR;
    m_nG = nG;
    m_nB = nB;
    m_nA = nA;
}

void RenderColor::changeRenderColor(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, m_nR, m_nG, m_nB, m_nA);
}

// ---------------------------------------------------------------------
// RenderObject

RenderObject::RenderObject(int nPositionZ) {
    m_nPositionZ = nPositionZ;
    m_bCanBeRemovedFromRenderer = false;
}

RenderObject::~RenderObject() {
    // nothing
}

int RenderObject::getPositionZ() {
    return m_nPositionZ;
}

bool RenderObject::canBeRemovedFromRenderer() {
    return m_bCanBeRemovedFromRenderer;
}

void RenderObject::removeFromRenderLater() {
    m_bCanBeRemovedFromRenderer = true;
}

void RenderObject::modify(const GameState& state, IRenderWindow* pRenderWindow) {
    // nothing modify in base object
}

int RenderObject::randomNoise() {
    int n = 7;
    int nRandom = std::rand() % n;
    return nRandom - (n / 2);
}