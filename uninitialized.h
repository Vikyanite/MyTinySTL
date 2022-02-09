#ifndef MY_TINY_STL_UNINITIALIZED_H
#define MY_TINY_STL_UNINITIALIZED_H
#include "type_traits.h"
#include "construct.h"
#include "algorithm.h"

namespace STL {
    /*********************  uninitialized_fill_n() *********************/
    template<class ForwardIterator, class Size, class T>
    ForwardIterator __uninitialized_fill_n_aux(ForwardIterator first,
                                               const Size& n,
                                               const T& x,
                                               __true_type) {
        return fill_n(first, n, x); ///交由高阶函数去实现
    }

    template<class ForwardIterator, class Size, class T>
    ForwardIterator __uninitialized_fill_n_aux(ForwardIterator first,
                                               Size n,
                                               const T& x,
                                               __false_type) {
        for ( ; n > 0; -- n, ++ first)
            construct(& *first, x);
        return first;
    }

    template<class ForwardIterator, class Size, class T, class T1>
    ForwardIterator __uninitialized_fill_n(ForwardIterator first,
                                           const Size& n,
                                           const T& x,
                                           T1 *) {
        typedef typename STL::__type_traits<T1>::is_POD_type is_POD;
        return __uninitialized_fill_n_aux(first, n, x, is_POD());
    }

    template<class ForwardIterator, class Size, class T>
    ForwardIterator uninitialized_fill_n(ForwardIterator first,
                                         Size n,
                                         const T& x) {
        return __uninitialized_fill_n(first, n, x, value_type(first));
    }


    /*********************  uninitialized_copy() *********************/
    template<class ForwardIterator, class InputIterator>
    ForwardIterator __uninitialized_copy_aux(InputIterator first,
                                             InputIterator last,
                                             ForwardIterator result,
                                             __true_type) {
        return copy(first, last, result); ///交由高阶函数去实现
    }

    template<class ForwardIterator, class InputIterator>
    ForwardIterator __uninitialized_copy_aux(InputIterator first,
                                             InputIterator last,
                                             ForwardIterator result,
                                             __false_type) {
        for ( ; first != last; ++ first, ++ result)
            construct(& *result, *first);
        return result;
    }
    template<class ForwardIterator, class InputIterator, class T>
    ForwardIterator __uninitialized_copy(ForwardIterator first,
                                         InputIterator last,
                                         ForwardIterator result,
                                         T*) {
        typedef typename STL::__type_traits<T>::is_POD_type is_POD;
        return __uninitialized_copy_aux(first, last, result, is_POD());
    }
    template<class ForwardIterator, class InputIterator>
    ForwardIterator uninitialized_copy(InputIterator first,
                                         InputIterator last,
                                         ForwardIterator result) {
        return __uninitialized_copy(first, last, result, value_type(result));
    }




    /*********************  uninitialized_fill() *********************/
    template<class ForwardIterator, class T>
    void __uninitialized_fill_aux(ForwardIterator first,
                                  ForwardIterator last,
                                  const T& x,
                                  __true_type) {
        fill(first, last, x); ///交由高阶函数去实现
    }

    template<class ForwardIterator, class T>
    void __uninitialized_fill_aux(ForwardIterator first,
                                  ForwardIterator last,
                                  const T& x,
                                  __false_type) {
        for (; first != last; ++first) {
            construct(& *first, x);
        }
    }
    template<class ForwardIterator, class T, class T1>
    void __uninitialized_fill(ForwardIterator first,
                              ForwardIterator last,
                              const T& x,
                              T1 *) {
        typedef typename STL::__type_traits<T1>::is_POD_type is_POD;
        __uninitialized_fill_aux(first, last, x, is_POD());
    }

    template<class ForwardIterator, class T>
    void uninitialized_fill(ForwardIterator first,
                                       ForwardIterator last,
                                       const T& x) {
        return __uninitialized_fill(first, last, x, value_type(first));
    }

}

#endif //MY_TINY_STL_UNINITIALIZED_H
