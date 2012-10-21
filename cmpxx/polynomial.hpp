#ifndef INCLUDE_CMPXX_POLYNOMIAL
#define INCLUDE_CMPXX_POLYNOMIAL

// polynomial

#include <utility>
#include <map>
#include <set>
#include <algorithm>
#include <iterator>
#include <memory>
#include "aux_xx.hpp"

namespace cmpxx{
    CMPXX_AUX_GENERATE_TMP_TMP_PARAM_ET(
        polynomial,
        (class Order, class Coefficient, bool HasInverseElements, class Alloc),
        (Order, Coefficient, HasInverseElements, Alloc),
        (class, class, bool , class)
    );

    template<class Order, class Coefficient, bool HasInverseElements, class Alloc = std::allocator<int>>
    class polynomial{
    private:
        using basic_ordered_container = std::map<
            Order,
            Coefficient,
            std::less<Order>,
            typename Alloc::template rebind<
                std::pair<const Order, Coefficient>
            >::other
        >;

    public:
        typedef typename basic_ordered_container::key_type order;
        typedef typename basic_ordered_container::mapped_type coefficient;
        static const bool has_inverse_elements = HasInverseElements;

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

        using taylor_expanded_result = std::map<order, polynomial>;

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

        template<template<class, class, bool, class> class Other, class Op, class L, class R, bool Calculated>
        inline polynomial(
            const expression_template_polynomial<
                Other, Order, Coefficient, HasInverseElements, Alloc,
                Op, L, R, Calculated
            > &expression
        ) : container(){ expression.eval(*this); }

        #define CMPXX_POLYNOMIAL_CTOR(nil, type) \
            inline polynomial(type value) :      \
                container()                      \
            { if(value != 0){ container.insert(typename ordered_container::ref_value_type(0, coefficient(value))); } }

        CMPXX_INVOKE_MACRO_WITH_BUILT_IN_TYPE(CMPXX_POLYNOMIAL_CTOR, nil);

    private:
        inline polynomial(const ordered_container &container_) :
            container(container_)
        {}

        inline polynomial(
            const typename basic_ordered_container::const_iterator &first,
            const typename basic_ordered_container::const_iterator last
        ) : container(first, last){}

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

        template<template<class, class, bool, class> class Other, class Op, class L, class R, bool Calculated>
        inline polynomial &operator =(
            const expression_template_polynomial<
                Other, Order, Coefficient, HasInverseElements, Alloc,
                Op, L, R, Calculated
            > &expression
        ){
            polynomial temp;
            expression.eval(temp);
            *this = std::move(temp);
            return *this;
        }

        #define CMPXX_POLYNOMIAL_DEFINE_ET_COMPOUND_OPERATOR(op)                                                    \
            template<template<class, class, bool, class> class Other, class Op, class L, class R, bool Calculated>  \
            inline polynomial &operator op(                                                                         \
                const expression_template_polynomial<                                                               \
                    Other, order, coefficient, has_inverse_elements, Alloc,                                         \
                    Op, L, R, Calculated                                                                            \
                > &expression                                                                                       \
            ){                                                                                                      \
                polynomial temp;                                                                                    \
                expression.eval(temp);                                                                              \
                *this op temp;                                                                                      \
                return *this;                                                                                       \
            }

        CMPXX_POLYNOMIAL_DEFINE_ET_COMPOUND_OPERATOR(+=);
        CMPXX_POLYNOMIAL_DEFINE_ET_COMPOUND_OPERATOR(-=);
        CMPXX_POLYNOMIAL_DEFINE_ET_COMPOUND_OPERATOR(*=);
        CMPXX_POLYNOMIAL_DEFINE_ET_COMPOUND_OPERATOR(/=);
        CMPXX_POLYNOMIAL_DEFINE_ET_COMPOUND_OPERATOR(%=);

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
            //*this = kar_mul_impl(*this, rhs);
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

        /*
         * 除算を行う
         * Rem : true であれば商と同時に剰余を得る
         * rem : 剰余
         * lhs : 左辺
         * rhs : 右辺
         * return : 商
         * 備考 : rhs が monic の場合は Newton iteration を使った高速除算が行われる
         * */
        template<bool Rem>
        static polynomial div(polynomial &rem, const polynomial &lhs, const polynomial &rhs){
            if(lhs.container.empty()){ return lhs; }
            if(rhs.is_monic()){
                return monic_div<Rem>(rem, lhs, rhs);
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

        /*
         * 古典的な gcd を行う
         * 備考 : coefficient は乗算に対して逆元を持っていてはならない
         * */
        inline static polynomial classical_gcd(const polynomial &x, const polynomial &y){
            static_assert(!has_inverse_elements, "has_inverse_elements == true");
            if(x >= y){
                return classical_gcd_impl(x, y);
            }else{
                return classical_gcd_impl(y, x);
            }
        }

        /*
         * 古典的な拡張ユークリッドアルゴリズムを行う
         * cl : 入力の一次結合による算出結果の表現に使われる左辺の係数
         * cr : 入力の一次結合による算出結果の表現に使われる右辺の係数
         * l  : 左辺
         * r  : 右辺
         * return : 結果
         * */
        inline static polynomial classical_eea(polynomial &cl, polynomial &cr, const polynomial &l, const polynomial &r){
            if(l >= r){
                return classical_eea_impl(cl, cr, l, r);
            }else{
                return classical_eea_impl(cr, cl, r, l);
            }
        }

        /*
         * 拡張ユークリッドアルゴリズムを行う
         * cl : 入力の一次結合による算出結果の表現に使われる左辺の係数
         * cr : 入力の一次結合による算出結果の表現に使われる右辺の係数
         * l  : 左辺
         * r  : 右辺
         * return : 結果
         * 備考 : coefficient は乗算に関して逆元を持っていなければならない
         * */
        inline static polynomial eea(polynomial &cl, polynomial &cr, const polynomial &l, const polynomial &r){
            static_assert(has_inverse_elements, "has_inverse_elements == false");
            if(l >= r){
                return eea_impl(cl, cr, l, r);
            }else{
                return eea_impl(cr, cl, r, l);
            }
        }

        /*
         * Newton iteration による逆元計算を行う
         * l : 法となるxの非負の整数による指数
         * return : f * g === 1 mod x^l となる g
         * 備考 : このメンバを呼び出す時, 0 次の係数が 1 でなければならない
         *        もしくは, coefficient が乗法に関して必ず逆元を持つ場合
         *        0 次の係数が 0 以外のなんらかの値でなければならない
         * */
        polynomial inverse(const order &l) const{
            const polynomial &f = *this;
            polynomial g = 1;
            if(has_inverse_elements){
                auto iter = f.container.find(0);
                g.container.begin()->second /= iter->second;
            }
            std::size_t r = l.ceil_log2();
            order rem = 2;
            for(std::size_t i = 0; i < r; ++i, rem *= rem){
                polynomial next_g = 2 * g - f * g * g;
                next_g.container.erase(next_g.container.lower_bound(rem), next_g.container.end());
                g = std::move(next_g);
            }
            return g;
        }

        /*
         * Newton iteration による p 進数逆元計算
         * f, g, p, l : f * g === 1 mod p となる f, g
         *              l は非負の整数でなければならない
         *              f, g の coefficient は乗法に関して逆元を持っていてはならない
         * return : f * g === 1 mod p^l なる g
         * */
        static polynomial inverse(const polynomial &f, const polynomial &g, const polynomial &p, const order &l){
            static_assert(!has_inverse_elements, "has_inverse_elements == true");
            polynomial g_i = g, p_2_i = p;
            for(std::size_t i = 0, r = l.ceil_log2(); i < r; ++i){
                p_2_i = p_2_i * p_2_i;
                polynomial next_g = (2 * g_i - f * g_i * g_i) % p_2_i;
                g_i = std::move(next_g);
            }
            return g_i;
        }

        /*
         * 一般 Taylor 展開
         * p : monic な多項式
         * return : this = Σ_{ 0 <= i < k } a_i * p^i (0 <= i < k, deg a_i < m, deg a < k * m, k = 2^n, n > 0)
         *          なる a_0, ..., a_{ k - 1 }
         * */
        inline taylor_expanded_result taylor_expansion(const polynomial &p) const{
            std::size_t n = coefficient::lower_bound_pow2_coefficient(deg(), p.deg());
            return taylor_expansion_impl(p, n);
        }

        /*
         * 最高次数
         * */
        inline const order &deg() const{
            return container.rbegin()->first;
        }

        /*
         * 最高次の係数
         * */
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

        /*
         * 現在の多項式が monic かどうか
         * */
        inline bool is_monic() const{
            return !container.empty() && lc() == 1;
        }

        /*
         * 現在の多項式の係数の符号を全て反転する
         * */
        inline void negative_sign(){
            for(auto &x : container){
                x.second *= -1;
            }
        }

        /*
         * 現在の多項式の係数を全て正規化する
         * coefficient のメンバに canonicalize 関数がなければならない
         * */
        inline void canonicalize(){
            for(auto &x : container){
                x.second.canonicalize();
            }
        }

        template<class Variable>
        inline std::string get_str(const Variable &v) const{
            return get_str_impl<std::string, char, Variable, std::ostringstream>(v, '*', '^', '+', '-', '0', '(', ')');
        }

        inline std::string get_str() const{
            return get_str("x");
        }

    private:
        taylor_expanded_result taylor_expansion_impl(const polynomial &p, std::size_t n) const{
            taylor_expanded_result result;
            if(n == 0){
                if(!container.empty()){ result.insert(std::make_pair(0, *this)); }
                return result;
            }
            --n;
            polynomial pt = aux::iterate_square_pow2(p, n);
            polynomial q, r;
            q = div<true>(r, *this, pt);
            {
                taylor_expanded_result result_q = q.taylor_expansion_impl(p, n);
                order p2 = 1;
                p2 << n;
                for(auto &a_i : result_q){
                    result.insert(std::make_pair(a_i.first + p2, std::move(const_cast<polynomial&>(a_i.second))));
                }
            }
            {
                taylor_expanded_result result_r = r.taylor_expansion_impl(p, n);
                for(auto &a_i : result_r){
                    result.insert(std::make_pair(std::move(const_cast<order&>(a_i.first)), std::move(const_cast<polynomial&>(a_i.second))));
                }
            }
            return result;
        }

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

    #define CMPXX_POLYNOMIAL_OPERATOR_OVERLOAD(op, a_op)                                \
        template<class Order, class Coefficient, bool HasInverseElements, class Alloc>  \
        inline polynomial<Order, Coefficient, HasInverseElements, Alloc>                \
        operator op(                                                                    \
            const polynomial<Order, Coefficient, HasInverseElements, Alloc> &l,         \
            const Coefficient &r                                                        \
        ){                                                                              \
            polynomial<Order, Coefficient, HasInverseElements, Alloc> ret(l);           \
            ret a_op r;                                                                 \
            return ret;                                                                 \
        }                                                                               \
        template<class Order, class Coefficient, bool HasInverseElements, class Alloc>  \
        inline polynomial<Order, Coefficient, HasInverseElements, Alloc>                \
        operator op(                                                                    \
            const Coefficient &l,                                                       \
            const polynomial<Order, Coefficient, HasInverseElements, Alloc> &r          \
        ){                                                                              \
            polynomial<Order, Coefficient, HasInverseElements, Alloc> ret(l);           \
            ret a_op r;                                                                 \
            return ret;                                                                 \
        }

    CMPXX_POLYNOMIAL_OPERATOR_OVERLOAD(+, +=);
    CMPXX_POLYNOMIAL_OPERATOR_OVERLOAD(-, -=);
    CMPXX_POLYNOMIAL_OPERATOR_OVERLOAD(*, *=);
    CMPXX_POLYNOMIAL_OPERATOR_OVERLOAD(/, /=);
    CMPXX_POLYNOMIAL_OPERATOR_OVERLOAD(%, %=);

    CMPXX_AUX_TMP_TMP_ET_OPERATOR_OVERLOAD(
        polynomial,
        (class Order, class Coefficient, bool HasInverseElements, class Alloc),
        (Order, Coefficient, HasInverseElements, Alloc),
        (class, class, bool, class),
        exp_operator_add<>,
        +
    );

    CMPXX_AUX_TMP_TMP_ET_OPERATOR_OVERLOAD(
        polynomial,
        (class Order, class Coefficient, bool HasInverseElements, class Alloc),
        (Order, Coefficient, HasInverseElements, Alloc),
        (class, class, bool, class),
        exp_operator_sub<>,
        -
    );

    CMPXX_AUX_TMP_TMP_ET_OPERATOR_OVERLOAD(
        polynomial,
        (class Order, class Coefficient, bool HasInverseElements, class Alloc),
        (Order, Coefficient, HasInverseElements, Alloc),
        (class, class, bool, class),
        exp_operator_mul<>,
        *
    );

    CMPXX_AUX_TMP_TMP_ET_OPERATOR_OVERLOAD(
        polynomial,
        (class Order, class Coefficient, bool HasInverseElements, class Alloc),
        (Order, Coefficient, HasInverseElements, Alloc),
        (class, class, bool, class),
        exp_operator_div<>,
        /
    );

    CMPXX_AUX_TMP_TMP_ET_OPERATOR_OVERLOAD(
        polynomial,
        (class Order, class Coefficient, bool HasInverseElements, class Alloc),
        (Order, Coefficient, HasInverseElements, Alloc),
        (class, class, bool, class),
        exp_operator_rem<>,
        %
    );

    namespace aux{
        template<class T = void>
        struct positive_sign_functor{
            template<class Polynomial>
            inline void operator ()(const Polynomial &x) const{
                /* empty */
            }
        };

        template<class T = void>
        struct negative_sign_functor{
            template<class Polynomial>
            inline void operator ()(Polynomial &x) const{
                x.negative_sign();
            }
        };
    }

    CMPXX_AUX_TMP_TMP_ET_UNARY_OPERATOR_OVERLOAD(
        polynomial,
        (class Order, class Coefficient, bool HasInverseElements, class Alloc),
        (Order, Coefficient, HasInverseElements, Alloc),
        (class, class, bool, class),
        exp_operator_positive<aux::positive_sign_functor<>>, +
    );

    CMPXX_AUX_TMP_TMP_ET_UNARY_OPERATOR_OVERLOAD(
        polynomial,
        (class Order, class Coefficient, bool HasInverseElements, class Alloc),
        (Order, Coefficient, HasInverseElements, Alloc),
        (class, class, bool, class),
        exp_operator_negative<aux::negative_sign_functor<>>, -
    );

    #define CMPXX_POLYNOMIAL_BUILT_IN_TYPE_OPERATOR_OVERLOAD(op, a_op, t)               \
        template<class Order, class Coefficient, bool HasInverseElements, class Alloc>  \
        inline polynomial<Order, Coefficient, HasInverseElements, Alloc>                \
        operator op(                                                                    \
            const polynomial<Order, Coefficient, HasInverseElements, Alloc> &l,         \
            t v                                                                         \
        ){                                                                              \
            polynomial<Order, Coefficient, HasInverseElements, Alloc> ret(l);           \
            ret a_op polynomial<Order, Coefficient, HasInverseElements, Alloc>(v);      \
            return ret;                                                                 \
        }                                                                               \
        template<class Order, class Coefficient, bool HasInverseElements, class Alloc>  \
        inline polynomial<Order, Coefficient, HasInverseElements, Alloc>                \
        operator op(                                                                    \
            t v,                                                                        \
            const polynomial<Order, Coefficient, HasInverseElements, Alloc> &r          \
        ){                                                                              \
            polynomial<Order, Coefficient, HasInverseElements, Alloc> ret(v);           \
            ret a_op r;                                                                 \
            return ret;                                                                 \
        }

    CMPXX_INVOKE_MACRO_WITH_BUILT_IN_TYPE(CMPXX_POLYNOMIAL_BUILT_IN_TYPE_OPERATOR_OVERLOAD, +, +=);
    CMPXX_INVOKE_MACRO_WITH_BUILT_IN_TYPE(CMPXX_POLYNOMIAL_BUILT_IN_TYPE_OPERATOR_OVERLOAD, -, -=);
    CMPXX_INVOKE_MACRO_WITH_BUILT_IN_TYPE(CMPXX_POLYNOMIAL_BUILT_IN_TYPE_OPERATOR_OVERLOAD, *, *=);
    CMPXX_INVOKE_MACRO_WITH_BUILT_IN_TYPE(CMPXX_POLYNOMIAL_BUILT_IN_TYPE_OPERATOR_OVERLOAD, /, /=);
    CMPXX_INVOKE_MACRO_WITH_BUILT_IN_TYPE(CMPXX_POLYNOMIAL_BUILT_IN_TYPE_OPERATOR_OVERLOAD, %, %=);

    #define CMPXX_POLYNOMIAL_BOOLEAN_OPERATOR_OVERLOAD_IMPL(op, t)                      \
        template<class Order, class Coefficient, bool HasInverseElements, class Alloc>  \
        inline bool operator op(                                                        \
            const polynomial<Order, Coefficient, HasInverseElements, Alloc> &l,         \
            const t &r                                                                  \
        ){ return l op polynomial<Order, Coefficient, HasInverseElements, Alloc>(r); }  \
        template<class Order, class Coefficient, bool HasInverseElements, class Alloc>  \
        inline bool operator op(                                                        \
            const t &l,                                                                 \
            const polynomial<Order, Coefficient, HasInverseElements, Alloc> &r          \
        ){ return polynomial<Order, Coefficient, HasInverseElements, Alloc>(l) op r; }

    #define CMPXX_POLYNOMIAL_BOOLEAN_OPERATOR_OVERLOAD(nil, t)  \
        CMPXX_POLYNOMIAL_BOOLEAN_OPERATOR_OVERLOAD_IMPL(<, t);  \
        CMPXX_POLYNOMIAL_BOOLEAN_OPERATOR_OVERLOAD_IMPL(>, t);  \
        CMPXX_POLYNOMIAL_BOOLEAN_OPERATOR_OVERLOAD_IMPL(<=, t); \
        CMPXX_POLYNOMIAL_BOOLEAN_OPERATOR_OVERLOAD_IMPL(>=, t); \
        CMPXX_POLYNOMIAL_BOOLEAN_OPERATOR_OVERLOAD_IMPL(==, t); \
        CMPXX_POLYNOMIAL_BOOLEAN_OPERATOR_OVERLOAD_IMPL(!=, t);

    CMPXX_POLYNOMIAL_BOOLEAN_OPERATOR_OVERLOAD_IMPL(<, Coefficient);
    CMPXX_POLYNOMIAL_BOOLEAN_OPERATOR_OVERLOAD_IMPL(>, Coefficient);
    CMPXX_POLYNOMIAL_BOOLEAN_OPERATOR_OVERLOAD_IMPL(>=, Coefficient);
    CMPXX_POLYNOMIAL_BOOLEAN_OPERATOR_OVERLOAD_IMPL(<=, Coefficient);
    CMPXX_POLYNOMIAL_BOOLEAN_OPERATOR_OVERLOAD_IMPL(==, Coefficient);
    CMPXX_POLYNOMIAL_BOOLEAN_OPERATOR_OVERLOAD_IMPL(!=, Coefficient);
    CMPXX_INVOKE_MACRO_WITH_BUILT_IN_TYPE(CMPXX_POLYNOMIAL_BOOLEAN_OPERATOR_OVERLOAD, nil);

    template<class Order, class Coefficient, bool HasInverseElements, class Alloc>
    std::ostream &operator <<(std::ostream &ostream, const polynomial<Order, Coefficient, HasInverseElements, Alloc> &value){
        ostream << value.get_str();
        return ostream;
    }
}

#endif

