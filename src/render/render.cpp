#include "render.h"
#include "wsjcpp_core.h"

// ---------------------------------------------------------------------
// RenderStateObjects

RenderStateObjects::RenderStateObjects() {
    m_nStartTime = 0;
    m_nElapsedTime = 0;
}

void RenderStateObjects::init() {
    m_nStartTime = WsjcppCore::getCurrentTimeInMilliseconds();
}

long RenderStateObjects::getElapsedTime() {
    return m_nElapsedTime;
}

// ---------------------------------------------------------------------
// RenderObject

RenderObject::RenderObject(int nPositionZ) {
    m_nPositionZ = nPositionZ;
}

int RenderObject::getPositionZ() {
    return m_nPositionZ;
}

void RenderObject::modify(const RenderStateObjects& state) {
    // nothing modify in base object
}

// ---------------------------------------------------------------------
// RenderLine

RenderLine::RenderLine(int x1, int y1, int x2, int y2, int nPositionZ) 
: RenderObject(nPositionZ) {
    m_beginX1 = x1;
    m_beginY1 = y1;
    m_beginX2 = x2;
    m_beginY2 = y2;
}

void RenderLine::draw(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
    SDL_RenderDrawLine(renderer, m_beginX1, m_beginY1, m_beginX2, m_beginY2);
}