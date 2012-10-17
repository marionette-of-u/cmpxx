#ifndef INCLUDE_CMPXX_MP_WRAPPER
#define INCLUDE_CMPXX_MP_WRAPPER

// integer/rational/floating wrapper 

#include <memory>
#include <utility>
#include <limits>
#include <sstream>
#include <type_traits>
#include <cmath>
#include <cstdlib>
#include "aux_xx.hpp"

namespace cmpxx{
    namespace aux{
        template<class T>
        struct expr_wrapper;

        template<class MPClass>
        class mp_wrapper;

        template<class T, class Expression>
        struct expr_wrapper<__gmp_expr<T, Expression>>{
            using type = __gmp_expr<T, Expression>;
            inline expr_wrapper(const type &expr_) : expr(expr_){}
            type expr;
            inline operator mp_wrapper<T>() const{
                return expr;
            }
        };

        template<class MPClass>
        class mp_wrapper{
        public:
            typedef MPClass mp_type;

            inline mp_wrapper() :
                value_()
            {}

            inline mp_wrapper(const mp_wrapper &other) :
                value_(other.get_raw_value())
            {}

            inline mp_wrapper(mp_wrapper &&other) :
                value_()
            { swap_mp(other.get_raw_value()); }

            inline mp_wrapper(mp_type &&value) :
                value_()
            { swap_mp(value); }

            template<class T>
            inline mp_wrapper(const T &value) :
                value_(value)
            {}

            template<class T>
            inline mp_wrapper(const expr_wrapper<T> &wrapped_expr) :
                value_(wrapped_expr.expr)
            {}

            inline mp_wrapper(const char *s, int base) :
                value_(s, base)
            {}

            inline explicit mp_wrapper(const std::string &s) :
                value_(s)
            {}
                
            inline mp_wrapper(const std::string &s, int base) :
                value_(s, base)
            {}

            inline mp_wrapper &operator =(const mp_wrapper &other){
                get_raw_value() = other.get_raw_value();
                return *this;
            }

            inline mp_wrapper &operator =(mp_wrapper &&other){
                swap_mp(other.get_raw_value());
                return *this;
            }

            template<class T>
            inline mp_wrapper &operator =(const expr_wrapper<T> &wrapped_expr){
                get_raw_value() = wrapped_expr.expr;
                return *this;
            }

            inline mp_wrapper &operator =(mp_type &&raw_value){
                swap_mp(raw_value);
                return *this;
            }

            #define CMPXX_MP_WRAPPER_DEFINE_ASSIGN_OPERATOR(op, e, r, type) \
                inline mp_wrapper &operator =(type value){                  \
                    get_raw_value() = value;                                \
                    return *this;                                           \
                }                                                           \
                inline mp_wrapper(type value) : value_(value){}

            CMPXX_INVOKE_MACRO_WITH_BUILT_IN_TYPE(CMPXX_MP_WRAPPER_DEFINE_ASSIGN_OPERATOR, nil, nil, nil);

            #define CMPXX_MP_WRAPPER_DEFINE_COMPOUND_OPERATOR_IMPL(op, e, r, type) \
                inline mp_wrapper &op(type value){                                 \
                    get_raw_value().op(value);                                     \
                    return *this;                                                  \
                }

            #define CMPXX_MP_WRAPPER_DEFINE_COMPOUND_OPERATOR(op)            \
                inline mp_wrapper &op(const mp_wrapper &other){              \
                    get_raw_value().op(other.get_raw_value());               \
                    return *this;                                            \
                }                                                            \
                inline mp_wrapper &op(mp_wrapper &&other){                   \
                    get_raw_value().op(other.get_raw_value());               \
                }                                                            \
                template<class T>                                            \
                inline mp_wrapper &op(const expr_wrapper<T> &wrapped_expr){  \
                    get_raw_value().op(wrapped_expr.expr);                   \
                    return *this;                                            \
                }                                                            \
                inline mp_wrapper &op(mp_type &&raw_value){                  \
                    get_raw_value().op(raw_value);                           \
                    return *this;                                            \
                }                                                            \
                CMPXX_INVOKE_MACRO_WITH_BUILT_IN_TYPE(CMPXX_MP_WRAPPER_DEFINE_COMPOUND_OPERATOR_IMPL, op, nil, nil);

            #define CMPXX_MP_WRAPPER_DEFINE_COMPOUND_OPERATOR_UI(op) \
                inline mp_wrapper &op(unsigned long int v){          \
                    get_raw_value().op(v);                           \
                    return *this;                                    \
                }

            #define CMPXX_MP_WRAPPER_DEFINE_ID_OPERATOR(op) \
                inline mp_wrapper &op(){                    \
                    get_raw_value().op();                   \
                    return *this;                           \
                }                                           \
                inline mp_wrapper &op(int){                 \
                    get_raw_value().op(int());              \
                    return *this;                           \
                }

            CMPXX_MP_WRAPPER_DEFINE_COMPOUND_OPERATOR(operator +=);
            CMPXX_MP_WRAPPER_DEFINE_COMPOUND_OPERATOR(operator -=);
            CMPXX_MP_WRAPPER_DEFINE_COMPOUND_OPERATOR(operator *=);
            CMPXX_MP_WRAPPER_DEFINE_COMPOUND_OPERATOR(operator /=);
            CMPXX_MP_WRAPPER_DEFINE_COMPOUND_OPERATOR(operator %=);
            CMPXX_MP_WRAPPER_DEFINE_COMPOUND_OPERATOR(operator &=);
            CMPXX_MP_WRAPPER_DEFINE_COMPOUND_OPERATOR(operator |=);
            CMPXX_MP_WRAPPER_DEFINE_COMPOUND_OPERATOR(operator ^=);
            CMPXX_MP_WRAPPER_DEFINE_COMPOUND_OPERATOR_UI(operator <<=);
            CMPXX_MP_WRAPPER_DEFINE_COMPOUND_OPERATOR_UI(operator >>=);
            CMPXX_MP_WRAPPER_DEFINE_ID_OPERATOR(operator ++);
            CMPXX_MP_WRAPPER_DEFINE_ID_OPERATOR(operator --);

            inline const mp_type &get_raw_value() const{
                return value_;
            }

            inline mp_type &get_raw_value(){
                return value_;
            }

            /*
             * 2^n に繰り上げた値を算出する
             * */
             mp_wrapper ceil_pow2() const{
                static_assert(std::is_same<MPClass, mpz_class>::value, "this type is not integer.");
                const std::size_t popcount = mpz_popcount(get_raw_value().get_mpz_t()), size = mpz_size(get_raw_value().get_mpz_t());
                if(popcount <= 1){
                    return *this;
                }
                mp_wrapper r(*this << 1);
                mpn_zero(r.get_raw_value().get_mpz_t()->_mp_d, size - 1);
                mp_limb_t x = r.get_raw_value().get_mpz_t()->_mp_d[size - 1];
                for(std::size_t i = 1; i < GMP_LIMB_BITS; i <<= 1){
                    x = x | (x >> i);
                }
                r.get_raw_value().get_mpz_t()->_mp_d[size - 1] = x - (x >> 1);
                return r;
            }

            /*
             * ceil(log_{2} x) = y なる y を算出する
             * */
            std::size_t ceil_log2() const{
                static_assert(std::is_same<MPClass, mpz_class>::value, "this type is not integer.");
                std::size_t size = mpz_size(get_raw_value().get_mpz_t());
                mp_limb_t *data = get_raw_value().get_mpz_t()->_mp_d, x = data[size - 1];
                std::size_t n = index_of_leftmost_flag(x);
                if(x - (1 << (n - 1)) != 0){
                    return (size - 1) * GMP_LIMB_BITS + n + 1;
                }
                std::size_t m = 0;
                for(std::size_t i = 0, length = size - 1; i < length; ++i){
                    if(data[i] != 0){
                        m = 1;
                        break;
                    }
                }
                return (size - 1) * GMP_LIMB_BITS + n + m;
            }

            /*
             * a > p について k * p > a, k = 2^n (n >= 1) なる k を算出する
             * */
            static std::size_t lower_bound_pow2_coefficient(const mp_wrapper &a, const mp_wrapper &p){
                static_assert(std::is_same<MPClass, mpz_class>::value, "this type is not integer.");
                std::size_t p_size = mpz_size(p.get_raw_value().get_mpz_t()), a_size = mpz_size(a.get_raw_value().get_mpz_t());
                mp_limb_t p_last = p.get_raw_value().get_mpz_t()->_mp_d[p_size - 1], a_last = a.get_raw_value().get_mpz_t()->_mp_d[a_size - 1];
                std::size_t p_deg2 = p_size * GMP_LIMB_BITS, a_deg2 = a_size * GMP_LIMB_BITS;
                p_deg2 += index_of_leftmost_flag(p_last), a_deg2 += index_of_leftmost_flag(a_last);
                std::size_t k = a_deg2 - p_deg2;
                int r = mpn_cmp(a.get_raw_value().get_mpz_t()->_mp_d + (a_size - p_size), p.get_raw_value().get_mpz_t()->_mp_d, p_size);
                if(r > 0){
                    ++k;
                }else{
                    mp_limb_t *a_rest = a.get_raw_value().get_mpz_t()->_mp_d;
                    for(std::size_t i = 0, length = a_size - p_size; i < length; ++i){
                        if(a_rest[i] == 0){ continue; }
                        ++k;
                        break;
                    }
                }
                return k != 0 ? k : 1;
            }

            inline static mp_wrapper absolute_max(const mp_wrapper &x, const mp_wrapper &y){
                if(x > 0){
                    if(y > 0){
                        return x > y ? x : y;
                    }else{
                        mp_wrapper temp = -y;
                        return x > temp ? x : temp;
                    }
                }else{
                    if(y > 0){
                        mp_wrapper temp = -x;
                        return temp > y ? temp : y;
                    }else{
                        return x < y ? x : y;
                    }
                }
            }

        private:
            inline void swap_mp(mp_type &value){
                typedef typename std::aligned_storage<
                    sizeof(mp_type),
                    std::alignment_of<mp_type>::value
                >::type pod_of_mp_type;
                std::swap(
                    *static_cast<pod_of_mp_type*>(static_cast<void*>(&value)),
                    *static_cast<pod_of_mp_type*>(static_cast<void*>(&value_))
                );
            }

            mp_type value_;
        };

        namespace{
            template<class T, decltype(&T::canonicalize) = &T::canonicalize>
            std::true_type check_value_canonicalize(int);

            template<class T>
            std::false_type check_value_canonicalize(long);

            template<class T>
            struct has_canonicalize : decltype(check_value_canonicalize<T>(0)){};
            
            template<class MPClass>
            inline std::string has_canonicalize_value_str(const mp_wrapper<MPClass> &value, std::true_type){
                mp_wrapper<MPClass> clone(value);
                clone.get_raw_value().canonicalize();
                return clone.get_raw_value().get_str();
            }

            template<class MPClass>
            inline std::string has_canonicalize_value_str(const mp_wrapper<MPClass> &value, std::false_type){
                return value.get_raw_value().get_str();
            }
        }

        template<class MPClass>
        std::ostream &operator <<(std::ostream &ostream, const mp_wrapper<MPClass> &value){
            ostream << has_canonicalize_value_str(value, has_canonicalize<MPClass>());
            return ostream;
        }
    }
}

#define CMPXX_MP_WRAPPER_DEFINE_UNARY_OVERLOAD(op, e)                                        \
    template<class T, class U>                                                               \
    cmpxx::aux::expr_wrapper<                                                                \
        __gmp_expr<T, __gmp_unary_expr<__gmp_expr<T, U>, e>>                                 \
    > op(const cmpxx::aux::mp_wrapper<__gmp_expr<T, U>> &x){ return op(x.get_raw_value()); } \
    template<class T, class U>                                                               \
    cmpxx::aux::expr_wrapper<                                                                \
        __gmp_expr<T, __gmp_unary_expr<__gmp_expr<T, U>, e>>                                 \
    > op(const cmpxx::aux::expr_wrapper<__gmp_expr<T, U>> &x){ return op(x.expr); }

#define CMPXX_MP_WRAPPER_DEFINE_BINARY_UI_OVERLOAD(op, e)                           \
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

#define CMPXX_MP_WRAPPER_DEFINE_BUILT_IN_TYPE_OVERLOAD_IMPL(op, e, r_type, type, bigtype) \
    template<class T, class U>                                                            \
    r_type(e, bigtype)                                                                    \
    op(const cmpxx::aux::mp_wrapper<__gmp_expr<T, U>> &l, type r){                        \
        return op(l.get_raw_value(), r);                                                  \
    }                                                                                     \
    template<class T, class U>                                                            \
    r_type(e, bigtype)                                                                    \
    op(type l, const cmpxx::aux::mp_wrapper<__gmp_expr<T, U>> &r){                        \
        return op(l, r.get_raw_value());                                                  \
    }                                                                                     \
    template<class T, class U>                                                            \
    r_type(e, bigtype)                                                                    \
    op(const cmpxx::aux::expr_wrapper<__gmp_expr<T, U>> &l, type r){                      \
        return op(l.expr, r);                                                             \
    }                                                                                     \
    template<class T, class U>                                                            \
    r_type(e, bigtype)                                                                    \
    op(type l, const cmpxx::aux::expr_wrapper<__gmp_expr<T, U>> &r){                      \
        return op(l ,r.expr);                                                             \
    }

#define CMPXX_MP_WRAPPER_DEFINE_BUILT_IN_TYPE_OVERLOAD_IMPL_S(op, e, r, t) \
    CMPXX_MP_WRAPPER_DEFINE_BUILT_IN_TYPE_OVERLOAD_IMPL(op, e, r, t, signed long int);

#define CMPXX_MP_WRAPPER_DEFINE_BUILT_IN_TYPE_OVERLOAD_IMPL_U(op, e, r, t) \
    CMPXX_MP_WRAPPER_DEFINE_BUILT_IN_TYPE_OVERLOAD_IMPL(op, e, r, t, unsigned long int);

#define CMPXX_MP_WRAPPER_DEFINE_BUILT_IN_TYPE_OVERLOAD_IMPL_D(op, e, r, t) \
    CMPXX_MP_WRAPPER_DEFINE_BUILT_IN_TYPE_OVERLOAD_IMPL(op, e, r, t, double);

#define CMPXX_MP_WRAPPER_DEFINE_BUILT_IN_TYPE_OVERLOAD_IMPL_LD(op, e, r, t) \
    CMPXX_MP_WRAPPER_DEFINE_BUILT_IN_TYPE_OVERLOAD_IMPL(op, e, r, t, long double);

#define CMPXX_MP_WRAPPER_DEFINE_BUILT_IN_TYPE_OVERLOAD(op, e, r) \
    CMPXX_INVOKE_MACRO_WITH_S_U_D_LD(                            \
        CMPXX_MP_WRAPPER_DEFINE_BUILT_IN_TYPE_OVERLOAD_IMPL_S,   \
        CMPXX_MP_WRAPPER_DEFINE_BUILT_IN_TYPE_OVERLOAD_IMPL_U,   \
        CMPXX_MP_WRAPPER_DEFINE_BUILT_IN_TYPE_OVERLOAD_IMPL_D,   \
        CMPXX_MP_WRAPPER_DEFINE_BUILT_IN_TYPE_OVERLOAD_IMPL_LD,  \
        op, e, r);

#define CMPXX_MP_WRAPPER_DEFINE_EXPR_OVERLOAD(op, e, r_type) \
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

#define CMPXX_MP_WRAPPER_DEFINE_OVERLOAD_RETURN_TYPE(e, bigtype)     \
    cmpxx::aux::expr_wrapper<                                        \
        __gmp_expr<                                                  \
            typename __gmp_resolve_expr<T, V>::value_type,           \
            __gmp_binary_expr<__gmp_expr<T, U>, __gmp_expr<V, W>, e> \
        >                                                            \
    >

#define CMPXX_WRAPPER_DEFINE_BUILT_IN_TYPE_OVERLOAD_RETURN_TYPE(e, bigtype) \
    cmpxx::aux::expr_wrapper<__gmp_expr<T, __gmp_binary_expr<__gmp_expr<T, U>, bigtype, e>>>

#define CMPXX_WRAPPER_DEFINE_OVERLOAD_RETURN_TYPE_BOOL(bigtype, e) bool

#define CMPXX_MP_WRAPPER_DEFINE_OVERLOAD(op, e)                 \
    CMPXX_MP_WRAPPER_DEFINE_EXPR_OVERLOAD(                      \
        op,                                                     \
        e,                                                      \
        CMPXX_MP_WRAPPER_DEFINE_OVERLOAD_RETURN_TYPE            \
    );                                                          \
    CMPXX_INVOKE_MACRO_WITH_S_U_D_LD(                           \
        CMPXX_MP_WRAPPER_DEFINE_BUILT_IN_TYPE_OVERLOAD_IMPL_S,  \
        CMPXX_MP_WRAPPER_DEFINE_BUILT_IN_TYPE_OVERLOAD_IMPL_U,  \
        CMPXX_MP_WRAPPER_DEFINE_BUILT_IN_TYPE_OVERLOAD_IMPL_D,  \
        CMPXX_MP_WRAPPER_DEFINE_BUILT_IN_TYPE_OVERLOAD_IMPL_LD, \
        op, e, CMPXX_WRAPPER_DEFINE_BUILT_IN_TYPE_OVERLOAD_RETURN_TYPE);

#define CMPXX_MP_WRAPPER_DEFINE_BOOLEAN_OVERLOAD(op, e)         \
    CMPXX_MP_WRAPPER_DEFINE_EXPR_OVERLOAD(                      \
        op,                                                     \
        e,                                                      \
        CMPXX_WRAPPER_DEFINE_OVERLOAD_RETURN_TYPE_BOOL          \
    );                                                          \
    CMPXX_INVOKE_MACRO_WITH_S_U_D_LD(                           \
        CMPXX_MP_WRAPPER_DEFINE_BUILT_IN_TYPE_OVERLOAD_IMPL_S,  \
        CMPXX_MP_WRAPPER_DEFINE_BUILT_IN_TYPE_OVERLOAD_IMPL_U,  \
        CMPXX_MP_WRAPPER_DEFINE_BUILT_IN_TYPE_OVERLOAD_IMPL_D,  \
        CMPXX_MP_WRAPPER_DEFINE_BUILT_IN_TYPE_OVERLOAD_IMPL_LD, \
        op, e, CMPXX_WRAPPER_DEFINE_OVERLOAD_RETURN_TYPE_BOOL);

CMPXX_MP_WRAPPER_DEFINE_UNARY_OVERLOAD(operator +, __gmp_unary_plus);
CMPXX_MP_WRAPPER_DEFINE_UNARY_OVERLOAD(operator -, __gmp_unary_minus);
CMPXX_MP_WRAPPER_DEFINE_UNARY_OVERLOAD(operator ~, __gmp_unary_com);
CMPXX_MP_WRAPPER_DEFINE_OVERLOAD(operator +, __gmp_binary_plus);
CMPXX_MP_WRAPPER_DEFINE_OVERLOAD(operator -, __gmp_binary_minus);
CMPXX_MP_WRAPPER_DEFINE_OVERLOAD(operator *, __gmp_binary_multiplies);
CMPXX_MP_WRAPPER_DEFINE_OVERLOAD(operator /, __gmp_binary_divides);
CMPXX_MP_WRAPPER_DEFINE_OVERLOAD(operator %, __gmp_binary_modulus);
CMPXX_MP_WRAPPER_DEFINE_OVERLOAD(operator &, __gmp_binary_and);
CMPXX_MP_WRAPPER_DEFINE_OVERLOAD(operator |, __gmp_binary_ior);
CMPXX_MP_WRAPPER_DEFINE_OVERLOAD(operator ^, __gmp_binary_xor);
CMPXX_MP_WRAPPER_DEFINE_BINARY_UI_OVERLOAD(operator <<, __gmp_binary_lshift);
CMPXX_MP_WRAPPER_DEFINE_BINARY_UI_OVERLOAD(operator >>, __gmp_binary_rshift);
CMPXX_MP_WRAPPER_DEFINE_BOOLEAN_OVERLOAD(operator <, __gmp_binary_less);
CMPXX_MP_WRAPPER_DEFINE_BOOLEAN_OVERLOAD(operator >, __gmp_binary_greater);
CMPXX_MP_WRAPPER_DEFINE_BOOLEAN_OVERLOAD(operator <=, __gmp_binary_less_equal);
CMPXX_MP_WRAPPER_DEFINE_BOOLEAN_OVERLOAD(operator >=, __gmp_binary_greater_equal);
CMPXX_MP_WRAPPER_DEFINE_BOOLEAN_OVERLOAD(operator ==, __gmp_binary_equal);
CMPXX_MP_WRAPPER_DEFINE_BOOLEAN_OVERLOAD(operator !=, __gmp_binary_not_equal);
CMPXX_MP_WRAPPER_DEFINE_UNARY_OVERLOAD(abs, __gmp_abs_function);
CMPXX_MP_WRAPPER_DEFINE_UNARY_OVERLOAD(trunc, __gmp_trunc_function);
CMPXX_MP_WRAPPER_DEFINE_UNARY_OVERLOAD(floor, __gmp_floor_function);
CMPXX_MP_WRAPPER_DEFINE_UNARY_OVERLOAD(ceil, __gmp_ceil_function);
CMPXX_MP_WRAPPER_DEFINE_UNARY_OVERLOAD(sqrt, __gmp_sqrt_function);

#endif

