
#pragma once
#include "render.h"
#include "game_tank0_state.h"
#include "game_rocket_state.h"

class RenderTank0 : public RenderObject {

    public:
        RenderTank0(
            GameTank0State *pTank0State,
            SDL_Texture* pTexture,
            SDL_Texture* pTexRocket,
            int nTextureTileWidth,
            int nTextureTileHeight,
            int nPositionZ = 0
        );
        virtual void modify(const GameState& state, IRenderWindow* pRenderWindow) override;
        virtual bool canDraw(const GameState& state) override;
        virtual void draw(SDL_Renderer* renderer) override;

    private:
        CoordXY m_coordRender;
        CoordXY m_coordRenderEnd;
        int m_nTextureTileWidth;
        int m_nTextureTileHeight;

        long m_nPrevPosition;
        GameTank0State *m_pTank0State;

        SDL_Rect m_currentFrame;
        SDL_Texture* m_pTexture;
        SDL_Texture* m_pTextureRocket;
};

class RenderTank0Rocket : public RenderObject {

    public:
        RenderTank0Rocket(
            GameRocketState *pTank0State,
            SDL_Texture* tex,
            int nTextureTileWidth,
            int nTextureTileHeight,
            int nPositionZ = 0
        );
        virtual ~RenderTank0Rocket();

        virtual void modify(const GameState& state, IRenderWindow* pRenderWindow) override;
        virtual bool canDraw(const GameState& state) override;
        virtual void draw(SDL_Renderer* renderer) override;

    private:
        YPos m_coordRender;
        YPos m_coordRenderEnd;
        YPos m_size;
        long m_nPrevPosition;
        int m_nLifeTime;
        int m_nMaxLifeTime;
        long m_nSpeedAnimation;
        GameRocketState *m_pRocketState;

        int m_nTextureTileWidth;
        int m_nTextureTileHeight;

        SDL_Rect m_currentFrame;
        SDL_Texture* m_pTexture;
};