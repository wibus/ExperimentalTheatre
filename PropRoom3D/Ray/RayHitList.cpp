#include "RayHitList.h"


namespace prop3
{
    RayHitList::RayHitList() :
        head(nullptr)
    {
    }

    RayHitList::~RayHitList()
    {
        clear();
        releaseMemoryPool();
    }

    void RayHitList::releaseMemoryPool()
    {
        size_t count = _memoryPool.size();
        for(size_t i=0; i < count; ++i)
            delete _memoryPool[i];

        _memoryPool.clear();
        _memoryPool.shrink_to_fit();
    }
}
