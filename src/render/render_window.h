#pragma once

#include "render_base.h"

class RenderWindow : public IRenderWindow {

    public:
        RenderWindow(const char* p_title, int p_w, int p_h);
        ~RenderWindow();
        virtual void addObject(RenderObject *pObject) override;
        void removeObject(RenderObject *pObject);
        void sortObjectsByPositionZ();

        SDL_Texture* loadTexture(const char* p_filePath);
        void cleanUp();
        void clear();
        void modifyObjects(const GameState& state);
        void drawObjects();
        SDL_Renderer* getRenderer();

    private:
        SDL_Window* window;
        SDL_Renderer* m_pRenderer;
        std::vector<RenderObject *> m_vObjects;
};