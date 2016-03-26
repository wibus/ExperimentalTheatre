#ifndef CELLARWORKBENCH_DISTRIBUTION_H
#define CELLARWORKBENCH_DISTRIBUTION_H

#include <random>

#include <GLM/glm.hpp>
#include <GLM/gtc/constants.hpp>

#include <CellarWorkbench/libCellarWorkbench_global.h>


namespace cellar
{
    class CELLAR_EXPORT RandomArray
    {
    public:
        RandomArray();
        ~RandomArray();

        void allocate();
        void refill();
        void deallocate();

        double next();
        double next(unsigned short& idx);

    private:
        double* _array;
        mutable unsigned short _sharedIdx;
    };

    CELLAR_EXPORT extern RandomArray g_masterRandomArray;



    class CELLAR_EXPORT LinearRand
    {
    public:
        LinearRand();
        ~LinearRand();

        double gen1() const;
        double gen1(double maxVal) const;
        double gen1(double minVal, double maxVal) const;

        glm::dvec2 gen2() const;
        glm::dvec2 gen2(glm::dvec2 maxVal) const;
        glm::dvec2 gen2(glm::dvec2 minVal, glm::dvec2 maxVal) const;

        glm::dvec3 gen3() const;
        glm::dvec3 gen3(glm::dvec3 maxVal) const;
        glm::dvec3 gen3(glm::dvec3 minVal, glm::dvec3 maxVal) const;


    private:
        mutable unsigned short _randArrayIdx;
    };


    class CELLAR_EXPORT DiskRand
    {
    public:
        DiskRand();
        ~DiskRand();

        glm::dvec2 gen(double radius) const;
        glm::dvec2 gen(double radius,
                       double zeroToOne,
                       double zeroToTwoPi) const;


    private:
        LinearRand _linearRand;
    };


    class CELLAR_EXPORT SphereRand
    {
    public:
        SphereRand();
        ~SphereRand();

        glm::dvec3 gen(double radius) const;
        glm::dvec3 gen(double radius,
                       double zeroToTwoPi,
                       double negOneToPosOne) const;


    private:
        LinearRand _linearRand;
    };




    // IMPLEMENTATION //
    inline double RandomArray::next()
    {
        assert(_array != nullptr);
        return _array[++_sharedIdx];
    }

    inline double RandomArray::next(unsigned short& idx)
    {
        assert(_array != nullptr);
        return _array[++idx];
    }

    inline double LinearRand::gen1() const
    {
        return g_masterRandomArray.next(_randArrayIdx);
    }

    inline double LinearRand::gen1(double maxVal) const
    {
        return g_masterRandomArray.next(_randArrayIdx) * maxVal;
    }

    inline double LinearRand::gen1(double minVal, double maxVal) const
    {
        return g_masterRandomArray.next(_randArrayIdx) * (maxVal - minVal) + minVal;
    }

    inline glm::dvec2 LinearRand::gen2() const
    {
        return glm::dvec2(gen1(), gen1());
    }

    inline glm::dvec2 LinearRand::gen2(glm::dvec2 maxVal) const
    {
        return glm::dvec2(gen1(maxVal.x), gen1(maxVal.y));
    }

    inline glm::dvec2 LinearRand::gen2(glm::dvec2 minVal, glm::dvec2 maxVal) const
    {
        return glm::dvec2(gen1(minVal.x, maxVal.x),
                          gen1(minVal.y, maxVal.y));
    }

    inline glm::dvec3 LinearRand::gen3() const
    {
        return glm::dvec3(gen1(), gen1(), gen1());
    }

    inline glm::dvec3 LinearRand::gen3(glm::dvec3 maxVal) const
    {
        return glm::dvec3(gen1(maxVal.x), gen1(maxVal.y), gen1(maxVal.z));
    }

    inline glm::dvec3 LinearRand::gen3(glm::dvec3 minVal, glm::dvec3 maxVal) const
    {
        return glm::dvec3(gen1(minVal.x, maxVal.x),
                          gen1(minVal.y, maxVal.y),
                          gen1(minVal.z, maxVal.z));
    }
    inline glm::dvec2 DiskRand::gen(double radius) const
    {
        return gen(radius,
                   _linearRand.gen1(),
                   _linearRand.gen1(2.0 * glm::pi<double>()));
    }

    inline glm::dvec2 DiskRand::gen(
            double radius,
            double zeroToOne,
            double zeroToTwoPi) const
    {
        return glm::dvec2(glm::cos(zeroToTwoPi), glm::sin(zeroToTwoPi))
                   * glm::sqrt(zeroToOne) * radius;
    }

    inline glm::dvec3 SphereRand::gen(double radius) const
    {
        return gen(radius,
                   _linearRand.gen1(2.0 * glm::pi<double>()),
                   _linearRand.gen1(-1.0, 1.0));
    }

    inline glm::dvec3 SphereRand::gen(double radius,
                                      double zeroToTwoPi,
                                      double negOneToPosOne) const
    {
        double z = negOneToPosOne;
        double r = glm::sqrt(1.0 - z*z);
        double x = r * glm::cos(zeroToTwoPi);
        double y = r * glm::sin(zeroToTwoPi);
        return glm::dvec3(x, y, z) * radius;
    }
}

#endif // CELLARWORKBENCH_DISTRIBUTION_H
