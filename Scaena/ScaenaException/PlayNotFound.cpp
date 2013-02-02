#include "PlayNotFound.h"

using namespace std;


namespace scaena
{
    PlayNotFound::PlayNotFound(const string& what) throw() :
        ScaenaException(what)
    {}

    PlayNotFound::~PlayNotFound() throw()
    {}
}
