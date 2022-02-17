
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
	/**     find()          **/
    template<class InputIterator,class T>
    InputIterator find(InputIterator first, InputIterator last, const T& value) {
        while (first != last&&*first != value) {
            ++first;
        }
        return first;
    }
    /**         accumulate      **/
    template<class InputIterator, class T>
    InputIterator find(InputIterator first, InputIterator last, T init) {
        for (; first != last; ++first) {
            init = init + *first;
        }
        return init;
    }
	/**         swap() (c++98)      **/
	template <class T>
	void swap(T& a, T& b) {
	    T tmp = a;
	    a = b;
	    b = tmp;
	}
    template <class FIter1, class FIter2>
    void iter_swap(FIter1 lhs, FIter2 rhs)
    {
        swap(*lhs, *rhs);
    }

    // lower_bound
    template <class ForwardIter, class T>
    ForwardIter
    __lower_bound(ForwardIter first, ForwardIter last,
                    const T& value, forward_iterator_tag)
    {
        auto len = STL::distance(first, last);
        auto half = len;
        ForwardIter middle;
        while (len > 0)
        {
            half = len >> 1;
            middle = first;
            STL::advance(middle, half);
            if (*middle < value)
            {
                first = middle;
                ++first;
                len = len - half - 1;
            }
            else
            {
                len = half;
            }
        }
        return first;
    }

    template <class RandomIter, class T>
    RandomIter
    __lower_bound(RandomIter first, RandomIter last,
                    const T& value, random_access_iterator_tag)
    {
        auto len = last - first;
        auto half = len;
        RandomIter middle;
        while (len > 0)
        {
            half = len >> 1;
            middle = first + half;
            if (*middle < value)
            {
                first = middle + 1;
                len = len - half - 1;
            }
            else
            {
                len = half;
            }
        }
        return first;
    }

    template <class ForwardIter, class T>
    ForwardIter
    lower_bound(ForwardIter first, ForwardIter last, const T& value)
    {
        return __lower_bound(first, last, value, iterator_category(first));
    }


    template <class ForwardIter, class T, class Compared>
    ForwardIter
    __lower_bound(ForwardIter first, ForwardIter last,
                    const T& value, forward_iterator_tag, Compared comp)
    {
        auto len = STL::distance(first, last);
        auto half = len;
        ForwardIter middle;
        while (len > 0)
        {
            half = len >> 1;
            middle = first;
            STL::advance(middle, half);
            if (comp(*middle, value))
            {
                first = middle;
                ++first;
                len = len - half - 1;
            }
            else
            {
                len = half;
            }
        }
        return first;
    }

    template <class RandomIter, class T, class Compared>
    RandomIter
    __lower_bound(RandomIter first, RandomIter last,
                    const T& value, random_access_iterator_tag, Compared comp)
    {
        auto len = last - first;
        auto half = len;
        RandomIter middle;
        while (len > 0)
        {
            half = len >> 1;
            middle = first + half;
            if (comp(*middle, value))
            {
                first = middle + 1;
                len = len - half - 1;
            }
            else
            {
                len = half;
            }
        }
        return first;
    }

    template <class ForwardIter, class T, class Compared>
    ForwardIter
    lower_bound(ForwardIter first, ForwardIter last, const T& value, Compared comp)
    {
        return __lower_bound(first, last, value, iterator_category(first), comp);
    }
    /**             heap related            **/
    template <class RandomIter, class Distance, class T>
    void push_heap_aux(RandomIter first, Distance holeIndex, Distance topIndex, T value)
    {
        auto parent = (holeIndex - 1) / 2;
        while (holeIndex > topIndex && *(first + parent) < value)
        {
            // 使用 operator<，所以 heap 为 max-heap
            *(first + holeIndex) = *(first + parent);
            holeIndex = parent;
            parent = (holeIndex - 1) / 2;
        }
        *(first + holeIndex) = value;
    }

    template <class RandomIter, class Distance>
    void push_heap_d(RandomIter first, RandomIter last, Distance*)
    {
        push_heap_aux(first, (last - first) - 1, static_cast<Distance>(0), *(last - 1));
    }

    template <class RandomIter>
    void push_heap(RandomIter first, RandomIter last)
    { // 新元素应该已置于底部容器的最尾端
        push_heap_d(first, last, distance_type(first));
    }

// 重载版本使用函数对象 comp 代替比较操作
    template <class RandomIter, class Distance, class T, class Compared>
    void push_heap_aux(RandomIter first, Distance holeIndex, Distance topIndex, T value,
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
    void push_heap_d(RandomIter first, RandomIter last, Distance*, Compared comp)
    {
        push_heap_aux(first, (last - first) - 1, static_cast<Distance>(0),
                             *(last - 1), comp);
    }

    template <class RandomIter, class Compared>
    void push_heap(RandomIter first, RandomIter last, Compared comp)
    {
        push_heap_d(first, last, difference_type(first), comp);
    }

    template <class RandomIter, class T, class Distance>
    void adjust_heap(RandomIter first, Distance holeIndex, Distance len, T value)
    {
        // 先进行下溯(percolate down)过程
        auto topIndex = holeIndex;
        auto rchild = 2 * holeIndex + 2;
        while (rchild < len)
        {
            if (*(first + rchild) < *(first + rchild - 1))
                --rchild;
            *(first + holeIndex) = *(first + rchild);
            holeIndex = rchild;
            rchild = 2 * (rchild + 1);
        }
        if (rchild == len)
        {  // 如果没有右子节点
            *(first + holeIndex) = *(first + (rchild - 1));
            holeIndex = rchild - 1;
        }
        // 再执行一次上溯(percolate up)过程
        push_heap_aux(first, holeIndex, topIndex, value);
    }

    template <class RandomIter, class T, class Distance>
    void pop_heap_aux(RandomIter first, RandomIter last, RandomIter result, T value,
                      Distance*)
    {
        // 先将首值调至尾节点，然后调整[first, last - 1)使之重新成为一个 max-heap
        *result = *first;
        adjust_heap(first, static_cast<Distance>(0), last - first, value);
    }

    template <class RandomIter>
    void pop_heap(RandomIter first, RandomIter last)
    {
        pop_heap_aux(first, last - 1, last - 1, *(last - 1), difference_type(first));
    }

// 重载版本使用函数对象 comp 代替比较操作
    template <class RandomIter, class T, class Distance, class Compared>
    void adjust_heap(RandomIter first, Distance holeIndex, Distance len, T value,
                     Compared comp)
    {
        // 先进行下溯(percolate down)过程
        auto topIndex = holeIndex;
        auto rchild = 2 * holeIndex + 2;
        while (rchild < len)
        {
            if (comp(*(first + rchild), *(first + rchild - 1)))  --rchild;
            *(first + holeIndex) = *(first + rchild);
            holeIndex = rchild;
            rchild = 2 * (rchild + 1);
        }
        if (rchild == len)
        {
            *(first + holeIndex) = *(first + (rchild - 1));
            holeIndex = rchild - 1;
        }
        // 再执行一次上溯(percolate up)过程
        push_heap_aux(first, holeIndex, topIndex, value, comp);
    }

    template <class RandomIter, class T, class Distance, class Compared>
    void pop_heap_aux(RandomIter first, RandomIter last, RandomIter result,
                      T value, Distance*, Compared comp)
    {
        *result = *first;  // 先将尾指设置成首值，即尾指为欲求结果
        adjust_heap(first, static_cast<Distance>(0), last - first, value, comp);
    }

    template <class RandomIter, class Compared>
    void pop_heap(RandomIter first, RandomIter last, Compared comp)
    {
        pop_heap_aux(first, last - 1, last - 1, *(last - 1),
                            difference_type(first), comp);
    }

/*****************************************************************************************/
// sort_heap
// 该函数接受两个迭代器，表示 heap 容器的首尾，不断执行 pop_heap 操作，直到首尾最多相差1
/*****************************************************************************************/
    template <class RandomIter>
    void sort_heap(RandomIter first, RandomIter last)
    {
        // 每执行一次 pop_heap，最大的元素都被放到尾部，直到容器最多只有一个元素，完成排序
        while (last - first > 1)
        {
            pop_heap(first, last--);
        }
    }

// 重载版本使用函数对象 comp 代替比较操作
    template <class RandomIter, class Compared>
    void sort_heap(RandomIter first, RandomIter last, Compared comp)
    {
        while (last - first > 1)
        {
            pop_heap(first, last--, comp);
        }
    }

/*****************************************************************************************/
// make_heap
// 该函数接受两个迭代器，表示 heap 容器的首尾，把容器内的数据变为一个 heap
/*****************************************************************************************/
    template <class RandomIter, class Distance>
    void make_heap_aux(RandomIter first, RandomIter last, Distance*)
    {
        if (last - first < 2)
            return;
        auto len = last - first;
        auto holeIndex = (len - 2) / 2;
        while (true)
        {
            // 重排以 holeIndex 为首的子树
            adjust_heap(first, holeIndex, len, *(first + holeIndex));
            if (holeIndex == 0)
                return;
            holeIndex--;
        }
    }

    template <class RandomIter>
    void make_heap(RandomIter first, RandomIter last)
    {
        make_heap_aux(first, last, difference_type(first));;
    }

// 重载版本使用函数对象 comp 代替比较操作
    template <class RandomIter, class Distance, class Compared>
    void make_heap_aux(RandomIter first, RandomIter last, Distance*, Compared comp)
    {
        if (last - first < 2)
            return;
        auto len = last - first;
        auto holeIndex = (len - 2) / 2;
        while (true)
        {
            // 重排以 holeIndex 为首的子树
            adjust_heap(first, holeIndex, len, *(first + holeIndex), comp);
            if (holeIndex == 0)
                return;
            holeIndex--;
        }
    }

    template <class RandomIter, class Compared>
    void make_heap(RandomIter first, RandomIter last, Compared comp)
    {
        make_heap_aux(first, last, distance_type(first), comp);
    }

/**********                 binary_search               ********/
    template <class ForwardIter, class T>
    bool binary_search(ForwardIter first, ForwardIter last, const T& value)
    {
        auto i = lower_bound(first, last, value);
        return i != last && !(value < *i);
    }

    template <class ForwardIter, class T, class Compared>
    bool binary_search(ForwardIter first, ForwardIter last, const T& value, Compared comp)
    {
        auto i = lower_bound(first, last, value);
        return i != last && !comp(value, *i);
    }

    /**         sort            **/

    constexpr static size_t kSmallSectionSize = 128;  // 小型区间的大小，在这个大小内采用插入排序
    // 用于控制分割恶化的情况
    template <class Size>
    Size slg2(Size n)
    { // 找出 lgk <= n 的 k 的最大值
        Size k = 0;
        for (; n > 1; n >>= 1)
            ++k;
        return k;
    }
    // 插入排序辅助函数 unchecked_linear_insert
    template <class RandomIter, class T>
    void unchecked_linear_insert(RandomIter last, const T& value)
    {
        auto next = last;
        --next;
        while (value < *next)
        {
            *last = *next;
            last = next;
            --next;
        }
        *last = value;
    }
// 插入排序函数 unchecked_insertion_sort
    template <class RandomIter>
    void unchecked_insertion_sort(RandomIter first, RandomIter last)
    {
        for (auto i = first; i != last; ++i)
        {
            unchecked_linear_insert(i, *i);
        }
    }


    template <class T>
    const T& median(const T& left, const T& mid, const T& right)
    {
        if (left < mid)
            if (mid < right)        // left < mid < right
                return mid;
            else if (left < right)  // left < right <= mid
                return right;
            else                    // right <= left < mid
                return left;
        else if (left < right)      // mid <= left < right
            return left;
        else if (mid < right)       // mid < right <= left
            return right;
        else                        // right <= mid <= left
            return mid;
    }
    template <class RandomIter>
    void partial_sort(RandomIter first, RandomIter middle,
                      RandomIter last)
    {
        make_heap(first, middle);
        for (auto i = middle; i < last; ++i)
        {
            if (*i < *first)
            {
                pop_heap_aux(first, middle, i, *i, difference_type(first));
            }
        }
        sort_heap(first, middle);
    }
// 重载版本使用函数对象 comp 代替比较操作
    template <class RandomIter, class Compared>
    void partial_sort(RandomIter first, RandomIter middle,
                      RandomIter last, Compared comp)
    {
        make_heap(first, middle, comp);
        for (auto i = middle; i < last; ++i)
        {
            if (comp(*i, *first))
            {
                pop_heap_aux(first, middle, i, *i, distance_type(first), comp);
            }
        }
        sort_heap(first, middle, comp);
    }

    // 分割函数 unchecked_partition
    template <class RandomIter, class T>
    RandomIter unchecked_partition(RandomIter first, RandomIter last, const T& pivot)
    {
        while (true)
        {
            while (*first < pivot)
                ++first;
            --last;
            while (pivot < *last)
                --last;
            if (!(first < last))
                return first;
            iter_swap(first, last);
            ++first;
        }
    }

    template <class RandomIter, class Size>
    void intro_sort(RandomIter first, RandomIter last, Size depth_limit)
    {
        while (static_cast<size_t>(last - first) > kSmallSectionSize)
        {
            if (depth_limit == 0)
            {                      // 到达最大分割深度限制
                partial_sort(first, last, last);  // 改用 heap_sort
                return;
            }
            --depth_limit;
            auto mid = median(*(first), *(first + (last - first) / 2), *(last - 1));
            auto cut = unchecked_partition(first, last, mid);
            intro_sort(cut, last, depth_limit);
            last = cut;
        }
    }
    // 插入排序函数 insertion_sort
    template <class RandomIter>
    void insertion_sort(RandomIter first, RandomIter last)
    {
        if (first == last)
            return;
        for (auto i = first + 1; i != last; ++i)
        {
            auto value = *i;
            if (value < *first)
            {
                copy_backward(first, i, i + 1);
                *first = value;
            }
            else
            {
                unchecked_linear_insert(i, value);
            }
        }
    }

    template <class RandomIter>
    void final_insertion_sort(RandomIter first, RandomIter last)
    {
        if (static_cast<size_t>(last - first) > kSmallSectionSize)
        {
            insertion_sort(first, first + kSmallSectionSize);
            unchecked_insertion_sort(first + kSmallSectionSize, last);
        }
        else
        {
            insertion_sort(first, last);
        }
    }

    template <class RandomIter>
    void sort(RandomIter first, RandomIter last)
    {
        if (first != last)
        {
            // 内省式排序，将区间分为一个个小区间，然后对整体进行插入排序
            intro_sort(first, last, slg2(last - first) * 2);
            final_insertion_sort(first, last);
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
