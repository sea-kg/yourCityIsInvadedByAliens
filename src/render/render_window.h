#pragma once

#include "render_base.h"

class RenderWindow : public IRenderWindow {

    public:
        RenderWindow(const char* p_title, int p_w, int p_h);
        ~RenderWindow();
        virtual void addObject(RenderObject *pObject) override;
        virtual void addRocket(GameRocketState *pState) override;
        virtual void addBioplast(GameBioplastState *pState) override;
        bool isFullscreen();
        void toggleFullscreen();
        void removeObject(RenderObject *pObject);
        void sortObjectsByPositionZ();

        SDL_Texture* loadTexture(const char* p_filePath);
        void loadTextureRocket(const char* p_filePath);
        void loadTextureBioplast(const char* p_filePath);
        void cleanUp();
        void clear();
        void modifyObjects(const GameState& state);
        void drawObjects();
        SDL_Renderer* getRenderer();

        void getWindowSize(int* w, int* h);
    private:
        bool m_bFullsreeen;
        SDL_Window* m_pWindow;
        SDL_Renderer* m_pRenderer;
        SDL_DisplayMode m_displayMode;
        std::vector<RenderObject *> m_vObjects;
        SDL_Texture* m_pTextureRocket;
        SDL_Texture* m_pTextureBioplast;
};