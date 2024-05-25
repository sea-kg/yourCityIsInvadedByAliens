#include "utils_start_dialog.h"
#include <chrono>
#include <thread>
#include "render_ui.h"

UtilsStartDialog::UtilsStartDialog(
    const std::wstring &sResourceDir,
    RenderWindow *pRenderWindow,
    GameState *pGameState
) {
    m_sResourceDir = sResourceDir;
    m_pRenderWindow = pRenderWindow;
    m_pGameState = pGameState;
}

UtilsStartDialog::~UtilsStartDialog() {
    // TODO remove objects from render
    for (int i = 0; i < m_vObjects.size(); i++) {
        m_pRenderWindow->removeObject(m_vObjects[i]);
    }
}

void UtilsStartDialog::init() {

    // m_pTextureLogo = m_pRenderWindow->loadTexture(m_sResourceDir + "/textures/logo.png");
    int nWindowWidth, nWindowHeight;

    m_pRenderWindow->getWindowSize(&nWindowWidth, &nWindowHeight);

    m_pTextureDialogBackground = m_pRenderWindow->loadTexture(m_sResourceDir + L"/app/textures/dialog-background.png");
    this->addObject(new RenderDialog(
        m_pTextureDialogBackground,
        550,
        450,
        3000
    ));

    auto *pTextureButtonStart = m_pRenderWindow->loadTexture(m_sResourceDir + L"/app/textures/button-start.png");
    SDL_Point size;
    SDL_QueryTexture(pTextureButtonStart, NULL, NULL, &size.x, &size.y);
    m_posButtonStartTopLeft = YPos(
        nWindowWidth/2 - size.x/2,
        nWindowHeight/2 - size.y/2 - 80
    );
    m_posButtonStartBottomRight = m_posButtonStartTopLeft + YPos(size.x, size.y);
    m_pRenderButtonStart = new RenderButtonSimple(
        pTextureButtonStart,
        m_posButtonStartTopLeft,
        3001
    );
    this->addObject(m_pRenderButtonStart);

    auto *pTextureButtonMisicOnOff = m_pRenderWindow->loadTexture(m_sResourceDir + L"/app/textures/music-on-off.png");
    SDL_Point size2;
    SDL_QueryTexture(pTextureButtonMisicOnOff, NULL, NULL, &size2.x, &size2.y);
    m_posTopLeftMusicOnOff = YPos(
        nWindowWidth/2 - size2.x/2,
        nWindowHeight/2 + 50
    );
    m_posBottomRightMusicOnOff = YPos(
        m_posTopLeftMusicOnOff.getX() + size2.x,
        m_posTopLeftMusicOnOff.getY() + size2.y/2
    );
    m_pRenderButtonMusicOnOff = new RenderButtonMusicOnOff(
        pTextureButtonMisicOnOff,
        m_posTopLeftMusicOnOff,
        3001
    );
    this->addObject(m_pRenderButtonMusicOnOff);

    SDL_Texture* pTextureCursor = m_pRenderWindow->loadTexture(m_sResourceDir + L"/app/textures/mouse-target.png");
    m_pMouseCursor = new RenderMouse(
        YPos(nWindowWidth/2,nWindowHeight/2),
        pTextureCursor,
        10000
    );
    this->addObject(m_pMouseCursor);
    this->draw();
}

void UtilsStartDialog::addObject(RenderObject *pObject) {
    m_pRenderWindow->addPanelsObject(pObject);
    m_vObjects.push_back(pObject);
}

void UtilsStartDialog::draw() {
    m_pRenderWindow->clear();
    m_pRenderWindow->modifyObjects(*m_pGameState);
    m_pRenderWindow->drawObjects(*m_pGameState);
}

bool UtilsStartDialog::start() {
    bool bPaused = true;
    bool bQuit = false;
    m_pGameState->setMouseCaptured(false);

    while (bPaused) {
        SDL_Event event;
        const Uint8 *keyboard_state_array = SDL_GetKeyboardState(NULL);

        // Get our controls and events
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                bPaused = false;
                bQuit = true;
                break;
            }
            if (event.type == SDL_MOUSEMOTION) {
               if (m_pGameState->isMouseCaptured()) {
                   m_mousePosition = YPos(event.motion.x, event.motion.y);
                   m_pMouseCursor->updateCoord(m_mousePosition);
                   m_pRenderButtonMusicOnOff->setAnimate(
                       m_mousePosition.isInsideRect(m_posTopLeftMusicOnOff, m_posBottomRightMusicOnOff)
                   );
                   m_pRenderButtonStart->setAnimate(
                        m_mousePosition.isInsideRect(m_posButtonStartTopLeft, m_posButtonStartBottomRight)
                   );
               }
            } else if (event.type == SDL_MOUSEBUTTONDOWN) {
                m_pGameState->setMouseCaptured(true);
                if (m_mousePosition.isInsideRect(m_posTopLeftMusicOnOff, m_posBottomRightMusicOnOff)) {
                    m_pGameState->togglePlayMusic();
                }
                if (m_mousePosition.isInsideRect(m_posButtonStartTopLeft, m_posButtonStartBottomRight)) {
                    bPaused = false;
                    bQuit = false;
                    break;
                }
            }
            if (event.type == SDL_KEYDOWN || event.type == SDL_KEYUP) {
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    std::cout << "Escaping" << std::endl;
                    if (m_pGameState->isMouseCaptured()) {
                        m_pGameState->setMouseCaptured(false);
                    } else {
                        bPaused = false;
                        bQuit = true;
                        break;
                    }
                }
            }
        }

        this->draw();
        std::this_thread::sleep_for(
            std::chrono::milliseconds(5)
        );
    }
    m_pGameState->setMouseCaptured(false);

    m_pRenderWindow->clear();
    return !bQuit;
}