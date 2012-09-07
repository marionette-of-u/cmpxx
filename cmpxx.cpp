#include <memory>
#include <utility>
#include <limits>
#include <cmath>
#include <cstdlib>
#include <cassert>
#include <gmpxx.h>

// debug
#include <iostream>

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

            #define CMPXX_MP_WRAPPER_DEFINE_ASSIGN_OPERATOR(type) \
                inline mp_wrapper &operator =(type value){        \
                    get_raw_value() = value;                      \
                    return *this;                                 \
                }

            CMPXX_MP_WRAPPER_DEFINE_ASSIGN_OPERATOR(signed char);
            CMPXX_MP_WRAPPER_DEFINE_ASSIGN_OPERATOR(unsigned char);
            CMPXX_MP_WRAPPER_DEFINE_ASSIGN_OPERATOR(signed int);
            CMPXX_MP_WRAPPER_DEFINE_ASSIGN_OPERATOR(unsigned int);
            CMPXX_MP_WRAPPER_DEFINE_ASSIGN_OPERATOR(signed short int);
            CMPXX_MP_WRAPPER_DEFINE_ASSIGN_OPERATOR(unsigned short int);
            CMPXX_MP_WRAPPER_DEFINE_ASSIGN_OPERATOR(signed long int);
            CMPXX_MP_WRAPPER_DEFINE_ASSIGN_OPERATOR(unsigned long int);
            CMPXX_MP_WRAPPER_DEFINE_ASSIGN_OPERATOR(float);
            CMPXX_MP_WRAPPER_DEFINE_ASSIGN_OPERATOR(double);
            CMPXX_MP_WRAPPER_DEFINE_ASSIGN_OPERATOR(long double);

            #define CMPXX_MP_WRAPPER_DEFINE_COMPOUND_OPERATOR_IMPL(op, type) \
                inline mp_wrapper &op(type value){                           \
                    get_raw_value().op(value);                               \
                    return *this;                                            \
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
                CMPXX_MP_WRAPPER_DEFINE_COMPOUND_OPERATOR_IMPL(op, signed char);        \
                CMPXX_MP_WRAPPER_DEFINE_COMPOUND_OPERATOR_IMPL(op, unsigned char);      \
                CMPXX_MP_WRAPPER_DEFINE_COMPOUND_OPERATOR_IMPL(op, signed int);         \
                CMPXX_MP_WRAPPER_DEFINE_COMPOUND_OPERATOR_IMPL(op, unsigned int);       \
                CMPXX_MP_WRAPPER_DEFINE_COMPOUND_OPERATOR_IMPL(op, signed short int);   \
                CMPXX_MP_WRAPPER_DEFINE_COMPOUND_OPERATOR_IMPL(op, unsigned short int); \
                CMPXX_MP_WRAPPER_DEFINE_COMPOUND_OPERATOR_IMPL(op, signed long int);    \
                CMPXX_MP_WRAPPER_DEFINE_COMPOUND_OPERATOR_IMPL(op, unsigned long int);  \
                CMPXX_MP_WRAPPER_DEFINE_COMPOUND_OPERATOR_IMPL(op, float);              \
                CMPXX_MP_WRAPPER_DEFINE_COMPOUND_OPERATOR_IMPL(op, double);             \
                CMPXX_MP_WRAPPER_DEFINE_COMPOUND_OPERATOR_IMPL(op, long double);

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

#define CMPXX_MP_WRAPPER_DEFINE_BUILT_IN_TYPE_OVERLOAD_IMPL(op, e, type, bigtype, r_type) \
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

#define CMPXX_MP_WRAPPER_DEFINE_BUILT_IN_TYPE_OVERLOAD_IMPL_S(op, e, t, r) \
    CMPXX_MP_WRAPPER_DEFINE_BUILT_IN_TYPE_OVERLOAD_IMPL(op, e, t, signed long int, r)

#define CMPXX_MP_WRAPPER_DEFINE_BUILT_IN_TYPE_OVERLOAD_IMPL_U(op, e, t, r) \
    CMPXX_MP_WRAPPER_DEFINE_BUILT_IN_TYPE_OVERLOAD_IMPL(op, e, t, unsigned long int, r)

#define CMPXX_MP_WRAPPER_DEFINE_BUILT_IN_TYPE_OVERLOAD_IMPL_D(op, e, t, r) \
    CMPXX_MP_WRAPPER_DEFINE_BUILT_IN_TYPE_OVERLOAD_IMPL(op, e, t, double, r)

#define CMPXX_MP_WRAPPER_DEFINE_BUILT_IN_TYPE_OVERLOAD_IMPL_LD(op, e, t, r) \
    CMPXX_MP_WRAPPER_DEFINE_BUILT_IN_TYPE_OVERLOAD_IMPL(op, e, t, long double, r)

#define CMPXX_MP_WRAPPER_DEFINE_BUILT_IN_TYPE_OVERLOAD(op, e, r)                        \
    CMPXX_MP_WRAPPER_DEFINE_BUILT_IN_TYPE_OVERLOAD_IMPL_S(op, e, signed char, r)        \
    CMPXX_MP_WRAPPER_DEFINE_BUILT_IN_TYPE_OVERLOAD_IMPL_U(op, e, unsigned char, r)      \
    CMPXX_MP_WRAPPER_DEFINE_BUILT_IN_TYPE_OVERLOAD_IMPL_S(op, e, signed int, r)         \
    CMPXX_MP_WRAPPER_DEFINE_BUILT_IN_TYPE_OVERLOAD_IMPL_U(op, e, unsigned int, r)       \
    CMPXX_MP_WRAPPER_DEFINE_BUILT_IN_TYPE_OVERLOAD_IMPL_S(op, e, signed short int, r)   \
    CMPXX_MP_WRAPPER_DEFINE_BUILT_IN_TYPE_OVERLOAD_IMPL_U(op, e, unsigned short int, r) \
    CMPXX_MP_WRAPPER_DEFINE_BUILT_IN_TYPE_OVERLOAD_IMPL_S(op, e, signed long int, r)    \
    CMPXX_MP_WRAPPER_DEFINE_BUILT_IN_TYPE_OVERLOAD_IMPL_U(op, e, unsigned long int, r)  \
    CMPXX_MP_WRAPPER_DEFINE_BUILT_IN_TYPE_OVERLOAD_IMPL_D(op, e, float, r)              \
    CMPXX_MP_WRAPPER_DEFINE_BUILT_IN_TYPE_OVERLOAD_IMPL_D(op, e, double, r)             \
    CMPXX_MP_WRAPPER_DEFINE_BUILT_IN_TYPE_OVERLOAD_IMPL_LD(op, e, long double, r)

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
    )                                                           \
    CMPXX_MP_WRAPPER_DEFINE_BUILT_IN_TYPE_OVERLOAD(             \
        op,                                                     \
        e,                                                      \
        CMPXX_WRAPPER_DEFINE_BUILT_IN_TYPE_OVERLOAD_RETURN_TYPE \
    )

#define CMPXX_MP_WRAPPER_DEFINE_BOOLEAN_OVERLOAD(op, e)  \
    CMPXX_MP_WRAPPER_DEFINE_EXPR_OVERLOAD(               \
        op,                                              \
        e,                                               \
        CMPXX_WRAPPER_DEFINE_OVERLOAD_RETURN_TYPE_BOOL   \
    )                                                    \
    CMPXX_MP_WRAPPER_DEFINE_BUILT_IN_TYPE_OVERLOAD(      \
        op,                                              \
        e,                                               \
        CMPXX_WRAPPER_DEFINE_OVERLOAD_RETURN_TYPE_BOOL   \
    )

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
CMPXX_MP_WRAPPER_DEFINE_UNARY_OVERLOAD(abs, __gmp_abs_function)
CMPXX_MP_WRAPPER_DEFINE_UNARY_OVERLOAD(trunc, __gmp_trunc_function)
CMPXX_MP_WRAPPER_DEFINE_UNARY_OVERLOAD(floor, __gmp_floor_function)
CMPXX_MP_WRAPPER_DEFINE_UNARY_OVERLOAD(ceil, __gmp_ceil_function)
CMPXX_MP_WRAPPER_DEFINE_UNARY_OVERLOAD(sqrt, __gmp_sqrt_function)

namespace cmpxx{
    typedef aux::mp_wrapper<mpz_class> integer;
    typedef aux::mp_wrapper<mpq_class> rational;
    
    namespace aux{
        template<std::size_t n>
        struct calc_rightmost_zero_seq_num{
            static const std::size_t value = 1 + calc_rightmost_zero_seq_num<(n >> 1)>::value;
        };

        template<>
        struct calc_rightmost_zero_seq_num<0>{
            static const std::size_t value = 0;
        };

        template<class Type>
        struct mp_info{
            static const std::size_t
                digits = std::numeric_limits<Type>::digits,
                mask = digits - 1,
                rightmost_zero_seq_num = calc_rightmost_zero_seq_num<digits>::value - 1;
        };

        template<class Type>
        class mp_float{
        public:
            typedef Type type;
            typedef mp_info<mp_limb_t> mp_info_type;
            static const std::size_t default_prec = 4;

            inline mp_float() : exp_(), frac_(){}

            inline mp_float(const mp_float &other) : exp_(other.exp_), frac_(other.frac_){}

            inline mp_float(mp_float &&other) :
                exp_(), frac_()
            { swap(other); }

            mp_float(double x) : exp_(), frac_(){
                set(x);
            }

            mp_float(long double x) : exp_(), frac_(){
                set(x);
            }

            inline mp_float &operator =(const mp_float &other){
                exp_ = other.exp_;
                frac_ = other.frac_;
                return *this;
            }

            inline mp_float &operator =(mp_float &&other){
                swap(other);
                return *this;
            }

            inline bool operator <(const mp_float &other) const{
                return less(*this, other);
            }

            inline bool operator <=(const mp_float &other) const{
                return less_equal(*this, other);
            }

            inline bool operator >(const mp_float &other) const{
                return greater(*this, other);
            }

            inline bool operator >=(const mp_float &other) const{
                return greater_equal(*this, other);
            }

            inline bool operator ==(const mp_float &other) const{
                return equal(*this, other);
            }

            inline bool operator !=(const mp_float &other) const{
                return not_equal(*this, other);
            }

            integer integer_portion() const{
                if(exp_ <= 0){ return integer::mp_type(0); }
                const std::size_t
                    digits = mp_info_type::digits,
                    p = prec();
                if(exp_ < static_cast<int>(p)){
                    return frac_ >> ((p - exp_) * digits);
                }else{
                    return frac_ << ((exp_ - p) * digits);
                }
            }

            mp_float frac_portion() const{
                if(exp_ <= 0){ return *this; }
                mp_float x;
                mpz_srcptr z = frac_.get_raw_value().get_mpz_t();
                mpz_ptr w = x.frac_.get_raw_value().get_mpz_t();
                const std::size_t
                    digits = mp_info_type::digits,
                    p = prec(),
                    e = static_cast<std::size_t>(exp_);
                std::size_t n = p - e;
                do{
                    if(n == 0){ return x; }
                    --n;
                }while(z->_mp_d[p - e - n] == 0);
                if(n == 0){ return x; }
                mpz_realloc2(w, n * digits);
                w->_mp_size = w->_mp_alloc;
                mpn_copyi(w->_mp_d, z->_mp_d + p - e - n, n);
                return x;
            }

            inline int exp() const{
                return exp_;
            }

            inline const integer &frac() const{
                return frac_;
            }

            inline const int sign() const{
                return mpz_sgn(frac_.get_raw_value().get_mpz_t());
            }

            inline std::size_t prec() const{
                return mpz_size(frac_.get_raw_value().get_mpz_t());
            }

            inline static std::size_t get_global_prec(){
                return prec_impl();
            }

            inline static void set_global_prec(std::size_t value){
                if(value < default_prec){ value = default_prec; }
                prec_impl() = value;
            }

            inline static mp_float add(const mp_float &lhs, const mp_float &rhs){
                mp_float r;
                add(r, lhs, rhs);
                return r;
            }

            inline static mp_float &add(mp_float &r, const mp_float &lhs, const mp_float &rhs){
                int lhs_sign = sgn(lhs.frac_.get_raw_value()), rhs_sign = sgn(rhs.frac_.get_raw_value());
                mpz_abs(r.frac_.get_raw_value().get_mpz_t(), r.frac_.get_raw_value().get_mpz_t());
                if(lhs_sign == rhs_sign){
                    if(lhs.exp_ >= rhs.exp_){
                        add_impl(r, lhs, rhs);
                        r.mul_sign(lhs_sign);
                        return r;
                    }else{
                        add_impl(r, rhs, lhs);
                        r.mul_sign(lhs_sign);
                        return r;
                    }
                }else{
                    if(lhs >= rhs){
                        sub_impl(r, lhs, rhs);
                        r.mul_sign(lhs_sign);
                        return r;
                    }else{
                        sub_impl(r, rhs, lhs);
                        r.mul_sign(rhs_sign);
                        return r;
                    }
                }
            }

            inline static mp_float sub(const mp_float &lhs, const mp_float &rhs){
                mp_float r;
                sub(r, lhs, rhs);
                return r;
            }

            inline static mp_float &sub(mp_float &r, const mp_float &lhs, const mp_float &rhs){
                int lhs_sign = lhs.sign(), rhs_sign = rhs.sign();
                mpz_abs(r.frac_.get_raw_value().get_mpz_t(), r.frac_.get_raw_value().get_mpz_t());
                if(lhs_sign == rhs_sign){
                    if(lhs >= rhs){
                        sub_impl(r, lhs, rhs);
                        r.mul_sign(lhs_sign);
                        return r;
                    }else{
                        sub_impl(r, rhs, lhs);
                        r.mul_sign(rhs_sign);
                        return r;
                    }
                }else{
                    if(lhs.exp_ >= rhs.exp_){
                        add_impl(r, lhs, rhs);
                        r.mul_sign(lhs_sign);
                        return r;
                    }else{
                        add_impl(r, rhs, lhs);
                        r.mul_sign(lhs_sign);
                        return r;
                    }
                }
            }

            inline static mp_float mul(const mp_float &lhs, const mp_float &rhs){
                mp_float r;
                mul(r, lhs, rhs);
                return r;
            }

            inline static mp_float &mul(mp_float &r, const mp_float &lhs, const mp_float &rhs){
                const std::size_t p = lhs.prec(), q = rhs.prec();
                std::size_t o;
                r.exp_ = lhs.exp_ + rhs.exp_;
                if(p == q){
                    r.frac_ = lhs.frac_ * rhs.frac_;
                    o = p;
                }else if(p > q){
                    r.frac_ = lhs.frac_ * (rhs.frac_ << (mp_info_type::digits * (p - q)));
                    o = p;
                }else /* if(p < q) */{
                    r.frac_ = (lhs.frac_ << (mp_info_type::digits * (q - p))) * rhs.frac_;
                    o = q;
                }
                mpz_ptr z = r.frac_.get_raw_value().get_mpz_t();
                mp_limb_t *data = z->_mp_d;
                r.frac_ += ~data[0] + 1;
                data = z->_mp_d;
                mpn_copyi(data, data + o, o);
                z->_mp_size = o;
                r.normalize(o);
                return r;
            }

            inline static mp_float div(const mp_float &lhs, const mp_float &rhs){
                mp_float r;
                div(r, lhs, rhs);
                return r;
            }

            inline static mp_float &div(mp_float &r, const mp_float &lhs, const mp_float &rhs){
                const std::size_t p = lhs.prec(), q = rhs.prec();
                std::size_t o;
                r.exp_ = lhs.exp_ - rhs.exp_ + 1;
                if(p == q){
                    o = p;
                    r.frac_ = ((lhs.frac_ << (mp_info_type::digits * o)) / rhs.frac_) >> mp_info_type::digits;
                }else if(p > q){
                    o = p;
                    r.frac_ = ((lhs.frac_ << (mp_info_type::digits * o)) / (rhs.frac_ << (mp_info_type::digits * (p - q)))) >> mp_info_type::digits;
                }else /* if(p < q) */{
                    o = q;
                    r.frac_ = ((lhs.frac_ << (mp_info_type::digits * (o + (q - p)))) / rhs.frac_) >> mp_info_type::digits;
                }
                r.normalize(o);
                return r;
            }

            inline static bool qeual(const mp_float &lhs, const mp_float &rhs){
                lhs.exp_ == rhs.exp_ && lhs.frac_ == rhs.frac_;
            }

            inline static bool not_equal(const mp_float &lhs, const mp_float &rhs){
                return !equal(lhs, rhs);
            }

            inline static bool less(const mp_float &lhs, const mp_float &rhs){
                int lhs_sign = lhs.sign();
                if(lhs_sign == rhs.sign()){
                    if(lhs_sign){
                        return lhs.exp_ < rhs.exp_ || (lhs.exp_ == rhs.exp_ && lhs.frac_ < rhs.frac_);
                    }else{
                        return lhs.exp_ > rhs.exp_ || (lhs.exp_ == rhs.exp_ && lhs.frac_ > rhs.frac_);
                    }
                }else{
                    return lhs_sign < 0;
                }
            }

            inline static bool greater(const mp_float &lhs, const mp_float &rhs){
                return less(rhs, lhs);
            }

            inline static bool less_equal(const mp_float &lhs, const mp_float &rhs){
                int lhs_sign = lhs.sign(), rhs_sign = rhs.sign();
                if(lhs_sign == rhs_sign){
                    if(lhs_sign >= 0){
                        if(lhs.exp_ < rhs.exp_){
                            return true;
                        }else if(lhs.exp_ == rhs.exp_){
                            return lhs.frac_ <= rhs.frac_;
                        }else{
                            return false;
                        }
                    }else{
                        if(lhs.exp_ > rhs.exp_){
                            return true;
                        }else if(lhs.exp_ == rhs.exp_){
                            return lhs.frac_ >= rhs.frac_;
                        }else{
                            return false;
                        }
                    }
                }else{
                    return (lhs_sign < 0 && rhs_sign) || (lhs.frac_ == 0 && rhs.frac_ == 0);
                }
            }

            inline static bool greater_equal(const mp_float &lhs, const mp_float &rhs){
                return less_equal(rhs, lhs);
            }

        private:
            inline static mp_float &add_impl(mp_float &r, const mp_float &lhs, const mp_float &rhs){
                const std::size_t p = lhs.prec(), q = rhs.prec();
                r.exp_ = lhs.exp_;
                int exp_diff = lhs.exp_ - rhs.exp_;
                if(exp_diff >= static_cast<int>(p + 2)){
                    r.frac_ = lhs.frac_;
                    return r;
                }
                if(p == q){
                    r.frac_ = (rhs.frac_ >> (exp_diff * mp_info_type::digits)) + lhs.frac_;
                    r.normalize(p);
                }else if(p > q){
                    r.frac_ = ((rhs.frac_ << ((p - q) * mp_info_type::digits)) >> (exp_diff * mp_info_type::digits)) + lhs.frac_;
                    r.normalize(p);
                }else /* if(p < q) */{
                    r.frac_ = (rhs.frac_ >> (exp_diff * mp_info_type::digits)) + (lhs.frac_ << ((q - p) * mp_info_type::digits));
                    r.normalize(q);
                }
                return r;
            }

            inline static mp_float &sub_impl(mp_float &r, const mp_float &lhs, const mp_float &rhs){
                const std::size_t p = lhs.prec(), q = rhs.prec();
                r.exp_ = lhs.exp_;
                int exp_diff = lhs.exp_ - rhs.exp_;
                if(exp_diff >= static_cast<int>(p + 2)){
                    r.frac_ = lhs.frac_;
                    return r;
                }
                if(p == q){
                    r.frac_ = lhs.frac_ - (rhs.frac_ >> (exp_diff * mp_info_type::digits));
                    r.normalize(p);
                }else if(p > q){
                    r.frac_ = lhs.frac_ - ((rhs.frac_ << ((p - q) * mp_info_type::digits)) >> (exp_diff * mp_info_type::digits));
                    r.normalize(p);
                }else /* if(p < q) */{
                    r.frac_ = (lhs.frac_ << ((q - p) * mp_info_type::digits)) - (rhs.frac_ >> (exp_diff * mp_info_type::digits));
                    r.normalize(q);
                }
                std::cout << r.frac_.get_raw_value().get_str() << "\n";
                return r;
            }

            inline void normalize(const std::size_t p){
                // n1
                if(frac_ == 0){
                    exp_ = 0;
                    return;
                }else if(prec() > p){
                    goto n4;
                }

                // n2
                n2:;
                if(
                    prec() == p &&
                    (
                        exp_ > 0 ||
                        (exp_ == 0 && frac_ >= static_cast<mp_limb_t>(1 << (mp_info_type::digits - 1)))
                    )
                ){ goto n5; }

                // n3
                --exp_;
                frac_ <<= mp_info_type::digits;
                goto n2;

                // n4
                n4:;
                ++exp_;
                frac_ >>= mp_info_type::digits;

                // n5
                n5:;
                {
                    mp_limb_t u = frac_.get_raw_value().get_mpz_t()->_mp_d[0];
                    if(u != 0){
                        frac_ += ~u + 1;
                        if(frac_.get_raw_value().get_mpz_t()->_mp_d[p - 1] == 1){ goto n4; }
                    }
                }
            }

            template<class Float>
            void set(Float x){
                const std::size_t
                    digits = mp_info_type::digits,
                    mask = mp_info_type::mask,
                    shift = mp_info_type::rightmost_zero_seq_num;
                if(x == 0.0){ return; }
                bool sign = x >= 0.0;
                if(x < 0.0){ x = -x; }
                std::size_t p = get_global_prec();
                mpz_ptr z = frac_.get_raw_value().get_mpz_t();
                Float x_frac, x_integer, x_mod;
                int x_exp;
                x_frac = std::frexp(x, &x_exp);
                x_mod = std::modf(x, &x_integer);
                if(x_exp >= 0){
                    exp_ = x_exp / digits;
                    if(x_exp % digits != 0){ ++exp_; }
                }else{
                    x_exp *= -1;
                    exp_ = x_exp / digits;
                    if(x_exp % digits != 0){ ++exp_; }
                    exp_ *= -1;
                }
                mpz_realloc2(z, p * digits);
                mpn_zero(z->_mp_d, p);
                z->_mp_size = z->_mp_alloc;
                mp_limb_t *data = z->_mp_d;
                std::size_t n = exp_ > 0 ? p - exp_ : p;
                for(
                    std::size_t bit_counter = 0;
                    (bit_counter >> shift) < p && x_integer >= 1.0;
                    ++bit_counter, x_integer /= 2.0
                ){
                    if(std::fmod(x_integer, 2.0) >= 1.0){
                        data[n + (bit_counter >> shift)] |= 1 << (bit_counter & mask);
                    }
                }
                Float frac_bit = 0.5;
                for(
                    std::size_t bit_counter = 0;
                    (bit_counter >> shift) < p && x_frac > 0.0 && frac_bit > 0.0;
                    ++bit_counter, frac_bit /= 2.0
                ){
                    if(x_mod >= frac_bit){
                        data[n - 1 - (bit_counter >> shift)] |= 1 << (digits - 1 - (bit_counter & mask));
                        x_mod -= frac_bit;
                    }
                }
                if(!sign){ z->_mp_size *= -1; }
            }

            inline void swap(mp_float &other){
                typedef std::aligned_storage<
                    sizeof(integer),
                    std::alignment_of<integer>::value
                >::type pod_of_integer;
                std::swap(exp_, other.exp_);
                std::swap(
                    *static_cast<pod_of_integer*>(static_cast<void*>(&frac_)),
                    *static_cast<pod_of_integer*>(static_cast<void*>(&other.frac_))
                );
            }

            inline void mul_sign(int s){
                frac_.get_raw_value().get_mpz_t()->_mp_size *= s;
            }

            inline void set_sign(int s){
                int a = frac_.get_raw_value().get_mpz_t()->_mp_size;
                a = std::abs(a);
                a *= s;
                frac_.get_raw_value().get_mpz_t()->_mp_size = a;
            }

            inline std::size_t frac_digits() const{
                const mp_limb_t *a = frac_.get_raw_value().get_mpz_t()->_mp_d;
                if(frac_ == 0){ return 0; }
                std::size_t n = 0;
                while(a[n] == 0){ ++n; }
                return prec() - n;
            }

            inline static std::size_t &prec_impl(){
                static std::size_t value = default_prec;
                return value;
            }

            integer frac_;
            int exp_;

            static_assert(sizeof(int) >= sizeof(mp_limb_t), "sizeof(int) < sizeof(mp_limb_t)");
        };
    }

    typedef aux::mp_float<void> mp_float;
}

// test
#include <iostream>

int main(){
    cmpxx::mp_float lhs = 1.0;
    std::cout << "lhs : " << lhs.frac().get_raw_value().get_str(16) << "\n";
    cmpxx::mp_float rhs = 8.0;
    std::cout << "rhs : " << rhs.frac().get_raw_value().get_str(16) << "\n";
    cmpxx::mp_float r;
    r = cmpxx::mp_float::div(lhs, rhs);
    std::cout << r.integer_portion().get_raw_value().get_str(16) << "\n";
    std::cout << r.frac_portion().frac().get_raw_value().get_str(16) << "\n";

    return 0;
}
