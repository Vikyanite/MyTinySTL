
#ifndef MY_TINY_STL_ALGORITHM_H
#define MY_TINY_STL_ALGORITHM_H
#include "iterator.h"
#include "functional.h"
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

	/**         swap() (c++98)      **/
	template <class T>
	void swap(T& a, T& b) {
	    T tmp = a;
	    a = b;
	    b = tmp;
	}

    /**             heap related            **/
    template <class RandomIter, class Distance, class T, class Compared>
    void __push_heap_aux(RandomIter first, Distance holeIndex, Distance topIndex, T value,
                       Compared comp)
    {
        auto parent = (holeIndex - 1) / 2;
        while (holeIndex > topIndex && comp(*(first + parent), value))
        {
            *(first + holeIndex) = *(first + parent);
            holeIndex = parent;
            parent = (holeIndex - 1) / 2;
        }
        *(first + holeIndex) = value;
    }

    template <class RandomIter, class Compared, class Distance>
    void __push_heap(RandomIter first, RandomIter last, Distance*, Compared comp)
    {
        __push_heap_aux(first, (last - first) - 1, static_cast<Distance>(0),
                             *(last - 1), comp);
    }

    template <class RandomIter, class Compared>
    void push_heap(RandomIter first, RandomIter last, Compared comp)
    {
        __push_heap(first, last, difference_type(first), comp);
    }

    template <class RandomAccessIterator, class Distance, class T, class Compare>
    void __adjust_heap(RandomAccessIterator first, Distance holeIndex, Distance len, T value, Compare com) {
        Distance topIndex = holeIndex;
        Distance secondChild = 2 * holeIndex + 2;
        while (secondChild < len) {
            if (com(*(first + secondChild), *(first + (secondChild - 1))))
                -- secondChild;
            *(first + holeIndex) = *(first + secondChild);
            holeIndex = secondChild;
            secondChild = 2 * (secondChild + 1);
        }
        if (secondChild == len) {
            *(first + holeIndex) = *(first + (secondChild - 1));
            holeIndex = secondChild - 1;
        }
        __push_heap_aux(first, holeIndex, topIndex, value, com);
    }

    template <class RandomIter, class T, class Distance, class Compared>
    void __pop_heap_aux(RandomIter first, RandomIter last, RandomIter result, T value, Distance*, const Compared& comp) {
        *result = *first;  // 先将尾指设置成首值，即尾指为欲求结果
        __adjust_heap(first, static_cast<Distance>(0), last - first, value, comp);
    }

    template <class RandomIter, class Compared >
    void pop_heap(RandomIter first, RandomIter last, const Compared &comp ) {
        __pop_heap_aux(first, last - 1, last - 1, *(last - 1), difference_type(first), comp);
    }

    template <class RandomIter, class Compared=greater<typename RandomIter::value_type>>
    void make_heap(RandomIter first, RandomIter last, const Compared &comp) {
        __make_heap_aux(first, last, distance_type(first), comp);
    }

    template <class RandomIter, class Distance, class Compared>
    void __make_heap_aux(RandomIter first, RandomIter last, Distance*, const Compared& comp)
    {
        if (last - first < 2)
            return;
        auto len = last - first;
        auto holeIndex = (len - 2) / 2;
        while (true) {
            // 重排以 holeIndex 为首的子树
            __adjust_heap(first, holeIndex, len, *(first + holeIndex), comp);
            if (holeIndex == 0)
                return;
            holeIndex--;
        }
    }

    template <class T1, class T2>
    struct pair {
        T1 first;
        T2 second;
        typedef T2 second_type;
        typedef T1 first_type;
        pair(const T1& v1=T1(), const T2& v2=T2()) : first(v1), second(v2) { }

        template<class T3, class T4>
        pair& operator=(pair<T3, T4> &t) {
            first = static_cast<T1>(t.first);
            second = static_cast<T4>(t.second);
            return *this;
        }
    };
    template <class T1, class T2>
    pair<T1, T2> make_pair(T1 t1, T2 t2) {
        return pair<T1, T2>(t1, t2);
    }


}

#endif //MY_TINY_STL_ALGORITHM_H
