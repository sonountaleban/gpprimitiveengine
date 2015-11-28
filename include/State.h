#ifndef _STATE_H_
#define _STATE_H_

enum StateTypes
{
    STATE_TYPE_NONE = -1,
    STATE_TYPE_MAIN,

    NUM_STATE_TYPES
};

#include "utils.h"

class State
{
public:
    virtual ~State();

    virtual void        Start() = 0;
    virtual void        Input() = 0;
    virtual void        Update() = 0;
    virtual void        Render() = 0;
    virtual void        End() = 0;

    virtual StateTypes  GetType();

protected:
    StateTypes          m_type;
};

#endif
