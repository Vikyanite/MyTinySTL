
#ifndef MY_TINY_STL_PRIORITY_QUEUE_H
#define MY_TINY_STL_PRIORITY_QUEUE_H
#include "vector.h"
#include "algorithm.h"
#include "functional.h"

namespace STL {
    template <class T, class Sequence=vector<T>, class Compare=less<typename Sequence::value_type> >
    class priority_queue {
    public:
        typedef typename Sequence::value_type       value_type;
        typedef typename Sequence::size_type        size_type;
        typedef typename Sequence::reference        reference;
    protected:
        Sequence c;
        Compare comp;
    public:
        priority_queue() : c() { }
        bool empty() { return c.empty(); }
        size_type size() { return c.size(); }
        const reference top() { return c.front(); }
        void push(const value_type& x) {

            c.push_back(x);
            push_heap(c.begin(), c.end(), comp);

        }
        void pop() {
            pop_heap(c.begin(), c.end(), comp);

            c.pop_back();
        }
    };
}

#endif //MY_TINY_STL_PRIORITY_QUEUE_H
