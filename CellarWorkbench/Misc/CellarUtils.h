#ifndef CELLARWORKBENCH_CELLARUTILS_H
#define CELLARWORKBENCH_CELLARUTILS_H

#include <cstdlib>
#include <sstream>
#include <cmath>
#include <tuple>


namespace cellar
{
    const double PI = 3.141592653589793;
    const double LIGHT_SPEED = 299792458.0;


    // INTERFACE //

    // Ranged random
    template <typename T>
    T randomRange(T min, T max);

    // Clipping
    template <typename T>
    T clamp(T value, T min, T max);

    template <typename T>
    T minVal(T val1, T val2);

    template <typename T>
    T maxVal(T val1, T val2);

    template <typename T>
    bool inRange(T val, T min, T max);


    // Absolute
    template <typename T>
    T absolute(T x);

    template <typename T>
    signed char sign(T x);

    // Modulate    
    int modulateInt(int& value, int minVal, int maxVal);

    // Convert anytype that ostream << accepts
    template <typename T>
    std::string toString(const T& donnee);

    // Convert a file into a single string
    std::string fileToString(const std::string& fileName, bool* ok = nullptr);

    // Quadratic resolution
    template <typename T>
    std::tuple<int, T, T> quadraticRoots(T a, T b, T c);




    // IMPLEMENTATION //
    template <typename T>
    inline T randomRange(T min, T max)
    {
        return (min + (rand() / static_cast<double>(RAND_MAX)) * (max - min));
    }

    template <typename T>
    inline T clamp(T value, T min, T max)
    {
        return value > max ? max : (value < min ? min : value);
    }

    template <typename T>
    inline T minVal(T val1, T val2)
    {
        return val1 < val2 ? val1 : val2;
    }

    template <typename T>
    inline T maxVal(T val1, T val2)
    {
        return val2 < val1 ? val1 : val2;
    }

    template <typename T>
    inline bool inRange(T val, T min, T max)
    {
        return val < min ? false : (val > max ? false : true);
    }

    template <typename T>
    inline T absolute(T x)
    {
        return x > 0 ? x : -x;
    }

    template <typename T>
    inline signed char sign(T x)
    {
        return x == 0 ? 0 : x < 0 ? -1 : 1;
    }

    template < typename T >
    std::string toString(const T& value)
    {
            std::stringstream textStream;
            textStream << value;
            return textStream.str();
    }

    template < typename T >
    void fromString(const std::string& str, T& value)
    {
            std::stringstream textStream;
            textStream << str;
            textStream >> value;
    }

    template <typename T>
    std::tuple<int, T, T> quadraticRoots(T a, T b, T c)
    {
        T delta = b*b - 4*a*c;
        if(delta < 0.0)
            return std::tuple<int, T, T>(0, 0.0, 0.0);

        T sqrtDelta = sqrt( delta );
        if(delta == 0.0)
            return std::tuple<int, T, T>
                    (1, (-b - sqrtDelta)/(2*a), 0.0);

        T ta = 2*a;
        return std::tuple<int, T, T>
                (1, (-b - sqrtDelta)/(ta), (-b + sqrtDelta)/(ta));
    }
}

#endif // CELLARWORKBENCH_CELLARUTILS_H
