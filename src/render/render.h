#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "game.h"
#include <vector>
#include <string>

class RenderObject {

    public:
        RenderObject(int nPositionZ);
        int getPositionZ();
        virtual void modify(const GameState& state);
        virtual void draw(SDL_Renderer* pRenderer) = 0;

    private:
        int m_nPositionZ;
};

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

class RenderLine : public RenderObject {

    public:
        RenderLine(
            const CoordXY &p1,
            const CoordXY &p2,
            const RenderColor &color,
            int nPositionZ = 0
        );
        virtual void modify(const GameState& state) override;
        virtual void draw(SDL_Renderer* renderer) override;

        const CoordXY &getAbsoluteCoord1();
        const CoordXY &getAbsoluteCoord2();
        const CoordXY &getCoord1();
        const CoordXY &getCoord2();
        void updateAbsoluteCoords(const CoordXY &p1, const CoordXY &p2);

    private:
        CoordXY m_startCoord1;
        CoordXY m_startCoord2;
        CoordXY m_coord1;
        CoordXY m_coord2;
        RenderColor m_color;
};

class RenderTriangle : public RenderObject {

    public:
        RenderTriangle(
            const CoordXY &p1,
            const CoordXY &p2,
            const CoordXY &p3,
            int nPositionZ = 0
        );
        virtual void modify(const GameState& state) override;
        virtual void draw(SDL_Renderer* renderer) override;

    private:
        RenderLine m_line1;
        RenderLine m_line2;
        RenderLine m_line3;
        RenderColor m_color;
};

class RenderTriangleAnimated1 : public RenderObject {

    public:
        RenderTriangleAnimated1(
            const CoordXY &p1,
            const CoordXY &p2,
            const CoordXY &p3,
            int nPositionZ = 0
        );
        virtual void modify(const GameState& state) override;
        virtual void draw(SDL_Renderer* renderer) override;

    private:
        RenderLine m_line1;
        RenderLine m_line2;
        RenderLine m_line3;
        CoordXY m_coordDirection;
        RenderColor m_color;
};

class RenderRectTexture : public RenderObject {

    public:
        RenderRectTexture(
            const CoordXY &p0,
            SDL_Texture* tex,
            int nPositionZ = 0
        );
        virtual void modify(const GameState& state) override;
        virtual void draw(SDL_Renderer* renderer) override;

    private:
        CoordXY m_coordCenter;
        CoordXY m_coordReal;

        SDL_Rect currentFrame;
        SDL_Texture* m_pTexture;
};


class RenderBackground : public RenderObject {

    public:
        RenderBackground(
            const CoordXY &p0,
            SDL_Texture* tex,
            int nPositionZ = 0
        );
        virtual void modify(const GameState& state) override;
        virtual void draw(SDL_Renderer* renderer) override;

    private:
        CoordXY m_coordCenter;
        CoordXY m_coordReal;

        SDL_Rect m_currentFrame;
        SDL_Texture* m_pTexture;
};

class RenderAbsoluteTextBlock : public RenderObject {

    public:
        RenderAbsoluteTextBlock(
            const CoordXY &p0,
            const std::string &sText,
            int nPositionZ = 0
        );
        virtual void modify(const GameState& state) override;
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

class RenderBuilding : public RenderObject {

    public:
        RenderBuilding(GameBuilding *pBuilding);
        virtual void modify(const GameState& state) override;
        virtual void draw(SDL_Renderer* renderer) override;

    private:
        GameBuilding *m_pBuilding;
        std::vector<RenderLine *> m_vBorderLines;
        std::vector<RenderLine *> m_vFillLines;

        void findMinMaxYCross(int nX, int &nMinY, int &nMaxY);
};


class RenderBuilding2 : public RenderObject {

    public:
        RenderBuilding2(GameBuilding *pBuilding, SDL_Texture* pTexture);
        virtual void modify(const GameState& state) override;
        virtual void draw(SDL_Renderer* renderer) override;
        CoordXY getMinPoint();
        CoordXY getMaxPoint();
    private:
        int m_nMaxX;
        int m_nMinX;
        int m_nMaxY;
        int m_nMinY;

        GameBuilding *m_pBuilding;
        std::vector<RenderLine *> m_vBorderLines;
        std::vector<CoordXY> m_vFillPointsAbsolute;
        std::vector<CoordXY> m_vFillPoints;
        std::vector<RenderLine *> m_vLines;
        SDL_Rect m_currentFrame;
        SDL_Texture* m_pTexture;

        bool containsPoint(const std::vector<CoordXY> &vPoints, const CoordXY &p);
};

class RenderPlayerAlienShip1 : public RenderObject {

    public:
        RenderPlayerAlienShip1(
            const CoordXY &p0,
            SDL_Texture* tex,
            int nPositionZ = 0
        );
        virtual void modify(const GameState& state) override;
        virtual void draw(SDL_Renderer* renderer) override;

    private:
        CoordXY m_coordCenter;
        CoordXY m_coordReal;

        SDL_Rect currentFrame;
        SDL_Texture* m_pTexture;
};


class RenderMouse : public RenderObject {

    public:
        RenderMouse(
            const CoordXY &p1,
            SDL_Texture* pTextureCursorTarget,
            int nPositionZ = 0
        );
        virtual void modify(const GameState& state) override;
        virtual void draw(SDL_Renderer* renderer) override;
        void updateCoord(const CoordXY &p0);

    private:
        int m_nCursorType;
        CoordXY m_p1;

        SDL_Texture* m_pTextureCursorTarget;
        SDL_Rect m_currentFrame;
};