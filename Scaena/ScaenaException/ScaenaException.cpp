#include "ScaenaException.h"

using namespace std;


namespace scaena
{
    ScaenaException::ScaenaException(const string& what) throw() :
        exception(),
        _what(what)
    {}

    ScaenaException::~ScaenaException() throw()
    {}

    const char* ScaenaException::what() const throw()
    {
        return _what.c_str();
    }
}
