#include "ActNotFound.h"

using namespace std;


namespace scaena
{
    ActNotFound::ActNotFound(const string& what) throw() :
        ScaenaException(what)
    {}

    ActNotFound::~ActNotFound() throw()
    {}
}
