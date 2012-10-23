// debug

#include <iostream>
#include <string>
#include <typeinfo>
#include <cstdio>
#include <cxxabi.h>

#include "cmpxx/integer.hpp"
#include "cmpxx/rational.hpp"
#include "cmpxx/floating.hpp"
#include "cmpxx/polynomial.hpp"
#include "cmpxx/quotient_ring.hpp"

// memory leaks!!
namespace cmpxx{
    namespace debug{
        inline std::string demangle(const std::string &str){
            int state;
            return abi::__cxa_demangle(str.c_str(), 0, 0, &state);
        }
    }
}

// dynamic-compile

#include <cstdlib>
#include <fstream>
#include <stdexcept>
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
        std::cout << "-------- integral domain test 1.\n";

        typedef cmpxx::polynomial<cmpxx::integer, cmpxx::rational, true> poly;
        {
            poly p, q;

            // p = 444x^333 + 222x^111
            p[111](222)[333](444);
            // q = 101x^999 + 888x^777 + 666x^555
            q[555](666)[777](888)[999](101);

            std::cout << "div.\n";
            std::cout << "lhs : " << q.get_str() << "\n";
            std::cout << "rhs : " << p.get_str() << "\n";
            std::cout << "div : " << poly(q / p).get_str() << "\n";
            std::cout << "mod : " << poly(q % p).get_str() << "\n";
            std::cout << std::endl;
        }

        {
            poly f, g, s, t;
            f[3](18)[2](-42)[1](30)[0](-6);
            g[2](-12)[1](10)[0](-2);
            std::cout << "eea.\n";
            std::cout << "lhs : " << f << "\n";
            std::cout << "rhs : " << g << "\n";
            std::cout << "eea : " << poly::eea(s, t, f, g) << "\n";
            std::cout << "s   : " << s << "\n";
            std::cout << "t   : " << t << "\n";
            std::cout << "eea : " << poly(s * f + t * g) << "\n";
            std::cout << std::endl;
        }

        {
            poly f, g, s, t;
            f[3](18)[2](-42)[1](30)[0](-6);
            g[2](-12)[1](10)[0](-2);
            std::cout << "eea.\n";
            std::cout << "lhs : " << f << "\n";
            std::cout << "rhs : " << g << "\n";
            std::cout << "eea : " << poly::classical_eea(s, t, f, g) << "\n";
            std::cout << "s   : " << s << "\n";
            std::cout << "t   : " << t << "\n";
            std::cout << "eea : " << poly(s * f + t * g) << "\n";
            std::cout << std::endl;
        }

        {
            poly x;
            x[10](1)[9](2)[8](3)[7](4)[6](5)[5](6)[4](7)[3](8)[2](9)[1](10);
            std::cout << poly(+x) << "\n";
            std::cout << poly(-x) << "\n";
            std::cout << poly(-(-x)) << "\n";
            std::cout << poly(-(-x + x + 1)) << "\n";
            std::cout << std::endl;
        }

        {
            std::cout << "p-radix taylor expansion\n";
            poly x, p;
            p[3](1)[2](-15);
            x[1]("5/4")[2]("4/3")[3]("3/2")[4]("2/3")[5]("1/4");
            std::cout << "target p : " << p << "\n" << "target x : " << x << "\n";
            auto t_result = x.taylor_expansion(p);
            for(auto &a_i : t_result){
                std::cout << a_i.first << " : " << a_i.second << "\n";
            }
        }

        std::cout << std::endl;
    }

    void polynomial_test_2(){
        std::cout << "-------- integral domain test 2.\n";

        typedef cmpxx::polynomial<cmpxx::integer, cmpxx::integer, false> poly;
        poly f = 126, g = 35;
        std::cout << "gcd(" << f.get_str() << ", " << g.get_str() << ") = " << poly::classical_gcd(f, g).get_str() << "\n";
        std::cout << std::endl;

        {
            poly s, t;
            std::cout << "eea : " << poly::classical_eea(s, t, 126, 35) << "\n";
            std::cout << "s   : " << s << "\n";
            std::cout << "t   : " << t << "\n";
            std::cout << "eea : " << poly(s * 126 + t * 35) << "\n";
            std::cout << std::endl;
        }

        {
            poly x, y, q, r;
            x[12](8)[7](7)[3](2);
            y[3](1)[2](4)[0](1);
            q = poly::div<true>(r, x, y);
            std::cout << "poly div monic poly\n";
            std::cout << poly(x / y) << "\n";
            std::cout << r << "\n";
            std::cout << poly(q * y + r) << "\n";
            std::cout << std::endl;
        }

        {
            std::cout << "p-radix taylor expansion\n";
            poly x, p;
            p[3](1)[2](-15);
            x[1](5)[2](4)[3](3)[4](2)[5](1);
            std::cout << "target p : " << p << "\n" << "target x : " << x << "\n";
            auto t_result = x.taylor_expansion(p);
            for(auto &a_i : t_result){
                std::cout << a_i.first << " : " << a_i.second << "\n";
            }
        }

        std::cout << std::endl;
    }

    struct test_type{
        test_type() : value(){}
        test_type(double value_) : value(value_){}

        template<class Operator, class L, class R>
        test_type(const cmpxx::aux::expression_template<test_type, Operator, L, R> &expression) :
            value()
        { expression.eval(*this); }

        template<class Operator, class L, class R>
        test_type &operator =(const cmpxx::aux::expression_template<test_type, Operator, L, R> &expression){
            test_type temp;
            expression.eval(temp);
            *this = temp;
            return *this;
        }

        test_type &operator =(const test_type &rhs){
            value = rhs.value;
            return *this;
        }

        test_type &operator +=(double v){
            value += v;
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

    struct test_type_negative_functor{
        inline void operator ()(test_type &x) const{
            x.value = -x.value;
        }
    };

    CMPXX_AUX_ET_OPERATOR_OVERLOAD(test_type, exp_operator_add<>, +);
    CMPXX_AUX_ET_OPERATOR_OVERLOAD(test_type, exp_operator_mul<>, *);
    CMPXX_AUX_ET_OPERATOR_OVERLOAD(test_type, exp_operator_sub<>, -);
    CMPXX_AUX_ET_OPERATOR_OVERLOAD(test_type, exp_operator_div<>, /);
    CMPXX_AUX_ET_UNARY_OPERATOR_OVERLOAD(test_type, exp_operator_negative<test_type_negative_functor>, -);

    CMPXX_AUX_GENERATE_TMP_TMP_PARAM_ET(test_type, (class T, class U), (T, U), (class, class));

    template<class T, class U>
    struct template_test_type{
        template_test_type() : x(), y(){}
        template_test_type(int value) : x(T(value)), y(T(value)){}

        template<template<class, class> class Other, class Operator, class L, class R>
        template_test_type(const expression_template_test_type<Other, T, U, Operator, L, R> &expression) :
            x(), y()
        { expression.eval(*this); }

        template<template<class, class> class Other, class Operator, class L, class R>
        template_test_type &operator =(const expression_template_test_type<Other, T, U, Operator, L, R> &expression){
            template_test_type temp;
            expression.eval(temp);
            *this = temp;
            return *this;
        }

        template_test_type &operator =(const template_test_type &rhs){
            x = rhs.x;
            y = rhs.y;
            return *this;
        }

        template_test_type &operator +=(const template_test_type &rhs){
            x += rhs.x;
            y += rhs.y;
            return *this;
        }

        template_test_type &operator +=(int rhs){
            x += static_cast<T>(rhs);
            y += static_cast<U>(rhs);
            return *this;
        }

        template_test_type &operator -=(const template_test_type &rhs){
            x -= rhs.x;
            y -= rhs.y;
            return *this;
        }

        template_test_type &operator *=(const template_test_type &rhs){
            x *= rhs.x;
            y *= rhs.y;
            return *this;
        }

        template_test_type &operator /=(const template_test_type &rhs){
            x /= rhs.x;
            y /= rhs.y;
            return *this;
        }

        T x;
        U y;
    };

    struct template_test_type_operator_negative{
        template<class T, class U>
        inline void operator ()(template_test_type<T, U> &a) const{
            a.x = -a.x;
            a.y = -a.y;
        }
    };

    CMPXX_AUX_TMP_TMP_ET_OPERATOR_OVERLOAD(test_type, (class T, class U), (T, U), (class, class), exp_operator_add<>, +);
    CMPXX_AUX_TMP_TMP_ET_OPERATOR_OVERLOAD(test_type, (class T, class U), (T, U), (class, class), exp_operator_mul<>, *);
    CMPXX_AUX_TMP_TMP_ET_OPERATOR_OVERLOAD(test_type, (class T, class U), (T, U), (class, class), exp_operator_sub<>, -);
    CMPXX_AUX_TMP_TMP_ET_OPERATOR_OVERLOAD(test_type, (class T, class U), (T, U), (class, class), exp_operator_div<>, /);
    CMPXX_AUX_TMP_TMP_ET_UNARY_OPERATOR_OVERLOAD(
        test_type,
        (class T, class U), (T, U), (class, class),
        exp_operator_negative<template_test_type_operator_negative>, -
    );

    void expression_template_test(){
        std::cout << "-------- expression template test.\n";

        {
            test_type x_1, x_2, y, z, w, v;
            x_1.value = 200, x_2.value = 100, y.value = 5, z.value = 0.1;
            w = x_1 - y / z - x_2 + y * z + 0.5;
            v = -(x_1 - y / z - x_2 + y * z + 0.5);
            std::cout << "w = " << w.value << "\n";
            std::cout << "v = " << v.value << "\n";
        }
        std::cout << "end of expression template test 1" << std::endl;

        {
            template_test_type<int, double> x_1, x_2, y, z, w, v;
            x_1.x = 200, x_1.y = 20, x_2.x = 100, x_2.y = 10, y.x = 5, y.y = 0.5, z.x = 1, z.y = 0.01;
            w = x_1 - y / z - x_2 + y * z + 50;
            v = -(x_1 - y / z - x_2 + y * z + 50);
            std::cout << "w.x = " << w.x << "\n";
            std::cout << "w.y = " << w.y << "\n";
            std::cout << "v.x = " << v.x << "\n";
            std::cout << "v.y = " << v.y << "\n";
        }
        std::cout << "end of expression template test 2" << std::endl;

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

        std::cout << std::endl;
    }

    void quotient_ring_test(){
        std::cout << "-------- quotient ring test.\n";

        {
            std::cout << "factor test\n";
            using quotient_ring = cmpxx::quotient_ring<int>;
            quotient_ring a(3), b(10, 13), c;
            a += b;
            a.canonicalize();
            std::cout << a << " (a)\n";
            b *= 3;
            std::cout << b << " (b)\n";
            b += 1;
            a = 3;
            a.p(11);
            c += a * b;
            std::cout << c << " (c)\n";
            std::cout << c.p() << " (c)\n";
            std::cout << std::endl;
        }

        {
            std::cout << "expression template for quotient ring\n";
            using quotient_ring = cmpxx::quotient_ring<int>;
            quotient_ring a(3, 7), b(5, a.shared_ptr_p()), c;
            c = a * b;
            std::cout << c << " (1)\n";
            c = a * 5;
            std::cout << c << " (2)\n";
            c = 3 * b;
            std::cout << c << " (3)\n";
            std::cout << std::endl;
        }

        std::cout << std::endl;
    }

    void common_test_1(){
        std::cout << "-------- common test.\n";

        auto lower_bound_pow2_coefficient_test = [](const cmpxx::integer &p, cmpxx::integer a){
            std::cout << "lower_bound_pow2_coefficient\n";
            std::cout << "a             = " << a.get_raw_value().get_str(10) << "\n";
            std::size_t e = cmpxx::integer::lower_bound_pow2_coefficient(p, a);
            std::cout << "e = "<< e << "\n";
            for(std::size_t i = 0; i < e - 1; ++i){
                a <<= 1;
            }
            std::cout << "a * 2^(e - 1) = " << a.get_raw_value().get_str(10) << "\n";
            std::cout << "p             = " << p.get_raw_value().get_str(10) << "\n";
            a <<= 1;
            std::cout << "a * 2^e       = " << a.get_raw_value().get_str(10) << "\n";
            std::cout << std::endl;
        };
        lower_bound_pow2_coefficient_test(5, 1);
        lower_bound_pow2_coefficient_test("42949673984", "4294967296");
        lower_bound_pow2_coefficient_test("34359738368", "34359738368");
        lower_bound_pow2_coefficient_test("68719477760", "34359738368");
        lower_bound_pow2_coefficient_test("68719477760", 1024);
        lower_bound_pow2_coefficient_test("68719477760", 1023);
        lower_bound_pow2_coefficient_test("18446744073709551617", 1023);

        auto square_test_1 = [](const cmpxx::integer &a, std::size_t n){
            std::cout << "square 1\n";
            std::cout << a << "^" << n << " = " << cmpxx::aux::iterate_square(a, n);
            std::cout << std::endl;
        };
        square_test_1(8, 13);
        square_test_1(9, 18);

        auto square_test_2 = [](const cmpxx::integer &a, std::size_t n){
            std::cout << "square 2\n";
            std::cout << a << "^" << n << " = " << cmpxx::aux::iterate_square(a, 1 << n) << " = " << cmpxx::aux::iterate_square_pow2(a, n);
            std::cout << std::endl;
        };
        square_test_2(13, 4);

        std::cout << std::endl;
    }
}

int main(){
    test::expression_template_test();
    test::polynomial_test_1();
    test::polynomial_test_2();
    test::quotient_ring_test();
    test::common_test_1();
    test::dynamic_link_test();

    return 0;
}

