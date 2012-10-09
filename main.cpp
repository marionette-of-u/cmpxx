#include "cmpxx/integer.hpp"
#include "cmpxx/rational.hpp"
#include "cmpxx/floating.hpp"
#include "cmpxx/polynomial.hpp"

// debug

#include <iostream>
#include <string>
#include <typeinfo>
#include <cstdio>
#include <cxxabi.h>

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
            p["111"]("222")["333"]("444");
            // q = 101x^999 + 888x^777 + 666x^555
            q["555"]("666")["777"]("888")["999"]("101");

            std::cout << "div.\n";
            std::cout << "lhs : " << q.get_str() << "\n";
            std::cout << "rhs : " << p.get_str() << "\n";
            std::cout << "div : " << poly(q / p).get_str() << "\n";
            std::cout << "mod : " << poly(q % p).get_str() << "\n";
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
            std::cout << "eea : " << poly(s * f + t * g) << "\n";
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
            std::cout << "eea : " << poly(s * f + t * g) << "\n";
            std::cout << std::endl;
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
            poly f = 5, g = -1, p = 3;
            poly::order l = 2;
            std::cout << "p-radix inverse\n";
            std::pair<poly, poly> gp = poly::inverse(f, g, p, l);
            std::cout << gp.first << "\n";
            std::cout << poly((gp.first * f) % gp.second) << "\n";
            std::cout << gp.second << "\n";
            std::cout << std::endl;
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

    CMPXX_AUX_ET_OPERATOR_OVERLOAD(test_type, exp_operator_add<>, +);
    CMPXX_AUX_ET_OPERATOR_OVERLOAD(test_type, exp_operator_mul<>, *);
    CMPXX_AUX_ET_OPERATOR_OVERLOAD(test_type, exp_operator_sub<>, -);
    CMPXX_AUX_ET_OPERATOR_OVERLOAD(test_type, exp_operator_div<>, /);

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

    CMPXX_AUX_TMP_TMP_ET_OPERATOR_OVERLOAD(test_type, (class T, class U), (T, U), (class, class), exp_operator_add<>, +);
    CMPXX_AUX_TMP_TMP_ET_OPERATOR_OVERLOAD(test_type, (class T, class U), (T, U), (class, class), exp_operator_mul<>, *);
    CMPXX_AUX_TMP_TMP_ET_OPERATOR_OVERLOAD(test_type, (class T, class U), (T, U), (class, class), exp_operator_sub<>, -);
    CMPXX_AUX_TMP_TMP_ET_OPERATOR_OVERLOAD(test_type, (class T, class U), (T, U), (class, class), exp_operator_div<>, /);

    void expression_template_test(){
        std::cout << "-------- expression template test.\n";

        {
            test_type x_1, x_2, y, z, w;
            x_1.value = 200, x_2.value = 100, y.value = 5, z.value = 0.1;
            w = x_1 - y / z - x_2 + y * z + 0.5;
            std::cout << "w = " << w.value << std::endl;
        }
        std::cout << "end of expression template test 1" << std::endl;

        {
            template_test_type<int, double> x_1, x_2, y, z, w;
            x_1.x = 200, x_1.y = 20, x_2.x = 100, x_2.y = 10, y.x = 5, y.y = 0.5, z.x = 1, z.y = 0.01;
            w = x_1 - y / z - x_2 + y * z + 50;
            std::cout << "w.x = " << w.x << std::endl;
            std::cout << "w.y = " << w.y << std::endl;
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
}

int main(int argc, char *argv[]){
    test::expression_template_test();
    test::polynomial_test_1();
    test::polynomial_test_2();
    test::dynamic_link_test();

    return 0;
}
