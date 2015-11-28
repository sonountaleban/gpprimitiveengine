#include "List.h"

Element List::m_nullElement;

List::List()
{
    m_nullElement.p = NULL;
    m_pElements = NULL;
    m_numElements = 0;
    m_capacityCurrent = 0;
    m_capacityIncrement = 20;
}

List::~List()
{
    ClearAll();
}

bool List::Init(int capacityCurrent, int capacityIncrement)
{
    int     a;
    Element *pNewList;

    if (capacityCurrent <= 0 || capacityIncrement <= 0)
    {
        return false;
    }

    if (m_pElements)
    {
        if (capacityCurrent == m_capacityCurrent)
        {
            return false;
        }

        pNewList = (Element *)malloc(sizeof(Element) * capacityCurrent);
        if (!pNewList)
        {
            return false;
        }
        memset(pNewList, 0, sizeof(Element) * capacityCurrent);

        if (capacityCurrent > m_capacityCurrent)
        {
            for (a = 0; a < m_capacityCurrent; a ++)
            {
                pNewList[a] = m_pElements[a];
            }
        }
        else if (capacityCurrent < m_capacityCurrent)
        {
            for (a = 0; a < capacityCurrent; a ++)
            {
                pNewList[a] = m_pElements[a];
            }
            if (m_numElements > capacityCurrent)
            {
                m_numElements = capacityCurrent;
            }
        }

        free(m_pElements);

        m_pElements = pNewList;
        m_capacityCurrent = capacityCurrent;
        m_capacityIncrement = capacityIncrement;
    }
    else
    {
        m_pElements = (Element *)malloc(sizeof(Element) * capacityCurrent);
        if (!m_pElements)
        {
            return false;
        }
        memset(m_pElements, 0, sizeof(Element) * capacityCurrent);

        m_numElements = 0;
        m_capacityCurrent = capacityCurrent;
        m_capacityIncrement = capacityIncrement;
    }

    return true;
}

int List::AddElement(Element &element)
{
    int num = m_numElements;

    if (m_pElements)
    {
        if (m_numElements >= m_capacityCurrent)
        {
            if (!Init(m_capacityCurrent + m_capacityIncrement, m_capacityIncrement))
            {
                return -1;
            }
        }
    }
    else
    {
        if (!Init(1, 1))
        {
            return -1;
        }
    }

    m_pElements[m_numElements ++] = element;

    return num;
}

bool List::InsertElement(Element &element, int position)
{
    int a;

    if (m_pElements)
    {
        if (position >= m_numElements)
        {
            if (AddElement(element) >= 0)
            {
                return true;
            }
            else
            {
                return false;
            }
        }

        if (m_numElements >= m_capacityCurrent)
        {
            if (!Init(m_capacityCurrent + m_capacityIncrement, m_capacityIncrement))
            {
                return false;
            }
        }

        for (a = m_numElements; a > position; a --)
        {
            m_pElements[a] = m_pElements[a - 1];
        }
        m_pElements[position] = element;
        m_numElements ++;
    }
    else
    {
        if (AddElement(element) < 0)
        {
            return false;
        }
    }

    return true;
}

bool List::SetElement(Element &element, int position)
{
    if (m_pElements)
    {
        if (position >= m_numElements)
        {
            if (AddElement(element) >= 0)
            {
                return true;
            }
            else
            {
                return false;
            }
        }
        else
        {
            m_pElements[position] = element;
        }
    }
    else
    {
        if (AddElement(element) < 0)
        {
            return false;
        }
    }

    return true;
}

bool List::RemoveElement(Element &element)
{
    int a;
    int index = -1;

    if (!m_pElements)
    {
        return false;
    }

    for (a = 0; a < m_numElements; a ++)
    {
        if (m_pElements[a].p == element.p)
        {
            index = a;

            break;
        }
    }

    if (index < 0)
    {
        return false;
    }

    for (a = index + 1; a < m_numElements; a ++)
    {
        m_pElements[a - 1] = m_pElements[a];
    }
    m_pElements[m_numElements - 1].p = NULL;
    m_numElements --;

    return true;
}

bool List::RemoveElement(int index)
{
    int a;

    if (!m_pElements || index >= m_numElements)
    {
        return false;
    }

    for (a = index + 1; a < m_numElements; a ++)
    {
        m_pElements[a - 1] = m_pElements[a];
    }
    m_pElements[m_numElements - 1].p = NULL;
    m_numElements --;

    return true;
}

void List::RemoveAllElements()
{
    if (m_pElements)
    {
        memset(m_pElements, 0, sizeof(Element) * m_numElements);
    }
    m_numElements = 0;
}

int List::GetElementIndex(Element &element)
{
    int a;

    if (!m_pElements)
    {
        return -1;
    }

    for (a = 0; a < m_numElements; a ++)
    {
        if (m_pElements[a].p == element.p)
        {
            return a;
        }
    }

    return -1;
}

Element &List::GetElement(int index)
{
    if (!m_pElements || index >= m_numElements)
    {
        return m_nullElement;
    }
    else
    {
        return m_pElements[index];
    }
}

bool List::CheckElement(Element &element)
{
    int a;

    if (!m_pElements)
    {
        return false;
    }

    for (a = 0; a < m_numElements; a ++)
    {
        if (m_pElements[a].p == element.p)
        {
            return true;
        }
    }

    return false;
}

void List::ClearAll()
{
    if (m_pElements)
    {
        free(m_pElements);
        m_pElements = NULL;
    }
    m_numElements = 0;
}

int List::GetNumElements()
{
    return m_numElements;
}

int List::GetCapacity()
{
    return m_capacityCurrent;
}

