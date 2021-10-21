
#pragma once
#include "render.h"
#include "ylog.h"

enum class RoadPart {
    NONE = -1,
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
    LEFT_RIGHT_DOWN = 10,
};

static RoadPart convertStringToRoadPart(const std::wstring &sRoadPart) {
    if (sRoadPart == L"vertical") {
        return RoadPart::VERTICAL;
    } else if (sRoadPart == L"horizontal") {
        return RoadPart::HORIZONTAL;
    } else if (sRoadPart == L"right-down") {
        return RoadPart::RIGHT_DOWN;
    } else if (sRoadPart == L"left-down") {
        return RoadPart::LEFT_DOWN;
    } else if (sRoadPart == L"left-up") {
        return RoadPart::LEFT_UP;
    } else if (sRoadPart == L"right-up") {
        return RoadPart::RIGHT_UP;
    } else if (sRoadPart == L"cross") {
        return RoadPart::CROSS;
    } else if (sRoadPart == L"left-up-down") {
        return RoadPart::LEFT_UP_DOWN;
    } else if (sRoadPart == L"left-right-up") {
        return RoadPart::LEFT_RIGHT_UP;
    } else if (sRoadPart == L"right-up-down") {
        return RoadPart::RIGHT_UP_DOWN;
    } else if (sRoadPart == L"left-right-down") {
        return RoadPart::LEFT_RIGHT_DOWN;
    }
    YLog::throw_err(L"convertStringToRoadPart", L"'" + sRoadPart + L"' not expected");
    return RoadPart::NONE;
};

class RenderRoad0 : public RenderObject {

    public:
        RenderRoad0(
            const CoordXY &pos,
            SDL_Texture* tex,
            RoadPart nTile
        );
        virtual void modify(const GameState& state, IRenderWindow* pRenderWindow) override;
        virtual bool canDraw(const GameState& state) override;
        virtual void draw(SDL_Renderer* renderer) override;

    private:
        CoordXY m_coordPos;
        CoordXY m_coordPosEnd;
        CoordXY m_coordRender;
        CoordXY m_coordRenderEnd;
        int m_nNumberOfTile;

        SDL_Rect m_currentFrame;
        SDL_Texture* m_pTexture;
};