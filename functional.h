
#ifndef MY_TINY_STL_FUNCTIONAL_H
#define MY_TINY_STL_FUNCTIONAL_H

namespace STL {
    //仿函数相应型别定义
    template <class Arg, class Result>
    struct unary_function {
        typedef Arg     argement_type;
        typedef Result  result_type;
    };

    template <class Arg1, class Arg2, class Result>
    struct binary_function {
        typedef Arg1     first_argement_type;
        typedef Arg2     second_argement_type;
        typedef Result   result_type;
    };

    //运算类仿函数
    template <class T>
    struct plus : public binary_function<T, T, T> {
        T operator()(const T& x, const T& y) const { return x + y;}
    };
    template <class T>
    struct minus : public binary_function<T, T, T> {
        T operator()(const T& x, const T& y) const { return x - y;}
    };
    template <class T>
    struct multiplies : public binary_function<T, T, T> {
        T operator()(const T& x, const T& y) const { return x * y;}
    };
    template <class T>
    struct divides : public binary_function<T, T, T> {
        T operator()(const T& x, const T& y) const { return x / y;}
    };
    template <class T>
    struct modulus : public binary_function<T, T, T> {
        T operator()(const T& x, const T& y) const { return x % y;}
    };
    template <class T>
    struct nagate : public unary_function<T, T> {
        T operator()(const T& x) const { return -x;}
    };

    //关系类仿函数
    template <class T>
    struct equal_to : public binary_function<T, T, T> {
        bool operator()(const T& x, const T& y) const { return x == y;}
    };
    template <class T>
    struct not_equal_to : public binary_function<T, T, T> {
        bool operator()(const T& x, const T& y) const { return x != y;}
    };
    template <class T>
    struct greater : public binary_function<T, T, T> {
        bool operator()(const T& x, const T& y) const { return x > y;}
    };
    template <class T>
    struct less : public binary_function<T, T, T> {
        bool operator()(const T& x, const T& y) const { return x < y;}
    };
    template <class T>
    struct greater_equal : public binary_function<T, T, T> {
        bool operator()(const T& x, const T& y) const { return x >= y;}
    };
    template <class T>
    struct less_equal : public binary_function<T, T, T> {
        bool operator()(const T& x, const T& y) const { return x <= y;}
    };

    //逻辑类仿函数
    template <class T>
    struct logical_and : public binary_function<T, T, T> {
        bool operator()(const T& x, const T& y) const { return x && y;}
    };
    template <class T>
    struct logical_or : public binary_function<T, T, T> {
        bool operator()(const T& x, const T& y) const { return x || y;}
    };
    template <class T>
    struct logial_not : public unary_function<T, T> {
        bool operator()(const T& x) const { return !x;}
    };
}
#endif //MY_TINY_STL_FUNCTIONAL_H
