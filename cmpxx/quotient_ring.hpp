#ifndef INCLUDE_CMPXX_QUOTIENT_RING
#define INCLUDE_CMPXX_QUOTIENT_RING

// quotient ring

#include <memory>
#include <utility>
#include "aux_xx.hpp"

namespace cmpxx{
    CMPXX_AUX_GENERATE_TMP_TMP_PARAM_ET(quotient_ring, (class Type), (Type), (class));

    template<class Type>
    class quotient_ring{
    public:
        using type = Type;

        inline quotient_ring() :
            value_(), p_(nullptr)
        {}

        inline quotient_ring(const quotient_ring &other) :
            value_(other.value_), p_(other.p_)
        {}

        inline quotient_ring(quotient_ring &&other) :
            value_(), p_(nullptr)
        {
            swap_value(other.value_);
            swap_p(other.p_);
        }

        inline quotient_ring(const type &value, const type &p) :
            value_(value), p_(new type(p))
        {}

        inline quotient_ring(const type &value, aux::shared_ptr<type> p) :
            value_(value), p_(p)
        {}

        inline quotient_ring &operator =(const quotient_ring &other){
            value_ = other.value_;
            p_ = other.p_;
            return *this;
        }

        inline quotient_ring &operator =(quotient_ring &&other){
            swap_value(other.value_);
            swap_p(other.p_);
            return *this;
        }

        template<template<class> class Other, class Op, class L, class R>
        inline quotient_ring &operator =(const expression_template_quotient_ring<Other, type, Op, L, R>  &expression){
            quotient_ring temp;
            temp.p_ = p_;
            expression.eval(temp);
            *this = std::move(temp);
            return *this;
        }

        #define CMPXX_QUOTIENT_RING_DEFINE_ASSIGN_OPERATOR(nil, t)  \
            inline quotient_ring &operator =(t value){              \
                value_ = value;                                     \
                return *this;                                       \
            }

        CMPXX_INVOKE_MACRO_WITH_BUILT_IN_TYPE(CMPXX_QUOTIENT_RING_DEFINE_ASSIGN_OPERATOR, nil);

        #define CMPXX_QUOTIENT_RING_DEFINE_COMPOUND_OPERATOR(op)            \
            inline quotient_ring &operator op(const quotient_ring &other){  \
                value_ op other.value_;                                     \
                return *this;                                               \
            }                                                               \
            template<template<class> class Other, class Op, class L, class R, bool Calculated>  \
            inline quotient_ring &operator op(                                                  \
                const expression_template_quotient_ring<Other, type, Op, L, R, Calculated> &e   \
            ){                                                                                  \
                quotient_ring temp;                                                             \
                e.eval(temp);                                                                   \
                value_ op temp.value_;                                                          \
                return *this;                                                                   \
            }

        CMPXX_QUOTIENT_RING_DEFINE_COMPOUND_OPERATOR(+=);
        CMPXX_QUOTIENT_RING_DEFINE_COMPOUND_OPERATOR(-=);
        CMPXX_QUOTIENT_RING_DEFINE_COMPOUND_OPERATOR(*=);
        CMPXX_QUOTIENT_RING_DEFINE_COMPOUND_OPERATOR(/=);
        CMPXX_QUOTIENT_RING_DEFINE_COMPOUND_OPERATOR(%=);

        inline void canonicalize(){
            value_ %= *p_;
        }

        inline const type &value() const{
            return value_;
        }

        inline const type &p() const{
            return *p_;
        }

        inline const aux::shared_ptr<type> &shared_ptr_p() const{
            return p_;
        }

    private:     
        inline void swap_value(type &value){
            using pod_of_type = typename std::aligned_storage<
                sizeof(type),
                std::alignment_of<type>::value
            >::type;
            std::swap(
                *static_cast<pod_of_type*>(static_cast<void*>(&value)),
                *static_cast<pod_of_type*>(static_cast<void*>(&value_))
            );
        }

        inline void swap_p(aux::shared_ptr<type> &sp){
            p_.swap(sp);
        }

        type value_;
        aux::shared_ptr<type> p_;
    };

    #define CMPXX_QUOTIENT_RING_BUILT_IN_TYPE_OPERATOR_OVERLOAD(op, a_op, t)        \
        template<class Type>                                                        \
        inline quotient_ring<Type> operator op(const quotient_ring<Type> &l, t v){  \
            quotient_ring<Type> ret(l);                                             \
            ret a_op quotient_ring<Type>(v, l.shared_ptr_p());                      \
            return ret;                                                             \
        }                                                                           \
        template<class Type>                                                        \
        inline quotient_ring<Type> operator op(t v, const quotient_ring<Type> &r){  \
            quotient_ring<Type> ret(v, r.shared_ptr_p());                           \
            ret a_op r;                                                             \
            return ret;                                                             \
        }

    #define CMPXX_QUOTIENT_RING_CANONICALIZE_BUILT_IN_TYPE_OPERATOR_OVERLOAD(op, a_op, t)   \
        template<class Type>                                                                \
        inline quotient_ring<Type> operator op(const quotient_ring<Type> &l, t v){          \
            quotient_ring<Type> ret(l);                                                     \
            ret a_op quotient_ring<Type>(v, l.shared_ptr_p());                              \
            ret.canonicalize();                                                             \
            return ret;                                                                     \
        }                                                                                   \
        template<class Type>                                                                \
        inline quotient_ring<Type> operator op(t v, const quotient_ring<Type> &r){          \
            quotient_ring<Type> ret(v, r.shared_ptr_p());                                   \
            ret a_op r;                                                                     \
            ret.canonicalize();                                                             \
            return ret;                                                                     \
        }


    CMPXX_INVOKE_MACRO_WITH_BUILT_IN_TYPE(CMPXX_QUOTIENT_RING_BUILT_IN_TYPE_OPERATOR_OVERLOAD, +, +=);
    CMPXX_INVOKE_MACRO_WITH_BUILT_IN_TYPE(CMPXX_QUOTIENT_RING_BUILT_IN_TYPE_OPERATOR_OVERLOAD, -, -=);
    CMPXX_INVOKE_MACRO_WITH_BUILT_IN_TYPE(CMPXX_QUOTIENT_RING_BUILT_IN_TYPE_OPERATOR_OVERLOAD, %, %=);
    CMPXX_INVOKE_MACRO_WITH_BUILT_IN_TYPE(CMPXX_QUOTIENT_RING_CANONICALIZE_BUILT_IN_TYPE_OPERATOR_OVERLOAD, *, *=);
    CMPXX_INVOKE_MACRO_WITH_BUILT_IN_TYPE(CMPXX_QUOTIENT_RING_CANONICALIZE_BUILT_IN_TYPE_OPERATOR_OVERLOAD, /, /=);

    namespace aux{
        template<bool Commutative = true>
        struct quotient_exp_operator_mul{
            static const bool commutative = Commutative;
            static const std::size_t operand_num = 2;

            template<class T, class U>
            inline static T &apply(T &target, const U &operand){
                target *= operand;
                if(target.shared_ptr_p()){
                    target.canonicalize();
                }
                return target;
            }
        };

        template<bool Commutative = false>
        struct quotient_exp_operator_div{
            static const bool commutative = Commutative;
            static const std::size_t operand_num = 2;

            template<class T, class U>
            inline static T &apply(T &target, const U &operand){
                target /= operand;
                if(target.shared_ptr_p()){
                    target.canonicalize();
                }
                return target;
            }
        };
    }

    CMPXX_AUX_TMP_TMP_ET_OPERATOR_OVERLOAD(quotient_ring, (class Type), (Type), (class), exp_operator_add<>, +);
    CMPXX_AUX_TMP_TMP_ET_OPERATOR_OVERLOAD(quotient_ring, (class Type), (Type), (class), exp_operator_sub<>, -);
    CMPXX_AUX_TMP_TMP_ET_OPERATOR_OVERLOAD(quotient_ring, (class Type), (Type), (class), quotient_exp_operator_mul<>, *);
    CMPXX_AUX_TMP_TMP_ET_OPERATOR_OVERLOAD(quotient_ring, (class Type), (Type), (class), quotient_exp_operator_div<>, /);
    CMPXX_AUX_TMP_TMP_ET_OPERATOR_OVERLOAD(quotient_ring, (class Type), (Type), (class), exp_operator_rem<>, %);
}

#endif

