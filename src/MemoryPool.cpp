#include "MemoryPool.h"
#include "utils.h"

MemoryPool::MemoryPool(int size)
{
    if (size > 0)
    {
        m_pBuffer = new char[size];
        m_pNextAddr = m_pBuffer;
        if (m_pBuffer)
        {
            m_freeBytes = size;
            m_originalSize = size;

            return;
        }
    }

    m_pBuffer = NULL;
    m_pNextAddr = NULL;
    m_freeBytes = 0;
    m_originalSize = 0;
}

MemoryPool::~MemoryPool()
{
    if (m_pBuffer)
    {
        delete[] m_pBuffer;
    }
}

char *MemoryPool::Allocate(int size, int align)
{
    char *pNextAddr = m_pNextAddr;

    if (pNextAddr && size > 0)
    {
        if ((size & (align - 1)) != 0)
        {
            size &= ~(align - 1);
            size += align;
        }

        if (size > m_freeBytes)
        {
            DEBUGLOG("MemoryPool::Allocate -> size > m_freeBytes!\n");
        }
        else
        {
            m_pNextAddr += sizeof(char) * size;
            m_freeBytes -= sizeof(char) * size;

            return pNextAddr;
        }
    }

    return NULL;
}

void MemoryPool::Reset()
{
    m_pNextAddr = m_pBuffer;
    m_freeBytes = m_originalSize;
}

int MemoryPool::GetFreeBytes()
{
    return m_freeBytes;
}

int MemoryPool::GetOriginalSize()
{
    return m_originalSize;
}
