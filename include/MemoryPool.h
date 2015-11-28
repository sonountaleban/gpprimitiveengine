#ifndef _MEMORY_POOL_H_
#define _MEMORY_POOL_H_

class MemoryPool
{
public:
    MemoryPool(int size);
    ~MemoryPool();

    char    *Allocate(int size, int align);

    void    Reset();

    int     GetFreeBytes();
    int     GetOriginalSize();

private:
    char    *m_pBuffer;
    char    *m_pNextAddr;
    int     m_freeBytes;
    int     m_originalSize;
};

#endif
