
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

    template <class T>
    struct identity : public unary_function<T, T> {
        const T& operator() (const T& x) const { return x; }
    };

    template <class Pair>
    struct select1st : public unary_function<Pair, typename Pair::first_type> {
        const typename Pair::first_type& operator() (const Pair& x) const {
            return x.first;
        }
    };

    template <class Pair>
    struct select2nd : public unary_function<Pair, typename Pair::second_type> {
        const typename Pair::second_type& operator() (const Pair& x) const {
            return x.second;
        }
    };

    // 哈希函数对象

// 对于大部分类型，hash function 什么都不做
    template <class Key>
    struct hash {};

// 针对指针的偏特化版本
    template <class T>
    struct hash<T*>
    {
        size_t operator()(T* p) const noexcept
        { return reinterpret_cast<size_t>(p); }
    };

// 对于整型类型，只是返回原值
#define MYSTL_TRIVIAL_HASH_FCN(Type)         \
template <> struct hash<Type>                \
{                                            \
  size_t operator()(Type val) const noexcept \
  { return static_cast<size_t>(val); }       \
};

    MYSTL_TRIVIAL_HASH_FCN(bool)

    MYSTL_TRIVIAL_HASH_FCN(char)

    MYSTL_TRIVIAL_HASH_FCN(signed char)

    MYSTL_TRIVIAL_HASH_FCN(unsigned char)

    MYSTL_TRIVIAL_HASH_FCN(wchar_t)

    MYSTL_TRIVIAL_HASH_FCN(char16_t)

    MYSTL_TRIVIAL_HASH_FCN(char32_t)

    MYSTL_TRIVIAL_HASH_FCN(short)

    MYSTL_TRIVIAL_HASH_FCN(unsigned short)

    MYSTL_TRIVIAL_HASH_FCN(int)

    MYSTL_TRIVIAL_HASH_FCN(unsigned int)

    MYSTL_TRIVIAL_HASH_FCN(long)

    MYSTL_TRIVIAL_HASH_FCN(unsigned long)

    MYSTL_TRIVIAL_HASH_FCN(long long)

    MYSTL_TRIVIAL_HASH_FCN(unsigned long long)

#undef MYSTL_TRIVIAL_HASH_FCN

// 对于浮点数，逐位哈希
    inline size_t bitwise_hash(const unsigned char* first, size_t count)
    {
#if (_MSC_VER && _WIN64) || ((__GNUC__ || __clang__) &&__SIZEOF_POINTER__ == 8)
        const size_t fnv_offset = 14695981039346656037ull;
        const size_t fnv_prime = 1099511628211ull;
#else
        const size_t fnv_offset = 2166136261u;
  const size_t fnv_prime = 16777619u;
#endif
        size_t result = fnv_offset;
        for (size_t i = 0; i < count; ++i)
        {
            result ^= (size_t)first[i];
            result *= fnv_prime;
        }
        return result;
    }

    template <>
    struct hash<float>
    {
        size_t operator()(const float& val)
        {
            return val == 0.0f ? 0 : bitwise_hash((const unsigned char*)&val, sizeof(float));
        }
    };

    template <>
    struct hash<double>
    {
        size_t operator()(const double& val)
        {
            return val == 0.0f ? 0 : bitwise_hash((const unsigned char*)&val, sizeof(double));
        }
    };

    template <>
    struct hash<long double>
    {
        size_t operator()(const long double& val)
        {
            return val == 0.0f ? 0 : bitwise_hash((const unsigned char*)&val, sizeof(long double));
        }
    };

}
#endif //MY_TINY_STL_FUNCTIONAL_H
