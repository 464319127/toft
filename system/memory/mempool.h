// Copyright (c) 2012, Tencent Inc.
// All rights reserved.

#ifndef TOFT_SYSTEM_MEMORY_MEMPOOL_H
#define TOFT_SYSTEM_MEMORY_MEMPOOL_H

#include <assert.h>
#include <deque>
// #include "common/base/deprecate.h"
#include "toft/system/threading/spinlock.h"

namespace toft {

class MemPool
{
    struct BLOCK_HEADER;
public:
    static const size_t MIN_UNIT_SIZE = 1024;
    static const size_t MAX_UNIT_SIZE = 16 * 2048 * MIN_UNIT_SIZE;
    static const int MAX_INDEX = 16;

public:
    explicit MemPool(size_t dwMaxMemSize);

    ~MemPool();

    // ��������:             ����һ����СΪdwSize��MemUnit
    // �������:             @����1: dwSize; MemUnit�Ĵ�С;
    // ����ֵ:               MemUnit��ָ��;
    void* Allocate(size_t dwSize);

    // ��������:             �ͷ�һ��MemUnit����Դ
    // �������:             @����1: pUnit, �ڴ���ָ��;
    // ����ֵ:               ��
    void Free(void* p);

    size_t GetBlockSize(const void* ptr) const;
    size_t GetAllocatedSize() const;
    size_t GetPooledSize() const;

private:
    BLOCK_HEADER* AllocateBlock(unsigned int index);
    BLOCK_HEADER* AllocateLargeBlock(size_t size);
    void FreeBlock(BLOCK_HEADER* block);
    bool IsValidMemoryBlock(const BLOCK_HEADER* block_header);
    int GetUnitIndex(size_t dwSize);
private:
    SpinLock                m_poolMutex;
    std::deque<BLOCK_HEADER*>   m_memPool[MAX_INDEX];
    size_t m_dwQueueNum;
    size_t m_allocated_memory_size;
    size_t m_pooled_memory_size;
    size_t m_dwAllocateCount;
    size_t m_dwFreeCount;
    size_t m_dwNewCount;
    size_t m_dwDeleteCount;
};

// DEPRECATED_BY(MemPool::MIN_UNIT_SIZE)
const size_t MIN_MEMUNIT_SIZE = MemPool::MIN_UNIT_SIZE;

// DEPRECATED_BY(MemPool::MAX_UNIT_SIZE)
const size_t MAX_MEMUNIT_SIZE = MemPool::MAX_UNIT_SIZE;

// DEPRECATED_BY(MemPool::MAX_INDEX_NUM)
const int MAX_INDEX_NUM = MemPool::MAX_INDEX;

//////////////////////////////////////////////////////////////////////////
// ��ʼ�� MemPool Ĭ��ʵ��
// ���أ��ڼ��α���ʼ��
// ע�⣺ֻ�е�һ��ʱ�������Ż��������õ���
int MemPool_Initialize(size_t dwMaxMemSize = MemPool::MAX_UNIT_SIZE);

//////////////////////////////////////////////////////////////////////////
// �ͷ� MemPool ʵ��
// ����: 0 �ͷųɹ�
//       >0 ����ǰ�Ķ�γ�ʼ����û��δ�ͷ�
int MemPool_Destroy();

//////////////////////////////////////////////////////////////////////////
// �̰߳�ȫ�ķ����ڴ���Դ
void* MemPool_Allocate(size_t dwSize);
void MemPool_Free(void* p);

size_t MemPool_GetAllocatedSize();
size_t MemPool_GetPooledSize();

size_t MemPool_GetBlockSize(const void* ptr);

} // namespace toft

#endif // TOFT_SYSTEM_MEMORY_MEMPOOL_H

