
#ifndef MY_TINY_STL_TYPE_TRAITS_H
#define MY_TINY_STL_TYPE_TRAITS_H

#include "algorithm.h"

namespace STL {

    struct __true_type {};
    struct __false_type {};



    template<class type>
    struct __type_traits {
        typedef __false_type has_trivial_default_constructor;
        typedef __false_type has_trivial_copy_constructor;
        typedef __false_type has_trivial_assignment_constructor;
        typedef __false_type has_trivial_destructor;
        typedef __false_type is_POD_type;

    };
    template<>
    struct __type_traits<bool> {
        typedef __true_type has_trivial_default_constructor;
        typedef __true_type has_trivial_copy_constructor;
        typedef __true_type has_trivial_assignment_constructor;
        typedef __true_type has_trivial_destructor;

        typedef __true_type is_POD_type;
    };
    template<>
    struct __type_traits<wchar_t> {
        typedef __true_type has_trivial_default_constructor;
        typedef __true_type has_trivial_copy_constructor;
        typedef __true_type has_trivial_assignment_constructor;
        typedef __true_type has_trivial_destructor;

        typedef __true_type is_POD_type;
    };
    template<>
    struct __type_traits<long long> {
        typedef __true_type has_trivial_default_constructor;
        typedef __true_type has_trivial_copy_constructor;
        typedef __true_type has_trivial_assignment_constructor;
        typedef __true_type has_trivial_destructor;

        typedef __true_type is_POD_type;
    };
    template<>
    struct __type_traits<unsigned long long> {
        typedef __true_type has_trivial_default_constructor;
        typedef __true_type has_trivial_copy_constructor;
        typedef __true_type has_trivial_assignment_constructor;
        typedef __true_type has_trivial_destructor;

        typedef __true_type is_POD_type;
    };

    template<>
    struct __type_traits<char> {
        typedef __true_type has_trivial_default_constructor;
        typedef __true_type has_trivial_copy_constructor;
        typedef __true_type has_trivial_assignment_constructor;
        typedef __true_type has_trivial_destructor;
        typedef __true_type is_POD_type;
    };
    template<>
    struct __type_traits<unsigned char> {
        typedef __true_type has_trivial_default_constructor;
        typedef __true_type has_trivial_copy_constructor;
        typedef __true_type has_trivial_assignment_constructor;
        typedef __true_type has_trivial_destructor;
        typedef __true_type is_POD_type;

    };
    template<>
    struct __type_traits<signed char> {
        typedef __true_type has_trivial_default_constructor;
        typedef __true_type has_trivial_copy_constructor;
        typedef __true_type has_trivial_assignment_constructor;
        typedef __true_type has_trivial_destructor;
        typedef __true_type is_POD_type;

    };
    template<>
    struct __type_traits<short> {
        typedef __true_type has_trivial_default_constructor;
        typedef __true_type has_trivial_copy_constructor;
        typedef __true_type has_trivial_assignment_constructor;
        typedef __true_type has_trivial_destructor;
        typedef __true_type is_POD_type;

    };
    template<>
    struct __type_traits<unsigned short> {
        typedef __true_type has_trivial_default_constructor;
        typedef __true_type has_trivial_copy_constructor;
        typedef __true_type has_trivial_assignment_constructor;
        typedef __true_type has_trivial_destructor;
        typedef __true_type is_POD_type;

    };
    template<>
    struct __type_traits<int> {
        typedef __true_type has_trivial_default_constructor;
        typedef __true_type has_trivial_copy_constructor;
        typedef __true_type has_trivial_assignment_constructor;
        typedef __true_type has_trivial_destructor;
        typedef __true_type is_POD_type;
    };
    template<>
    struct __type_traits<unsigned int> {
        typedef __true_type has_trivial_default_constructor;
        typedef __true_type has_trivial_copy_constructor;
        typedef __true_type has_trivial_assignment_constructor;
        typedef __true_type has_trivial_destructor;
        typedef __true_type is_POD_type;

    };
    template<>
    struct __type_traits<long> {
        typedef __true_type has_trivial_default_constructor;
        typedef __true_type has_trivial_copy_constructor;
        typedef __true_type has_trivial_assignment_constructor;
        typedef __true_type has_trivial_destructor;
        typedef __true_type is_POD_type;

    };
    template<>
    struct __type_traits<unsigned long> {
        typedef __true_type has_trivial_default_constructor;
        typedef __true_type has_trivial_copy_constructor;
        typedef __true_type has_trivial_assignment_constructor;
        typedef __true_type has_trivial_destructor;
        typedef __true_type is_POD_type;
    };
    template<>
    struct __type_traits<float> {
        typedef __true_type has_trivial_default_constructor;
        typedef __true_type has_trivial_copy_constructor;
        typedef __true_type has_trivial_assignment_constructor;
        typedef __true_type has_trivial_destructor;
        typedef __true_type is_POD_type;

    };
    template<>
    struct __type_traits<double> {
        typedef __true_type has_trivial_default_constructor;
        typedef __true_type has_trivial_copy_constructor;
        typedef __true_type has_trivial_assignment_constructor;
        typedef __true_type has_trivial_destructor;
        typedef __true_type is_POD_type;

    };
    template<>
    struct __type_traits<long double> {
        typedef __true_type has_trivial_default_constructor;
        typedef __true_type has_trivial_copy_constructor;
        typedef __true_type has_trivial_assignment_constructor;
        typedef __true_type has_trivial_destructor;
        typedef __true_type is_POD_type;

    };

    template<class T>
    struct __type_traits<T *> {
        typedef __true_type has_trivial_default_constructor;
        typedef __true_type has_trivial_copy_constructor;
        typedef __true_type has_trivial_assignment_constructor;
        typedef __true_type has_trivial_destructor;
        typedef __true_type is_POD_type;
    };
    template<class T>
    struct __type_traits<const T *> {
        typedef __true_type has_trivial_default_constructor;
        typedef __true_type has_trivial_copy_constructor;
        typedef __true_type has_trivial_assignment_constructor;
        typedef __true_type has_trivial_destructor;
        typedef __true_type is_POD_type;
    };
    template<>
    struct __type_traits<char *> {
        typedef __true_type has_trivial_default_constructor;
        typedef __true_type has_trivial_copy_constructor;
        typedef __true_type has_trivial_assignment_constructor;
        typedef __true_type has_trivial_destructor;
        typedef __true_type is_POD_type;

    };
    template<>
    struct __type_traits<unsigned char *> {
        typedef __true_type has_trivial_default_constructor;
        typedef __true_type has_trivial_copy_constructor;
        typedef __true_type has_trivial_assignment_constructor;
        typedef __true_type has_trivial_destructor;
        typedef __true_type is_POD_type;

    };
    template<>
    struct __type_traits<signed char *> {
        typedef __true_type has_trivial_default_constructor;
        typedef __true_type has_trivial_copy_constructor;
        typedef __true_type has_trivial_assignment_constructor;
        typedef __true_type has_trivial_destructor;
        typedef __true_type is_POD_type;

    };
    template<>
    struct __type_traits<const char *> {
        typedef __true_type has_trivial_default_constructor;
        typedef __true_type has_trivial_copy_constructor;
        typedef __true_type has_trivial_assignment_constructor;
        typedef __true_type has_trivial_destructor;
        typedef __true_type is_POD_type;

    };
    template<>
    struct __type_traits<const signed char *> {
        typedef __true_type has_trivial_default_constructor;
        typedef __true_type has_trivial_copy_constructor;
        typedef __true_type has_trivial_assignment_constructor;
        typedef __true_type has_trivial_destructor;
        typedef __true_type is_POD_type;

    };
    template<>
    struct __type_traits<const unsigned char *> {
        typedef __true_type has_trivial_default_constructor;
        typedef __true_type has_trivial_copy_constructor;
        typedef __true_type has_trivial_assignment_constructor;
        typedef __true_type has_trivial_destructor;
        typedef __true_type is_POD_type;

    };

    template <class T, T v>
    struct integral_constant {
        static constexpr T value = v;
        using value_type = T;
        using type = integral_constant;
        operator value_type() const {
            return value;
        }
        value_type operator()() const {
            return value;
        }
    };

    using true_type = integral_constant<bool, true>;
    using false_type = integral_constant<bool, false>;
    template <class T1, class T2>
    struct pair;
    template <class T>
    struct is_pair : false_type {};

    template <class T1, class T2>
    struct is_pair<pair<T1, T2>> : true_type {};

}
#endif //MY_TINY_STL_TYPE_TRAITS_H
