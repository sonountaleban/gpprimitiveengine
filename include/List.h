#ifndef _LIST_H_
#define _LIST_H_

#include "utils.h"

union Element
{
    void    *p;
    int     i;
    char    c;
};

class List
{
public:
    List();
    ~List();

    bool            Init(int capacityCurrent, int capacityIncrement);

    int             AddElement(Element &element);
    bool            InsertElement(Element &element, int position);
    bool            SetElement(Element &element, int position);

    bool            RemoveElement(Element &element);
    bool            RemoveElement(int index);
    void            RemoveAllElements();

    int             GetElementIndex(Element &element);
    Element         &GetElement(int index);
    bool            CheckElement(Element &element);

    void            ClearAll();

    int             GetNumElements();

    int             GetCapacity();

private:
    static Element  m_nullElement;
    Element         *m_pElements;
    int             m_numElements;
    int             m_capacityCurrent;
    int             m_capacityIncrement;
};

#endif
