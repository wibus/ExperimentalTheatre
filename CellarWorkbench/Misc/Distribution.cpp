#include "Distribution.h"

#include <stdlib.h>


namespace cellar
{
    RandomArray g_masterRandomArray;

    RandomArray::RandomArray() :
        _array(nullptr)
    {

    }

    RandomArray::~RandomArray()
    {
        deallocate();
    }

    void RandomArray::allocate()
    {
        if(_array == nullptr)
        {
            size_t arraySize = 1 << (sizeof(decltype(_sharedIdx)) * 8);
            _array = new double[arraySize];
            refill();
        }
    }

    void RandomArray::refill()
    {
        size_t arraySize = 1 << (sizeof(decltype(_sharedIdx)) * 8);

        /*
        std::random_device rd;
        std::mt19937_64 gen(rd());
        std::uniform_real_distribution<double> dis(0.0, 1.0);

        for(size_t i=0; i < arraySize; ++i)
            _array[i] = dis(gen);
        /*/

        srand(0);

        for(size_t i=0; i < arraySize; ++i)
            _array[i] = rand() / double(RAND_MAX);
        //*/
    }

    void RandomArray::deallocate()
    {
        delete[] _array;
        _array = nullptr;
    }

    LinearRand::LinearRand() :
        _randArrayIdx(0)
    {

    }

    LinearRand::~LinearRand()
    {

    }


    DiskRand::DiskRand() :
        _linearRand()
    {

    }

    DiskRand::~DiskRand()
    {

    }


    SphereRand::SphereRand() :
        _linearRand()
    {

    }

    SphereRand::~SphereRand()
    {

    }
}
