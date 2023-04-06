#pragma once

#include "render_base.h"

class RanderWindowLayer {
    public:
        RanderWindowLayer(const std::wstring &sName);
        void addObject(RenderObject *pObject);
        void removeObject(RenderObject *pObject);
        void drawObjects(const GameState& state, SDL_Renderer *pRenderer);
        void modifyObjects(const GameState& state, IRenderWindow *pWindow);
        void sortObjectsByPositionZ();

    private:
        std::wstring m_sName;
        std::vector<RenderObject *> m_vObjects;
};

class RenderWindow : public IRenderWindow {

    public:
        RenderWindow(const std::wstring &sTitle, int p_w, int p_h);
        ~RenderWindow();
        virtual void addGroundObject(RenderObject *pObject) override;
        virtual void addRoadsObject(RenderObject *pObject) override;
        virtual void addVegetationObject(RenderObject *pObject) override;
        virtual void addBuildingsObject(RenderObject *pObject) override;
        virtual void addTransportsObject(RenderObject *pObject) override;
        virtual void addFlyingShadowObject(RenderObject *pObject) override;
        virtual void addRocketsObject(RenderObject *pObject) override;
        virtual void addFlyingObject(RenderObject *pObject) override;
        virtual void addCloudsObject(RenderObject *pObject) override;
        virtual void addPanelsObject(RenderObject *pObject) override;
        virtual void addScreenEffectsObject(RenderObject *pObject) override;
        virtual void addLoaderObject(RenderObject *pObject) override;
        
        virtual void addRocket(GameRocketState *pState, RenderObject *pObject) override;
        virtual void addBioplast(GameBioplastState *pState) override;
        bool isFullscreen();
        bool toggleFullscreen();
        void removeObject(RenderObject *pObject);
        void sortObjectsByPositionZ();

        SDL_Texture* loadTexture(const std::wstring &sFilePath);
        void loadTextureBioplast(const std::wstring &sFilePath);
        void cleanUp();
        void clear();
        void modifyObjects(const GameState& state);
        void drawObjects(const GameState& state);
        SDL_Renderer* getRenderer();

        void getWindowSize(int* w, int* h);

        std::vector<GameRocketState *> m_vRockets;
        std::vector<GameBioplastState *> m_vBioplasts;
        
    private:
        int createRenderWindowLayer(const std::wstring &sName);
        RanderWindowLayer *getLayer(int nLayer);

        bool m_bFullsreeen;
        SDL_Window* m_pWindow;
        SDL_Renderer* m_pRenderer;
        SDL_DisplayMode m_displayMode;
        SDL_Texture* m_pTextureBioplast;
        std::vector<RanderWindowLayer *> m_vLayers;

        int m_nLayerGround;
        int m_nLayerRoads;
        int m_nLayerVegetation;
        int m_nLayerBuildings;
        int m_nLayerTransport;
        int m_nLayerFlyingShadow;
        int m_nLayerRockets;
        int m_nLayerFlying;
        int m_nLayerClouds;
        int m_nLayerPanels;
        int m_nLayerScreenEffects;
        int m_nLayerLoader;
};