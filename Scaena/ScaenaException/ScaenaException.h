#ifndef SCAENA_SCAENA_EXCEPTIONS_H
#define SCAENA_SCAENA_EXCEPTIONS_H

#include <exception>
#include <string>

#include "libScaena_global.h"

namespace scaena
{
    class SCAENA_EXPORT ScaenaException : public std::exception
    {
    public:
        ScaenaException(const std::string& what) throw();
        virtual ~ScaenaException() throw();

        virtual const char* what() const throw();
    private:
        std::string _what;
    };
}

#endif // SCAENA_SCAENA_EXCEPTIONS_H
