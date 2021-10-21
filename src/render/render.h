#pragma once
#include "render_window.h"

// #include "game_state.h"
#include "primitives/render_line.h"

#include <vector>
#include <string>
#include <iostream>

class RenderAbsoluteTextBlock : public RenderObject {

    public:
        RenderAbsoluteTextBlock(
            const CoordXY &p0,
            const std::wstring &sText,
            int nPositionZ = 0
        );
        virtual void modify(const GameState& state, IRenderWindow* pRenderWindow) override;
        virtual bool canDraw(const GameState& state) override;
        virtual void draw(SDL_Renderer* renderer) override;
        void updateText(const std::wstring &sNewText);

    private:
        CoordXY m_coordCenter;
        std::wstring m_sText;
        std::wstring m_sUpdateText;
        TTF_Font* m_pFont;
        SDL_Color m_color;

        SDL_Rect currentFrame;
};

