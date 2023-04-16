#pragma once

#include <yassets_service.h>
#include <game_tank0_state.h>

class YAssetTank : public YAsset, public RenderObject {
    public:
        YAssetTank(
            YAssetsService *pAssetsService,
            SDL_Texture *pTextureTank,
            SDL_Texture *pTextureRocket,
            int nFrameWidth,
            int nFrameHeight
        );

        // void setAbsolutePosition(bool bAbsolutePosition);
        // void setPosition(int nX, int nY);

        void setGameStateTank(GameTank0State *pTank0State);

        // YAsset
        virtual void setOrderZ(int nOrder) override;

        // RenderObject
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


YASSET_DECLARE_INLINE(YAssetTank)

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

