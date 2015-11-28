#include "StateMain.h"
#include "utils.h"
#include "Application.h"
#include "TexturesManager.h"
#include "Engine3D/Engine3D.h"
#include "Engine3D/Color.h"
#include "Engine3D/Line3D.h"
#include "Engine3D/Triangle3D.h"
#include "Engine3D/Point3D.h"
#include "Engine3D/ExtVector3D.h"

StateMain::StateMain()
{
    m_type = STATE_TYPE_MAIN;
}

StateMain::~StateMain()
{
}

void StateMain::Start()
{
    int             a;
    int             x;
    int             z;
    Color           white(255, 255, 255);
    Color           blue(0, 0, 255);
    Color           red(255, 0, 0);
    Color           yellow(255, 255, 0);
    Color           gray(128, 128, 160);
    Color           brightYellow(255, 255, 190);
    Element         e;
    sfg::Box::Ptr   pBox;

#ifdef _DEBUG
    DEBUGLOG("StateMain::Start()\n\n");
#endif

    m_tick = 0;

    m_mouseStartPosX = -1;
    m_mouseStartPosY = -1;

    g_pApplication->SetClearColor(25, 128, 240, 255);

    m_pCamera = new Camera(230.0f, 0.0f, -100.0f, 180.0f, 215.0f, 180.0f);
    //m_pCamera = new Camera(71.0f, 11.8f, 151.9f, 180.0f, 240.0f, 180.0f);
    if (m_pCamera)
    {
        m_pCamera->SetScreen(60, g_pApplication->GetWindowLogicalWidth(), g_pApplication->GetWindowLogicalHeight(), 5.0f, 1000.0f);
    }

    g_pEngine3D->Reset();
    g_pEngine3D->SetCamera(m_pCamera);

    g_pApplication->SetCameraToRender(m_pCamera);

    e.p = new ExtVector3D(-50.0f, -50.0f, 150.0f, &white,
                          g_pTexturesManager->GetTextureSheetFrame(TexturesManager::TEXTURE_TEX01, TexturesManager::TEXTURE_SHEET_TEX01_PICTURE0)->u[0],
                          g_pTexturesManager->GetTextureSheetFrame(TexturesManager::TEXTURE_TEX01, TexturesManager::TEXTURE_SHEET_TEX01_PICTURE0)->v[0]);
    m_listVertexes.AddElement(e);

    e.p = new ExtVector3D(50.0f, -50.0f, 150.0f, &white,
                          g_pTexturesManager->GetTextureSheetFrame(TexturesManager::TEXTURE_TEX01, TexturesManager::TEXTURE_SHEET_TEX01_PICTURE0)->u[1],
                          g_pTexturesManager->GetTextureSheetFrame(TexturesManager::TEXTURE_TEX01, TexturesManager::TEXTURE_SHEET_TEX01_PICTURE0)->v[1]);
    m_listVertexes.AddElement(e);

    e.p = new ExtVector3D(50.0f, 50.0f, 150.0f, &white,
                          g_pTexturesManager->GetTextureSheetFrame(TexturesManager::TEXTURE_TEX01, TexturesManager::TEXTURE_SHEET_TEX01_PICTURE0)->u[2],
                          g_pTexturesManager->GetTextureSheetFrame(TexturesManager::TEXTURE_TEX01, TexturesManager::TEXTURE_SHEET_TEX01_PICTURE0)->v[2]);
    m_listVertexes.AddElement(e);

    e.p = new ExtVector3D(-50.0f, 50.0f, 150.0f, &white,
                          g_pTexturesManager->GetTextureSheetFrame(TexturesManager::TEXTURE_TEX01, TexturesManager::TEXTURE_SHEET_TEX01_PICTURE0)->u[3],
                          g_pTexturesManager->GetTextureSheetFrame(TexturesManager::TEXTURE_TEX01, TexturesManager::TEXTURE_SHEET_TEX01_PICTURE0)->v[3]);
    m_listVertexes.AddElement(e);

    e.p = new ExtVector3D(-50.0f, -50.0f, 250.0f, &red);
    m_listVertexes.AddElement(e);

    e.p = new ExtVector3D(50.0f, -50.0f, 250.0f, &red);
    m_listVertexes.AddElement(e);

    e.p = new ExtVector3D(50.0f, 50.0f, 250.0f, &red);
    m_listVertexes.AddElement(e);

    e.p = new ExtVector3D(-50.0f, 50.0f, 250.0f, &red);
    m_listVertexes.AddElement(e);

    e.p = new ExtVector3D(0.0f, 0.0f, 100.0f, &blue);
    ((ExtVector3D *)e.p)->m_processColor = false;
    m_listVertexes.AddElement(e);

    e.p = new ExtVector3D(0.0f, 0.0f, 300.0f, &yellow);
    ((ExtVector3D *)e.p)->m_processColor = false;
    m_listVertexes.AddElement(e);

    m_firstVertexPoints = m_listVertexes.GetNumElements();

    for (x = -200.0f; x <= 200.0f; x += 10.0f)
    {
        for (z = 0.0f; z <= 600.0f; z += 10.0f)
        {
            e.p = new ExtVector3D(x, 50.0f, z, &white);
            ((ExtVector3D *)e.p)->m_processColor = false;
            m_listVertexes.AddElement(e);
        }
    }

    /*for (x = -200.0f; x <= 200.0f; x += 10.0f)
    {
        for (z = 0.0f; z <= 600.0f; z += 10.0f)
        {
            e.p = new ExtVector3D(x, -50.0f, z, &white);
            ((ExtVector3D *)e.p)->m_processColor = false;
            m_listVertexes.AddElement(e);
        }
    }*/

    for (a = m_firstVertexPoints; a < m_listVertexes.GetNumElements(); a ++)
    {
        e.p = new Point3D(a);
        m_listPoints3D.AddElement(e);
    }

    e.p = new Line3D(8, 9);
    m_listLines3D.AddElement(e);

    e.p = new Triangle3D(TexturesManager::TEXTURE_NONE, 0, 4, 1);
    ((Triangle3D *)e.p)->m_culling = false;
    m_listTriangles3D.AddElement(e);

    e.p = new Triangle3D(TexturesManager::TEXTURE_NONE, 5, 1, 4);
    ((Triangle3D *)e.p)->m_culling = false;
    m_listTriangles3D.AddElement(e);

    e.p = new Triangle3D(TexturesManager::TEXTURE_NONE, 1, 5, 6);
    ((Triangle3D *)e.p)->m_culling = false;
    m_listTriangles3D.AddElement(e);

    e.p = new Triangle3D(TexturesManager::TEXTURE_NONE, 1, 6, 2);
    ((Triangle3D *)e.p)->m_culling = false;
    m_listTriangles3D.AddElement(e);

    e.p = new Triangle3D(TexturesManager::TEXTURE_NONE, 4, 0, 3);
    ((Triangle3D *)e.p)->m_culling = false;
    m_listTriangles3D.AddElement(e);

    e.p = new Triangle3D(TexturesManager::TEXTURE_NONE, 4, 3, 7);
    ((Triangle3D *)e.p)->m_culling = false;
    m_listTriangles3D.AddElement(e);

    e.p = new Triangle3D(TexturesManager::TEXTURE_NONE, 5, 4, 7);
    ((Triangle3D *)e.p)->m_culling = false;
    m_listTriangles3D.AddElement(e);

    e.p = new Triangle3D(TexturesManager::TEXTURE_NONE, 5, 7, 6);
    ((Triangle3D *)e.p)->m_culling = false;
    m_listTriangles3D.AddElement(e);

    e.p = new Triangle3D(TexturesManager::TEXTURE_NONE, 3, 2, 6);
    ((Triangle3D *)e.p)->m_culling = false;
    m_listTriangles3D.AddElement(e);

    e.p = new Triangle3D(TexturesManager::TEXTURE_NONE, 3, 6,7);
    ((Triangle3D *)e.p)->m_culling = false;
    m_listTriangles3D.AddElement(e);

    e.p = new Triangle3D(TexturesManager::TEXTURE_TEX01, 0, 1, 3);
    m_listTriangles3D.AddElement(e);

    e.p = new Triangle3D(TexturesManager::TEXTURE_TEX01, 2, 3, 1);
    m_listTriangles3D.AddElement(e);

    m_lightType = Light::TYPE_DIRECTIONAL;
    m_lightAngleY = 0.0f;

    m_pLightAmbient = new Light(&gray);
    g_pEngine3D->AddLight(m_pLightAmbient);

    m_pLightDirectional = new Light(&brightYellow, 180.0f, m_lightAngleY);
    g_pEngine3D->AddLight(m_pLightDirectional);

    m_pLightPoint = new Light(&brightYellow, 0.0f, 0.0f, 0.0f);

    m_pLightSpot = new Light(&brightYellow, 0.0f, 0.0f, 0.0f, 180.0f, 0.0f);

    m_pShip = new Object3D();
    if (m_pShip)
    {
        m_pShip->m_scale.Set(30.0f, 30.0f, 30.0f);

        m_pShip->UpdateMatrix();

        m_pShip->Load3DS("ship");
    }

    g_pApplication->GetWindowGUI()->SetTitle("Settings");

    pBox = sfg::Box::Create(sfg::Box::Orientation::VERTICAL, 5.0f);

    m_pRadioButtonFlatShading = sfg::RadioButton::Create("Flat shading");
    m_pRadioButtonGouraudShading = sfg::RadioButton::Create("Gouraud shading", m_pRadioButtonFlatShading->GetGroup());

    if (g_pEngine3D->m_shading == Engine3D::SHADING_TYPE_FLAT)
    {
        m_pRadioButtonFlatShading->SetActive(true);
    }
    else
    {
        m_pRadioButtonGouraudShading->SetActive(true);
    }

    pBox->Pack(m_pRadioButtonFlatShading);
    pBox->Pack(m_pRadioButtonGouraudShading);

    pBox->Pack(sfg::Separator::Create());

    m_pRadioButtonOnlyAmbientLight = sfg::RadioButton::Create("Only ambient light");
    m_pRadioButtonDirectionalLight = sfg::RadioButton::Create("Directional light", m_pRadioButtonOnlyAmbientLight->GetGroup());
    m_pRadioButtonPointLight = sfg::RadioButton::Create("Point light", m_pRadioButtonOnlyAmbientLight->GetGroup());
    m_pRadioButtonSpotLight = sfg::RadioButton::Create("Spot light", m_pRadioButtonOnlyAmbientLight->GetGroup());

    m_pRadioButtonDirectionalLight->SetActive(true);

    pBox->Pack(m_pRadioButtonOnlyAmbientLight);
    pBox->Pack(m_pRadioButtonDirectionalLight);
    pBox->Pack(m_pRadioButtonPointLight);
    pBox->Pack(m_pRadioButtonSpotLight);

    g_pApplication->GetWindowGUI()->Add(pBox);

    m_pRadioButtonFlatShading->GetSignal(sfg::ToggleButton::OnToggle).Connect(std::bind(&StateMain::ButtonsSelected, this));
    m_pRadioButtonGouraudShading->GetSignal(sfg::ToggleButton::OnToggle).Connect(std::bind(&StateMain::ButtonsSelected, this));

    m_pRadioButtonOnlyAmbientLight->GetSignal(sfg::ToggleButton::OnToggle).Connect(std::bind(&StateMain::ButtonsSelected, this));
    m_pRadioButtonDirectionalLight->GetSignal(sfg::ToggleButton::OnToggle).Connect(std::bind(&StateMain::ButtonsSelected, this));
    m_pRadioButtonPointLight->GetSignal(sfg::ToggleButton::OnToggle).Connect(std::bind(&StateMain::ButtonsSelected, this));
    m_pRadioButtonSpotLight->GetSignal(sfg::ToggleButton::OnToggle).Connect(std::bind(&StateMain::ButtonsSelected, this));
}

void StateMain::Input()
{
    static int      oldKeyPressed = sf::Keyboard::Unknown;
    int             newKeyPressed = sf::Keyboard::Unknown;
    int             diffX;
    int             diffY;
    int             mouseNewPosX;
    int             mouseNewPosY;
    sf::Vector2i    localPosition = g_pApplication->GetMousePosition();

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
    {
        newKeyPressed = sf::Keyboard::Up;

        m_pCamera->Move(Camera::MOVEMENT_TYPE_FORWARD, 5.0f);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
    {
        newKeyPressed = sf::Keyboard::Left;

        m_pCamera->Move(Camera::MOVEMENT_TYPE_LEFT, 5.0f);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
    {
        newKeyPressed = sf::Keyboard::Down;

        m_pCamera->Move(Camera::MOVEMENT_TYPE_BACK, 5.0f);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
    {
        newKeyPressed = sf::Keyboard::Right;

        m_pCamera->Move(Camera::MOVEMENT_TYPE_RIGHT, 5.0f);
    }

    /*if (sf::Keyboard::isKeyPressed(sf::Keyboard::F1))
    {
        newKeyPressed = sf::Keyboard::F1;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::F2))
    {
        newKeyPressed = sf::Keyboard::F2;
    }*/

    if (newKeyPressed == sf::Keyboard::Unknown)
    {
        /*if (oldKeyPressed == sf::Keyboard::F1)
        {
            if (g_pEngine3D->m_shading == Engine3D::SHADING_TYPE_FLAT)
            {
                g_pEngine3D->m_shading = Engine3D::SHADING_TYPE_GOURAUD;
            }
            else if (g_pEngine3D->m_shading == Engine3D::SHADING_TYPE_GOURAUD)
            {
                g_pEngine3D->m_shading = Engine3D::SHADING_TYPE_FLAT;
            }
        }
        else if (oldKeyPressed == sf::Keyboard::F2)
        {
            if (m_lightType == Light::TYPE_NONE)
            {
                m_lightType = Light::TYPE_DIRECTIONAL;

                g_pEngine3D->AddLight(m_pLightDirectional);

                //g_pEngine3D->AddPrimitive3D((Line3D *)m_listLines3D.GetElement(0).p);
            }
            else if (m_lightType == Light::TYPE_DIRECTIONAL)
            {
                m_lightType = Light::TYPE_POINT;

                g_pEngine3D->AddLight(m_pLightPoint);
            }
            else if (m_lightType == Light::TYPE_POINT)
            {
                m_lightType = Light::TYPE_SPOT;

                g_pEngine3D->AddLight(m_pLightSpot);
            }
            else if (m_lightType == Light::TYPE_SPOT)
            {
                m_lightType = Light::TYPE_NONE;

                g_pEngine3D->AddLight(NULL);

                //g_pEngine3D->RemovePrimitive3D((Line3D *)m_listLines3D.GetElement(0).p);
            }
        }*/

        oldKeyPressed = sf::Keyboard::Unknown;
    }
    else
    {
        oldKeyPressed = newKeyPressed;
    }

    if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && m_mouseStartPosX >= 0 && m_mouseStartPosY >= 0)
    {
        mouseNewPosX = localPosition.x;
        mouseNewPosY = localPosition.y;

        diffX = m_mouseStartPosY - mouseNewPosY;
        diffY = m_mouseStartPosX - mouseNewPosX;

        m_mouseStartPosX = mouseNewPosX;
        m_mouseStartPosY = mouseNewPosY;

        m_pCamera->Rotate(90.0f / (float)g_pApplication->GetWindowPhysicalWidth() * (float)diffX,
                          90.0f / (float)g_pApplication->GetWindowPhysicalHeight() * (float)diffY,
                          0.0f);
    }
    else
    {
        m_mouseStartPosX = localPosition.x;
        m_mouseStartPosY = localPosition.y;
    }
}

void StateMain::Update()
{
    static Vector3D pos1(0.0f, -20.0f, 0.0f);
    static Vector3D pos2(200.0f, 0.0f, 200.0f);
    static Vector3D angle1(0.0f, 0.0f, 0.0f);
    static Vector3D angle2(0.0f, 0.0f, 0.0f);
    int             a;
    Element         e;

    m_tick ++;

    g_pEngine3D->ClearListVertexes();
    g_pEngine3D->ClearListPrimitives();

    for (a = 0; a < m_listVertexes.GetNumElements(); a ++)
    {
        e = m_listVertexes.GetElement(a);
        if (e.p)
        {
            g_pEngine3D->AddVertex3D((ExtVector3D *)e.p);
        }
    }

    for (a = 0; a < m_listPoints3D.GetNumElements(); a ++)
    {
        e = m_listPoints3D.GetElement(a);
        if (e.p)
        {
            g_pEngine3D->AddPrimitive3D((Point3D *)e.p);
        }
    }

    for (a = 0; a < m_listLines3D.GetNumElements(); a ++)
    {
        e = m_listLines3D.GetElement(a);
        if (e.p)
        {
            g_pEngine3D->AddPrimitive3D((Line3D *)e.p);
        }
    }

    for (a = 0; a < m_listTriangles3D.GetNumElements(); a ++)
    {
        e = m_listTriangles3D.GetElement(a);
        if (e.p)
        {
            g_pEngine3D->AddPrimitive3D((Triangle3D *)e.p);
        }
    }

    if (m_lightType == Light::TYPE_DIRECTIONAL)
    {
        m_lightAngleY += 1.0f;

        m_pLightDirectional->m_angle.m_y = m_lightAngleY;
        m_pLightDirectional->UpdateAngulation();

        e = m_listVertexes.GetElement(8);

        ((ExtVector3D *)e.p)->m_pos.Set(m_pLightDirectional->m_vectorRot.m_x * 150.0f,
                                        m_pLightDirectional->m_vectorRot.m_y * 150.0f,
                                        m_pLightDirectional->m_vectorRot.m_z * 150.0f + 200.0f);

        e = m_listVertexes.GetElement(9);

        ((ExtVector3D *)e.p)->m_pos.Set(m_pLightDirectional->m_vectorRot.m_x * 130.0f,
                                        m_pLightDirectional->m_vectorRot.m_y * 130.0f,
                                        m_pLightDirectional->m_vectorRot.m_z * 130.0f + 200.0f);
    }
    else if (m_lightType == Light::TYPE_POINT)
    {
        m_lightAngleY += 1.0f;

        m_pLightPoint->m_pos.m_x = sinf(TO_RADIANS(m_lightAngleY + 180.0f)) * 140.0f;
        m_pLightPoint->m_pos.m_y = 0.0f;
        m_pLightPoint->m_pos.m_z = cosf(TO_RADIANS(m_lightAngleY + 180.0f)) * 140.0f + 200.0f;

        e = m_listVertexes.GetElement(8);

        ((ExtVector3D *)e.p)->m_pos.Set(m_pLightPoint->m_pos.m_x,
                                        m_pLightPoint->m_pos.m_y - 10.0f,
                                        m_pLightPoint->m_pos.m_z);

        e = m_listVertexes.GetElement(9);

        ((ExtVector3D *)e.p)->m_pos.Set(m_pLightPoint->m_pos.m_x,
                                        m_pLightPoint->m_pos.m_y + 10.0f,
                                         m_pLightPoint->m_pos.m_z);
    }
    else if (m_lightType == Light::TYPE_SPOT)
    {
        m_lightAngleY += 1.0f;

        m_pLightSpot->m_pos.m_x = sinf(TO_RADIANS(m_lightAngleY + 180.0f)) * 140.0f;
        m_pLightSpot->m_pos.m_y = 0.0f;
        m_pLightSpot->m_pos.m_z = cosf(TO_RADIANS(m_lightAngleY + 180.0f)) * 140.0f + 200.0f;

        e = m_listVertexes.GetElement(8);

        ((ExtVector3D *)e.p)->m_pos.Set(m_pLightSpot->m_pos.m_x,
                                        m_pLightSpot->m_pos.m_y,
                                        m_pLightSpot->m_pos.m_z);

        e = m_listVertexes.GetElement(9);

        ((ExtVector3D *)e.p)->m_pos.Set(m_pLightSpot->m_pos.m_x + m_pLightSpot->m_vectorRot.m_x * -20.0f,
                                        m_pLightSpot->m_pos.m_y + m_pLightSpot->m_vectorRot.m_y * -20.0f,
                                        m_pLightSpot->m_pos.m_z + m_pLightSpot->m_vectorRot.m_z * -20.0f);
    }

    angle1.m_z += 1.0f;
    angle2.m_y -= 2.0f;

    m_pShip->m_pos.Set(pos1.m_x, pos1.m_y, pos1.m_z);
    m_pShip->m_angle.Set(angle1.m_x, angle1.m_y, angle1.m_z);

    m_pShip->UpdateMatrix();
    m_pShip->DestroyVertexesAndTriangles();
    m_pShip->InitVertexesAndTriangles();

    m_pShip->m_pos.Set(pos2.m_x, pos2.m_y, pos2.m_z);
    m_pShip->m_angle.Set(angle2.m_x, angle2.m_y, angle2.m_z);

    m_pShip->UpdateMatrix();
    m_pShip->InitVertexesAndTriangles();
}

void StateMain::Render()
{
    g_pEngine3D->ClearBackground();
    g_pEngine3D->ClearZBuffer();

    if (m_pShip)
    {
        m_pShip->Render();
    }

    g_pEngine3D->Render();
}

void StateMain::End()
{
    int     a;
    Element e;

#ifdef _DEBUG
    DEBUGLOG("StateMain::End()\n\n");
#endif

    if (m_pCamera)
    {
        delete m_pCamera;
        m_pCamera = NULL;
    }

    g_pEngine3D->SetCamera(NULL);

    g_pApplication->SetCameraToRender(NULL);

    if (m_pLightAmbient)
    {
        delete m_pLightAmbient;
        m_pLightAmbient = NULL;
    }

    if (m_pLightDirectional)
    {
        delete m_pLightDirectional;
        m_pLightDirectional = NULL;
    }

    if (m_pLightPoint)
    {
        delete m_pLightPoint;
        m_pLightPoint = NULL;
    }

    if (m_pLightSpot)
    {
        delete m_pLightSpot;
        m_pLightSpot = NULL;
    }

    for (a = 0; a < m_listVertexes.GetNumElements(); a ++)
    {
        e = m_listVertexes.GetElement(a);
        if (e.p)
        {
            delete (ExtVector3D *)e.p;
        }
    }
    m_listVertexes.RemoveAllElements();

    for (a = 0; a < m_listPoints3D.GetNumElements(); a ++)
    {
        e = m_listPoints3D.GetElement(a);
        if (e.p)
        {
            delete (Point3D *)e.p;
        }
    }
    m_listPoints3D.RemoveAllElements();

    for (a = 0; a < m_listLines3D.GetNumElements(); a ++)
    {
        e = m_listLines3D.GetElement(a);
        if (e.p)
        {
            delete (Line3D *)e.p;
        }
    }
    m_listLines3D.RemoveAllElements();

    for (a = 0; a < m_listTriangles3D.GetNumElements(); a ++)
    {
        e = m_listTriangles3D.GetElement(a);
        if (e.p)
        {
            delete (Triangle3D *)e.p;
        }
    }
    m_listTriangles3D.RemoveAllElements();

    if (m_pShip)
    {
        delete m_pShip;
        m_pShip = NULL;
    }

    g_pApplication->GetWindowGUI()->RemoveAll();
}

void StateMain::ButtonsSelected()
{
    if (m_pRadioButtonFlatShading->IsActive())
    {
        g_pEngine3D->m_shading = Engine3D::SHADING_TYPE_FLAT;
    }
    else if (m_pRadioButtonGouraudShading->IsActive())
    {
        g_pEngine3D->m_shading = Engine3D::SHADING_TYPE_GOURAUD;
    }

    if (m_pRadioButtonOnlyAmbientLight->IsActive())
    {
        m_lightType = Light::TYPE_NONE;

        g_pEngine3D->AddLight(NULL);

        //g_pEngine3D->RemovePrimitive3D((Line3D *)m_listLines3D.GetElement(0).p);
    }
    else if (m_pRadioButtonDirectionalLight->IsActive())
    {
        m_lightType = Light::TYPE_DIRECTIONAL;

        g_pEngine3D->AddLight(m_pLightDirectional);

        //g_pEngine3D->AddPrimitive3D((Line3D *)m_listLines3D.GetElement(0).p);
    }
    else if (m_pRadioButtonPointLight->IsActive())
    {
        m_lightType = Light::TYPE_POINT;

        g_pEngine3D->AddLight(m_pLightPoint);

        //g_pEngine3D->AddPrimitive3D((Line3D *)m_listLines3D.GetElement(0).p);
    }
    else if (m_pRadioButtonSpotLight->IsActive())
    {
        m_lightType = Light::TYPE_SPOT;

        g_pEngine3D->AddLight(m_pLightSpot);

        //g_pEngine3D->AddPrimitive3D((Line3D *)m_listLines3D.GetElement(0).p);
    }
}
