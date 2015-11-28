#ifndef _APPLICATION_H_
#define _APPLICATION_H_

#include "utils.h"
#include "List.h"
#include "State.h"
#include "Engine3D/Camera.h"

class Camera;

class Application
{
public:
    Application();
    ~Application();

    bool                Init();
    void                Start();

    bool                GetWindowFullScreen();
    int                 GetWindowPhysicalWidth();
    int                 GetWindowPhysicalHeight();
    int                 GetWindowLogicalWidth();
    int                 GetWindowLogicalHeight();
    int                 GetWindowBPP();
    float               GetWindowScaleFactor();

    int                 GetCurrentState();
    void                GoToNextState(int state);

    void                SetClearColor(int red, int green, int blue, int alpha);

    void                SetCameraToRender(Camera *pCamera);

    sf::Vector2i        GetMousePosition();

    sfg::Window::Ptr    GetWindowGUI();

private:
    bool                m_windowFullScreen;
    int                 m_windowPhysicalWidth;
    int                 m_windowPhysicalHeight;
    int                 m_windowLogicalWidth;
    int                 m_windowLogicalHeight;
    int                 m_windowBPP;
    float               m_windowScaleFactor;
    sf::RenderWindow    *m_pWindow;
    sf::Texture         m_windowTexture;
    sf::Sprite          m_windowSprite;

    int                 m_fps;
    int                 m_frames;
    int                 m_timeOld;
    int                 m_timeStart;
    int                 m_timeEnd;
    int                 m_fpsAverage;
    int                 m_fpsHighest;

    int                 m_stateCurr;
    int                 m_stateNext;
    List                m_listStates;

    sf::Color           m_clearColor;

    Camera              *m_pCameraToRender;

    sfg::SFGUI          m_sfgui;
    sfg::Window::Ptr    m_pWindowGUI;

    // ------------------------------------------------------------------

    void                GetBestVideoMode();

    void                Input();
    void                Update();
    void                Render();

    State               *GetState(int type);

    void                ScreenDump(const char *pDestFile);

    bool                InitEngine3D();
};

extern Application  *g_pApplication;

#endif // _APPLICATION_H_
