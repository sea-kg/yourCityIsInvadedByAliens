
#pragma once
#include "render.h"

enum class RoadPart {
    VERTICAL = 0,
    HORIZONTAL = 1,
    RIGHT_DOWN = 2,
    LEFT_DOWN = 3,
    LEFT_UP = 4,
    RIGHT_UP = 5,
    CROSS = 6,
    LEFT_UP_DOWN = 7,
    LEFT_RIGHT_UP = 8,
    RIGHT_UP_DOWN = 9,
    LEFT_RIGHT_DOWN = 10
};

class RenderRoad0 : public RenderObject {

    public:
        RenderRoad0(
            CoordXY pos,
            SDL_Texture* tex,
            RoadPart nTile
        );
        virtual void modify(const GameState& state, IRenderWindow* pRenderWindow) override;
        virtual void draw(SDL_Renderer* renderer) override;

    private:
        CoordXY m_cAbsolutePos;
        CoordXY m_coordRender;
        int m_nNumberOfTile;

        SDL_Rect m_currentFrame;
        SDL_Texture* m_pTexture;
};