#ifndef INCLUDE_CMPXX_INTEGER
#define INCLUDE_CMPXX_INTEGER

// integer

#include "mp_wrapper.hpp"
#include "aux_xx.hpp"

namespace cmpxx{
    using integer = aux::mp_wrapper<mpz_class>;
}

#endif

