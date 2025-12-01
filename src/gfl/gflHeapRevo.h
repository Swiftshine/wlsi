#pragma once

#include <revolution/MEM.h>
#include "types.h"

namespace gfl {

class Heap { };

class HeapRevo : public Heap {
public:
    static MEMAllocatorFuncs AllocFuncs;
    
    HeapRevo(const char* pHeapName, u8 heapID, size_t size, u16 optFlag, uint heapType);
    HeapRevo(const char* pHeapName, u8 heapID);

    /* Class Methods */

    void    SetName(const char* pName);
    void    Init(size_t size, u16 optFlag, s32 memType);
    void    Reset();
    static void*    GetArenaLo(s32 memType);
    static void*    GetArenaHi(s32 memType);
    static void     SetArenaLo(s32 memType, void* pBoundary);
    static void     SetArenaHi(s32 memType, void* pBoundary);
    static size_t   GetArenaSize(s32 memType);
    static void*    GetArena(s32 memType, size_t* pSize);
    static void     SetArena(s32 memType, void* pStart, void* pEnd, bool useArenaHi);
    void    SetMEMAllocatorParameters(MEMAllocator* pAllocator, s32 alignment);
    static void*    HeapAlloc(MEMAllocator* pAllocator, size_t size);
    static void     HeapFree(MEMAllocator* pAllocator, void* pData);

    /* Virtual Methods */

    /* 0x08 */ virtual void*    Alloc(size_t size);
    /* 0x0C */ virtual void*    Alloc(size_t size, s32 alignment);
    /* 0x10 */ virtual void     Free(void* pData);
    /* 0x14 */ virtual size_t   GetTotalFreeSizeForExpHeap();
    /* 0x18 */ virtual size_t   GetExpHeapSize();
    /* 0x1C */ virtual bool     WithinRange(void* pData);
    /* 0x20 */ virtual s32      vf20(/* arguments unknown */);
    /* 0x24 */ virtual MEMiHeapHead* GetExpHeap();
    /* 0x28 */ virtual void* GetExpHeapEnd();
    /* 0x2C */ virtual ~HeapRevo();

    /* Class Members */

    /* 0x04 */ u8 mHeapID;
    /* 0x05 */ char mName[25];
    /* 0x1E */ bool mUseArenaHi;
    /* 0x20 */ MEMiHeapHead* mExpHeap;
    /* 0x24 */ MEMAllocator mAllocator1;
    /* 0x34 */ MEMAllocator mAllocator2;
    /* 0x44 */ u32 mMemType;
};

}
