#include "ValuesManager.h"

ValuesManager *g_pValuesManager = NULL;

ValuesManager::ValuesManager()
{
    m_stringsTags.Init(1, 1);
    m_stringsValues.Init(1, 1);

    m_intsTags.Init(1, 1);
    m_intsValues.Init(1, 1);
}

ValuesManager::~ValuesManager()
{
    ClearAll();
}

bool ValuesManager::AddStringValue(char *pTag, char *pValue)
{
    int     a;
    char    *pTagTemp;
    Element e1;
    Element e2;

    if (!pTag || strlen(pTag) <= 0 || !pValue || strlen(pValue) <= 0)
    {
        return false;
    }

    for (a = 0; a < m_stringsTags.GetNumElements(); a ++)
    {
        pTagTemp = (char *)m_stringsTags.GetElement(a).p;
        if (pTagTemp)
        {
            if (_strcasecmp(pTag, pTagTemp))
            {
                return false;
            }
        }
    }

    e1.p = pTag;
    e2.p = pValue;

    if (m_stringsTags.AddElement(e1) >= 0 && m_stringsValues.AddElement(e2) >= 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool ValuesManager::AddIntValue(char *pTag, int value)
{
    int     a;
    char    *pTagTemp;
    Element e1;
    Element e2;

    if (!pTag || strlen(pTag) <= 0)
    {
        return false;
    }

    for (a = 0; a < m_intsTags.GetNumElements(); a ++)
    {
        pTagTemp = (char *)m_intsTags.GetElement(a).p;
        if (pTagTemp)
        {
            if (_strcasecmp(pTag, pTagTemp))
            {
                return false;
            }
        }
    }

    e1.p = pTag;
    e2.i = value;

    if (m_intsTags.AddElement(e1) >= 0 && m_intsValues.AddElement(e2) >= 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool ValuesManager::RemoveStringValue(char *pTag)
{
    int     a;
    char    *pTagTemp;

    if (!pTag || strlen(pTag) <= 0)
    {
        return false;
    }

    for (a = 0; a < m_stringsTags.GetNumElements(); a ++)
    {
        pTagTemp = (char *)m_stringsTags.GetElement(a).p;
        if (pTagTemp)
        {
            if (_strcasecmp(pTag, pTagTemp))
            {
                if (m_stringsTags.RemoveElement(a) && m_stringsValues.RemoveElement(a))
                {
                    return true;
                }
                else
                {
                    return false;
                }
            }
        }
    }

    return false;
}

bool ValuesManager::RemoveIntValue(char *pTag)
{
    int     a;
    char    *pTagTemp;

    if (!pTag || strlen(pTag) <= 0)
    {
        return false;
    }

    for (a = 0; a < m_intsTags.GetNumElements(); a ++)
    {
        pTagTemp = (char *)m_intsTags.GetElement(a).p;
        if (pTagTemp)
        {
            if (_strcasecmp(pTag, pTagTemp))
            {
                if (m_intsTags.RemoveElement(a) && m_intsValues.RemoveElement(a))
                {
                    return true;
                }
                else
                {
                    return false;
                }
            }
        }
    }

    return false;
}

void ValuesManager::ClearAll()
{
    /*u32     a;
    char    *pTag;

    for (a = 0; a < m_stringsTags.GetNumElements(); a ++)
    {
        pTag = (char *)m_stringsTags.GetElement(a);
        if (pTag)
        {
            free(pTag);
        }
    }

    for (a = 0; a < m_stringsValues.GetNumElements(); a ++)
    {
        pTag = (char *)m_stringsValues.GetElement(a);
        if (pTag)
        {
            free(pTag);
        }
    }

    for (a = 0; a < m_intsTags.GetNumElements(); a ++)
    {
        pTag = (char *)m_intsTags.GetElement(a);
        if (pTag)
        {
            free(pTag);
        }
    }*/

    m_stringsTags.RemoveAllElements();
    m_stringsValues.RemoveAllElements();

    m_intsTags.RemoveAllElements();
    m_intsValues.RemoveAllElements();
}

bool ValuesManager::SetStringValue(char *pTag, char *pValue)
{
    int     a;
    char    *pTagTemp;
    Element e;

    if (!pTag || strlen(pTag) <= 0 || !pValue || strlen(pValue) <= 0)
    {
        return false;
    }

    for (a = 0; a < m_stringsTags.GetNumElements(); a ++)
    {
        pTagTemp = (char *)m_stringsTags.GetElement(a).p;
        if (pTagTemp)
        {
            if (_strcasecmp(pTag, pTagTemp))
            {
                e.p = pValue;

                return m_stringsValues.SetElement(e, a);
            }
        }
    }

    return false;
}

bool ValuesManager::SetIntValue(char *pTag, int value)
{
    int     a;
    char    *pTagTemp;
    Element e;

    if (!pTag || strlen(pTag) <= 0)
    {
        return false;
    }

    for (a = 0; a < m_intsTags.GetNumElements(); a ++)
    {
        pTagTemp = (char *)m_intsTags.GetElement(a).p;
        if (pTagTemp)
        {
            if (_strcasecmp(pTag, pTagTemp))
            {
                e.i = value;

                return m_intsValues.SetElement(e, a);
            }
        }
    }

    return false;
}

char *ValuesManager::GetStringTag(const char *pValue)
{
    int     a;
    char    *pValueTemp;

    if (!pValue || strlen(pValue) <= 0)
    {
        return NULL;
    }

    for (a = 0; a < m_stringsValues.GetNumElements(); a ++)
    {
        pValueTemp = (char *)m_stringsValues.GetElement(a).p;
        if (pValueTemp)
        {
            if (_strcasecmp(pValue, pValueTemp))
            {
                return (char *)m_stringsTags.GetElement(a).p;
            }
        }
    }

    return NULL;
}

char *ValuesManager::GetStringValue(const char *pTag)
{
    int     a;
    char    *pTagTemp;

    if (!pTag || strlen(pTag) <= 0)
    {
        return NULL;
    }

    for (a = 0; a < m_stringsTags.GetNumElements(); a ++)
    {
        pTagTemp = (char *)m_stringsTags.GetElement(a).p;
        if (pTagTemp)
        {
            if (_strcasecmp(pTag, pTagTemp))
            {
                return (char *)m_stringsValues.GetElement(a).p;
            }
        }
    }

    return NULL;
}

char *ValuesManager::GetIntTag(int value)
{
    int a;
    int valueTemp;

    for (a = 0; a < m_intsValues.GetNumElements(); a ++)
    {
        valueTemp = m_intsValues.GetElement(a).i;
        if (value == valueTemp)
        {
            return (char *)m_intsTags.GetElement(a).p;
        }
    }

    return NULL;
}

int ValuesManager::GetIntValue(const char *pTag)
{
    int     a;
    char    *pTagTemp;

    if (!pTag || strlen(pTag) <= 0)
    {
        return 0;
    }

    for (a = 0; a < m_intsTags.GetNumElements(); a ++)
    {
        pTagTemp = (char *)m_intsTags.GetElement(a).p;
        if (pTagTemp)
        {
            if (_strcasecmp(pTag, pTagTemp))
            {
                return m_intsValues.GetElement(a).i;
            }
        }
    }

    return 0;
}

void ValuesManager::ToString()
{
    int a;

    for (a = 0; a < m_stringsTags.GetNumElements(); a ++)
    {
        DEBUGLOG("%s : %s\n", (char *)m_stringsTags.GetElement(a).p, (char *)m_stringsValues.GetElement(a).p);
    }

    DEBUGLOG("\n");

    for (a = 0; a < m_intsTags.GetNumElements(); a ++)
    {
        DEBUGLOG("%s : %d\n", (char *)m_intsTags.GetElement(a).p, (int)m_intsValues.GetElement(a).i);
    }

    DEBUGLOG("\n");
}
