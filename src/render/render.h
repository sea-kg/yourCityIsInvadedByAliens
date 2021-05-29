#pragma once
#include "render_window.h"

#include "game_state.h"
#include "primitives/render_line.h"

#include <vector>
#include <string>
#include <iostream>

class RenderTriangle : public RenderObject {

    public:
        RenderTriangle(
            const CoordXY &p1,
            const CoordXY &p2,
            const CoordXY &p3,
            int nPositionZ = 0
        );
        virtual void modify(const GameState& state, IRenderWindow* pRenderWindow) override;
        virtual bool canDraw(const GameState& state) override;
        virtual void draw(SDL_Renderer* renderer) override;

    private:
        RenderLine m_line1;
        RenderLine m_line2;
        RenderLine m_line3;
        RenderColor m_color;
};

class RenderRectTexture : public RenderObject {

    public:
        RenderRectTexture(
            const CoordXY &p0,
            SDL_Texture* tex,
            int nTextureWidth, 
            int nTextureHeight,
            int nPositionZ = 0
        );
        virtual void modify(const GameState& state, IRenderWindow* pRenderWindow) override;
        virtual bool canDraw(const GameState& state) override;
        virtual void draw(SDL_Renderer* renderer) override;

    private:
        CoordXY m_coordPos;
        CoordXY m_coordPosEnd;
        CoordXY m_coordRender;
        CoordXY m_coordRenderEnd;

        SDL_Rect currentFrame;
        SDL_Texture* m_pTexture;
};

class RenderAbsoluteTextBlock : public RenderObject {

    public:
        RenderAbsoluteTextBlock(
            const CoordXY &p0,
            const std::string &sText,
            int nPositionZ = 0
        );
        virtual void modify(const GameState& state, IRenderWindow* pRenderWindow) override;
        virtual bool canDraw(const GameState& state) override;
        virtual void draw(SDL_Renderer* renderer) override;
        void updateText(const std::string &sNewText);

    private:
        CoordXY m_coordCenter;
        std::string m_sText;
        std::string m_sUpdateText;
        TTF_Font* m_pFont;
        SDL_Color m_color;

        SDL_Rect currentFrame;
};

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

class RenderMouse : public RenderObject {

    public:
        RenderMouse(
            const CoordXY &p1,
            SDL_Texture* pTextureCursorTarget,
            int nPositionZ = 0
        );
        virtual void modify(const GameState& state, IRenderWindow* pRenderWindow) override;
        virtual bool canDraw(const GameState& state) override;
        virtual void draw(SDL_Renderer* renderer) override;
        void updateCoord(const CoordXY &p0);

    private:
        int m_nCursorType;
        CoordXY m_p1;

        SDL_Texture* m_pTextureCursorTarget;
        SDL_Rect m_currentFrame;
};
