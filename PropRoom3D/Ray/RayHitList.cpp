#include "RayHitList.h"


namespace prop3
{
    RayHitList::RayHitList(std::vector<RayHitReport*>& memoryPool) :
        head(nullptr),
        memoryPool(memoryPool)
    {
    }

    RayHitList::~RayHitList()
    {
        clear();
    }

    void RayHitList::releaseMemoryPool(std::vector<RayHitReport*>& pool)
    {
        int count = pool.size();
        for(int i=0; i < count; ++i)
            delete pool[i];

        pool.clear();
        pool.shrink_to_fit();
    }
}
