#pragma once

// #include "types.h"
#include "types.h"

namespace gfl {

class HeapRevo {
public:
    /**
    @note Start: 0x8001580C
    @note End:   0x80015890
    */
    HeapRevo(const char* pHeapName, u8 heapID, size_t size, u16 optFlag, uint heapType);
    HeapRevo();
};

}
