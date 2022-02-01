
#ifndef MY_TINY_STL_ALGORITHM_H
#define MY_TINY_STL_ALGORITHM_H
#include "iterator.h"
#include "type_traits.h"
#include <cstring>

namespace STL {
    /**         fill()          **/
    template<class ForwardIterator, class T>
    void fill(ForwardIterator first, ForwardIterator last, const T& value) {
        for (; first != last; ++ first)
            *first = value;
    }
    void fill(char* first, const char* last, const char& value) {
        memset(first, value, last - first);
    }
    void fill(wchar_t* first, const wchar_t* last, const wchar_t& value) {
        memset(first, value, (last - first) * sizeof(wchar_t ));
    }
    /**         fill_n()          **/
    template<class ForwardIterator, class Size, class T>
    ForwardIterator fill_n(ForwardIterator first,
                           Size n,
                           const T& value) {
        for (; n > 0; -- n, ++ first)
            *first = value;
        return first;
    }
    template<class Size>
    char* fill_n(char* first, const Size& n, const char& value) {
        memset(first, value, n);
        return first + n;
    }
    template<class Size>
    char* fill_n(wchar_t* first, const Size& n, const wchar_t& value) {
        memset(first, value, n * sizeof(wchar_t));
        return first + n;
    }
    //先使用std自带的泛型函数
    /**         copy_backward()          **/
    template< class BidirIt1, class BidirIt2 >
    BidirIt2 copy_backward(BidirIt1 first, BidirIt1 last, BidirIt2 d_last){
        while (first != last) {
            *(--d_last) = *(--last);
        }
        return d_last;
    }
    /**         copy()          **/
    template<class InputIterator, class ForwardIterator>
    ForwardIterator copy(InputIterator first, InputIterator last, ForwardIterator d_first) {
        while (first != last) {
            *d_first ++ = *first ++;
        }
        return d_first;
    }

}

#endif //MY_TINY_STL_ALGORITHM_H
