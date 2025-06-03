#include "gflHeapRevo.h"
#include "gflMemoryManagement.h"
#include "gflMemoryRevo.h"
#include "revolution/MEM/mem_allocator.h"

using namespace gfl;

HeapRevo::HeapRevo(const char* pHeapName, u8 heapID, size_t size, u16 optFlag, uint memType) {
    mHeapID = heapID;
    SetName(pHeapName);
    Init(size, optFlag, memType);

    MemoryRevo::GetInstance()->SetHeap(this);
}

// Nonmatching: https://decomp.me/scratch/lUfx0
HeapRevo::HeapRevo(const char* pHeapName, u8 heapID) {
    mMemType = 0;
    mHeapID = heapID;
    SetName(pHeapName);
    mExpHeap = nullptr;
    
    MemoryRevo::GetInstance()->SetHeap(this);
}

HeapRevo::~HeapRevo() {
    Reset();

    MemoryRevo::GetInstance()->ClearHeap(this);
}

// Nonmatching: https://decomp.me/scratch/HkTCy
void HeapRevo::Init(size_t size, u16 optFlag, s32 memType) {
    mMemType = memType;

    MEMiHeapHead* expHeap = MEMCreateExpHeapEx(GetArena(mMemType, &size), size, optFlag);

    mUseArenaHi = false;
    mExpHeap = expHeap;

    SetMEMAllocatorParameters(&mAllocator1, 0x20);
    SetMEMAllocatorParameters(&mAllocator2, 0x10);
}

void HeapRevo::Reset() {
    bool useArenaHi;
    s32 memType;
    void* end;

    MEMiHeapHead* expHeap = mExpHeap;
    if (expHeap != nullptr) {
        end = expHeap->end;
        MEMDestroyExpHeap(expHeap);

        useArenaHi = mUseArenaHi;
        memType = mMemType;

        SetArena(memType, expHeap, end, useArenaHi);

        mExpHeap = nullptr;
        mMemType = 0;
    }
}

void* HeapRevo::Alloc(size_t size) {
    return MEMAllocFromExpHeapEx(mExpHeap, size, 4);
}

void* HeapRevo::Alloc(size_t size, s32 alignment) {
    return MEMAllocFromExpHeapEx(mExpHeap, size, alignment);
}

void HeapRevo::Free(void* pData) {
    MEMFreeToExpHeap(mExpHeap, pData);
}

size_t HeapRevo::GetTotalFreeSizeForExpHeap() {
    return (size_t)MEMGetHeapTotalSize(mExpHeap);
}

size_t HeapRevo::GetExpHeapSize() {
    return (size_t)mExpHeap->end - (size_t)mExpHeap->start;
}

s32 HeapRevo::vf20(/* args unknown */) {
    return 1;
}

// Nonmatching
bool HeapRevo::WithinRange(void* pData) {
    void* end = mExpHeap->end;

    return pData >= mExpHeap && pData <= end;
}

MEMiHeapHead* HeapRevo::GetExpHeap() {
    return mExpHeap;
}

void* HeapRevo::GetExpHeapEnd() {
    return mExpHeap->end;
}

// Nonmatching
void* HeapRevo::GetArenaLo(s32 memType) {
    void* arena;

    switch (memType) {
        case 1: {
            arena = OSGetMEM1ArenaLo();
            break;
        }

        case 2: {
            arena = OSGetMEM2ArenaLo();
            break;
        }

        default: {
            arena = nullptr;
            break;
        }

    }

    return arena;
}

// Nonmatching
void* HeapRevo::GetArenaHi(s32 memType) {
    void* arena;

    switch (memType) {
        case 1: {
            arena = OSGetArenaHi();
            break;
        }

        case 2: {
            arena = OSGetMEM2ArenaHi();
            break;
        }

        default: {
            arena = nullptr;
            break;
        }
    }

    return arena;
}

// Nonmatching
void HeapRevo::SetArenaLo(s32 memType, void* pBoundary) {
    switch (memType) {
        case 1: {
            OSSetMEM1ArenaLo(pBoundary);
            break;
        }

        case 2: {
            OSSetMEM2ArenaLo(pBoundary);
            break;
        }
    }
}

// Nonmatching
void HeapRevo::SetArenaHi(s32 memType, void* pBoundary) {
    switch (memType) {
        case 1: {
            OSSetMEM1ArenaHi(pBoundary);
            break;
        }

        case 2: {
            OSSetMEM2ArenaHi(pBoundary);
            break;
        }
    }
}

size_t HeapRevo::GetArenaSize(s32 memType) {
    return (size_t)GetArenaHi(memType) - (size_t)GetArenaLo(memType);
}

// Nonmatching
void* HeapRevo::GetArena(s32 memType, size_t* pSize) {
    void* arenaLo = GetArenaLo(memType);
    void* arenaHi = GetArenaHi(memType);
    void* start;

    if (*pSize == 0xFFFFFFFF) { 
        *pSize = (u32)arenaHi - (u32)arenaLo;
        start = arenaHi;
    } else {
        arenaHi = (void*)((u32)arenaLo + *pSize);
        start = arenaHi;
    }

    SetArenaLo(memType, start);

    return arenaLo;
}

void HeapRevo::SetArena(s32 memType, void* pStart, void* pEnd, bool useArenaHi) {
    void* arenaLo = GetArenaLo(memType);
    void* arenaHi = GetArenaHi(memType);

    if (useArenaHi) {
        if ((u32)pStart == (u32)arenaHi + 1) {
            SetArenaHi(memType, (void*)((u32)pEnd - 1));
        }
    } else if (pEnd == arenaLo) {
        SetArenaLo(memType, pStart);
    }
}

void HeapRevo::SetMEMAllocatorParameters(MEMAllocator* pAllocator, s32 alignment) {
    AllocFuncs.allocFunc = &HeapAlloc;
    AllocFuncs.freeFunc = &HeapFree;

    pAllocator->funcs = &AllocFuncs;
    pAllocator->heap = (MEMiHeapHead*)(this);
    pAllocator->heapParam1 = alignment;
    pAllocator->heapParam2 = 0;
}

void* HeapRevo::HeapAlloc(MEMAllocator *pAllocator, size_t size) {
    return gfl::Alloc((HeapRevo*)pAllocator->heap, size, pAllocator->heapParam1);
}

void HeapRevo::HeapFree(MEMAllocator* pAllocator, void* pData) {
    gfl::Free(pData);
}
