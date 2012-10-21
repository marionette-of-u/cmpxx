#ifndef INCLUDE_CMPXX_RATIONAL
#define INCLUDE_CMPXX_RATIONAL

// rational

#include "mp_wrapper.hpp"

namespace cmpxx{
    using rational = aux::mp_wrapper<mpq_class>;
}

#endif

