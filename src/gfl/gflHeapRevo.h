#pragma once

#include <revolution/MEM.h>
#include "types.h"

namespace gfl {

class HeapRevo {
public:
    HeapRevo(const char* pHeapName, u8 heapID, size_t size, u16 optFlag, uint heapType);
    
    HeapRevo(const char* pHeapName, u8 heapID);

    virtual ~HeapRevo();

    void Init(size_t size, u16 optFlag);
    void Reset();
    
    /* 0x08 */ virtual void*    Alloc(size_t size);
    /* 0x0C */ virtual void*    Alloc(size_t size, int alignment);
    /* 0x10 */ virtual void     Free();
    /* 0x14 */ virtual size_t   GetTotalFreeSizeForExpHeap();
    /* 0x18 */ virtual size_t   GetExpHeapSize();
    /* 0x1C */ virtual bool     WithinRange(void* pData);
    /* 0x20 */ virtual int      vf20(/* arguments unknown */);
    /* 0x24 */ virtual MEMiHeapHead* GetExpHeap();
    
};

}
