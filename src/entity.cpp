#include "Entity.hpp"
#include <SDL.h>
#include <SDL_image.h>

Entity::Entity(CoordXY p_pos, SDL_Texture* p_tex) {
    pos = p_pos;
    tex = p_tex;
    currentFrame.x = 0;
    currentFrame.y = 0;
    currentFrame.w = 32;
    currentFrame.h = 32;
}

CoordXY& Entity::getPos() {
    return pos;
}

SDL_Texture* Entity::getTex() {
    return tex;
}

SDL_Rect Entity::getCurrentFrame() {
    return currentFrame;
}
