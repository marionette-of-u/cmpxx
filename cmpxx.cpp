#include <memory>
#include <utility>
#include <gmpxx.h>

namespace cmpxx{
    namespace aux{
        template<class T>
        struct expr_wrapper{
            typedef T type;
            expr_wrapper(const type &expr_) : expr(expr_){}
            type expr;
        };

        template<class MPClass>
        class mp_wrapper{
        public:
            typedef MPClass mp_type;

            mp_wrapper() :
                value_()
            {}

            mp_wrapper(const mp_wrapper &other) :
                value_(other.get_raw_value())
            {}

            mp_wrapper(mp_wrapper &&other) :
                value_()
            { swap_mp(other.value_); }

            mp_wrapper(mp_type &&value) :
                value_()
            { swap_mp(value); }

            template<class T>
            mp_wrapper(const expr_wrapper<T> &wrapped_expr) :
                value_(wrapped_expr.expr)
            {}

            mp_wrapper(const std::string &s, int base = 0) :
                value_(s, base)
            {}

            mp_wrapper &operator =(const mp_wrapper &other){
                get_raw_value() = other.get_raw_value();
                return *this;
            }

            mp_wrapper &operator =(mp_wrapper &&other){
                swap_mp(other.get_raw_value());
                return *this;
            }

            template<class T>
            mp_wrapper &operator =(const expr_wrapper<T> &wrapped_expr){
                get_raw_value() = wrapped_expr.expr;
                return *this;
            }

            mp_wrapper &operator =(mp_type &&raw_value){
                swap_mp(raw_value);
                return *this;
            }

            #define CMPXX_DEFINE_COMPOUND_OPERATOR(op)               \
                mp_wrapper &op(const mp_wrapper &other){             \
                    get_raw_value().op(other.get_raw_value());       \
                    return *this;                                    \
                }                                                    \
                template<class T>                                    \
                mp_wrapper &op(const expr_wrapper<T> &wrapped_expr){ \
                    get_raw_value().op(wrapped_expr.expr);           \
                    return *this;                                    \
                }                                                    \
                mp_wrapper &op(MPClass &&raw_value){                 \
                    get_raw_value().op(raw_value);                   \
                    return *this;                                    \
                }

            #define CMPXX_DEFINE_COMPOUND_OPERATOR_UI(op) \
                mp_wrapper &op(unsigned long int v){      \
                    get_raw_value().op(v);                \
                    return *this;                         \
                }

            #define CMPXX_DEFINE_ID_OPERATOR(op) \
                mp_wrapper &op(){                \
                    get_raw_value().op();        \
                    return *this;                \
                }                                \
                mp_wrapper &op(int){             \
                    get_raw_value().op(int());   \
                    return *this;                \
                }                                \

            CMPXX_DEFINE_COMPOUND_OPERATOR(operator +=);
            CMPXX_DEFINE_COMPOUND_OPERATOR(operator -=);
            CMPXX_DEFINE_COMPOUND_OPERATOR(operator *=);
            CMPXX_DEFINE_COMPOUND_OPERATOR(operator /=);
            CMPXX_DEFINE_COMPOUND_OPERATOR(operator %=);
            CMPXX_DEFINE_COMPOUND_OPERATOR(operator &=);
            CMPXX_DEFINE_COMPOUND_OPERATOR(operator |=);
            CMPXX_DEFINE_COMPOUND_OPERATOR(operator ^=);
            CMPXX_DEFINE_COMPOUND_OPERATOR_UI(operator <<=);
            CMPXX_DEFINE_COMPOUND_OPERATOR_UI(operator >>=);
            CMPXX_DEFINE_ID_OPERATOR(operator ++);
            CMPXX_DEFINE_ID_OPERATOR(operator --);

            const mp_type &get_raw_value() const{
                return value_;
            }

            mp_type &get_raw_value(){
                return value_;
            }

        private:
            void swap_mp(mp_type &value){
                typedef typename std::aligned_storage<
                    sizeof(mp_type),
                    std::alignment_of<mp_type>::value
                >::type pod_of_mp_class;
                std::swap(
                    *static_cast<pod_of_mp_class*>(static_cast<void*>(&value_)),
                    *static_cast<pod_of_mp_class*>(static_cast<void*>(&value))
                );
            }

            mp_type value_;
        };
    }
}

#define CMPXX_DEFINE_UNARY_OVERLOAD(op, e)                                                   \
    template<class T, class U>                                                               \
    cmpxx::aux::expr_wrapper<                                                                \
        __gmp_expr<T, __gmp_unary_expr<__gmp_expr<T, U>, e>>                                 \
    > op(const cmpxx::aux::mp_wrapper<__gmp_expr<T, U>> &x){ return op(x.get_raw_value()); } \
    template<class T, class U>                                                               \
    cmpxx::aux::expr_wrapper<                                                                \
        __gmp_expr<T, __gmp_unary_expr<__gmp_expr<T, U>, e>>                                 \
    > op(const cmpxx::aux::expr_wrapper<__gmp_expr<T, U>> &x){ return op(x.expr); }

#define CMPXX_DEFINE_BINARY_UI_OVERLOAD(op, e)                                      \
    template<class T, class U>                                                      \
    cmpxx::aux::expr_wrapper<                                                       \
        __gmp_expr<T, __gmp_binary_expr<__gmp_expr<T, U>, unsigned long int, e>>    \
    > op(const cmpxx::aux::mp_wrapper<__gmp_expr<T, U>> &l, unsigned long int r){   \
        return op(l.get_raw_value(), r);                                            \
    }                                                                               \
    template<class T, class U>                                                      \
    cmpxx::aux::expr_wrapper<                                                       \
        __gmp_expr<T, __gmp_binary_expr<__gmp_expr<T, U>, unsigned long int, e>>    \
    > op(const cmpxx::aux::expr_wrapper<__gmp_expr<T, U>> &l, unsigned long int r){ \
        return op(l.expr, r);                                                       \
    }

#define CMPXX_DEFINE_BUILT_IN_TYPE_OVERLOAD_IMPL(op, e, type, bigtype, r_type) \
    template<class T, class U>                                                 \
    r_type(e, bigtype)                                                         \
    op(const cmpxx::aux::mp_wrapper<__gmp_expr<T, U>> &l, type r){             \
        return op(l.get_raw_value(), r);                                       \
    }                                                                          \
    template<class T, class U>                                                 \
    r_type(e, bigtype)                                                         \
    op(type l, const cmpxx::aux::mp_wrapper<__gmp_expr<T, U>> &r){             \
        return op(l, r.get_raw_value());                                       \
    }                                                                          \
    template<class T, class U>                                                 \
    r_type(e, bigtype)                                                         \
    op(const cmpxx::aux::expr_wrapper<__gmp_expr<T, U>> &l, type r){           \
        return op(l.expr, r);                                                  \
    }                                                                          \
    template<class T, class U>                                                 \
    r_type(e, bigtype)                                                         \
    op(type l, const cmpxx::aux::expr_wrapper<__gmp_expr<T, U>> &r){           \
        return op(l ,r.expr);                                                  \
    }

#define CMPXX_DEFINE_BUILT_IN_TYPE_OVERLOAD_IMPL_S(op, e, t, r) \
    CMPXX_DEFINE_BUILT_IN_TYPE_OVERLOAD_IMPL(op, e, t, signed long int, r)

#define CMPXX_DEFINE_BUILT_IN_TYPE_OVERLOAD_IMPL_U(op, e, t, r) \
    CMPXX_DEFINE_BUILT_IN_TYPE_OVERLOAD_IMPL(op, e, t, unsigned long int, r)

#define CMPXX_DEFINE_BUILT_IN_TYPE_OVERLOAD_IMPL_D(op, e, t, r) \
    CMPXX_DEFINE_BUILT_IN_TYPE_OVERLOAD_IMPL(op, e, t, double, r)

#define CMPXX_DEFINE_BUILT_IN_TYPE_OVERLOAD_IMPL_LD(op, e, t, r) \
    CMPXX_DEFINE_BUILT_IN_TYPE_OVERLOAD_IMPL(op, e, t, long double, r)

#define CMPXX_DEFINE_BUILT_IN_TYPE_OVERLOAD(op, e, r)                        \
    CMPXX_DEFINE_BUILT_IN_TYPE_OVERLOAD_IMPL_S(op, e, signed char, r)        \
    CMPXX_DEFINE_BUILT_IN_TYPE_OVERLOAD_IMPL_U(op, e, unsigned char, r)      \
    CMPXX_DEFINE_BUILT_IN_TYPE_OVERLOAD_IMPL_S(op, e, signed int, r)         \
    CMPXX_DEFINE_BUILT_IN_TYPE_OVERLOAD_IMPL_U(op, e, unsigned int, r)       \
    CMPXX_DEFINE_BUILT_IN_TYPE_OVERLOAD_IMPL_S(op, e, signed short int, r)   \
    CMPXX_DEFINE_BUILT_IN_TYPE_OVERLOAD_IMPL_U(op, e, unsigned short int, r) \
    CMPXX_DEFINE_BUILT_IN_TYPE_OVERLOAD_IMPL_S(op, e, signed long int, r)    \
    CMPXX_DEFINE_BUILT_IN_TYPE_OVERLOAD_IMPL_U(op, e, unsigned long int, r)  \
    CMPXX_DEFINE_BUILT_IN_TYPE_OVERLOAD_IMPL_D(op, e, float, r)              \
    CMPXX_DEFINE_BUILT_IN_TYPE_OVERLOAD_IMPL_D(op, e, double, r)             \
    CMPXX_DEFINE_BUILT_IN_TYPE_OVERLOAD_IMPL_LD(op, e, long double, r)

#define CMPXX_DEFINE_EXPR_OVERLOAD(op, e, r_type)            \
    template<class T, class U, class V, class W>             \
    r_type(e, nil) op(                                       \
        const cmpxx::aux::mp_wrapper<__gmp_expr<T, U>> &l,   \
        const cmpxx::aux::mp_wrapper<__gmp_expr<V, W>> &r    \
    ){ return op(l.get_raw_value() ,r.get_raw_value()); }    \
    template<class T, class U, class V, class W>             \
    r_type(e, nil) op(                                       \
        const cmpxx::aux::expr_wrapper<__gmp_expr<T, U>> &l, \
        const cmpxx::aux::expr_wrapper<__gmp_expr<V, W>> &r  \
    ){ return op(l.expr ,r.expr); }                          \
    template<class T, class U, class V, class W>             \
    r_type(e, nil) op(                                       \
        const cmpxx::aux::mp_wrapper<__gmp_expr<T, U>> &l,   \
        const cmpxx::aux::expr_wrapper<__gmp_expr<V, W>> &r  \
    ){ return op(l.get_raw_value() ,r.expr); }               \
    template<class T, class U, class V, class W>             \
    r_type(e, nil) op(                                       \
        const cmpxx::aux::expr_wrapper<__gmp_expr<T, U>> &l, \
        const cmpxx::aux::mp_wrapper<__gmp_expr<V, W>> &r    \
    ){ return op(l.expr ,r.get_raw_value()); }

#define CMPXX_DEFINE_OVERLOAD_RETURN_TYPE(e, bigtype)                \
    cmpxx::aux::expr_wrapper<                                        \
        __gmp_expr<                                                  \
            typename __gmp_resolve_expr<T, V>::value_type,           \
            __gmp_binary_expr<__gmp_expr<T, U>, __gmp_expr<V, W>, e> \
        >                                                            \
    >

#define CMPXX_DEFINE_BUILT_IN_TYPE_OVERLOAD_RETURN_TYPE(e, bigtype) \
    cmpxx::aux::expr_wrapper<__gmp_expr<T, __gmp_binary_expr<__gmp_expr<T, U>, bigtype, e>>>

#define CMPXX_DEFINE_OVERLOAD_RETURN_TYPE_BOOL(bigtype, e) bool

#define CMPXX_DEFINE_OVERLOAD(op, e)                    \
    CMPXX_DEFINE_EXPR_OVERLOAD(                         \
        op,                                             \
        e,                                              \
        CMPXX_DEFINE_OVERLOAD_RETURN_TYPE               \
    )                                                   \
    CMPXX_DEFINE_BUILT_IN_TYPE_OVERLOAD(                \
        op,                                             \
        e,                                              \
        CMPXX_DEFINE_BUILT_IN_TYPE_OVERLOAD_RETURN_TYPE \
    )

#define CMPXX_DEFINE_BOOLIAN_OVERLOAD(op, e)   \
    CMPXX_DEFINE_EXPR_OVERLOAD(                \
        op,                                    \
        e,                                     \
        CMPXX_DEFINE_OVERLOAD_RETURN_TYPE_BOOL \
    )                                          \
    CMPXX_DEFINE_BUILT_IN_TYPE_OVERLOAD(       \
        op,                                    \
        e,                                     \
        CMPXX_DEFINE_OVERLOAD_RETURN_TYPE_BOOL \
    )

CMPXX_DEFINE_UNARY_OVERLOAD(operator +, __gmp_unary_plus);
CMPXX_DEFINE_UNARY_OVERLOAD(operator -, __gmp_unary_minus);
CMPXX_DEFINE_UNARY_OVERLOAD(operator ~, __gmp_unary_com);
CMPXX_DEFINE_OVERLOAD(operator +, __gmp_binary_plus);
CMPXX_DEFINE_OVERLOAD(operator -, __gmp_binary_minus);
CMPXX_DEFINE_OVERLOAD(operator *, __gmp_binary_multiplies);
CMPXX_DEFINE_OVERLOAD(operator /, __gmp_binary_divides);
CMPXX_DEFINE_OVERLOAD(operator %, __gmp_binary_modulus);
CMPXX_DEFINE_OVERLOAD(operator &, __gmp_binary_and);
CMPXX_DEFINE_OVERLOAD(operator |, __gmp_binary_ior);
CMPXX_DEFINE_OVERLOAD(operator ^, __gmp_binary_xor);
CMPXX_DEFINE_BINARY_UI_OVERLOAD(operator <<, __gmp_binary_lshift);
CMPXX_DEFINE_BINARY_UI_OVERLOAD(operator >>, __gmp_binary_rshift);
CMPXX_DEFINE_BOOLIAN_OVERLOAD(operator <, __gmp_binary_less);
CMPXX_DEFINE_BOOLIAN_OVERLOAD(operator >, __gmp_binary_greater);
CMPXX_DEFINE_BOOLIAN_OVERLOAD(operator <=, __gmp_binary_less_equal);
CMPXX_DEFINE_BOOLIAN_OVERLOAD(operator >=, __gmp_binary_greater_equal);
CMPXX_DEFINE_BOOLIAN_OVERLOAD(operator ==, __gmp_binary_equal);
CMPXX_DEFINE_BOOLIAN_OVERLOAD(operator !=, __gmp_binary_not_equal);
CMPXX_DEFINE_UNARY_OVERLOAD(abs, __gmp_abs_function)
CMPXX_DEFINE_UNARY_OVERLOAD(trunc, __gmp_trunc_function)
CMPXX_DEFINE_UNARY_OVERLOAD(floor, __gmp_floor_function)
CMPXX_DEFINE_UNARY_OVERLOAD(ceil, __gmp_ceil_function)
CMPXX_DEFINE_UNARY_OVERLOAD(sqrt, __gmp_sqrt_function)

namespace cmpxx{
    typedef aux::mp_wrapper<mpz_class> integer;
    typedef aux::mp_wrapper<mpq_class> rational;
}

// test

#include <iostream>
#include <boost/timer.hpp>

cmpxx::integer value_a("99999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999");
mpz_class value_b("99999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999");

cmpxx::integer return_a(){
    return value_a;
}

mpz_class return_b(){
    return value_b;
}

int main(){
    boost::timer t;

    t.restart();
    for(int i = 0; i < (1 << 20); ++i){
        cmpxx::integer x = return_a();
    }
    std::cout << t.elapsed() << "\n";

    t.restart();
    for(int i = 0; i < (1 << 20); ++i){
        mpz_class x = return_b();
    }
    std::cout << t.elapsed() << "\n";

    return 0;
}
