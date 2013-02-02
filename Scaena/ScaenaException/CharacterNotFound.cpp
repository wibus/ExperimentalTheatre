#include "CharacterNotFound.h"

using namespace std;


namespace scaena
{
    CharacterNotFound::CharacterNotFound(const string& what) throw() :
        ScaenaException(what)
    {}

    CharacterNotFound::~CharacterNotFound() throw()
    {}
}
