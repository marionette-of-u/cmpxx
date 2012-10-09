#ifndef INCLUDE_CMPXX_INTEGER
#define INCLUDE_CMPXX_INTEGER

// integer

#include "mp_wrapper.hpp"

namespace cmpxx{
    typedef aux::mp_wrapper<mpz_class> integer;
}

#endif
