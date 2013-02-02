#include "StageNotFound.h"

using namespace std;


namespace scaena
{
    StageNotFound::StageNotFound(const string& what) throw() :
        ScaenaException(what)
    {}

    StageNotFound::~StageNotFound() throw()
    {}
}
