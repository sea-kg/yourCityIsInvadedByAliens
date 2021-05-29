#pragma once
#include "primitives/render_line.h"

class RenderBuilding2 : public RenderObject {

    public:
        RenderBuilding2(GameBuilding *pBuilding, SDL_Texture* pTexture);
        virtual void modify(const GameState& state, IRenderWindow* pRenderWindow) override;
        virtual bool canDraw(const GameState& state) override;
        virtual void draw(SDL_Renderer* renderer) override;
        CoordXY getMinPoint();
        CoordXY getMaxPoint();
    private:
        CoordXY m_minPos;
        CoordXY m_maxPos;

        GameBuilding *m_pBuilding;
        std::vector<RenderLine *> m_vBorderLines;
        std::vector<CoordXY> m_vFillPointsAbsolute;
        std::vector<CoordXY> m_vFillPoints;
        std::vector<RenderLine *> m_vLines;
        SDL_Rect m_currentFrame;
        SDL_Texture* m_pTexture;

        bool containsPoint(const std::vector<CoordXY> &vPoints, const CoordXY &p);
};