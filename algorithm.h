
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

    /**         max()          **/
    template <class T>
	const T& max(const T& a, const T& b) {
		return (a < b) ? b : a;
	}
	template <class T, class Compare>
	const T& max(const T& a, const T& b, Compare comp) {
		return (comp(a, b)) ? b : a;
	}


    /**             heap related            **/
    template<class RandomAccessIterator, class Distance, class T, class Compare>
    void __push_heap(RandomAccessIterator first, Distance holeIndex,
                     Distance topIndex, T value, Compare com) {
        Distance parent = (holeIndex - 1) / 2;
        while (holeIndex > topIndex && com(*(first + parent), value) == true) {
            *(first + holeIndex) = *(first + parent);
            holeIndex = parent;
            parent = (holeIndex - 1) / 2;
        }
        *(first + holeIndex) = value;
    }

    template<class RandomAccessIterator, class Distance, class T, class Compare>    //获取distance_type与value_type
    void __push_heap_aux(RandomAccessIterator first, RandomAccessIterator last, Distance*, T* , Compare com) {
        __push_heap(first, Distance(first - last - 1), Distance(0), T(*(last - 1)), com);
    }

    template<class RandomAccessIterator, class Compare>
    void push_heap(RandomAccessIterator first, RandomAccessIterator last, Compare com) {
        __push_heap_aux(first, last, distance_type(first), value_type(first), com);
    }



}

#endif //MY_TINY_STL_ALGORITHM_H
