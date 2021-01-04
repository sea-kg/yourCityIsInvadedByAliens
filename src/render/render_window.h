#pragma once
#include "render.h"

class RenderWindow  {

    public:
        RenderWindow(const char* p_title, int p_w, int p_h);
        ~RenderWindow();
        void addObject(RenderObject *pObject);
        void sortObjectsByPositionZ();

        SDL_Texture* loadTexture(const char* p_filePath);
        void cleanUp();
        void clear();
        void modifyObjects(const GameState& state);
        void drawObjects();
        
    private:
        SDL_Window* window;
        SDL_Renderer* renderer;
        std::vector<RenderObject *> m_vObjects;
};