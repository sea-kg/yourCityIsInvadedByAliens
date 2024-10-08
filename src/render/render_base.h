#pragma once

#include "SDL.h"
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <algorithm>
#include <cmath>
#define PI 3.14159265

#include "game_state.h"
#include "game_rocket_state.h"
#include "game_bioplast_state.h"

class RenderColor {
    public:
        RenderColor(int nR, int nG, int nB, int nA);
        void changeRenderColor(SDL_Renderer* renderer);

    private:
        int m_nR;
        int m_nG;
        int m_nB;
        int m_nA;
};

class RenderObject;

class IRenderWindow {
    public:
        virtual void addGroundObject(RenderObject *pObject) = 0;
        virtual void addRoadsObject(RenderObject *pObject) = 0;
        virtual void addVegetationObject(RenderObject *pObject) = 0;
        virtual void addBuildingsObject(RenderObject *pObject) = 0;
        virtual void addTransportsObject(RenderObject *pObject) = 0;
        virtual void addFlyingShadowObject(RenderObject *pObject) = 0;
        virtual void addRocketsObject(RenderObject *pObject) = 0;
        virtual void addFlyingObject(RenderObject *pObject) = 0;
        virtual void addCloudsObject(RenderObject *pObject) = 0;
        virtual void addPanelsObject(RenderObject *pObject) = 0;
        virtual void addScreenEffectsObject(RenderObject *pObject) = 0;
        virtual void addLoaderObject(RenderObject *pObject) = 0;
        virtual void addRocket(GameRocketState *pRocketState, RenderObject *pObject) = 0;
        virtual void addBioplast(GameBioplastState *pState) = 0;
        virtual int getNumberOfObjects() = 0;
};

class RenderObject {

    public:
        RenderObject(int nPositionZ = 1000);
        virtual ~RenderObject();
        int getPositionZ();
        bool canBeRemovedFromRenderer();
        void removeFromRenderLater();
        virtual void modify(const GameState& state, IRenderWindow* pRenderWindow);
        virtual bool canDraw(const GameState& state) = 0;
        virtual void draw(SDL_Renderer* pRenderer) = 0;
        int randomNoise();

    protected:
        bool m_bCanBeRemovedFromRenderer;
        int m_nPositionZ;
};
