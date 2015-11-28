#ifndef _STATE_MAIN_H_
#define _STATE_MAIN_H_

#include "State.h"
#include "List.h"
#include "Engine3D/Camera.h"
#include "Engine3D/Object3D.h"

class StateMain : public State
{
public:
    StateMain();
    ~StateMain();

    void                    Start();
    void                    Input();
    void                    Update();
    void                    Render();
    void                    End();

private:
    int                     m_tick;

    Camera                  *m_pCamera;

    Light::Types            m_lightType;
    float                   m_lightAngleY;
    Light                   *m_pLightAmbient;
    Light                   *m_pLightDirectional;
    Light                   *m_pLightPoint;
    Light                   *m_pLightSpot;

    int                     m_firstVertexPoints;
    List                    m_listVertexes;
    List                    m_listPoints3D;
    List                    m_listLines3D;
    List                    m_listTriangles3D;

    Object3D                *m_pShip;

    int                     m_mouseStartPosX;
    int                     m_mouseStartPosY;

    sfg::RadioButton::Ptr   m_pRadioButtonFlatShading;
    sfg::RadioButton::Ptr   m_pRadioButtonGouraudShading;
    sfg::RadioButton::Ptr   m_pRadioButtonOnlyAmbientLight;
    sfg::RadioButton::Ptr   m_pRadioButtonDirectionalLight;
    sfg::RadioButton::Ptr   m_pRadioButtonPointLight;
    sfg::RadioButton::Ptr   m_pRadioButtonSpotLight;

    // ------------------------------------------------------------------

    void                    ButtonsSelected();
};

#endif
