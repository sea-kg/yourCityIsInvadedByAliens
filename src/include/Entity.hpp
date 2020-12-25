#pragma once
#include "render.h"

class Entity {

    public:
        Entity(CoordXY p_pos, SDL_Texture* p_tex);
        CoordXY& getPos();
        SDL_Texture* getTex();
        SDL_Rect getCurrentFrame();

    private:
        CoordXY pos;

        SDL_Rect currentFrame;
        SDL_Texture* tex;
};