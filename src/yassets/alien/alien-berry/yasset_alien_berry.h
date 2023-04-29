#pragma once

#include <yassets_service.h>

class YAssetAlienBerry : public YAsset, public RenderObject {
    public:
        YAssetAlienBerry(
            YAssetsService *pAssetsService,
            SDL_Texture *pTextureAlienBerry,
            int nFrameWidth,
            int nFrameHeight,
            int nFrameNumber
        );

        // void setAbsolutePosition(bool bAbsolutePosition);
        void setPosition(int nX, int nY);

        int getFrameWidth();
        int getFrameHeight();

        // YAsset
        virtual void setOrderZ(int nOrder) override;

        // RenderObject
        virtual void modify(const GameState& state, IRenderWindow* pRenderWindow) override;
        virtual bool canDraw(const GameState& state) override;
        virtual void draw(SDL_Renderer* renderer) override;

    private:
        CoordXY m_coordRender;
        CoordXY m_coordRenderEnd;
        int m_nFrameWidth;
        int m_nFrameHeight;
        int m_nFrameNumber;
        CoordXY m_position;

        long m_nPrevPosition;

        SDL_Rect m_currentFrame;
        SDL_Texture* m_pTexture;
};


YASSET_DECLARE_INLINE(YAssetAlienBerry)

class RenderAlienBerry0Rocket : public RenderObject {

    public:
        RenderAlienBerry0Rocket(
            GameRocketState *pAlienBerry0State,
            SDL_Texture* tex,
            int nTextureTileWidth,
            int nTextureTileHeight,
            int nPositionZ = 0
        );
        virtual ~RenderAlienBerry0Rocket();

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

