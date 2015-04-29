#ifndef CELLARWORKBENCH_CELLARUTILS_H
#define CELLARWORKBENCH_CELLARUTILS_H

#include <sstream>

#include "../libCellarWorkbench_global.h"


namespace cellar
{
    // INTERFACE //

    // Convert anytype that ostream << accepts
    template <typename T>
    std::string toString(const T& donnee);

    // Convert a file into a single string
    CELLAR_EXPORT std::string fileToString(const std::string& fileName, bool* ok = nullptr);




    // IMPLEMENTATION //

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
}

#endif // CELLARWORKBENCH_CELLARUTILS_H
