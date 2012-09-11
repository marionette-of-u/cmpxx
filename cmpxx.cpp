#include <memory>
#include <utility>
#include <limits>
#include <sstream>
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

        template<class MPClass>
        std::ostream &operator <<(std::ostream &ostream, const mp_wrapper<MPClass> &value){
            ostream << value.get_raw_value().get_str();
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
CMPXX_MP_WRAPPER_DEFINE_UNARY_OVERLOAD(abs, __gmp_abs_function);
CMPXX_MP_WRAPPER_DEFINE_UNARY_OVERLOAD(trunc, __gmp_trunc_function);
CMPXX_MP_WRAPPER_DEFINE_UNARY_OVERLOAD(floor, __gmp_floor_function);
CMPXX_MP_WRAPPER_DEFINE_UNARY_OVERLOAD(ceil, __gmp_ceil_function);
CMPXX_MP_WRAPPER_DEFINE_UNARY_OVERLOAD(sqrt, __gmp_sqrt_function);

namespace cmpxx{
    typedef aux::mp_wrapper<mpz_class> integer;
    typedef aux::mp_wrapper<mpq_class> rational;
    typedef aux::mp_wrapper<mpf_class> floating;
}

#include <utility>
#include <map>
#include <algorithm>
#include <iterator>

namespace cmpxx{
    template<class Order, class Coefficient, bool CommutativeRing, class Alloc = std::allocator<int>>
    class polynomial{
    private:
        typedef std::map<
            Order,
            Coefficient,
            std::less<Order>,
            typename Alloc::template rebind<
                std::pair<const Order, Coefficient>
            >::other
        > basic_ordered_container;

    public:
        typedef typename basic_ordered_container::key_type order;
        typedef typename basic_ordered_container::mapped_type coefficient;
        static const bool commutative_ring = CommutativeRing;

        class ordered_container : public basic_ordered_container{
        public:
            typedef basic_ordered_container base_type;
            typedef std::pair<const order&, const coefficient&> ref_value_type;

            inline ordered_container() : basic_ordered_container(){}

            template<class Iter>
            inline ordered_container(const Iter &first, const Iter &last) :
                basic_ordered_container(first, last)
            {}

            inline typename base_type::iterator assign(const ref_value_type &v){
                typename base_type::iterator iter;
                if(v.second == 0){
                    iter = base_type::find(v.first);
                    if(iter != base_type::end()){
                        base_type::erase(static_cast<typename base_type::const_iterator>(iter));
                    }
                    return base_type::end();
                }
                iter = base_type::find(v.first);
                if(iter != base_type::end()){
                    iter->second = v.second;
                }else{
                    base_type::insert(iter, std::make_pair(v.first, v.second));
                }
                return iter;
            }

            inline typename base_type::iterator add(const ref_value_type &v){
                std::pair<typename base_type::iterator, bool> result = base_type::insert(v);
                coefficient &coe(result.first->second);
                if(!result.second){
                    coe += v.second;
                }
                if(coe == 0){
                    base_type::erase(static_cast<typename base_type::const_iterator>(result.first));
                    return base_type::end();
                }else{
                    return result.first;
                }
            }

            inline typename base_type::iterator sub(const ref_value_type &v){
                std::pair<typename base_type::iterator, bool> result = base_type::insert(v);
                coefficient &coe(result.first->second);
                if(result.second){
                    result.first->second *= -1;
                }else{
                    coe -= v.second;
                }
                if(coe == 0){
                    base_type::erase(static_cast<typename base_type::const_iterator>(result.first));
                    return base_type::end();
                }
                return result.first;
            }

            inline typename base_type::iterator mul(const ref_value_type &v){
                typename base_type::iterator iter = base_type::find(v.first);
                if(iter == base_type::end()){
                    return base_type::end();
                }
                coefficient &coe(iter->second);
                coe *= v.second;
                if(coe == 0){
                    base_type::erase(static_cast<typename base_type::const_iterator>(iter));
                    return base_type::end();
                }
                return iter;
            }

            inline typename base_type::iterator div(const ref_value_type &v){
                typename base_type::iterator iter = base_type::find(v.first);
                if(iter == base_type::end()){
                    return base_type::end();
                }
                coefficient &coe(iter->second);
                coe /= v.second;
                if(coe == 0){
                    base_type::erase(static_cast<typename base_type::const_iterator>(iter));
                    return base_type::end();
                }
                return iter;
            }
        };

    public:
        inline polynomial() : container(){}
       
        inline polynomial(const polynomial &other) :
            container(other.container)
        {}

        inline polynomial(polynomial &&other) :
            container(other.container)
        {}

        inline polynomial(const coefficient &coe) :
            container()
        { if(coe != 0){ container.insert(ordered_container::value_type(0, coe)); } }

    private:
        inline polynomial(const ordered_container &container_) :
            container(container_)
        {}

        inline polynomial(
            const typename basic_ordered_container::const_iterator &first,
            const typename basic_ordered_container::const_iterator last
        ) : container(first, last)
        {}

    public:
        inline ~polynomial(){}

        inline const ordered_container &get_container() const{
            return container;
        }

        inline polynomial &operator =(const polynomial &other){
            container = other.container;
            return *this;
        }

        inline polynomial &operator =(polynomial &&other){
            swap_ordered_container(other);
            return *this;
        }

    private:
        class coefficient_proxy{
        public:
            inline coefficient_proxy(ordered_container *container_, const order &v) :
                container(container_),
                order_(v)
            {}

            inline coefficient_proxy(const coefficient_proxy &other) :
                container(other.container),
                order_(other.order_)
            {}

            inline coefficient_proxy(coefficient_proxy &&other) :
                container(other.container),
                order_(other.order_)
            {}

            inline const coefficient_proxy &operator =(const coefficient &v) const{
                if(v == 0){
                    typename ordered_container::const_iterator iter = container->find(order_);
                    if(iter != container->end()){
                        container->erase(iter);
                    }
                }else{
                    container->assign(typename ordered_container::ref_value_type(order_, v));
                }
                return *this;
            }

            inline const coefficient_proxy &operator +=(const coefficient &v) const{
                container->add(typename ordered_container::ref_value_type(order_, v));
                return *this;
            }

            inline const coefficient_proxy &operator -=(const coefficient &v) const{
                container->sub(typename ordered_container::ref_value_type(order_, v));
                return *this;
            }

            inline const coefficient_proxy &operator *=(const coefficient &v) const{
                container->mul(typename ordered_container::ref_value_type(order_, v));
                return *this;
            }

            inline const coefficient_proxy &operator /=(const coefficient &v) const{
                container->div(typename ordered_container::ref_value_type(order_, v));
                return *this;
            }

            inline const coefficient_proxy &operator ()(const coefficient &v) const{
                return operator =(v);
            }

            inline coefficient_proxy operator [](const order &v) const{
                coefficient_proxy r(container, v);
                return r;
            }

        private:
            ordered_container *container;
            const order &order_;
        };

    public:
        inline coefficient_proxy operator [](const order &order_){
            return coefficient_proxy(&container, order_);
        }

        template<class Variable>
        inline std::string get_str(const Variable &v) const{
            return get_str_impl<std::string, char, Variable, std::ostringstream>(v, '*', '^', '+', '-', '0', '(', ')');
        }

        inline std::string get_str() const{
            return get_str("x");
        }

    private:
        template<class Str, class Char, class Variable, class OStringStream>
        inline Str get_str_impl(
            const Variable &v,
            Char mul,
            Char pow,
            Char pls,
            Char min,
            Char zero,
            Char l_pare,
            Char r_pare
        ) const{
            Str result;
            if(container.empty()){
                result += zero;
                return result;
            }
            typename ordered_container::const_reverse_iterator first = container.rbegin();
            for(typename ordered_container::const_reverse_iterator iter = container.rbegin(), end = container.rend(); iter != end; ++iter){
                const order &order_(iter->first);
                const coefficient &coe(iter->second);
                if(coe == 0){ continue; }
                if(first != iter && coe > 0){
                    result += pls;
                }
                if(coe == 1){
                    if(order_ == 0){
                        OStringStream os;
                        os << coe;
                        result += os.str();
                    }
                }else if(coe == -1){
                    if(order_ == 0){
                        OStringStream os;
                        os << coe;
                        result += os.str();
                    }else{
                        result += min;
                    }
                }else if(coe > 0 || coe < 0){
                    OStringStream os;
                    os << coe;
                    result += os.str();
                }
                if(order_ != 0){
                    result += v;
                    if(order_ != 1){
                        result += pow;
                        if(order_ < 0){ result += l_pare; }
                        OStringStream os;
                        os << order_;
                        result += os.str();
                        if(order_ < 0){ result += r_pare; }
                    }
                }
            }
            return result;
        }

        inline void swap_ordered_container(polynomial &other){
            typedef typename std::aligned_storage<
                sizeof(ordered_container),
                std::alignment_of<ordered_container>::value
            >::type pod_of_ordered_container_type;
            std::swap(
                *static_cast<pod_of_ordered_container_type*>(static_cast<void*>(&other.container)),
                *static_cast<pod_of_ordered_container_type*>(static_cast<void*>(&container))
            );
            return *this;
        }

        ordered_container container;
    };
}

// test
#include <iostream>

int main(){
    typedef cmpxx::polynomial<cmpxx::integer, cmpxx::integer, true> poly;
    poly p;
    // assign
    p[10](1);
    // add
    p[0] += 10;
    // div
    p[0] /= 5;
    // add
    p[2] += 2;
    // mul
    p[2] *= 5;
    // sub
    p[5] -= 9;
    // assign
    p[6](1);
    std::cout << p.get_str() << "\n";
    return 0;
}
