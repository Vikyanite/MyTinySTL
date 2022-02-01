
#ifndef MY_TINY_STL_CONSTRUCT_H
#define MY_TINY_STL_CONSTRUCT_H

#include "type_traits.h"
#include <new>

namespace STL {
    template<typename T1, typename T2>
    void construct(T1* p, const T2& value) {
        new(p) T1(value);
    }
    //析构函数的两个版本
    template<typename T>
    void destroy(T* point) {
        point->~T();
    }

    template<class ForwordIterator>
    void __destroy_aux(ForwordIterator first, ForwordIterator last, __true_type) { };

    template<class ForwordIterator>
    void __destroy_aux(ForwordIterator first, ForwordIterator last, __false_type) {
        for (; first < last; ++ first) {
            destroy(& (*first));
        }
    }

    template<class ForwordIterator>
    void __destroy(ForwordIterator first, ForwordIterator last) {
        typedef typename STL::__type_traits<ForwordIterator>::has_trivial_destructor trivial_destructor;
        __destroy_aux(first, last, trivial_destructor());
    }

    template<class ForwordIterator>
    void destroy(ForwordIterator first, ForwordIterator last) {
        __destroy(first, last);
    }



};
#endif //MY_TINY_STL_CONSTRUCT_H
