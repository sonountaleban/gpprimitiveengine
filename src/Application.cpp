#include "Application.h"
#include "TexturesManager.h"
#include "ValuesManager.h"
#include "StateMain.h"
#include "Engine3D/Engine3D.h"

#define NUM_MAX_FPS 60

Application *g_pApplication = NULL;

Application::Application()
{
    m_windowFullScreen = false;
    m_windowPhysicalWidth = 0;
    m_windowPhysicalHeight = 0;
    m_windowLogicalWidth = 0;
    m_windowLogicalHeight = 0;
    m_windowBPP = 0;
    m_windowScaleFactor = 1.0f;
    m_pWindow = NULL;

    m_fps = 0;
    m_frames = 0;
    m_timeOld = 0;
    m_timeStart = 0;
    m_timeEnd = 0;
    m_fpsAverage = 0;
    m_fpsHighest = 0;

    m_stateCurr = STATE_TYPE_NONE;
    m_stateNext = STATE_TYPE_NONE;

    m_pCameraToRender = NULL;

    m_pWindowGUI = NULL;
}

Application::~Application()
{
    int     a;
    State   *pState;

    for (a = 0; a < m_listStates.GetNumElements(); a ++)
    {
        pState = (State *)m_listStates.GetElement(a).p;
        if (pState)
        {
            if (pState->GetType() == m_stateCurr)
            {
                pState->End();
            }
            delete pState;
        }
    }

    if (g_pEngine3D)
    {
        delete g_pEngine3D;
    }

    if (g_pTexturesManager)
    {
        delete g_pTexturesManager;
    }

    if (g_pValuesManager)
    {
        delete g_pValuesManager;
    }

    if (m_pWindowGUI)
    {
        m_pWindowGUI->RemoveAll();
    }

    if (m_pWindow)
    {
        delete m_pWindow;
    }
}

bool Application::Init()
{
    Element     e;
    sf::String  title;

    DEBUGLOG("Init SFML: ");

    if (!m_pWindow)
    {
        m_pWindow = new sf::RenderWindow();
    }

    if (m_pWindow)
    {
        DEBUGLOG("OK.\n\n");
    }
    else
    {
        DEBUGLOG("Error!\n");

        return false;
    }

    GetBestVideoMode();

    DEBUGLOG("Init window texture: ");

    if (m_windowTexture.create(m_windowLogicalWidth, m_windowLogicalHeight))
    {
        m_windowTexture.setSmooth(false);

        DEBUGLOG("OK.\n\n");
    }
    else
    {
        DEBUGLOG("Error!\n");

        return false;
    }

    if (!InitEngine3D())
    {
        return false;
    }

    title = TITLE_APPLICATION;
    title += " v";
    title += (const char)('0' + VERSION_MAJ_APPLICATION);
    title += ".";
    title += (const char)('0' + VERSION_MIN_APPLICATION);

    if (m_windowFullScreen)
    {
        m_pWindow->create(sf::VideoMode(m_windowPhysicalWidth, m_windowPhysicalHeight, m_windowBPP),
                          title, sf::Style::Fullscreen);
    }
    else
    {
        m_pWindow->create(sf::VideoMode(m_windowPhysicalWidth, m_windowPhysicalHeight, m_windowBPP),
                          title, sf::Style::Titlebar | sf::Style::Close);
    }

    m_pWindow->setVerticalSyncEnabled(true);
    m_pWindow->setActive();
    m_pWindow->setMouseCursorVisible(!m_windowFullScreen);
    m_pWindow->setFramerateLimit(NUM_MAX_FPS);

    g_pValuesManager = new ValuesManager();

    g_pTexturesManager = new TexturesManager();

    g_pTexturesManager->AddTextureSheet(g_pTexturesManager->GetTextureFileName(TexturesManager::TEXTURE_TEX00));
    g_pTexturesManager->AddTextureSheet(g_pTexturesManager->GetTextureFileName(TexturesManager::TEXTURE_TEX01));

    m_windowSprite.setTexture(m_windowTexture);
    m_windowSprite.setScale(m_windowScaleFactor, m_windowScaleFactor);

    m_pWindowGUI = sfg::Window::Create();
    if (m_pWindowGUI)
    {
        m_pWindowGUI->Show(false);
    }

    DEBUGLOG("Start!\n\n");

    e.p = new StateMain();

    m_listStates.AddElement(e);

    GoToNextState(STATE_TYPE_MAIN);

    return true;
}

void Application::Start()
{
    char        string[100];
    bool        exit = false;
    State       *pCurrState;
    State       *pNextState;
    sf::Clock   clock;
    sf::Event   event;
    sf::String  title;

    m_timeOld = clock.getElapsedTime().asMilliseconds();

    while (m_pWindow->isOpen() && !exit)
    {
        m_timeStart = clock.getElapsedTime().asMilliseconds();

        while (m_pWindow->pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                exit = true;
            }
            else if (event.type == sf::Event::KeyPressed)
            {
                switch (event.key.code)
                {
                    case sf::Keyboard::Escape:
                        exit = true;

                        break;

                    default:
                        break;
                }
            }
            else if (event.type == sf::Event::KeyReleased)
            {
                switch (event.key.code)
                {
                    case sf::Keyboard::Up:

                        break;

                    case sf::Keyboard::Left:

                        break;

                    case sf::Keyboard::Right:

                        break;

                    case sf::Keyboard::Down:

                        break;

                    case sf::Keyboard::S:
                        ScreenDump("./screenshot.png");

                        break;

                    case sf::Keyboard::F1:
                        if (m_pWindowGUI)
                        {
                            m_pWindowGUI->Show(true);
                        }

                        break;

                    case sf::Keyboard::F2:
                        if (m_pWindowGUI)
                        {
                            m_pWindowGUI->Show(false);
                        }

                        break;

                    default:
                        break;
                }
            }

            if (m_pWindowGUI && m_pWindowGUI->IsGloballyVisible())
            {
                m_pWindowGUI->HandleEvent(event);
            }
        }

        if (!m_pWindowGUI || !m_pWindowGUI->IsGloballyVisible())
        {
            Input();
        }
        Update();
        Render();

        pCurrState = GetState(m_stateCurr);
        pNextState = GetState(m_stateNext);
        if (pNextState)
        {
            if (pCurrState)
            {
                pCurrState->End();
                pNextState->Start();
            }
            else
            {
                pNextState->Start();
            }

            m_stateCurr = m_stateNext;
            m_stateNext = STATE_TYPE_NONE;
        }

        m_timeEnd = clock.getElapsedTime().asMilliseconds();

        if (m_timeEnd - m_timeStart < 1000 / NUM_MAX_FPS)
        {
            //sf::sleep(sf::milliseconds(1000 / NUM_MAX_FPS - (m_timeEnd - m_timeStart)));
        }

        m_frames ++;
        if (m_timeEnd - m_timeOld >= 1000)
        {
            m_fps = m_frames;
            m_fpsAverage += m_fps;
            m_fpsAverage >>= 1;
            if (m_fps > m_fpsHighest)
            {
                m_fpsHighest = m_fps;
            }
            m_frames = 0;
            m_timeOld = clock.getElapsedTime().asMilliseconds();
        }

        sprintf(string, "%d  fpsAvv:%d  fpsHigh:%d", m_fps, m_fpsAverage, m_fpsHighest);

        title = TITLE_APPLICATION;
        title += " v";
        title += (const char)('0' + VERSION_MAJ_APPLICATION);
        title += ".";
        title += (const char)('0' + VERSION_MIN_APPLICATION);
        title += "  fps:";
        title += string;

        if (m_pCameraToRender)
        {
            sprintf(string, "  x: %.1f  y: %.1f  z: %.1f", m_pCameraToRender->m_pos.m_x, m_pCameraToRender->m_pos.m_y, m_pCameraToRender->m_pos.m_z);

            title += string;
        }

        m_pWindow->setTitle(title);
    }

    m_pWindow->close();

    sf::sleep(sf::milliseconds(500));
}

bool Application::GetWindowFullScreen()
{
    return m_windowFullScreen;
}

int Application::GetWindowPhysicalWidth()
{
    return m_windowPhysicalWidth;
}

int Application::GetWindowPhysicalHeight()
{
    return m_windowPhysicalHeight;
}

int Application::GetWindowLogicalWidth()
{
    return m_windowLogicalWidth;
}

int Application::GetWindowLogicalHeight()
{
    return m_windowLogicalHeight;
}

int Application::GetWindowBPP()
{
    return m_windowBPP;
}

float Application::GetWindowScaleFactor()
{
    return m_windowScaleFactor;
}

int Application::GetCurrentState()
{
    return m_stateCurr;
}

void Application::GoToNextState(int state)
{
    m_stateNext = state;
}

void Application::SetClearColor(int red, int green, int blue, int alpha)
{
    m_clearColor.r = red;
    m_clearColor.g = green;
    m_clearColor.b = blue;
    m_clearColor.a = alpha;
}

void Application::SetCameraToRender(Camera *pCameraToRender)
{
    m_pCameraToRender = pCameraToRender;
}

sf::Vector2i Application::GetMousePosition()
{
    return sf::Mouse::getPosition(*m_pWindow);
}

sfg::Window::Ptr Application::GetWindowGUI()
{
    return m_pWindowGUI;
}

void Application::GetBestVideoMode()
{
    sf::VideoMode               desktop = sf::VideoMode::getDesktopMode();
    //std::vector<sf::VideoMode>  modes = sf::VideoMode::getFullscreenModes();

    m_windowFullScreen = false;
    m_windowPhysicalWidth = 0;
    m_windowPhysicalHeight = 0;
    m_windowLogicalWidth = 0;
    m_windowLogicalHeight = 0;
    m_windowBPP = 0;

    /*if (modes.size() <= 0)
    {
        DEBUGLOG("No fullscreen modes available!\n\n");
    }
    else
    {
        DEBUGLOG("Available fullscreen modes:\n");

        for (int i = 0; i < (int)modes.size(); i ++)
        {
            sf::VideoMode mode = modes[i];

            if (m_windowPhysicalWidth <= 0 && mode.width >= 640 && mode.bitsPerPixel == 32 && mode.isValid())
            {
                m_windowFullScreen = true;
                m_windowPhysicalWidth = mode.width;
                m_windowPhysicalHeight = mode.height;
                m_windowBPP = mode.bitsPerPixel;
            }

            DEBUGLOG("%d x %d  %d bpp\n", mode.width, mode.height, mode.bitsPerPixel);
        }

        DEBUGLOG("\n");
    }*/

    if (m_windowPhysicalWidth <= 0)
    {
        if (desktop.width >= 640)
        {
            m_windowPhysicalWidth = 640;
            m_windowPhysicalHeight = 480;
        }
        else
        {
            m_windowPhysicalWidth = desktop.width;
            m_windowPhysicalHeight = desktop.height;
        }
        m_windowBPP = desktop.bitsPerPixel;
    }

    m_windowLogicalWidth = 640;
    m_windowLogicalHeight = (int)(640.0f / ((float)m_windowPhysicalWidth / (float)m_windowPhysicalHeight));

    m_windowScaleFactor = (float)m_windowPhysicalWidth / (float)m_windowLogicalWidth;

    DEBUGLOG("Video mode selected: %d x %d\n", m_windowPhysicalWidth, m_windowPhysicalHeight);
    DEBUGLOG("Logical screen: %d x %d\n", m_windowLogicalWidth, m_windowLogicalHeight);
    DEBUGLOG("Scale factor: %f\n\n", m_windowScaleFactor);
}

void Application::Input()
{
    State *pState = GetState(m_stateCurr);

    if (pState)
    {
        pState->Input();
    }
}

void Application::Update()
{
    State *pState = GetState(m_stateCurr);

    if (pState)
    {
        pState->Update();
    }

    if (m_pWindowGUI)
    {
        m_pWindowGUI->Update(0.0f);
    }
}

void Application::Render()
{
    State *pState = GetState(m_stateCurr);

    m_pWindow->clear(m_clearColor);

    if (pState)
    {
        pState->Render();
    }

    if (m_pCameraToRender && m_pCameraToRender->m_pScreenBuffer)
    {
        m_windowTexture.update((const sf::Uint8 *)m_pCameraToRender->m_pScreenBuffer);

        m_pWindow->draw(m_windowSprite);
    }

    m_sfgui.Display(*m_pWindow);

    m_pWindow->display();
}

State *Application::GetState(int type)
{
    int     a;
    State   *pState;

    for (a = 0; a < m_listStates.GetNumElements(); a ++)
    {
        pState = (State *)m_listStates.GetElement(a).p;
        if (pState)
        {
            if (pState->GetType() == type)
            {
                return pState;
            }
        }
    }

    return NULL;
}

void Application::ScreenDump(const char *pDestFile)
{
    std::string destFile;

    destFile.append(pDestFile);

    m_pWindow->capture().saveToFile(destFile);
}

bool Application::InitEngine3D()
{
    Color backgroundColor(0, 140, 190);

    DEBUGLOG("Init Engine3D: ");

    g_pEngine3D = new Engine3D(&backgroundColor, Engine3D::SHADING_TYPE_GOURAUD);
    if (g_pEngine3D)
    {
        DEBUGLOG("OK.\n\n");
    }
    else
    {
        DEBUGLOG("Error!\n");

        return false;
    }

    return true;
}
