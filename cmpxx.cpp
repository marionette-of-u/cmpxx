// debug

#include <iostream>
#include <string>
#include <typeinfo>
#include <cxxabi.h>

// caution!! memory leaks!!
namespace cmpxx{
    namespace debug{
        inline std::string demangle(const std::string &str){
            int state;
            return abi::__cxa_demangle(str.c_str(), 0, 0, &state);
        }
    }
}

// aux

#include <utility>
#include <string>
#include <sstream>
#include <gmpxx.h>

#define CMPXX_INVOKE_MACRO_WITH_BUILT_IN_TYPE(macro, op, e, r) \
    macro(op, e, r, signed char);        \
    macro(op, e, r, unsigned char);      \
    macro(op, e, r, signed int);         \
    macro(op, e, r, unsigned int);       \
    macro(op, e, r, signed short int);   \
    macro(op, e, r, unsigned short int); \
    macro(op, e, r, signed long int);    \
    macro(op, e, r, unsigned long int);  \
    macro(op, e, r, float);              \
    macro(op, e, r, double);             \
    macro(op, e, r, long double);

#define CMPXX_INVOKE_MACRO_WITH_S_U_D_LD(s, u, d, ld, op, e, r) \
    s(op, e, r, signed char)        \
    u(op, e, r, unsigned char)      \
    s(op, e, r, signed int)         \
    u(op, e, r, unsigned int)       \
    s(op, e, r, signed short int)   \
    u(op, e, r, unsigned short int) \
    s(op, e, r, signed long int)    \
    u(op, e, r, unsigned long int)  \
    d(op, e, r, float)              \
    d(op, e, r, double)             \
    ld(op, e, r, long double)

namespace cmpxx{
    namespace aux{
        template<bool Commutative = false>
        struct exp_operator_assign{
            static const bool commutative = Commutative;
            static const std::size_t operand_num = 2;

            template<class T>
            inline static T &apply(T &target, const T &operand){
                target = operand;
                return target;
            }
        };

        template<bool Commutative = true>
        struct exp_operator_add{
            static const bool commutative = Commutative;
            static const std::size_t operand_num = 2;

            template<class T>
            inline static T &apply(T &target, const T &operand){
                target += operand;
                return target;
            }
        };

        template<bool Commutative = false>
        struct exp_operator_sub{
            static const bool commutative = Commutative;
            static const std::size_t operand_num = 2;

            template<class T>
            inline static T &apply(T &target, const T &operand){
                target -= operand;
                return target;
            }
        };

        template<bool Commutative = true>
        struct exp_operator_mul{
            static const bool commutative = Commutative;
            static const std::size_t operand_num = 2;

            template<class T>
            inline static T &apply(T &target, const T &operand){
                target *= operand;
                return target;
            }
        };

        template<bool Commutative = false>
        struct exp_operator_div{
            static const bool commutative = Commutative;
            static const std::size_t operand_num = 2;

            template<class T>
            inline static T &apply(T &target, const T &operand){
                target /= operand;
                return target;
            }
        };

        template<bool Commutative = false>
        struct exp_operator_rem{
            static const bool commutative = Commutative;
            static const std::size_t operand_num = 2;

            template<class T>
            inline static T &apply(T &target, const T &operand){
                target %= operand;
                return target;
            }
        };

        template<bool Commutative = false>
        struct exp_operator_lshift{
            static const bool commutative = Commutative;
            static const std::size_t operand_num = 2;

            template<class T, class U>
            inline static T &apply(T &target, const U &operand){
                target <<= operand;
                return target;
            }
        };

        template<bool Commutative = false>
        struct exp_operator_rshift{
            static const bool commutative = Commutative;
            static const std::size_t operand_num = 2;

            template<class T, class U>
            inline static T &apply(T &target, const U &operand){
                target >>= operand;
                return target;
            }
        };

        template<bool Commutative = true>
        struct exp_operator_and{
            static const bool commutative = Commutative;
            static const std::size_t operand_num = 2;

            template<class T>
            inline static T &apply(T &target, const T &operand){
                target &= operand;
                return target;
            }
        };

        template<bool Commutative = true>
        struct exp_operator_or{
            static const bool commutative = Commutative;
            static const std::size_t operand_num = 2;

            template<class T>
            inline static T &apply(T &target, const T &operand){
                target |= operand;
                return target;
            }
        };

        template<bool Commutative = false>
        struct exp_operator_xor{
            static const bool commutative = Commutative;
            static const std::size_t operand_num = 2;

            template<class T>
            inline static T &apply(T &target, const T &operand){
                target ^= operand;
                return target;
            }
        };

        template<class PositiveFunctor>
        struct exp_operator_positive{
            static const bool commutative = false;
            static const std::size_t operand_num = 1;

            template<class T>
            inline static T &apply(T &target){
                PositiveFunctor()(target);
                return target;
            };
        };

        template<class NegativeFunctor>
        struct exp_operator_negative{
            static const bool commutative = false;
            static const std::size_t operand_num = 1;

            template<class T>
            inline static T &apply(T &target){
                NegativeFunctor()(target);
                return target;
            }
        };

        template<class IncFunctor>
        struct exp_operator_inc{
            static const bool commutative = false;
            static const std::size_t operand_num = 1;

            template<class T>
            inline static T &apply(T &target){
                IncFunctor()(target);
                return target;
            }
        };

        template<class DecFunctor>
        struct exp_operator_dec{
            static const bool commutative = false;
            static const std::size_t operand_num = 1;

            template<class T>
            inline static T &apply(T &target){
                DecFunctor()(target);
                return target;
            }
        };

        template<class AndFunctor, bool Commutative = true>
        struct exp_operator_logical_and{
            static const bool commutative = Commutative;
            static const std::size_t operand_num = 2;

            template<class T>
            inline static bool apply(T &target, const T &operand){
                return AndFunctor()(target, operand);
            }
        };

        template<class OrFunctor, bool Commutative = true>
        struct exp_operator_logical_or{
            static const bool commutative = Commutative;
            static const std::size_t operand_num = 2;

            template<class T>
            inline static bool apply(T &target, const T &operand){
                return OrFunctor()(target, operand);
            }
        };

        template<class NotFunctor>
        struct exp_operator_not{
            static const bool commutative = false;
            static const std::size_t operand_num = 1;

            template<class T>
            inline static bool apply(T &target){
                return NotFunctor()(target);
            }
        };

        template<class LessFunctor, bool Commutative = false>
        struct exp_operator_less{
            static const bool commutative = Commutative;
            static const std::size_t operand_num = 2;

            template<class T, class U>
            inline static bool apply(const T &target, const U &operand){
                return LessFunctor()(target, operand);
            }
        };

        template<class GreaterFunctor, bool Commutative = false>
        struct exp_operator_greater{
            static const bool commutative = Commutative;
            static const std::size_t operand_num = 2;

            template<class T, class U>
            inline static bool apply(const T &target, const U &operand){
                return GreaterFunctor()(target, operand);
            }
        };

        template<class LessEqualFunctor, bool Commutative = false>
        struct exp_operator_less_equal{
            static const bool commutative = Commutative;
            static const std::size_t operand_num = 2;

            template<class T, class U>
            inline static bool apply(const T &target, const U &operand){
                return LessEqualFunctor()(target, operand);
            }
        };

        template<class GreaterEqualFunctor, bool Commutative = false>
        struct exp_operator_greater_equal{
            static const bool commutative = Commutative;
            static const std::size_t operand_num = 2;

            template<class T, class U>
            inline static bool apply(const T &target, const U &operand){
                return GreaterEqualFunctor()(target, operand);
            }
        };

        template<class EqualFunctor, bool Commutative = true>
        struct exp_operator_equal{
            static const bool commutative = Commutative;
            static const std::size_t operand_num = 2;

            template<class T, class U>
            inline static bool apply(const T &target, const U &operand){
                return EqualFunctor()(target, operand);
            }
        };

        template<class NotEqualFunctor, bool Commutative = true>
        struct exp_operator_not_equal{
            static const bool commutative = Commutative;
            static const std::size_t operand_num = 2;

            template<class T, class U>
            inline static bool apply(const T &target, const U &operand){
                return NotEqualFunctor()(target, operand);
            }
        };

        template<class Operator, bool Commutative, bool Reverse, std::size_t OperandNum>
        struct specialized_evaluator;

        template<class Operator, bool Commutative, bool Reverse>
        struct specialized_evaluator<Operator, Commutative, Reverse, 1>{
            template<class T>
            inline static decltype(Operator::apply(T())) &eval(T &target){
                return Operator::apply(target);
            }
        };

        template<class Operator, bool Commutative, bool Reverse>
        struct specialized_evaluator<Operator, Commutative, Reverse, 2>{
             template<class T>
            inline static void eval(T &target, const T &operand){
                Operator::apply(target, operand);
            }

        };

        template<class Type>
        struct identity_expression{
            using type = Type;
            static const bool calculated = true;

            inline identity_expression(const type &value_a) :
                value_(value_a)
            {}

            inline identity_expression(const identity_expression &other) :
                value_(other.value_)
            {}

            inline void eval(type &result) const{
                result = value_;
            }

            const type &value_;
        };

        template<class Type, class BinaryOp, class L, class R, bool Calculated = false>
        struct binary_expression{
            using type = Type;
            using binary_operator = BinaryOp;
            using lhs = L;
            using rhs = R;
            static const bool calculated = Calculated;

            inline binary_expression(const lhs &lhs_a, const rhs &rhs_a) :
                lhs_(lhs_a), rhs_(rhs_a)
            {}

            inline binary_expression(const binary_expression &other) :
                lhs_(other.lhs_), rhs_(other.rhs_)
            {}

            inline void eval(type &result) const{
                type rhs;
                lhs_.eval(result);
                rhs_.eval(rhs);
                specialized_evaluator<
                    binary_operator,
                    binary_operator::commutative,
                    false,
                    binary_operator::operand_num
                >::eval(result, rhs);
            }

            const lhs &lhs_;
            const rhs &rhs_;
        };

        template<class Type, class BinaryOp, bool Calculated>
        struct binary_expression<Type, BinaryOp, identity_expression<Type>, identity_expression<Type>, Calculated>{
            using type = Type;
            using binary_operator = BinaryOp;
            using lhs = type;
            using rhs = type;
            static const bool calculated = Calculated;

            inline binary_expression(const lhs &lhs_a, const rhs &rhs_a) :
                lhs_(lhs_a), rhs_(rhs_a)
            {}

            inline binary_expression(const binary_expression &other) :
                lhs_(other.lhs_), rhs_(other.rhs_)
            {}

            inline void eval(type &result) const{
                result = lhs_;
                specialized_evaluator<
                    binary_operator,
                    binary_operator::commutative,
                    false,
                    binary_operator::operand_num
                >::eval(result, rhs_);
            }

            const lhs &lhs_;
            const rhs &rhs_;
        };

        template<class Type, class BinaryOp, class R, bool Calculated>
        struct binary_expression<Type, BinaryOp, identity_expression<Type>, R, Calculated>{
            using type = Type;
            using binary_operator = BinaryOp;
            using lhs = type;
            using rhs = R;
            static const bool calculated = Calculated;

            inline binary_expression(const lhs &lhs_a, const rhs &rhs_a) :
                lhs_(lhs_a), rhs_(rhs_a)
            {}

            inline binary_expression(const binary_expression &other) :
                lhs_(other.lhs_), rhs_(other.rhs_)
            {}

            inline void eval(type &result) const{
                type rhs;
                result = lhs_;
                rhs_.eval(rhs);
                specialized_evaluator<
                    binary_operator,
                    binary_operator::commutative,
                    false,
                    binary_operator::operand_num
                >::eval(result, rhs);
            }

            const lhs &lhs_;
            const rhs &rhs_;
        };

        template<class Type, class BinaryOp, class L, bool Calculated>
        struct binary_expression<Type, BinaryOp, L, identity_expression<Type>, Calculated>{
            using type = Type;
            using binary_operator = BinaryOp;
            using lhs = L;
            using rhs = type;
            static const bool calculated = Calculated;

            inline binary_expression(const lhs &lhs_a, const rhs &rhs_a) :
                lhs_(lhs_a), rhs_(rhs_a)
            {}

            inline binary_expression(const binary_expression &other) :
                lhs_(other.lhs_), rhs_(other.rhs_)
            {}

            inline void eval(type &result) const{
                lhs_.eval(result);
                specialized_evaluator<
                    binary_operator,
                    binary_operator::commutative,
                    false,
                    binary_operator::operand_num
                >::eval(result, rhs_);
            }

            const lhs &lhs_;
            const rhs &rhs_;
        };

        template<class Type, class UnaryOp, class T, bool Calculated = false>
        struct unary_expression{
            using type = Type;
            using unary_operator = UnaryOp;
            using operand = T;
            static const bool calculated = Calculated;

            inline unary_expression(const operand &x) :
                x_(x)
            {}

            inline unary_expression(const unary_expression &other) :
                x_(other.x_)
            {}

            inline const type &eval(type &result) const{
                result = specialized_evaluator<
                    unary_operator,
                    unary_operator::commutative,
                    false,
                    unary_operator::operand_num
                >::eval(x_);
            }

            const operand &x_;
        };

        template<bool Cond>
        struct boolean_type{
            static const bool value = Cond;
        };

        template<class Dir, class Parent>
        struct dir_and_parent{
            using dir = Dir;
            using parent = Parent;
        };

        using root_of_dir_and_parent = dir_and_parent<void, void>;

        template<class DirAndParent, class True, class False>
        struct if_dir;

        template<class Parent, class True, class False>
        struct if_dir<dir_and_parent<boolean_type<true>, Parent>, True, False>{
            using type = typename True::type;
        };

        template<class Parent, class True, class False>
        struct if_dir<dir_and_parent<boolean_type<false>, Parent>, True, False>{
            using type = typename False::type;
        };

        template<class True, class False>
        struct if_dir<root_of_dir_and_parent, True, False>{
            using type = root_of_dir_and_parent;
        };

        template<class Condition, class True, class False>
        struct if_type;

        template<class True, class False>
        struct if_type<boolean_type<true>, True, False>{
            using type = typename True::type;
        };

        template<class True, class False>
        struct if_type<boolean_type<false>, True, False>{
            using type = typename False::type;
        };

        template<class Type>
        struct type_identity{
            using type = Type;
        };

        template<class Type>
        struct type_next{
            using type = typename Type::next;
        };

        template<class DirAndParent>
        struct type_parent_trace_rhs{
            using parent_trace_rhs = typename DirAndParent::parent::trace_rhs;
            using type = typename if_type<
                typename parent_trace_rhs::calculated,
                type_identity<typename DirAndParent::parent::calculation>,
                type_identity<typename parent_trace_rhs::next>
            >::type;
        };

        template<class DirAndParent>
        struct type_parent_calculation{
            using type = typename DirAndParent::parent::calculation;
        };

        template<class Expression, std::size_t Depth = 0, class DirAndParent = root_of_dir_and_parent>
        struct trace_expression;

        template<class Type, class Op, class L, class R, std::size_t Depth, class DirAndParent>
        struct trace_expression<binary_expression<Type, Op, L, R, true>, Depth, DirAndParent>{
            using expression = binary_expression<Type, Op, L, R, true>;
            using calculated = boolean_type<true>;
            using next = typename if_dir<
                DirAndParent,
                type_parent_trace_rhs<DirAndParent>,
                type_parent_calculation<DirAndParent>
            >::type;
            static const std::size_t depth = Depth;
        };

        template<class Type, class Op, class L, class R, std::size_t Depth, class DirAndParent>
        struct trace_expression<binary_expression<Type, Op, L, R, false>, Depth, DirAndParent>{
            using this_type = trace_expression;
            using expression = binary_expression<Type, Op, L, R, false>;
            using parent = typename DirAndParent::parent;
            using trace_lhs = trace_expression<L, Depth + 1, dir_and_parent<boolean_type<true>, this_type>>;
            using trace_rhs = trace_expression<R, Depth + 1, dir_and_parent<boolean_type<false>, this_type>>;
            using calculated = boolean_type<false>;
            using calculation = trace_expression<binary_expression<Type, Op, L, R, true>, Depth, DirAndParent>;
            using next = typename if_type<
                typename trace_lhs::calculated,
                if_type<
                    typename trace_rhs::calculated,
                    type_identity<calculation>,
                    type_next<trace_rhs>
                >,
                type_next<trace_lhs>
            >::type;
        };

        template<class Type, std::size_t Depth, class DirAndParent>
        struct trace_expression<identity_expression<Type>, Depth, DirAndParent>{
            using expression = identity_expression<Type>;
            using calculated = boolean_type<true>;
            using next = typename if_dir<
                DirAndParent,
                type_identity<typename DirAndParent::parent::trace_rhs>,
                type_identity<typename DirAndParent::parent::trace_lhs>
            >::type;
        };

    #define CMPXX_AUX_ET_OPERATOR_OVERLOAD(type, op, op_code)               \
        cmpxx::aux::binary_expression<                                      \
            type,                                                           \
            cmpxx::aux::op,                                                 \
            cmpxx::aux::identity_expression<type>,                          \
            cmpxx::aux::identity_expression<type>                           \
        > operator op_code(const type &lhs, const type &rhs){               \
            return cmpxx::aux::binary_expression<                           \
                type,                                                       \
                cmpxx::aux::op,                                             \
                cmpxx::aux::identity_expression<type>,                      \
                cmpxx::aux::identity_expression<type>                       \
            >(lhs, rhs);                                                    \
        }                                                                   \
        template<class T>                                                   \
        cmpxx::aux::binary_expression<                                      \
            type,                                                           \
            cmpxx::aux::op,                                                 \
            cmpxx::aux::identity_expression<type>,                          \
            T                                                               \
        > operator op_code(const type &lhs, const T &rhs){                  \
            return cmpxx::aux::binary_expression<                           \
                type,                                                       \
                cmpxx::aux::op,                                             \
                cmpxx::aux::identity_expression<type>,                      \
                T                                                           \
            >(lhs, rhs);                                                    \
        }                                                                   \
        template<class T>                                                   \
        cmpxx::aux::binary_expression<                                      \
            type,                                                           \
            cmpxx::aux::op,                                                 \
            T,                                                              \
            cmpxx::aux::identity_expression<type>                           \
        > operator op_code(const T &lhs, const type &rhs){                  \
            return cmpxx::aux::binary_expression<                           \
                type,                                                       \
                cmpxx::aux::op,                                             \
                T,                                                          \
                cmpxx::aux::identity_expression<type>                       \
            >(lhs, rhs);                                                    \
        }                                                                   \
        template<class OpT, class OpU, class T, class U, class V, class W>  \
        cmpxx::aux::binary_expression<                                      \
            type,                                                           \
            cmpxx::aux::op,                                                 \
            cmpxx::aux::binary_expression<type, OpT, T, U>,                 \
            cmpxx::aux::binary_expression<type, OpU, V, W>                  \
        > operator op_code(                                                 \
            const cmpxx::aux::binary_expression<type, OpT, T, U> &lhs,      \
            const cmpxx::aux::binary_expression<type, OpU, V, W> &rhs       \
        ){                                                                  \
            return cmpxx::aux::binary_expression<                           \
                type,                                                       \
                cmpxx::aux::op,                                             \
                cmpxx::aux::binary_expression<type, OpT, T, U>,             \
                cmpxx::aux::binary_expression<type, OpU, V, W>              \
            >(lhs, rhs);                                                    \
        };


        template<class T>
        inline std::string lexical_cast(const T &value){
            std::string str;
            std::stringstream sstream;
            sstream << value;
            sstream >> str;
            return str;
        }

        inline std::size_t index_of_leftmost_flag(mp_limb_t x){
            mp_limb_t y, n = GMP_LIMB_BITS, c = n >> 1;
            do{
                y = x >> c;
                if(y != 0){
                    n -= c;
                    x = y;
                }
                c >>= 1;
            }while(c != 0);
            return GMP_LIMB_BITS - n - x + 2;
        }

        inline std::size_t index_of_rightmost_flag(mp_limb_t x){
            if(x == 0){ return 32; }
            mp_limb_t y, n = GMP_LIMB_BITS - 1, c = GMP_LIMB_BITS >> 1;
            do{
                y = x << c;
                if(y != 0){
                    n -= c;
                    x = y;
                }
                c >>= 1;
            }while(c != 0);
            return n;
        }

        inline mp_limb_t ceil_pow2(mp_limb_t n){
            --n;
            for(std::size_t i = 1; i < GMP_LIMB_BITS; i <<= 1){
                n = n | (n >> i);
            }
            return n + 1;
        }
    }
}

// integer/rational/floating/polynomial

#include <memory>
#include <utility>
#include <limits>
#include <sstream>
#include <type_traits>
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

            inline mp_wrapper ceil_pow2() const{
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

            inline std::size_t ceil_log2() const{
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

#define CMPXX_MP_WRAPPER_DEFINE_BUILT_IN_TYPE_OVERLOAD_IMPL_S(op, e, r, t) \
    CMPXX_MP_WRAPPER_DEFINE_BUILT_IN_TYPE_OVERLOAD_IMPL(op, e, t, signed long int, r);

#define CMPXX_MP_WRAPPER_DEFINE_BUILT_IN_TYPE_OVERLOAD_IMPL_U(op, e, r, t) \
    CMPXX_MP_WRAPPER_DEFINE_BUILT_IN_TYPE_OVERLOAD_IMPL(op, e, t, unsigned long int, r);

#define CMPXX_MP_WRAPPER_DEFINE_BUILT_IN_TYPE_OVERLOAD_IMPL_D(op, e, r, t) \
    CMPXX_MP_WRAPPER_DEFINE_BUILT_IN_TYPE_OVERLOAD_IMPL(op, e, t, double, r);

#define CMPXX_MP_WRAPPER_DEFINE_BUILT_IN_TYPE_OVERLOAD_IMPL_LD(op, e, r, t) \
    CMPXX_MP_WRAPPER_DEFINE_BUILT_IN_TYPE_OVERLOAD_IMPL(op, e, t, long double, r);

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
    namespace aux{
    }

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
        { if(coe != 0){ container.insert(typename ordered_container::value_type(0, coe)); } }

        #define CMPXX_POLYNOMIAL_CTOR(op, e, r, type) \
            inline polynomial(type value) :           \
                container()                           \
            { if(value != 0){ container.insert(typename ordered_container::ref_value_type(0, coefficient(value))); } }

        CMPXX_INVOKE_MACRO_WITH_BUILT_IN_TYPE(CMPXX_POLYNOMIAL_CTOR, nil, nil, nil);

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

        inline polynomial &operator +=(const polynomial &rhs){
            add_order_n(rhs, 0);
            return *this;
        }

        inline polynomial &operator -=(const polynomial &rhs){
            sub_iterator(rhs.container.begin(), rhs.container.end());
            return *this;
        }

        inline polynomial &operator *=(const polynomial &rhs){
            // *this = kar_mul_impl(*this, rhs);
            *this = square_mul(*this, rhs);
            return *this;
        }

        inline polynomial &operator /=(const polynomial &rhs){
            polynomial rem;
            *this = div<false>(rem, *this, rhs);
            return *this;
        }

        inline polynomial &operator %=(const polynomial &rhs){
            polynomial rem;
            div<true>(rem, *this, rhs);
            *this = std::move(rem);
            return *this;
        }

        inline bool operator <(const polynomial &rhs) const{
            return base_less_equal(false, rhs);
        }

        inline bool operator >(const polynomial &rhs) const{
            return rhs.base_less_equal(false, *this);
        }

        inline bool operator <=(const polynomial &rhs) const{
            return base_less_equal(true, rhs);
        }
        
        inline bool operator >=(const polynomial &rhs) const{
            return rhs.base_less_equal(true, *this);
        }

        inline bool operator ==(const polynomial &rhs) const{
            return container == rhs.container;
        }

        inline bool operator !=(const polynomial &rhs) const{
            return container != rhs.container;
        }

        template<bool Rem>
        static polynomial div(polynomial &rem, const polynomial &lhs, const polynomial &rhs){
            if(lhs.container.empty()){ return lhs; }
            if(commutative_ring && rhs.is_monic()){
                polynomial r = monic_div<Rem>(rem, lhs, rhs);
                return r;
            }
            polynomial r;
            rem = lhs;
            const auto &rhs_last_iter = rhs.container.rbegin();
            const order &rhs_order = rhs_last_iter->first;
            const coefficient &rhs_coe = rhs_last_iter->second;
            while(!rem.container.empty()){
                const auto &rem_last_iter = rem.container.rbegin();
                const order &rem_order = rem_last_iter->first;
                const coefficient &rem_coe = rem_last_iter->second;
                if(rem_order >= rhs_order){
                    order n = rem_order - rhs_order;
                    coefficient q = rem_coe / rhs_coe;
                    if(q == 0){ break; }
                    rem.sub_n_q(rhs, n, q);
                    r.add_order_coe(n, q);
                }else{ break; }
            }
            return r;
        }

        inline static polynomial classical_gcd(const polynomial &x, const polynomial &y){
            static_assert(!commutative_ring, "coefficient type is commutative ring.");
            if(x >= y){
                return classical_gcd_impl(x, y);
            }else{
                return classical_gcd_impl(y, x);
            }
        }

        inline static polynomial classical_eea(polynomial &cl, polynomial &cr, const polynomial &l, const polynomial &r){
            if(l >= r){
                return classical_eea_impl(cl, cr, l, r);
            }else{
                return classical_eea_impl(cr, cl, r, l);
            }
        }

        inline static polynomial eea(polynomial &cl, polynomial &cr, const polynomial &l, const polynomial &r){
            static_assert(commutative_ring, "coefficient type is non-commutative ring.");
            if(l >= r){
                return eea_impl(cl, cr, l, r);
            }else{
                return eea_impl(cr, cl, r, l);
            }
        }

        polynomial inverse(const order &l) const{
            const polynomial &f = *this;
            polynomial g = 1;
            std::size_t r = l.ceil_log2();
            order rem = 2;
            for(std::size_t i = 0; i < r; ++i, rem *= rem){
                polynomial next_g = 2 * g - f * g * g;
                next_g.container.erase(next_g.container.lower_bound(rem), next_g.container.end());
                g = std::move(next_g);
            }
            return g;
        }

        inline const order &deg() const{
            return container.rbegin()->first;
        }

        inline const coefficient &lc() const{
            return container.rbegin()->second;
        }

        inline void lu(){
            if(container.empty()){
                *this = 1;
            }else{
                *this = lc();
            }
        }

        inline static polynomial normal(const polynomial &x){
            if(x.container.empty()){
                return 0;
            }
            polynomial result = x;
            result /= x.lc();
            return result;
        }

        inline void normalize(){
            *this = normal(*this);
        }

        inline coefficient intinity_norm() const{
            coefficient r = 0;
            for(auto &iter : container){
                r = coefficient::absolute_max(r, iter.second);
            }
            if(r < 0){ r *= -1; }
            return r;
        }

        inline polynomial rev() const{
            polynomial result;
            const order &n = deg();
            for(const auto &iter : container){
                order m = n - iter.first;
                result.container.insert(typename ordered_container::ref_value_type(m, iter.second));
            }
            return result;
        }

        inline bool is_monic() const{
            return !container.empty() && lc() == 1;
        }

        template<class Variable>
        inline std::string get_str(const Variable &v) const{
            return get_str_impl<std::string, char, Variable, std::ostringstream>(v, '*', '^', '+', '-', '0', '(', ')');
        }

        inline std::string get_str() const{
            return get_str("x");
        }

    private:
        static polynomial eea_impl(polynomial &c_lhs, polynomial &c_rhs, const polynomial &lhs, const polynomial &rhs){
            if(rhs.container.empty()){
                c_lhs = 0;
                c_rhs = 0;
                return 0;
            }
            polynomial
                r_0 = normal(lhs),
                r_1 = normal(rhs),
                s_0 = polynomial(1) / lhs.lc(),
                s_1 = 0,
                t_0 = 0,
                t_1 = polynomial(1) / rhs.lc();
            int counter = 0;
            while(!r_1.container.empty()){
                polynomial q = r_0 / r_1, rho = r_0 - q * r_1, r_m = r_1, s_m = s_1, t_m = t_1;
                rho.lu();
                r_1 = (r_0 - q * r_1) / rho;
                s_1 = (s_0 - q * s_1) / rho;
                t_1 = (t_0 - q * t_1) / rho;
                r_0 = std::move(r_m);
                s_0 = std::move(s_m);
                t_0 = std::move(t_m);
            }
            c_lhs = std::move(s_0);
            c_rhs = std::move(t_0);
            return r_0;
        }

        static polynomial classical_eea_impl(polynomial &c_lhs, polynomial &c_rhs, const polynomial &lhs, const polynomial &rhs){
            polynomial r_0 = lhs, r_1 = rhs, s_0 = 1, s_1 = 0, t_0 = 0, t_1 = 1;
            while(!r_1.container.empty()){
                polynomial q = r_0 / r_1, r_m = r_1, s_m = s_1, t_m = t_1;
                r_1 = r_0 - q * r_1;
                s_1 = s_0 - q * s_1;
                t_1 = t_0 - q * t_1;
                r_0 = std::move(r_m);
                s_0 = std::move(s_m);
                t_0 = std::move(t_m);
            }
            c_lhs = std::move(s_0);
            c_rhs = std::move(t_0);
            return r_0;
        }

        static polynomial classical_gcd_impl(polynomial lhs, polynomial rhs){
            polynomial result;
            if(rhs.container.empty()){ return result; }
            polynomial *operands[3] = { &lhs, &rhs, &result };
            while(true){
                *operands[2] = *operands[0] % *operands[1];
                if(operands[2]->container.empty()){ break; }
                polynomial *ptr = operands[0];
                for(int i = 0; i < 2; ++i){ operands[i] = operands[i + 1]; }
                operands[2] = ptr;
            }
            return *operands[1];
        }

        bool base_less_equal(bool f, const polynomial &rhs) const{
            const ordered_container &rhs_container = rhs.container;
            typename ordered_container::const_reverse_iterator
                lhs_iter = container.rbegin(),
                lhs_end = container.rend(),
                rhs_iter = rhs_container.rbegin(),
                rhs_end = rhs_container.rend();
            bool end_l = lhs_iter == lhs_end, end_r = rhs_iter == rhs_end;
            if(!end_l || !end_r){
                if(end_l && !end_r){ return false; }
                if(!end_l && end_r){ return true; }
                for(; lhs_iter != lhs_end && rhs_iter != rhs_end; ++lhs_iter, ++rhs_iter){
                    const order &lhs_order(lhs_iter->first), &rhs_order(rhs_iter->first);
                    if(lhs_order == rhs_order){
                        const coefficient &lhs_coe(lhs_iter->second), &rhs_coe(rhs_iter->second);
                        if(lhs_coe < rhs_coe){
                            return true;
                        }else if(lhs_coe > rhs_coe){
                            return false;
                        }
                    }else if(lhs_order < rhs_order){
                        return true;
                    }else{
                        return false;
                    }
                }
                if(lhs_iter == lhs_end && rhs_iter != rhs_end){ return false; }
                if(lhs_iter != lhs_end && rhs_iter == rhs_end){ return true; }
            }
            return f;
        }

        inline void add_order_n(const polynomial &rhs, const order &n){
            for(auto &rhs_value : rhs.container){
                order o = rhs_value.first + n;
                const coefficient &coe = rhs_value.second;
                typename ordered_container::iterator iter = container.find(o);
                if(iter == container.end()){
                    container.insert(iter, typename ordered_container::ref_value_type(o, coe));
                }else{
                    coefficient &lhs_coe(iter->second);
                    lhs_coe += coe;
                    if(lhs_coe == 0){ container.erase(static_cast<typename ordered_container::const_iterator>(iter)); }
                }
            }
        }

        inline void sub_iterator(
            const typename ordered_container::const_iterator &iter,
            const typename ordered_container::const_iterator &end
        ){ sub_iterator(iter, end, 0); }

        inline void sub_iterator(
            typename ordered_container::const_iterator rhs_iter,
            typename ordered_container::const_iterator rhs_end,
            const order &n
        ){
            for(; rhs_iter != rhs_end; ++rhs_iter){
                order o = rhs_iter->first - n;
                const coefficient &coe = rhs_iter->second;
                typename ordered_container::iterator iter = container.find(o);
                if(iter == container.end()){
                    iter = container.add(typename ordered_container::ref_value_type(o, coe));
                    if(iter != container.end()){ iter->second = -iter->second; }
                }else{
                    coefficient &lhs_coe(iter->second);
                    lhs_coe -= coe;
                    if(lhs_coe == 0){ container.erase(static_cast<typename ordered_container::const_iterator>(iter)); }
                }
            }
        }

        template<bool Rem>
        static polynomial monic_div(polynomial &rem, const polynomial &a, const polynomial &b){
            if(a.deg() < b.deg()){
                if(Rem){ rem = a; }
                return polynomial();
            }
            polynomial q;
            order mpp = a.deg() - b.deg() + 1;
            q = a.rev() * b.rev().inverse(mpp);
            q.container.erase(q.container.lower_bound(mpp), q.container.end());
            if(Rem){
                q = q.rev();
                rem = a - b * q;
                return q;
            }else{
                return q.rev();
            }
        }

        static polynomial kar_mul_impl(const polynomial &f, const polynomial &g){
            if(f.container.empty() || g.container.empty()){
                return polynomial();
            }
            const order &order_f(f.container.rbegin()->first), &order_g(g.container.rbegin()->first);
            order n = order_f > order_g ? order_f.ceil_pow2() : order_g.ceil_pow2();
            if(n < 2){
                return square_mul(f, g);
            }
            n >>= 1;
            typename ordered_container::const_iterator
                upper_bound_f = f.container.upper_bound(n),
                upper_bound_g = g.container.upper_bound(n);
            polynomial
                f_0 = kar_mul_add_ffgg(f.container.begin(), upper_bound_f, 0),
                g_0 = kar_mul_add_ffgg(g.container.begin(), upper_bound_g, 0),
                f_1 = kar_mul_add_ffgg(upper_bound_f, f.container.end(), n),
                g_1 = kar_mul_add_ffgg(upper_bound_g, g.container.end(), n),
                ff = f_0 + f_1,
                gg = g_0 + g_1,
                fg_0 = kar_mul_impl(f_0, g_0),
                fg_1 = kar_mul_impl(f_1, g_1),
                ffgg = kar_mul_impl(ff, gg);
            f_0 = 0, f_1 = 0;
            g_0 = 0, g_1 = 0;
            ff = 0, gg = 0;
            ffgg -= fg_0, ffgg -= fg_1;
            polynomial r;
            r += fg_0;
            ffgg.radix_shift(n);
            r += ffgg;
            n <<= 1;
            fg_1.radix_shift(n);
            r += fg_1;
            return r;
        }

        inline static polynomial kar_mul_add_ffgg(
            typename ordered_container::const_iterator first,
            typename ordered_container::const_iterator last,
            const order &n
        ){
            polynomial r;
            for(; first != last; ++first){
                r.container[first->first - n] = first->second;
            }
            return r;
        }

        inline static polynomial square_mul(const polynomial &lhs, const polynomial &rhs){
            return square_mul_impl(lhs.container.begin(), lhs.container.end(), rhs.container.begin(), rhs.container.end());
        }

        inline static polynomial square_mul_impl(
            const typename ordered_container::const_iterator &lhs_first,
            const typename ordered_container::const_iterator &lhs_last,
            const typename ordered_container::const_iterator &rhs_first,
            const typename ordered_container::const_iterator &rhs_last
        ){
            polynomial r;
            for(auto lhs_iter = lhs_first; lhs_iter != lhs_last; ++lhs_iter){
                const order &lhs_order(lhs_iter->first);
                const coefficient &lhs_coe(lhs_iter->second);
                for(auto rhs_iter = rhs_first; rhs_iter != rhs_last; ++rhs_iter){
                    const order &rhs_order(rhs_iter->first);
                    const coefficient &rhs_coe(rhs_iter->second);
                    r.add_order_coe(lhs_order + rhs_order, lhs_coe * rhs_coe);
                }
            }
            return r;
        }

        inline void add_order_coe(const order &o, const coefficient &coe){
            typename ordered_container::iterator iter = container.find(o);
            if(iter == container.end()){
                container.insert(typename ordered_container::ref_value_type(o, coe));
            }else{
                coefficient &lhs_coe(iter->second);
                lhs_coe += coe;
                if(lhs_coe == 0){ container.erase(static_cast<typename ordered_container::const_iterator>(iter)); }
            }
        }

        inline void radix_shift(const order &n){
            for(auto &t : container){
                const_cast<order&>(t.first) += n;
            }
        }

        inline void sub_n_q(const polynomial &rhs, const order &n, const coefficient &q){
            for(auto &rhs_iter : rhs.container){
                const order o(rhs_iter.first);
                order new_o = o;
                new_o += n;
                const coefficient &coe(rhs_iter.second);
                coefficient new_coe = coe * q;
                auto iter = container.find(new_o);
                if(iter == container.end()){
                    iter = container.add(typename ordered_container::ref_value_type(new_o, new_coe));
                    if(iter != container.end()){ iter->second = -new_coe; }
                }else{
                    coefficient &lhs_coe = iter->second;
                    lhs_coe -= new_coe;
                    if(lhs_coe == 0){ container.erase(static_cast<typename ordered_container::const_iterator>(iter)); }
                }
            }
        }

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
            container.swap(other.container);
        }

        ordered_container container;
    };

    #define CMPXX_POLYNOMIAL_OPERATOR_OVERLOAD(op, a_op)                            \
        template<class Order, class Coefficient, bool CommutativeRing, class Alloc> \
        inline polynomial<Order, Coefficient, CommutativeRing, Alloc>               \
        operator op(                                                                \
            const polynomial<Order, Coefficient, CommutativeRing, Alloc> &l,        \
            const polynomial<Order, Coefficient, CommutativeRing, Alloc> &r         \
        ){                                                                          \
            polynomial<Order, Coefficient, CommutativeRing, Alloc> ret(l);          \
            ret a_op r;                                                             \
            return ret;                                                             \
        }                                                                           \
        template<class Order, class Coefficient, bool CommutativeRing, class Alloc> \
        inline polynomial<Order, Coefficient, CommutativeRing, Alloc>               \
        operator op(                                                                \
            const polynomial<Order, Coefficient, CommutativeRing, Alloc> &l,        \
            const Coefficient &r                                                    \
        ){                                                                          \
            polynomial<Order, Coefficient, CommutativeRing, Alloc> ret(l);          \
            ret a_op r;                                                             \
            return ret;                                                             \
        }                                                                           \
        template<class Order, class Coefficient, bool CommutativeRing, class Alloc> \
        inline polynomial<Order, Coefficient, CommutativeRing, Alloc>               \
        operator op(                                                                \
            const Coefficient &l,                                                   \
            const polynomial<Order, Coefficient, CommutativeRing, Alloc> &r         \
        ){                                                                          \
            polynomial<Order, Coefficient, CommutativeRing, Alloc> ret(l);          \
            ret a_op r;                                                             \
            return ret;                                                             \
        }

    #define CMPXX_POLYNOMIAL_BUILT_IN_TYPE_OPERATOR_OVERLOAD(op, a_op, e, t)        \
        template<class Order, class Coefficient, bool CommutativeRing, class Alloc> \
        inline polynomial<Order, Coefficient, CommutativeRing, Alloc>               \
        operator op(                                                                \
            const polynomial<Order, Coefficient, CommutativeRing, Alloc> &l,        \
            t v                                                                     \
        ){                                                                          \
            polynomial<Order, Coefficient, CommutativeRing, Alloc> ret(l);          \
            ret a_op polynomial<Order, Coefficient, CommutativeRing, Alloc>(v);     \
            return ret;                                                             \
        }                                                                           \
        template<class Order, class Coefficient, bool CommutativeRing, class Alloc> \
        inline polynomial<Order, Coefficient, CommutativeRing, Alloc>               \
        operator op(                                                                \
            t v,                                                                    \
            const polynomial<Order, Coefficient, CommutativeRing, Alloc> &r         \
        ){                                                                          \
            polynomial<Order, Coefficient, CommutativeRing, Alloc> ret(v);          \
            ret a_op r;                                                             \
            return ret;                                                             \
        }

    CMPXX_POLYNOMIAL_OPERATOR_OVERLOAD(+, +=);
    CMPXX_POLYNOMIAL_OPERATOR_OVERLOAD(-, -=);
    CMPXX_POLYNOMIAL_OPERATOR_OVERLOAD(*, *=);
    CMPXX_POLYNOMIAL_OPERATOR_OVERLOAD(/, /=);
    CMPXX_POLYNOMIAL_OPERATOR_OVERLOAD(%, %=);

    CMPXX_INVOKE_MACRO_WITH_BUILT_IN_TYPE(CMPXX_POLYNOMIAL_BUILT_IN_TYPE_OPERATOR_OVERLOAD, +, +=, nil);
    CMPXX_INVOKE_MACRO_WITH_BUILT_IN_TYPE(CMPXX_POLYNOMIAL_BUILT_IN_TYPE_OPERATOR_OVERLOAD, -, -=, nil);
    CMPXX_INVOKE_MACRO_WITH_BUILT_IN_TYPE(CMPXX_POLYNOMIAL_BUILT_IN_TYPE_OPERATOR_OVERLOAD, *, *=, nil);
    CMPXX_INVOKE_MACRO_WITH_BUILT_IN_TYPE(CMPXX_POLYNOMIAL_BUILT_IN_TYPE_OPERATOR_OVERLOAD, /, /=, nil);
    CMPXX_INVOKE_MACRO_WITH_BUILT_IN_TYPE(CMPXX_POLYNOMIAL_BUILT_IN_TYPE_OPERATOR_OVERLOAD, %, %=, nil);

    #define CMPXX_POLYNOMIAL_BOOLEAN_OPERATOR_OVERLOAD_IMPL(op, t)                  \
        template<class Order, class Coefficient, bool CommutativeRing, class Alloc> \
        inline bool operator op(                                                    \
            const polynomial<Order, Coefficient, CommutativeRing, Alloc> &l,        \
            const t &r                                                              \
        ){ return l op polynomial<Order, Coefficient, CommutativeRing, Alloc>(r); } \
        template<class Order, class Coefficient, bool CommutativeRing, class Alloc> \
        inline bool operator op(                                                    \
            const t &l,                                                             \
            const polynomial<Order, Coefficient, CommutativeRing, Alloc> &r         \
        ){ return polynomial<Order, Coefficient, CommutativeRing, Alloc>(l) op r; }

    #define CMPXX_POLYNOMIAL_BOOLEAN_OPERATOR_OVERLOAD(op, e, r, t) \
        CMPXX_POLYNOMIAL_BOOLEAN_OPERATOR_OVERLOAD_IMPL(<, t);      \
        CMPXX_POLYNOMIAL_BOOLEAN_OPERATOR_OVERLOAD_IMPL(>, t);      \
        CMPXX_POLYNOMIAL_BOOLEAN_OPERATOR_OVERLOAD_IMPL(<=, t);     \
        CMPXX_POLYNOMIAL_BOOLEAN_OPERATOR_OVERLOAD_IMPL(>=, t);     \
        CMPXX_POLYNOMIAL_BOOLEAN_OPERATOR_OVERLOAD_IMPL(==, t);     \
        CMPXX_POLYNOMIAL_BOOLEAN_OPERATOR_OVERLOAD_IMPL(!=, t);

    CMPXX_POLYNOMIAL_BOOLEAN_OPERATOR_OVERLOAD_IMPL(<, Coefficient);
    CMPXX_POLYNOMIAL_BOOLEAN_OPERATOR_OVERLOAD_IMPL(>, Coefficient);
    CMPXX_POLYNOMIAL_BOOLEAN_OPERATOR_OVERLOAD_IMPL(>=, Coefficient);
    CMPXX_POLYNOMIAL_BOOLEAN_OPERATOR_OVERLOAD_IMPL(<=, Coefficient);
    CMPXX_POLYNOMIAL_BOOLEAN_OPERATOR_OVERLOAD_IMPL(==, Coefficient);
    CMPXX_POLYNOMIAL_BOOLEAN_OPERATOR_OVERLOAD_IMPL(!=, Coefficient);
    CMPXX_INVOKE_MACRO_WITH_BUILT_IN_TYPE(CMPXX_POLYNOMIAL_BOOLEAN_OPERATOR_OVERLOAD, nil, nil, nil);

    template<class Order, class Coefficient, bool CommutativeRing, class Alloc>
    std::ostream &operator <<(std::ostream &ostream, const polynomial<Order, Coefficient, CommutativeRing, Alloc> &value){
        ostream << value.get_str();
        return ostream;
    }
}

// dynamic-compile

#include <cstdlib>
#include <fstream>
#include <exception>
#include <dlfcn.h>

#define CMPXX_EXPORT "extern \"C\" "
#define CMPXX_IMPORT "extern \"C\" "

#ifdef _WIN32
  #define CMPXX_DLEXT ".dll"
#else
  #define CMPXX_DLEXT ".so"
#endif

namespace cmpxx{
    class ofile_error : public std::runtime_error{
    public:
        inline ofile_error(std::string str) : std::runtime_error(str){}
    };

    class compile_error : public std::runtime_error{
    public:
        inline compile_error(std::string str) : std::runtime_error(str){}
    };

    namespace aux{
        void *compile(const std::string code){
            static int count = 0;
            std::string cpp_file_name, o_file_name, so_file_name, clang_command_line;
            ++count;
            std::string count_str = lexical_cast(count);
            cpp_file_name = "cmpxx-temp-" + count_str + ".cpp";
            o_file_name = "cmpxx-temp-" + count_str + ".o";
            so_file_name = "./cmpxx-temp-" + count_str + CMPXX_DLEXT;
            {
                std::ofstream ofile(cpp_file_name.c_str());
                if(ofile.fail()){
                    throw(ofile_error("cannot open file '" + cpp_file_name + "'."));
                }
                ofile << code;
            }
            int status;
            clang_command_line = "clang++ -std=c++11 -c -O2 " + cpp_file_name;
            status = std::system(clang_command_line.c_str());
            if(status != 0){
                throw(compile_error("compile failed. '" + clang_command_line + "'."));
            }
            clang_command_line = "clang++ -shared -o " + so_file_name + " " + o_file_name + " -lgmpxx -lgmp";
            status = std::system(clang_command_line.c_str());
            if(status != 0){
                throw(compile_error("compile failed. '" + clang_command_line + "'."));
            }
            void *handle = dlopen(so_file_name.c_str(), RTLD_LAZY);
            if(!handle){
                throw(compile_error("dlopen failed. '" + so_file_name + "'."));
            }
            return handle;
        }
    }
}

// test

#include <iostream>

namespace test{
    void dynamic_link_test(){
        std::cout << "-------- dynamic link test.\n";

        try{
            void *handle = cmpxx::aux::compile(std::string(CMPXX_IMPORT) + "int add(int x, int y){ return x + y; }");
            typedef int (*func_type)(int x, int y);
            func_type func = nullptr;
            func = (func_type)dlsym(handle, "add");
            std::cout << func(10, 20) << std::endl;
        }catch(std::runtime_error e){
            std::cerr << e.what() << std::endl;
        }

        std::cout << std::endl;
    }

    void polynomial_test_1(){
        std::cout << "-------- commutative ring test.\n";

        typedef cmpxx::polynomial<cmpxx::integer, cmpxx::rational, true> poly;
        {
            poly p, q;

            // p = 444x^333 + 222x^111
            p["111"]("222")["333"]("444");
            // q = 101x^999 + 888x^777 + 666x^555
            q["555"]("666")["777"]("888")["999"]("101");

            std::cout << "div.\n";
            std::cout << "lhs : " << q.get_str() << "\n";
            std::cout << "rhs : " << p.get_str() << "\n";
            std::cout << "div : " << (q / p).get_str() << "\n";
            std::cout << "mod : " << (q % p).get_str() << "\n";
            std::cout << std::endl;
        }

        {
            poly f, g, s, t;
            f["3"]("18")["2"]("-42")["1"]("30")["0"]("-6");
            g["2"]("-12")["1"]("10")["0"]("-2");
            std::cout << "eea.\n";
            std::cout << "lhs : " << f << "\n";
            std::cout << "rhs : " << g << "\n";
            std::cout << "eea : " << poly::eea(s, t, f, g) << "\n";
            std::cout << "s   : " << s << "\n";
            std::cout << "t   : " << t << "\n";
            std::cout << "eea : " << (s * f + t * g) << "\n";
            std::cout << std::endl;
        }

        {
            poly f, g, s, t;
            f["3"]("18")["2"]("-42")["1"]("30")["0"]("-6");
            g["2"]("-12")["1"]("10")["0"]("-2");
            std::cout << "eea.\n";
            std::cout << "lhs : " << f << "\n";
            std::cout << "rhs : " << g << "\n";
            std::cout << "eea : " << poly::classical_eea(s, t, f, g) << "\n";
            std::cout << "s   : " << s << "\n";
            std::cout << "t   : " << t << "\n";
            std::cout << "eea : " << (s * f + t * g) << "\n";
            std::cout << std::endl;
        }

        std::cout << std::endl;
    }

    void polynomial_test_2(){
        std::cout << "-------- non-commutative ring test.\n";

        typedef cmpxx::polynomial<cmpxx::integer, cmpxx::integer, false> poly;
        poly f = 126, g = 35;
        std::cout << "gcd(" << f.get_str() << ", " << g.get_str() << ") = " << poly::classical_gcd(f, g).get_str() << "\n";
        std::cout << std::endl;

        {
            poly s, t;
            std::cout << "eea : " << poly::classical_eea(s, t, 126, 35) << "\n";
            std::cout << "s   : " << s << "\n";
            std::cout << "t   : " << t << "\n";
            std::cout << "eea : " << (s * 126 + t * 35) << "\n";
            std::cout << std::endl;
        }

        std::cout << std::endl;
    }

    struct test_type{
        test_type() : value(0){}

        template<class Operator, class L, class R>
        test_type(const cmpxx::aux::binary_expression<test_type, Operator, L, R> &expression) :
            value()
        { expression.eval(*this); }

        template<class Operator, class L, class R>
        test_type &operator =(const cmpxx::aux::binary_expression<test_type, Operator, L, R> &expression){
            expression.eval(*this);
            return *this;
        }

        test_type &operator =(const test_type &rhs){
            value = rhs.value;
            return *this;
        }

        test_type &operator +=(const test_type &rhs){
            value += rhs.value;
            return *this;
        }

        test_type &operator -=(const test_type &rhs){
            value -= rhs.value;
            return *this;
        }

        test_type &operator *=(const test_type &rhs){
            value *= rhs.value;
            return *this;
        }

        test_type &operator /=(const test_type &rhs){
            value /= rhs.value;
            return *this;
        }

        double value;
    };

    CMPXX_AUX_ET_OPERATOR_OVERLOAD(test_type, exp_operator_add<>, +);
    CMPXX_AUX_ET_OPERATOR_OVERLOAD(test_type, exp_operator_mul<>, *);
    CMPXX_AUX_ET_OPERATOR_OVERLOAD(test_type, exp_operator_sub<>, -);
    CMPXX_AUX_ET_OPERATOR_OVERLOAD(test_type, exp_operator_div<>, /);

    void expression_template_test(){
        std::cout << "-------- expression template test.\n";

        {
            test_type x_1, x_2, y, z, w;
            x_1.value = 200, x_2.value = 100, y.value = 5, z.value = 0.1;
            w = x_1 - y / z - x_2 + (y * z);
            std::cout << "x_1 - y / z - x_2 = " << w.value << std::endl;
        }
        std::cout << std::endl;

        {
            test_type a;
            {
                using expression = cmpxx::aux::trace_expression<decltype(a + a * a - a)>;
                using next_1 = expression::next;
                using next_2 = next_1::next;
                using next_3 = next_2::next;
            }

            {
                using expression = cmpxx::aux::trace_expression<decltype((a * a + a / a) - (a / a - a * a))>;
                using next_1 = expression::next;
                using next_2 = next_1::next;
                using next_3 = next_2::next;
                using next_4 = next_3::next;
                using next_5 = next_4::next;
                using next_6 = next_5::next;
                using next_7 = next_6::next;
            }
        }
    }
}

int main(int argc, char *argv[]){
    test::expression_template_test();
    test::polynomial_test_1();
    test::polynomial_test_2();
    test::dynamic_link_test();

    return 0;
}

