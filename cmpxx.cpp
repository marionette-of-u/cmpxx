#include <memory>
#include <utility>
#include <limits>
#include <cmath>
#include <cstdlib>
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

            #define CMPXX_DEFINE_ASSIGN_OPERATOR(type)     \
                inline mp_wrapper &operator =(type value){ \
                    get_raw_value() = value;               \
                    return *this;                          \
                }

            CMPXX_DEFINE_ASSIGN_OPERATOR(signed char);
            CMPXX_DEFINE_ASSIGN_OPERATOR(unsigned char);
            CMPXX_DEFINE_ASSIGN_OPERATOR(signed int);
            CMPXX_DEFINE_ASSIGN_OPERATOR(unsigned int);
            CMPXX_DEFINE_ASSIGN_OPERATOR(signed short int);
            CMPXX_DEFINE_ASSIGN_OPERATOR(unsigned short int);
            CMPXX_DEFINE_ASSIGN_OPERATOR(signed long int);
            CMPXX_DEFINE_ASSIGN_OPERATOR(unsigned long int);
            CMPXX_DEFINE_ASSIGN_OPERATOR(float);
            CMPXX_DEFINE_ASSIGN_OPERATOR(double);
            CMPXX_DEFINE_ASSIGN_OPERATOR(long double);

            #define CMPXX_DEFINE_COMPOUND_OPERATOR_IMPL(op, type) \
                inline mp_wrapper &op(type value){                \
                    get_raw_value().op(value);                    \
                    return *this;                                 \
                }

            #define CMPXX_DEFINE_COMPOUND_OPERATOR(op)                       \
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
                CMPXX_DEFINE_COMPOUND_OPERATOR_IMPL(op, signed char);        \
                CMPXX_DEFINE_COMPOUND_OPERATOR_IMPL(op, unsigned char);      \
                CMPXX_DEFINE_COMPOUND_OPERATOR_IMPL(op, signed int);         \
                CMPXX_DEFINE_COMPOUND_OPERATOR_IMPL(op, unsigned int);       \
                CMPXX_DEFINE_COMPOUND_OPERATOR_IMPL(op, signed short int);   \
                CMPXX_DEFINE_COMPOUND_OPERATOR_IMPL(op, unsigned short int); \
                CMPXX_DEFINE_COMPOUND_OPERATOR_IMPL(op, signed long int);    \
                CMPXX_DEFINE_COMPOUND_OPERATOR_IMPL(op, unsigned long int);  \
                CMPXX_DEFINE_COMPOUND_OPERATOR_IMPL(op, float);              \
                CMPXX_DEFINE_COMPOUND_OPERATOR_IMPL(op, double);             \
                CMPXX_DEFINE_COMPOUND_OPERATOR_IMPL(op, long double);

            #define CMPXX_DEFINE_COMPOUND_OPERATOR_UI(op)        \
                inline mp_wrapper &op(unsigned long int v){      \
                    get_raw_value().op(v);                       \
                    return *this;                                \
                }

            #define CMPXX_DEFINE_ID_OPERATOR(op)        \
                inline mp_wrapper &op(){                \
                    get_raw_value().op();               \
                    return *this;                       \
                }                                       \
                inline mp_wrapper &op(int){             \
                    get_raw_value().op(int());          \
                    return *this;                       \
                }

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

            inline const mp_type &get_raw_value() const{
                return value_;
            }

            inline mp_type &get_raw_value(){
                return value_;
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
    
    namespace aux{
        template<class Type>
        class fpoint{
        public:
            typedef Type type;

            inline fpoint() : exp_(), frac_(){}

            inline fpoint(const fpoint &other) : exp_(other.exp_), frac_(other.frac_){}

            inline fpoint(fpoint &&other) :
                exp_(), frac_()
            {
                typedef std::aligned_storage<
                    sizeof(integer),
                    std::alignment_of<integer>::value
                >::type pod_of_integer;
                std::swap(
                    *static_cast<pod_of_integer*>(static_cast<void*>(&exp_)),
                    *static_cast<pod_of_integer*>(static_cast<void*>(&other.exp_))
                );
                std::swap(
                    *static_cast<pod_of_integer*>(static_cast<void*>(&frac_)),
                    *static_cast<pod_of_integer*>(static_cast<void*>(&other.frac_))
                );
            }

            fpoint(double x) : exp_(), frac_(){
                static const std::size_t
                    digits = static_cast<std::size_t>(std::numeric_limits<mp_limb_t>::digits),
                    mask = digits - 1,
                    shift = rightmost_zero_seq_num(digits);
                bool sign = x >= 0.0;
                if(x < 0.0){ x = -x; }
                std::size_t prec = get_prec();
                mpz_ptr z = frac_.get_raw_value().get_mpz_t();
                mp_limb_t *data = z->_mp_d;
                double x_frac;
                int x_exp;
                x_frac = std::frexp(x, &x_exp);
                if(x_exp >= 0){
                    exp_ = x_exp / digits;
                    if(x_exp % digits != 0){ ++exp_; }
                }else{
                    x_exp *= -1;
                    exp_ = x_exp / digits;
                    if(x_exp % digits != 0){ ++exp_; }
                    exp_.get_raw_value().get_mpz_t()->_mp_size *= -1;
                }
                std::size_t exp = exp_.get_raw_value().get_ui();
                std::size_t bit_counter = 0;
                double frac_bit = 0.5;
                if(exp_ > 0){
                    for(
                        ;
                        (bit_counter >> shift) < prec &&
                            bit_counter < static_cast<std::size_t>(x_exp) &&
                            x_frac > 0 &&
                            frac_bit > 0;
                        ++bit_counter
                    ){
                        frac_ <<= 1;
                        if(x_frac >= frac_bit){
                            frac_ += 1;
                            x_frac -= frac_bit;
                        }
                        frac_bit /= 2.0;
                    }
                    bit_counter = 0;
                }
                if(exp < prec){
                    std::size_t n = prec - exp;
                    frac_ <<= n * digits;
                    if(z->_mp_size == 0){
                        mpz_realloc2(z, prec * digits);
                        mpn_zero(z->_mp_d, prec);
                        z->_mp_size = z->_mp_alloc;
                    }
                    data = z->_mp_d;
                    for(
                        ;
                        (bit_counter >> shift) < prec && x_frac > 0 && frac_bit > 0;
                        ++bit_counter
                    ){
                        if(x_frac >= frac_bit){
                            data[n - 1 - (bit_counter >> shift)] |= 1 << (digits - 1 - (bit_counter & mask));
                            x_frac -= frac_bit;
                        }
                        frac_bit /= 2.0;
                    }
                }
                if(!sign){ z->_mp_size *= -1; }
            }

            integer integer_portion() const{
                if(exp_ <= 0){ return integer::mp_type(0); }
                const std::size_t
                    digits = std::numeric_limits<mp_limb_t>::digits,
                    prec = mpz_size(frac_.get_raw_value().get_mpz_t()),
                    exp = exp_.get_raw_value().get_ui();
                if(exp < prec){
                    return frac_ >> ((prec - exp) * digits);
                }else{
                    return frac_ << ((exp - prec) * digits);
                }
            }

            fpoint frac_portion() const{
                fpoint x;
                mpz_srcptr z = frac_.get_raw_value().get_mpz_t();
                mpz_ptr w = x.frac_.get_raw_value().get_mpz_t();
                const std::size_t
                    digits = std::numeric_limits<mp_limb_t>::digits,
                    prec = mpz_size(z),
                    e = exp_.get_raw_value().get_ui();
                std::size_t n = prec - e;
                while(z->_mp_d[prec - e - n] == 0){ --n; }
                mpz_realloc2(w, n * digits);
                w->_mp_size = w->_mp_alloc;
                mpn_copyi(w->_mp_d, z->_mp_d + prec - e - n, n);
                return x;
            }

            inline static void set_prec(std::size_t value){
                if(value < 2){ value = 2; }
                prec_impl() = value;
            }

            inline static std::size_t get_prec(){
                return prec_impl();
            }

            inline const integer &exp() const{
                return exp_;
            }

            inline const integer &frac() const{
                return frac_;
            }

        private:
            inline static std::size_t rightmost_zero_seq_num(std::size_t i){
                std::size_t n = 0;
                while((i & 1) == 0){
                    ++n;
                    i >>= 1;
                }
                return n;
            }

            inline static std::size_t &prec_impl(){
                static std::size_t value = 2;
                return value;
            }

            integer exp_, frac_;
        };
    }

    typedef aux::fpoint<void> fpoint;
}

// test
#include <iostream>

int main(){
    cmpxx::fpoint::set_prec(8);
    cmpxx::fpoint f = 4294967296.0 + 1.0 / 2.0 + 1.0 / 8.0 + 1.0 / 16.0;
    std::cout
        << f.integer_portion().get_raw_value().get_str(16) << "\n"
        << f.frac_portion().frac().get_raw_value().get_str(16) << "\n";

    return 0;
}
