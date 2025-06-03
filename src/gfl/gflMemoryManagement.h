#pragma once

#include <revolution/OS.h>
#include "types.h"

namespace gfl {
    class HeapRevo;

    ENUM_CLASS(HeapID,
        None        = 0,
        LIB1        = 1,
        LIB2        = 2,
        String      = 3,
        ETC         = 4,
        Sound       = 5,

        Work        = 10,
        Resource    = 11,
    );

    void* Alloc(u8 heapID, size_t size, uint align);
    void* Alloc(HeapRevo* pHeap, size_t size, uint align);
    void Free(void*);
}
