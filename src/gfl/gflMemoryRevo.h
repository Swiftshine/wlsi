#pragma once

namespace gfl {
    
class HeapRevo;

class MemoryRevo {
public:
    static MemoryRevo* GetInstance();
    void SetHeap(HeapRevo* pHeap);
    void ClearHeap(HeapRevo* pHeap);
};
}
