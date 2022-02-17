
#ifndef MY_TINY_STL_ITERATOR_H
#define MY_TINY_STL_ITERATOR_H

#include <cstddef>

namespace STL {
    //五种迭代器类型
    struct input_iterator_tag {};
    struct output_iterator_tag {};
    struct forward_iterator_tag : public input_iterator_tag {};
    struct bidirectional_iterator_tag : public forward_iterator_tag {};
    struct random_access_iterator_tag : public bidirectional_iterator_tag {};

    template<class Category, class T, class Distance = ptrdiff_t, class Pointer = T*, class Reffrence = T&>
    struct iterator {
        typedef Category    iterator_category;
        typedef T           value_type;
        typedef Distance    difference_type;
        typedef Pointer     pointer;
        typedef Reffrence   reference;
    };

    template <class Iterator>
    struct iterator_traits {
        typedef typename Iterator::Category    iterator_category;
        typedef typename Iterator::T           value_type;
        typedef typename Iterator::Distance    difference_type;
        typedef typename Iterator::Pointer     pointer;
        typedef typename Iterator::Reffrence   reference;
    };

    template <class T>
    struct iterator_traits<T* > {
        typedef random_access_iterator_tag  iterator_category;
        typedef T                           value_type;
        typedef ptrdiff_t                   difference_type;
        typedef T*                          pointer;
        typedef T&                          reference;
    };

    template <class T>
    struct iterator_traits<const T* > {
        typedef random_access_iterator_tag  iterator_category;
        typedef T                           value_type;
        typedef ptrdiff_t                   difference_type;
        typedef const T*                    pointer;
        typedef const T&                    reference;
    };

    template <class Iterator>
    typename iterator_traits<Iterator>::iterator_category
    iterator_category(const Iterator&) {
        typedef typename iterator_traits<Iterator>::iterator_category category;
        return category();
    }

    template <class Iterator>
    typename iterator_traits<Iterator>::value_type*
    value_type(const Iterator&) {
        return static_cast<typename iterator_traits<Iterator>::value_type* >(0);
    }

    template <class Iterator>
    typename iterator_traits<Iterator>::difference_type*
    difference_type(const Iterator&) {
        return static_cast<typename iterator_traits<Iterator>::difference_type* >(0);
    }

    template <class InputIterator>
    typename iterator_traits<InputIterator>::difference_type
    __distance(InputIterator first, InputIterator last,
               input_iterator_tag) {
        typename iterator_traits<InputIterator>::difference_typen n = 0;
        while (first != last) {
            ++ first, ++ n;
        }
        return n;
    }

    template <class RandomAccessIterator>
    typename iterator_traits<RandomAccessIterator>::difference_type
    __distance(RandomAccessIterator first, RandomAccessIterator last,
               random_access_iterator_tag) {
        return last - first;
    }

    template <class InputIterator>
    typename iterator_traits<InputIterator>::difference_type
    distance (InputIterator first,InputIterator last) {
        typedef typename iterator_traits<InputIterator>::iterator_category category;
        return __distance(first, last, category());
    }
    template <class InputIterator, class Distance>
    void __advance(InputIterator& i, Distance n, input_iterator_tag) {
        while (n--) ++i;
    }

    template <class RandAccessIterator, class Distance>
    void __advance(RandAccessIterator& i, Distance n, random_access_iterator_tag) {
        i += n;
    }

    template <class BidirectionalIterator, class Distance>
    void __advance(BidirectionalIterator& i, Distance n, bidirectional_iterator_tag) {
        if (n >= 0)
            while (n--) ++i;
        else
            while (n++) --i;
    }

    template <class InputIterator, class Distance>
    void advance (InputIterator &i, Distance n) {
        return __distance(i, n, iterator_category(i));
    }






}
#endif //MY_TINY_STL_ITERATOR_H
