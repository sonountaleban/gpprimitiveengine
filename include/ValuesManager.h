#ifndef _VALUES_MANAGER_H_
#define _VALUES_MANAGER_H_

#include "List.h"

class ValuesManager
{
public:
    ValuesManager();
    ~ValuesManager();

    bool    AddStringValue(char *pTag, char *pValue);

    bool    AddIntValue(char *pTag, int value);

    bool    RemoveStringValue(char *pTag);

    bool    RemoveIntValue(char *pTag);

    void    ClearAll();

    bool    SetStringValue(char *pTag, char *pValue);

    bool    SetIntValue(char *pTag, int value);

    char    *GetStringTag(const char *pValue);
    char    *GetStringValue(const char *pTag);

    char    *GetIntTag(int value);
    int     GetIntValue(const char *pTag);

    void    ToString();

private:
    List    m_stringsTags;
    List    m_stringsValues;

    List    m_intsTags;
    List    m_intsValues;
};

extern ValuesManager *g_pValuesManager;

#endif
