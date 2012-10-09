#ifndef INCLUDE_CMPXX_AUX
#define INCLUDE_CMPXX_AUX

// aux

#include <utility>
#include <string>
#include <sstream>
#include <gmpxx.h>

#define CMPXX_AUX_EXPAND_MACRO_ARGS(...) __VA_ARGS__

#define CMPXX_INVOKE_MACRO_WITH_BUILT_IN_TYPE(macro, ...)   \
    macro(__VA_ARGS__, signed char);                        \
    macro(__VA_ARGS__, unsigned char);                      \
    macro(__VA_ARGS__, signed int);                         \
    macro(__VA_ARGS__, unsigned int);                       \
    macro(__VA_ARGS__, signed short int);                   \
    macro(__VA_ARGS__, unsigned short int);                 \
    macro(__VA_ARGS__, signed long int);                    \
    macro(__VA_ARGS__, unsigned long int);                  \
    macro(__VA_ARGS__, float);                              \
    macro(__VA_ARGS__, double);                             \
    macro(__VA_ARGS__, long double);

#define CMPXX_INVOKE_MACRO_WITH_S_U_D_LD(s, u, d, ld, ...)  \
    s(__VA_ARGS__, signed char);                            \
    u(__VA_ARGS__, unsigned char);                          \
    s(__VA_ARGS__, signed int);                             \
    u(__VA_ARGS__, unsigned int);                           \
    s(__VA_ARGS__, signed short int);                       \
    u(__VA_ARGS__, unsigned short int);                     \
    s(__VA_ARGS__, signed long int);                        \
    u(__VA_ARGS__, unsigned long int);                      \
    d(__VA_ARGS__, float);                                  \
    d(__VA_ARGS__, double);                                 \
    ld(__VA_ARGS__, long double);

namespace cmpxx{
    namespace aux{
        template<bool Commutative = false>
        struct exp_operator_assign{
            static const bool commutative = Commutative;
            static const std::size_t operand_num = 2;

            template<class T, class U>
            inline static T &apply(T &target, const U &operand){
                target = operand;
                return target;
            }
        };

        template<bool Commutative = true>
        struct exp_operator_add{
            static const bool commutative = Commutative;
            static const std::size_t operand_num = 2;

            template<class T, class U>
            inline static T &apply(T &target, const U &operand){
                target += operand;
                return target;
            }
        };

        template<bool Commutative = false>
        struct exp_operator_sub{
            static const bool commutative = Commutative;
            static const std::size_t operand_num = 2;

            template<class T, class U>
            inline static T &apply(T &target, const U &operand){
                target -= operand;
                return target;
            }
        };

        template<bool Commutative = true>
        struct exp_operator_mul{
            static const bool commutative = Commutative;
            static const std::size_t operand_num = 2;

            template<class T, class U>
            inline static T &apply(T &target, const U &operand){
                target *= operand;
                return target;
            }
        };

        template<bool Commutative = false>
        struct exp_operator_div{
            static const bool commutative = Commutative;
            static const std::size_t operand_num = 2;

            template<class T, class U>
            inline static T &apply(T &target, const U &operand){
                target /= operand;
                return target;
            }
        };

        template<bool Commutative = false>
        struct exp_operator_rem{
            static const bool commutative = Commutative;
            static const std::size_t operand_num = 2;

            template<class T, class U>
            inline static T &apply(T &target, const U &operand){
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

            template<class T, class U>
            inline static T &apply(T &target, const U &operand){
                target &= operand;
                return target;
            }
        };

        template<bool Commutative = true>
        struct exp_operator_or{
            static const bool commutative = Commutative;
            static const std::size_t operand_num = 2;

            template<class T, class U>
            inline static T &apply(T &target, const U &operand){
                target |= operand;
                return target;
            }
        };

        template<bool Commutative = false>
        struct exp_operator_xor{
            static const bool commutative = Commutative;
            static const std::size_t operand_num = 2;

            template<class T, class U>
            inline static T &apply(T &target, const U &operand){
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
            inline static bool apply(T &target, const U &operand){
                return LessFunctor()(target, operand);
            }
        };

        template<class GreaterFunctor, bool Commutative = false>
        struct exp_operator_greater{
            static const bool commutative = Commutative;
            static const std::size_t operand_num = 2;

            template<class T, class U>
            inline static bool apply(T &target, const U &operand){
                return GreaterFunctor()(target, operand);
            }
        };

        template<class LessEqualFunctor, bool Commutative = false>
        struct exp_operator_less_equal{
            static const bool commutative = Commutative;
            static const std::size_t operand_num = 2;

            template<class T, class U>
            inline static bool apply(T &target, const U &operand){
                return LessEqualFunctor()(target, operand);
            }
        };

        template<class GreaterEqualFunctor, bool Commutative = false>
        struct exp_operator_greater_equal{
            static const bool commutative = Commutative;
            static const std::size_t operand_num = 2;

            template<class T, class U>
            inline static bool apply(T &target, const U &operand){
                return GreaterEqualFunctor()(target, operand);
            }
        };

        template<class EqualFunctor, bool Commutative = true>
        struct exp_operator_equal{
            static const bool commutative = Commutative;
            static const std::size_t operand_num = 2;

            template<class T, class U>
            inline static bool apply(T &target, const U &operand){
                return EqualFunctor()(target, operand);
            }
        };

        template<class NotEqualFunctor, bool Commutative = true>
        struct exp_operator_not_equal{
            static const bool commutative = Commutative;
            static const std::size_t operand_num = 2;

            template<class T, class U>
            inline static bool apply(T &target, const U &operand){
                return NotEqualFunctor()(target, operand);
            }
        };

        template<class Operator, bool Commutative, bool Reverse, std::size_t OperandNum>
        struct specialized_evaluator;

        template<class Operator, bool Commutative, bool Reverse>
        struct specialized_evaluator<Operator, Commutative, Reverse, 2>{
             template<class T, class U>
            inline static void eval(T &target, const U &operand){
                Operator::apply(target, operand);
            }
        };

        // note : ExpressionTemplate for TemplateTemplateParameterClass
        // decl_tmp_tmp_param : "(class T, class U, class V, ...)"
        // tmp_tmp_param      : "(T, U, V, ...)"
        // tmp_tmp_place      : "(class, class, class, ...)"

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

        #define CMPXX_AUX_TMP_TMP_ET_IDENTITY_EXPRESSION(identifier, decl_tmp_tmp_param, tmp_tmp_param, tmp_tmp_place) \
            template<template<CMPXX_AUX_EXPAND_MACRO_ARGS tmp_tmp_place> class Type, CMPXX_AUX_EXPAND_MACRO_ARGS decl_tmp_tmp_param> \
            struct identity_expression_ ## identifier{ \
                using type = Type<CMPXX_AUX_EXPAND_MACRO_ARGS tmp_tmp_param>; \
                static const bool calculated = true; \
                inline identity_expression_ ## identifier(const type &value_a) : \
                    value_(value_a) \
                {} \
                inline identity_expression_ ## identifier(const identity_expression_ ## identifier &other) : \
                    value_(other.value_) \
                {} \
                inline void eval(type &result) const{ \
                    result = value_; \
                } \
                const type &value_; \
            };

        template<class Type, class BinaryOp, class L, class R, bool Calculated = false>
        struct expression_template{
            using type = Type;
            using binary_operator = BinaryOp;
            using lhs = L;
            using rhs = R;
            static const bool calculated = Calculated;

            inline expression_template(const lhs &lhs_a, const rhs &rhs_a) :
                lhs_(lhs_a), rhs_(rhs_a)
            {}

            inline expression_template(const expression_template &other) :
                lhs_(other.lhs_), rhs_(other.rhs_)
            {}

            inline operator type() const{
                type ret;
                eval(ret);
                return ret;
            }
            
            inline void eval(type &result) const{
                type rhs;
                lhs_.eval(result);
                rhs_.eval(rhs);
                specialized_evaluator<
                    binary_operator,
                    binary_operator::commutative,
                    false,
                    binary_operator::operand_num
                >::eval(result, result);
            }

            const lhs &lhs_;
            const rhs &rhs_;
        };

        #define CMPXX_AUX_TMP_TMP_ET_EXPRESSION_TEMPLATE(identifier, decl_tmp_tmp_param, tmp_tmp_param, tmp_tmp_place) \
            template<template<CMPXX_AUX_EXPAND_MACRO_ARGS tmp_tmp_place> class Type, CMPXX_AUX_EXPAND_MACRO_ARGS decl_tmp_tmp_param, class BinaryOp, class L, class R, bool Calculated = false> \
            struct expression_template_ ## identifier{ \
                using type = Type<CMPXX_AUX_EXPAND_MACRO_ARGS tmp_tmp_param>; \
                using binary_operator = BinaryOp; \
                using lhs = L; \
                using rhs = R; \
                static const bool calculated = Calculated; \
                inline expression_template_ ## identifier(const lhs &lhs_a, const rhs &rhs_a) : \
                    lhs_(lhs_a), rhs_(rhs_a) \
                {} \
                inline expression_template_ ## identifier(const expression_template_ ## identifier &other) : \
                    lhs_(other.lhs_), rhs_(other.rhs_) \
                {} \
                inline operator type() const{ \
                    type ret; \
                    eval(ret); \
                    return ret; \
                } \
               inline void eval(type &result) const{ \
                    type rhs; \
                    lhs_.eval(result); \
                    rhs_.eval(rhs); \
                    cmpxx::aux::specialized_evaluator< \
                        binary_operator, \
                        binary_operator::commutative, \
                        false, \
                        binary_operator::operand_num \
                    >::eval(result, rhs); \
                } \
                const lhs &lhs_; \
                const rhs &rhs_; \
            };

        template<class Type, class BinaryOp, bool Calculated>
        struct expression_template<Type, BinaryOp, identity_expression<Type>, identity_expression<Type>, Calculated>{
            using type = Type;
            using binary_operator = BinaryOp;
            using lhs = type;
            using rhs = type;
            static const bool calculated = Calculated;

            inline expression_template(const lhs &lhs_a, const rhs &rhs_a) :
                lhs_(lhs_a), rhs_(rhs_a)
            {}

            inline expression_template(const expression_template &other) :
                lhs_(other.lhs_), rhs_(other.rhs_)
            {}

            inline operator type() const{
                type ret;
                eval(ret);
                return ret;
            }

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

        #define CMPXX_AUX_TMP_TMP_ET_EXPRESSION_TEMPLATE_IDENTITY_IDENTITY(identifier, decl_tmp_tmp_param, tmp_tmp_param, tmp_tmp_place) \
            template<template<CMPXX_AUX_EXPAND_MACRO_ARGS tmp_tmp_place> class Type, CMPXX_AUX_EXPAND_MACRO_ARGS decl_tmp_tmp_param, class BinaryOp, bool Calculated> \
            struct expression_template_ ## identifier< \
                Type, \
                CMPXX_AUX_EXPAND_MACRO_ARGS tmp_tmp_param, \
                BinaryOp, \
                identity_expression_ ## identifier<Type, CMPXX_AUX_EXPAND_MACRO_ARGS tmp_tmp_param>, \
                identity_expression_ ## identifier<Type, CMPXX_AUX_EXPAND_MACRO_ARGS tmp_tmp_param>, \
                Calculated \
            >{ \
                using type = Type<CMPXX_AUX_EXPAND_MACRO_ARGS tmp_tmp_param>; \
                using binary_operator = BinaryOp; \
                using lhs = type; \
                using rhs = type; \
                static const bool calculated = Calculated; \
                inline expression_template_ ## identifier(const lhs &lhs_a, const rhs &rhs_a) : \
                    lhs_(lhs_a), rhs_(rhs_a) \
                {} \
                inline expression_template_ ## identifier(const expression_template_ ## identifier &other) : \
                    lhs_(other.lhs_), rhs_(other.rhs_) \
                {} \
                inline operator type() const{ \
                    type ret; \
                    eval(ret); \
                    return ret; \
                } \
                inline void eval(type &result) const{ \
                    result = lhs_; \
                    cmpxx::aux::specialized_evaluator< \
                        binary_operator, \
                        binary_operator::commutative, \
                        false, \
                        binary_operator::operand_num \
                    >::eval(result, rhs_); \
                } \
                const lhs &lhs_; \
                const rhs &rhs_; \
            };

        #define CMPXX_AUX_ET_OPERATOR_OVERLOAD_TYPE_TO_BUILT_IN_TYPE(nil, built_in_type)                        \
            template<class Type, class BinaryOp, bool Calculated>                                               \
            struct expression_template<Type, BinaryOp, identity_expression<Type>, built_in_type, Calculated>{   \
                using type = Type;                                                                              \
                using binary_operator = BinaryOp;                                                               \
                using lhs = type;                                                                               \
                using rhs = built_in_type;                                                                      \
                static const bool calculated = Calculated;                                                      \
                inline expression_template(const lhs &lhs_a, const rhs &rhs_a) :                                \
                    lhs_(lhs_a), rhs_(rhs_a)                                                                    \
                {}                                                                                              \
                inline expression_template(const expression_template &other) :                                  \
                    lhs_(other.lhs_), rhs_(other.rhs_)                                                          \
                {}                                                                                              \
                inline operator type() const{                                                                   \
                    type ret;                                                                                   \
                    eval(ret);                                                                                  \
                    return ret;                                                                                 \
                }                                                                                               \
                inline void eval(type &result) const{                                                           \
                    result = lhs_;                                                                              \
                    specialized_evaluator<                                                                      \
                        binary_operator,                                                                        \
                        binary_operator::commutative,                                                           \
                        false,                                                                                  \
                        binary_operator::operand_num                                                            \
                    >::eval(result, rhs_);                                                                      \
                }                                                                                               \
                const lhs &lhs_;                                                                                \
                const rhs &rhs_;                                                                                \
            };                                                                                                  \
            template<class Type, class BinaryOp, bool Calculated>                                               \
            struct expression_template<Type, BinaryOp, built_in_type, identity_expression<Type>, Calculated>{   \
                using type = Type;                                                                              \
                using binary_operator = BinaryOp;                                                               \
                using lhs = built_in_type;                                                                      \
                using rhs = type;                                                                               \
                static const bool calculated = Calculated;                                                      \
                inline expression_template(const lhs &lhs_a, const rhs &rhs_a) :                                \
                    lhs_(lhs_a), rhs_(rhs_a)                                                                    \
                {}                                                                                              \
                inline expression_template(const expression_template &other) :                                  \
                    lhs_(other.lhs_), rhs_(other.rhs_)                                                          \
                {}                                                                                              \
                inline operator type() const{                                                                   \
                    type ret;                                                                                   \
                    eval(ret);                                                                                  \
                    return ret;                                                                                 \
                }                                                                                               \
                inline void eval(type &result) const{                                                           \
                    result = lhs_;                                                                              \
                    specialized_evaluator<                                                                      \
                        binary_operator,                                                                        \
                        binary_operator::commutative,                                                           \
                        false,                                                                                  \
                        binary_operator::operand_num                                                            \
                    >::eval(result, rhs_);                                                                      \
                }                                                                                               \
                const lhs &lhs_;                                                                                \
                const rhs &rhs_;                                                                                \
            };                                                                                                  \
            template<                                                                                           \
                class Type,                                                                                     \
                class BinaryOp,                                                                                 \
                class OtherBinaryOp,                                                                            \
                class OtherL,                                                                                   \
                class OtherR,                                                                                   \
                bool OtherCalc,                                                                                 \
                bool Calculated                                                                                 \
            > struct expression_template<                                                                       \
                Type,                                                                                           \
                BinaryOp,                                                                                       \
                expression_template<Type, OtherBinaryOp, OtherL, OtherR, OtherCalc>,                            \
                built_in_type,                                                                                  \
                Calculated                                                                                      \
            >{                                                                                                  \
                using type = Type;                                                                              \
                using binary_operator = BinaryOp;                                                               \
                using lhs = expression_template<Type, OtherBinaryOp, OtherL, OtherR, OtherCalc>;                \
                using rhs = built_in_type;                                                                      \
                static const bool calculated = Calculated;                                                      \
                inline expression_template(const lhs &lhs_a, const rhs &rhs_a) :                                \
                    lhs_(lhs_a), rhs_(rhs_a)                                                                    \
                {}                                                                                              \
                inline expression_template(const expression_template &other) :                                  \
                    lhs_(other.lhs_), rhs_(other.rhs_)                                                          \
                {}                                                                                              \
                inline operator type() const{                                                                   \
                    type ret;                                                                                   \
                    eval(ret);                                                                                  \
                    return ret;                                                                                 \
                }                                                                                               \
                inline void eval(type &result) const{                                                           \
                    result = lhs_;                                                                              \
                    specialized_evaluator<                                                                      \
                        binary_operator,                                                                        \
                        binary_operator::commutative,                                                           \
                        false,                                                                                  \
                        binary_operator::operand_num                                                            \
                    >::eval(result, rhs_);                                                                      \
                }                                                                                               \
                const lhs &lhs_;                                                                                \
                const rhs &rhs_;                                                                                \
            };                                                                                                  \
            template<                                                                                           \
                class Type,                                                                                     \
                class BinaryOp,                                                                                 \
                class OtherBinaryOp,                                                                            \
                class OtherL,                                                                                   \
                class OtherR,                                                                                   \
                bool OtherCalc,                                                                                 \
                bool Calculated                                                                                 \
            > struct expression_template<                                                                       \
                Type,                                                                                           \
                BinaryOp,                                                                                       \
                built_in_type,                                                                                  \
                expression_template<Type, OtherBinaryOp, OtherL, OtherR, OtherCalc>,                            \
                Calculated                                                                                      \
            >{                                                                                                  \
                using type = Type;                                                                              \
                using binary_operator = BinaryOp;                                                               \
                using lhs = expression_template<Type, OtherBinaryOp, OtherL, OtherR, OtherCalc>;                \
                using rhs = built_in_type;                                                                      \
                static const bool calculated = Calculated;                                                      \
                inline expression_template(const lhs &lhs_a, const rhs &rhs_a) :                                \
                    lhs_(lhs_a), rhs_(rhs_a)                                                                    \
                {}                                                                                              \
                inline expression_template(const expression_template &other) :                                  \
                    lhs_(other.lhs_), rhs_(other.rhs_)                                                          \
                {}                                                                                              \
                inline operator type() const{                                                                   \
                    type ret;                                                                                   \
                    eval(ret);                                                                                  \
                    return ret;                                                                                 \
                }                                                                                               \
                inline void eval(type &result) const{                                                           \
                    lhs_.eval(result);                                                                          \
                    specialized_evaluator<                                                                      \
                        binary_operator,                                                                        \
                        binary_operator::commutative,                                                           \
                        false,                                                                                  \
                        binary_operator::operand_num                                                            \
                    >::eval(result, rhs_);                                                                      \
                }                                                                                               \
                const lhs &lhs_;                                                                                \
                const rhs &rhs_;                                                                                \
            };

        CMPXX_INVOKE_MACRO_WITH_BUILT_IN_TYPE(CMPXX_AUX_ET_OPERATOR_OVERLOAD_TYPE_TO_BUILT_IN_TYPE, nil);

        template<class Type, class BinaryOp, class R, bool Calculated>
        struct expression_template<Type, BinaryOp, identity_expression<Type>, R, Calculated>{
            using type = Type;
            using binary_operator = BinaryOp;
            using lhs = type;
            using rhs = R;
            static const bool calculated = Calculated;

            inline expression_template(const lhs &lhs_a, const rhs &rhs_a) :
                lhs_(lhs_a), rhs_(rhs_a)
            {}

            inline expression_template(const expression_template &other) :
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
        struct expression_template<Type, BinaryOp, L, identity_expression<Type>, Calculated>{
            using type = Type;
            using binary_operator = BinaryOp;
            using lhs = L;
            using rhs = type;
            static const bool calculated = Calculated;

            inline expression_template(const lhs &lhs_a, const rhs &rhs_a) :
                lhs_(lhs_a), rhs_(rhs_a)
            {}

            inline expression_template(const expression_template &other) :
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

        #define CMPXX_AUX_TMP_TMP_ET_TEMPLATE_IDENTITY_COMBINATION(identifier, decl_tmp_tmp_param, tmp_tmp_param, tmp_tmp_place) \
            template<template<CMPXX_AUX_EXPAND_MACRO_ARGS tmp_tmp_place> class Type, CMPXX_AUX_EXPAND_MACRO_ARGS decl_tmp_tmp_param, class BinaryOp, class R, bool Calculated> \
            struct expression_template_ ## identifier<Type, CMPXX_AUX_EXPAND_MACRO_ARGS tmp_tmp_param, BinaryOp, identity_expression_ ## identifier<Type, CMPXX_AUX_EXPAND_MACRO_ARGS tmp_tmp_param>, R, Calculated>{ \
                using type = Type<CMPXX_AUX_EXPAND_MACRO_ARGS tmp_tmp_param>; \
                using binary_operator = BinaryOp; \
                using lhs = type; \
                using rhs = R; \
                static const bool calculated = Calculated; \
                inline expression_template_ ## identifier(const lhs &lhs_a, const rhs &rhs_a) : \
                    lhs_(lhs_a), rhs_(rhs_a) \
                {} \
                inline expression_template_ ## identifier(const expression_template_ ## identifier &other) : \
                    lhs_(other.lhs_), rhs_(other.rhs_) \
                {} \
                inline void eval(type &result) const{ \
                    type rhs; \
                    result = lhs_; \
                    rhs_.eval(rhs); \
                    cmpxx::aux::specialized_evaluator< \
                        binary_operator, \
                        binary_operator::commutative, \
                        false, \
                        binary_operator::operand_num \
                    >::eval(result, rhs); \
                } \
                const lhs &lhs_; \
                const rhs &rhs_; \
            }; \
            template<template<CMPXX_AUX_EXPAND_MACRO_ARGS tmp_tmp_place> class Type, CMPXX_AUX_EXPAND_MACRO_ARGS decl_tmp_tmp_param, class BinaryOp, class L, bool Calculated> \
            struct expression_template_ ## identifier<Type, CMPXX_AUX_EXPAND_MACRO_ARGS tmp_tmp_param, BinaryOp, L, identity_expression_ ## identifier<Type, CMPXX_AUX_EXPAND_MACRO_ARGS tmp_tmp_param>, Calculated>{ \
                using type = Type<CMPXX_AUX_EXPAND_MACRO_ARGS tmp_tmp_param>; \
                using binary_operator = BinaryOp; \
                using lhs = L; \
                using rhs = type; \
                static const bool calculated = Calculated; \
                inline expression_template_ ## identifier(const lhs &lhs_a, const rhs &rhs_a) : \
                    lhs_(lhs_a), rhs_(rhs_a) \
                {} \
                inline expression_template_ ## identifier(const expression_template_ ## identifier &other) : \
                    lhs_(other.lhs_), rhs_(other.rhs_) \
                {} \
                inline void eval(type &result) const{ \
                    lhs_.eval(result); \
                    cmpxx::aux::specialized_evaluator< \
                        binary_operator, \
                        binary_operator::commutative, \
                        false, \
                        binary_operator::operand_num \
                    >::eval(result, rhs_); \
                } \
                const lhs &lhs_; \
                const rhs &rhs_; \
            };

        #define CMPXX_AUX_TMP_TMP_ET_OPERATOR_OVERLOAD_TYPE_TO_BUILT_IN_TYPE(identifier, decl_tmp_tmp_param, tmp_tmp_param, tmp_tmp_place, built_in_type) \
            template<template<CMPXX_AUX_EXPAND_MACRO_ARGS tmp_tmp_place> class Type, CMPXX_AUX_EXPAND_MACRO_ARGS decl_tmp_tmp_param, class BinaryOp, bool Calculated> \
            struct expression_template_ ## identifier<Type, CMPXX_AUX_EXPAND_MACRO_ARGS tmp_tmp_param, BinaryOp, identity_expression_ ## identifier<Type, CMPXX_AUX_EXPAND_MACRO_ARGS tmp_tmp_param>, built_in_type, Calculated>{ \
                using type = Type<CMPXX_AUX_EXPAND_MACRO_ARGS tmp_tmp_param>; \
                using binary_operator = BinaryOp; \
                using lhs = type; \
                using rhs = built_in_type; \
                static const bool calculated = Calculated; \
                inline expression_template_ ## identifier(const lhs &lhs_a, const rhs &rhs_a) : \
                    lhs_(lhs_a), rhs_(rhs_a) \
                {} \
                inline expression_template_ ## identifier(const expression_template_ ## identifier &other) : \
                    lhs_(other.lhs_), rhs_(other.rhs_) \
                {} \
                inline operator type() const{ \
                    type ret; \
                    eval(ret); \
                    return ret; \
                } \
                inline void eval(type &result) const{ \
                    result = lhs_; \
                    cmpxx::aux::specialized_evaluator< \
                        binary_operator, \
                        binary_operator::commutative, \
                        false, \
                        binary_operator::operand_num \
                    >::eval(result, rhs_); \
                } \
                const lhs &lhs_; \
                const rhs &rhs_; \
            }; \
            template<template<CMPXX_AUX_EXPAND_MACRO_ARGS tmp_tmp_place> class Type, CMPXX_AUX_EXPAND_MACRO_ARGS decl_tmp_tmp_param, class BinaryOp, bool Calculated> \
            struct expression_template_ ## identifier<Type, CMPXX_AUX_EXPAND_MACRO_ARGS tmp_tmp_param, BinaryOp, built_in_type, identity_expression_ ## identifier<Type, CMPXX_AUX_EXPAND_MACRO_ARGS tmp_tmp_param>, Calculated>{ \
                using type = Type<CMPXX_AUX_EXPAND_MACRO_ARGS tmp_tmp_param>; \
                using binary_operator = BinaryOp; \
                using lhs = built_in_type; \
                using rhs = type; \
                static const bool calculated = Calculated; \
                inline expression_template_ ## identifier(const lhs &lhs_a, const rhs &rhs_a) : \
                    lhs_(lhs_a), rhs_(rhs_a) \
                {} \
                inline expression_template_ ## identifier(const expression_template_ ## identifier &other) : \
                    lhs_(other.lhs_), rhs_(other.rhs_) \
                {} \
                inline operator type() const{ \
                    type ret; \
                    eval(ret); \
                    return ret; \
                } \
                inline void eval(type &result) const{ \
                    result = lhs_; \
                    cmpxx::aux::specialized_evaluator< \
                        binary_operator, \
                        binary_operator::commutative, \
                        false, \
                        binary_operator::operand_num \
                    >::eval(result, rhs_); \
                } \
                const lhs &lhs_; \
                const rhs &rhs_; \
            }; \
            template< \
                template<CMPXX_AUX_EXPAND_MACRO_ARGS tmp_tmp_place> class Type, \
                CMPXX_AUX_EXPAND_MACRO_ARGS decl_tmp_tmp_param, \
                class BinaryOp, \
                class OtherBinaryOp, \
                class OtherL, \
                class OtherR, \
                bool OtherCalc, \
                bool Calculated \
            > struct expression_template_ ## identifier< \
                Type, \
                CMPXX_AUX_EXPAND_MACRO_ARGS tmp_tmp_param, \
                BinaryOp, \
                expression_template_ ## identifier<Type, CMPXX_AUX_EXPAND_MACRO_ARGS tmp_tmp_param, OtherBinaryOp, OtherL, OtherR, OtherCalc>, \
                built_in_type, \
                Calculated \
            >{ \
                using type = Type<CMPXX_AUX_EXPAND_MACRO_ARGS tmp_tmp_param>; \
                using binary_operator = BinaryOp; \
                using lhs = expression_template_ ## identifier<Type, CMPXX_AUX_EXPAND_MACRO_ARGS tmp_tmp_param, OtherBinaryOp, OtherL, OtherR, OtherCalc>; \
                using rhs = built_in_type; \
                static const bool calculated = Calculated; \
                inline expression_template_ ## identifier(const lhs &lhs_a, const rhs &rhs_a) : \
                    lhs_(lhs_a), rhs_(rhs_a) \
                {} \
                inline expression_template_ ## identifier(const expression_template_ ## identifier &other) : \
                    lhs_(other.lhs_), rhs_(other.rhs_) \
                {} \
                inline operator type() const{ \
                    type ret; \
                    eval(ret); \
                    return ret; \
                } \
                inline void eval(type &result) const{ \
                    result = lhs_; \
                    cmpxx::aux::specialized_evaluator< \
                        binary_operator, \
                        binary_operator::commutative, \
                        false, \
                        binary_operator::operand_num \
                    >::eval(result, rhs_); \
                } \
                const lhs &lhs_; \
                const rhs &rhs_; \
            }; \
            template< \
                template<CMPXX_AUX_EXPAND_MACRO_ARGS tmp_tmp_place> class Type, \
                CMPXX_AUX_EXPAND_MACRO_ARGS decl_tmp_tmp_param, \
                class BinaryOp, \
                class OtherBinaryOp, \
                class OtherL, \
                class OtherR, \
                bool OtherCalc, \
                bool Calculated \
            > struct expression_template_ ## identifier< \
                Type, \
                CMPXX_AUX_EXPAND_MACRO_ARGS tmp_tmp_param, \
                BinaryOp, \
                built_in_type, \
                expression_template_ ## identifier<Type, CMPXX_AUX_EXPAND_MACRO_ARGS tmp_tmp_param, OtherBinaryOp, OtherL, OtherR, OtherCalc>, \
                Calculated \
            >{ \
                using type = Type<CMPXX_AUX_EXPAND_MACRO_ARGS tmp_tmp_param>; \
                using binary_operator = BinaryOp; \
                using lhs = expression_template_ ## identifier<Type, CMPXX_AUX_EXPAND_MACRO_ARGS tmp_tmp_param, OtherBinaryOp, OtherL, OtherR, OtherCalc>; \
                using rhs = built_in_type; \
                static const bool calculated = Calculated; \
                inline expression_template_ ## identifier(const lhs &lhs_a, const rhs &rhs_a) : \
                    lhs_(lhs_a), rhs_(rhs_a) \
                {} \
                inline expression_template_ ## identifier(const expression_template_ ## identifier &other) : \
                    lhs_(other.lhs_), rhs_(other.rhs_) \
                {} \
                inline operator type() const{ \
                    type ret; \
                    eval(ret); \
                    return ret; \
                } \
                inline void eval(type &result) const{ \
                    lhs_.eval(result); \
                    cmpxx::aux::specialized_evaluator< \
                        binary_operator, \
                        binary_operator::commutative, \
                        false, \
                        binary_operator::operand_num \
                    >::eval(result, rhs_); \
                } \
                const lhs &lhs_; \
                const rhs &rhs_; \
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
        struct trace_expression<expression_template<Type, Op, L, R, true>, Depth, DirAndParent>{
            using expression = expression_template<Type, Op, L, R, true>;
            using calculated = boolean_type<true>;
            using next = typename if_dir<
                DirAndParent,
                type_parent_trace_rhs<DirAndParent>,
                type_parent_calculation<DirAndParent>
            >::type;
            static const std::size_t depth = Depth;
        };

        template<class Type, class Op, class L, class R, std::size_t Depth, class DirAndParent>
        struct trace_expression<expression_template<Type, Op, L, R, false>, Depth, DirAndParent>{
            using this_type = trace_expression;
            using expression = expression_template<Type, Op, L, R, false>;
            using parent = typename DirAndParent::parent;
            using trace_lhs = trace_expression<L, Depth + 1, dir_and_parent<boolean_type<true>, this_type>>;
            using trace_rhs = trace_expression<R, Depth + 1, dir_and_parent<boolean_type<false>, this_type>>;
            using calculated = boolean_type<false>;
            using calculation = trace_expression<expression_template<Type, Op, L, R, true>, Depth, DirAndParent>;
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

    #define CMPXX_AUX_ET_OPERATOR_OVERLOAD_BUILT_IN_TYPE(op, op_code, type, built_in_type)  \
            template<class Op, class T, class U>                                            \
            cmpxx::aux::expression_template<                                                \
                type,                                                                       \
                cmpxx::aux::op,                                                             \
                cmpxx::aux::expression_template<type, Op, T, U>,                            \
                built_in_type                                                               \
            > operator op_code(                                                             \
                const cmpxx::aux::expression_template<type, Op, T, U> &lhs,                 \
                const built_in_type &rhs                                                    \
            ){                                                                              \
                return cmpxx::aux::expression_template<                                     \
                    type,                                                                   \
                    cmpxx::aux::op,                                                         \
                    cmpxx::aux::expression_template<type, Op, T, U>,                        \
                    built_in_type                                                           \
                >(lhs, rhs);                                                                \
            }                                                                               \
            template<class Op, class T, class U>                                            \
            cmpxx::aux::expression_template<                                                \
                type,                                                                       \
                cmpxx::aux::op,                                                             \
                built_in_type,                                                              \
                cmpxx::aux::expression_template<type, Op, T, U>                             \
            > operator op_code(                                                             \
                const built_in_type &lhs,                                                   \
                const cmpxx::aux::expression_template<type, Op, T, U> &rhs                  \
            ){                                                                              \
                return cmpxx::aux::expression_template<                                     \
                    type,                                                                   \
                    cmpxx::aux::op,                                                         \
                    built_in_type,                                                          \
                    cmpxx::aux::expression_template<type, Op, T, U>                         \
                >(lhs, rhs);                                                                \
            }

        #define CMPXX_AUX_ET_OPERATOR_OVERLOAD(type, op, op_code)               \
            cmpxx::aux::expression_template<                                    \
                type,                                                           \
                cmpxx::aux::op,                                                 \
                cmpxx::aux::identity_expression<type>,                          \
                cmpxx::aux::identity_expression<type>                           \
            > operator op_code(const type &lhs, const type &rhs){               \
                return cmpxx::aux::expression_template<                         \
                    type,                                                       \
                    cmpxx::aux::op,                                             \
                    cmpxx::aux::identity_expression<type>,                      \
                    cmpxx::aux::identity_expression<type>                       \
                >(lhs, rhs);                                                    \
            }                                                                   \
            template<class Op, class T, class U>                                \
            cmpxx::aux::expression_template<                                    \
                type,                                                           \
                cmpxx::aux::op,                                                 \
                cmpxx::aux::identity_expression<type>,                          \
                cmpxx::aux::expression_template<type, Op, T, U>                 \
            > operator op_code(                                                 \
                const type &lhs,                                                \
                const cmpxx::aux::expression_template<type, Op, T, U> &rhs      \
            ){                                                                  \
                return cmpxx::aux::expression_template<                         \
                    type,                                                       \
                    cmpxx::aux::op,                                             \
                    cmpxx::aux::identity_expression<type>,                      \
                    cmpxx::aux::expression_template<type, Op, T, U>             \
                >(lhs, rhs);                                                    \
            }                                                                   \
            template<class Op, class T, class U>                                \
            cmpxx::aux::expression_template<                                    \
                type,                                                           \
                cmpxx::aux::op,                                                 \
                cmpxx::aux::expression_template<type, Op, T, U>,                \
                cmpxx::aux::identity_expression<type>                           \
            > operator op_code(                                                 \
                const cmpxx::aux::expression_template<type, Op, T, U> &lhs,     \
                const type &rhs                                                 \
            ){                                                                  \
                return cmpxx::aux::expression_template<                         \
                    type,                                                       \
                    cmpxx::aux::op,                                             \
                    cmpxx::aux::expression_template<type, Op, T, U>,            \
                    cmpxx::aux::identity_expression<type>                       \
                >(lhs, rhs);                                                    \
            }                                                                   \
            template<class OpT, class OpU, class T, class U, class V, class W>  \
            cmpxx::aux::expression_template<                                    \
                type,                                                           \
                cmpxx::aux::op,                                                 \
                cmpxx::aux::expression_template<type, OpT, T, U>,               \
                cmpxx::aux::expression_template<type, OpU, V, W>                \
            > operator op_code(                                                 \
                const cmpxx::aux::expression_template<type, OpT, T, U> &lhs,    \
                const cmpxx::aux::expression_template<type, OpU, V, W> &rhs     \
            ){                                                                  \
                return cmpxx::aux::expression_template<                         \
                    type,                                                       \
                    cmpxx::aux::op,                                             \
                    cmpxx::aux::expression_template<type, OpT, T, U>,           \
                    cmpxx::aux::expression_template<type, OpU, V, W>            \
                >(lhs, rhs);                                                    \
            }                                                                   \
            CMPXX_INVOKE_MACRO_WITH_BUILT_IN_TYPE(CMPXX_AUX_ET_OPERATOR_OVERLOAD_BUILT_IN_TYPE, op, op_code, type)

        #define CMPXX_AUX_TMP_TMP_ET_OPERATOR_OVERLOAD_BUILT_IN_TYPE(identifier, decl_tmp_tmp_param, tmp_tmp_param, tmp_tmp_place, op, op_code, built_in_type) \
            template<template<CMPXX_AUX_EXPAND_MACRO_ARGS tmp_tmp_place> class type, CMPXX_AUX_EXPAND_MACRO_ARGS decl_tmp_tmp_param, class CMPXX_AUX_TYPE_Op, class CMPXX_AUX_TYPE_T, class CMPXX_AUX_TYPE_U> \
            expression_template_ ## identifier< \
                type, \
                CMPXX_AUX_EXPAND_MACRO_ARGS tmp_tmp_param, \
                cmpxx::aux::op, \
                expression_template_ ## identifier<type, CMPXX_AUX_EXPAND_MACRO_ARGS tmp_tmp_param, CMPXX_AUX_TYPE_Op, CMPXX_AUX_TYPE_T, CMPXX_AUX_TYPE_U>, \
                built_in_type \
            > operator op_code( \
                const expression_template_ ## identifier<type, CMPXX_AUX_EXPAND_MACRO_ARGS tmp_tmp_param, CMPXX_AUX_TYPE_Op, CMPXX_AUX_TYPE_T, CMPXX_AUX_TYPE_U> &lhs, \
                const built_in_type &rhs \
            ){ \
                return expression_template_ ## identifier< \
                    type, \
                    CMPXX_AUX_EXPAND_MACRO_ARGS tmp_tmp_param, \
                    cmpxx::aux::op, \
                    expression_template_ ## identifier<type, CMPXX_AUX_EXPAND_MACRO_ARGS tmp_tmp_param, CMPXX_AUX_TYPE_Op, CMPXX_AUX_TYPE_T, CMPXX_AUX_TYPE_U>, \
                    built_in_type \
                >(lhs, rhs); \
            } \
            template<template<CMPXX_AUX_EXPAND_MACRO_ARGS tmp_tmp_place> class type, CMPXX_AUX_EXPAND_MACRO_ARGS decl_tmp_tmp_param, class CMPXX_AUX_TYPE_Op, class CMPXX_AUX_TYPE_T, class CMPXX_AUX_TYPE_U> \
            expression_template_ ## identifier< \
                type, \
                CMPXX_AUX_EXPAND_MACRO_ARGS tmp_tmp_param, \
                cmpxx::aux::op, \
                built_in_type, \
                expression_template_ ## identifier<type, CMPXX_AUX_EXPAND_MACRO_ARGS tmp_tmp_param, CMPXX_AUX_TYPE_Op, CMPXX_AUX_TYPE_T, CMPXX_AUX_TYPE_U> \
            > operator op_code( \
                const built_in_type &lhs, \
                const expression_template_ ## identifier<type, CMPXX_AUX_EXPAND_MACRO_ARGS tmp_tmp_param, CMPXX_AUX_TYPE_Op, CMPXX_AUX_TYPE_T, CMPXX_AUX_TYPE_U> &rhs \
            ){ \
                return expression_template_ ## identifier< \
                    type, \
                    CMPXX_AUX_EXPAND_MACRO_ARGS tmp_tmp_param, \
                    cmpxx::aux::op, \
                    built_in_type, \
                    expression_template_ ## identifier<type, CMPXX_AUX_EXPAND_MACRO_ARGS tmp_tmp_param, CMPXX_AUX_TYPE_Op, CMPXX_AUX_TYPE_T, CMPXX_AUX_TYPE_U> \
                >(lhs, rhs); \
            }

        #define CMPXX_AUX_TMP_TMP_ET_OPERATOR_OVERLOAD(identifier, decl_tmp_tmp_param, tmp_tmp_param, tmp_tmp_place, op, op_code) \
            template<template<CMPXX_AUX_EXPAND_MACRO_ARGS decl_tmp_tmp_param> class type, CMPXX_AUX_EXPAND_MACRO_ARGS decl_tmp_tmp_param> \
            expression_template_ ## identifier< \
                type, \
                CMPXX_AUX_EXPAND_MACRO_ARGS tmp_tmp_param, \
                cmpxx::aux::op, \
                identity_expression_ ## identifier<type, CMPXX_AUX_EXPAND_MACRO_ARGS tmp_tmp_param>, \
                identity_expression_ ## identifier<type, CMPXX_AUX_EXPAND_MACRO_ARGS tmp_tmp_param> \
            > operator op_code(const type<CMPXX_AUX_EXPAND_MACRO_ARGS tmp_tmp_param> &lhs, const type<CMPXX_AUX_EXPAND_MACRO_ARGS tmp_tmp_param> &rhs){ \
                return expression_template_ ## identifier< \
                    type, \
                    CMPXX_AUX_EXPAND_MACRO_ARGS tmp_tmp_param, \
                    cmpxx::aux::op, \
                    identity_expression_ ## identifier<type, CMPXX_AUX_EXPAND_MACRO_ARGS tmp_tmp_param>, \
                    identity_expression_ ## identifier<type, CMPXX_AUX_EXPAND_MACRO_ARGS tmp_tmp_param> \
                >(lhs, rhs); \
            } \
            template<template<CMPXX_AUX_EXPAND_MACRO_ARGS tmp_tmp_place> class type, CMPXX_AUX_EXPAND_MACRO_ARGS decl_tmp_tmp_param, class CMPXX_AUX_TYPE_Op, class CMPXX_AUX_TYPE_T, class CMPXX_AUX_TYPE_U> \
            expression_template_ ## identifier< \
                type, \
                CMPXX_AUX_EXPAND_MACRO_ARGS tmp_tmp_param, \
                cmpxx::aux::op, \
                identity_expression_ ## identifier<type, CMPXX_AUX_EXPAND_MACRO_ARGS tmp_tmp_param>, \
                expression_template_ ## identifier<type, CMPXX_AUX_EXPAND_MACRO_ARGS tmp_tmp_param, CMPXX_AUX_TYPE_Op, CMPXX_AUX_TYPE_T, CMPXX_AUX_TYPE_U> \
            > operator op_code( \
                const type<CMPXX_AUX_EXPAND_MACRO_ARGS tmp_tmp_param> &lhs, \
                const expression_template_ ## identifier<type, CMPXX_AUX_EXPAND_MACRO_ARGS tmp_tmp_param, CMPXX_AUX_TYPE_Op, CMPXX_AUX_TYPE_T, CMPXX_AUX_TYPE_U> &rhs \
            ){ \
                return expression_template_ ## identifier< \
                    type, \
                    CMPXX_AUX_EXPAND_MACRO_ARGS tmp_tmp_param, \
                    cmpxx::aux::op, \
                    identity_expression_ ## identifier<type, CMPXX_AUX_EXPAND_MACRO_ARGS tmp_tmp_param>, \
                    expression_template_ ## identifier<type, CMPXX_AUX_EXPAND_MACRO_ARGS tmp_tmp_param, CMPXX_AUX_TYPE_Op, CMPXX_AUX_TYPE_T, CMPXX_AUX_TYPE_U> \
                >(lhs, rhs); \
            } \
            template<template<CMPXX_AUX_EXPAND_MACRO_ARGS tmp_tmp_place> class type, CMPXX_AUX_EXPAND_MACRO_ARGS decl_tmp_tmp_param, class CMPXX_AUX_TYPE_Op, class CMPXX_AUX_TYPE_T, class CMPXX_AUX_TYPE_U> \
            expression_template_ ## identifier< \
                type, \
                CMPXX_AUX_EXPAND_MACRO_ARGS tmp_tmp_param, \
                cmpxx::aux::op, \
                expression_template_ ## identifier<type, CMPXX_AUX_EXPAND_MACRO_ARGS tmp_tmp_param, CMPXX_AUX_TYPE_Op, CMPXX_AUX_TYPE_T, CMPXX_AUX_TYPE_U>, \
                identity_expression_ ## identifier<type, CMPXX_AUX_EXPAND_MACRO_ARGS tmp_tmp_param> \
            > operator op_code( \
                const expression_template_ ## identifier<type, CMPXX_AUX_EXPAND_MACRO_ARGS tmp_tmp_param, CMPXX_AUX_TYPE_Op, CMPXX_AUX_TYPE_T, CMPXX_AUX_TYPE_U> &lhs, \
                const type<CMPXX_AUX_EXPAND_MACRO_ARGS tmp_tmp_param> &rhs \
            ){ \
                return expression_template_ ## identifier< \
                    type, \
                    CMPXX_AUX_EXPAND_MACRO_ARGS tmp_tmp_param, \
                    cmpxx::aux::op, \
                    expression_template_ ## identifier<type, CMPXX_AUX_EXPAND_MACRO_ARGS tmp_tmp_param, CMPXX_AUX_TYPE_Op, CMPXX_AUX_TYPE_T, CMPXX_AUX_TYPE_U>, \
                    identity_expression_ ## identifier<type, CMPXX_AUX_EXPAND_MACRO_ARGS tmp_tmp_param> \
                >(lhs, rhs); \
            } \
            template<template<CMPXX_AUX_EXPAND_MACRO_ARGS tmp_tmp_place> class type, CMPXX_AUX_EXPAND_MACRO_ARGS decl_tmp_tmp_param, class CMPXX_AUX_TYPE_OpT, class CMPXX_AUX_TYPE_OpU, class CMPXX_AUX_TYPE_T, class CMPXX_AUX_TYPE_U, class CMPXX_AUX_TYPE_V, class CMPXX_AUX_TYPE_W> \
            expression_template_ ## identifier< \
                type, \
                CMPXX_AUX_EXPAND_MACRO_ARGS tmp_tmp_param, \
                cmpxx::aux::op, \
                expression_template_ ## identifier<type, CMPXX_AUX_EXPAND_MACRO_ARGS tmp_tmp_param, CMPXX_AUX_TYPE_OpT, CMPXX_AUX_TYPE_T, CMPXX_AUX_TYPE_U>, \
                expression_template_ ## identifier<type, CMPXX_AUX_EXPAND_MACRO_ARGS tmp_tmp_param, CMPXX_AUX_TYPE_OpU, CMPXX_AUX_TYPE_V, CMPXX_AUX_TYPE_W> \
            > operator op_code( \
                const expression_template_ ## identifier<type, CMPXX_AUX_EXPAND_MACRO_ARGS tmp_tmp_param, CMPXX_AUX_TYPE_OpT, CMPXX_AUX_TYPE_T, CMPXX_AUX_TYPE_U> &lhs, \
                const expression_template_ ## identifier<type, CMPXX_AUX_EXPAND_MACRO_ARGS tmp_tmp_param, CMPXX_AUX_TYPE_OpU, CMPXX_AUX_TYPE_V, CMPXX_AUX_TYPE_W> &rhs \
            ){ \
                return expression_template_ ## identifier< \
                    type, \
                    CMPXX_AUX_EXPAND_MACRO_ARGS tmp_tmp_param, \
                    cmpxx::aux::op, \
                    expression_template_ ## identifier<type, CMPXX_AUX_EXPAND_MACRO_ARGS tmp_tmp_param, CMPXX_AUX_TYPE_OpT, CMPXX_AUX_TYPE_T, CMPXX_AUX_TYPE_U>, \
                    expression_template_ ## identifier<type, CMPXX_AUX_EXPAND_MACRO_ARGS tmp_tmp_param, CMPXX_AUX_TYPE_OpU, CMPXX_AUX_TYPE_V, CMPXX_AUX_TYPE_W> \
                >(lhs, rhs); \
            } \
            CMPXX_INVOKE_MACRO_WITH_BUILT_IN_TYPE(CMPXX_AUX_TMP_TMP_ET_OPERATOR_OVERLOAD_BUILT_IN_TYPE, identifier, decl_tmp_tmp_param, tmp_tmp_param, tmp_tmp_place, op, op_code)

        #define CMPXX_AUX_GENERATE_TMP_TMP_PARAM_ET(identifier, decl_tmp_tmp_param, tmp_tmp_param, tmp_tmp_place) \
            CMPXX_AUX_TMP_TMP_ET_IDENTITY_EXPRESSION(identifier, decl_tmp_tmp_param, tmp_tmp_param, tmp_tmp_place); \
            CMPXX_AUX_TMP_TMP_ET_EXPRESSION_TEMPLATE(identifier, decl_tmp_tmp_param, tmp_tmp_param, tmp_tmp_place); \
            CMPXX_AUX_TMP_TMP_ET_TEMPLATE_IDENTITY_COMBINATION(identifier, decl_tmp_tmp_param, tmp_tmp_param, tmp_tmp_place); \
            CMPXX_INVOKE_MACRO_WITH_BUILT_IN_TYPE(CMPXX_AUX_TMP_TMP_ET_OPERATOR_OVERLOAD_TYPE_TO_BUILT_IN_TYPE, identifier, decl_tmp_tmp_param, tmp_tmp_param, tmp_tmp_place); \
            CMPXX_AUX_TMP_TMP_ET_EXPRESSION_TEMPLATE_IDENTITY_IDENTITY(identifier, decl_tmp_tmp_param, tmp_tmp_param, tmp_tmp_place);

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

#endif
