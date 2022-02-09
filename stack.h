
#ifndef MY_TINY_STL_STACK_H
#define MY_TINY_STL_STACK_H
#include "list.h"
namespace STL {
    template <class T, class Sequence=list<T>>
    class stack {
    public:
        typedef typename Sequence::value_type               value_type;
        typedef typename Sequence::size_type                size_type;
        typedef typename Sequence::reference_type           reference_type;
        typedef typename Sequence::const_reference_type     const_reference_type;
    protected:
        Sequence c;
    public:
        bool empty() {return c.empty();}
        size_type size() { return c.size();}
        reference_type top() {return c.back();}
        void push(value_type & val) { c.push_back(val);}
        void pop() { c.pop_back();}
    };
}
#endif //MY_TINY_STL_STACK_H
