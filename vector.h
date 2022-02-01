#ifndef MY_TINY_STL_VECTOR_H
#define MY_TINY_STL_VECTOR_H
#include "allocator.h"
#include "iterator.h"
#include "uninitialized.h"
namespace STL {
    template <class T, class Alloc = allocator<T>>
    class vector {
    public:
        typedef T           value_type;
        typedef value_type* pointer;
        typedef value_type* iterator;
        typedef value_type& reference;
        typedef size_t      size_type;
        typedef ptrdiff_t   ptrdiff_t;

    private:
        iterator start;
        iterator finish;
        iterator mem_end;
        typedef Alloc data_allocator;
        void insert_aux(iterator position, const T& x);
        void deallocate();
        void fill_initialize(const size_type &n, const T& value) ;
        iterator allocate_and_fill(const size_type& n, const T& x);

    public:
        //构造函数，复制构造函数，析构函数
        vector() : start(0), finish(0), mem_end(0){};
        vector(const size_type& n, const value_type& value) ;
        explicit vector(const size_type& n);
        ~vector();

        //操作符重载
        bool operator ==(const vector& v) const;
        bool operator !=(const vector& v) const;
        //迭代器相关
        iterator begin();
        iterator end();
        //容量相关
        size_type size();
        size_type capacity();
        bool empty();
        void resize(const size_t& n, value_type value=value_type());
        void reserve(const size_t& n);
        void shrink_yo_fit();
        //元素访问
        /*为什么这里返回的是引用？如果不是引用的话就相当于返回的是一个拷贝，
         这样的话就不能通过调用T& tmp = vector.front()去修改第一个元素了
        */
        reference front();
        reference back();
        reference operator[](const size_type& n);
        pointer data();

        //元素调整
        void push_back(const value_type& val);
        void pop_back();
        iterator erase(iterator position);
        iterator erase(iterator first, iterator last);
        void insert(iterator position, const size_type& n, const value_type& val);
        iterator insert(iterator position, const value_type& value);
        void clear();

    };
    //构造，析构，赋值
    template<class T, class Alloc>
    vector<T, Alloc>::~vector() {
        deallocate();
    }

    template<class T, class Alloc>
    vector<T, Alloc>::vector(const vector::size_type &n) {
        fill_initialize(n, T());
    }


    template<class T, class Alloc>
    void vector<T, Alloc>::deallocate() {
        if (start) {
            data_allocator::destroy(start, finish);
            data_allocator::deallocate(start, capacity());
        }
    }

    template<class T, class Alloc>
    vector<T, Alloc>::vector(const vector::size_type &n, const value_type &value) {
        fill_initialize(n, value);
    }

    //空间配置器相关
    template<class T, class Alloc>
    void vector<T, Alloc>::fill_initialize(const vector::size_type &n, const T &value) {
        start = allocate_and_fill(n, value);
        finish = start + n;
        mem_end = finish;
    }

    template<class T, class Alloc>
    typename vector<T, Alloc>::iterator vector<T, Alloc>::allocate_and_fill(const vector::size_type &n, const T &x) {
        iterator result = data_allocator::allocate(n);
        uninitialized_fill_n(result, n, x);
        return result;
    }

    //大小，容量相关
    template<class T, class Alloc>
    typename vector<T, Alloc>::size_type vector<T, Alloc>::capacity() {
        return mem_end - begin();
    }

    template<class T, class Alloc>
    typename vector<T, Alloc>::size_type vector<T, Alloc>::size() {
        return end() - begin();
    }

    template<class T, class Alloc>
    bool vector<T, Alloc>::empty() {
        return begin() == end();
    }

    template<class T, class Alloc>
    void vector<T, Alloc>::resize(const size_t &n, value_type value) {
        if (n < size()) {
            data_allocator ::destroy(start + n, finish);
            finish = start + n;
        }
        else if (n > size() && n <= capacity()) {
            auto needInsertSize = n - size();
            finish = uninitialized_fill_n(finish, needInsertSize, value);
        }
        else {
            auto needInsertSize = n - size();
            iterator newStart = data_allocator ::allocate(n);
            std::cout << *begin() << std::endl;
            iterator newFinish = uninitialized_copy(begin(), end(), newStart);
            newFinish = uninitialized_fill_n(newFinish, needInsertSize, value);

            deallocate();

            start = newStart;
            finish = newFinish;
            mem_end = start + n;
        }
    }

    template<class T, class Alloc>
    void vector<T, Alloc>::reserve(const size_t &n) {
        if (n <= capacity()) { ///只增不减
            return ;
        }
        std::cout << *begin() << std::endl;
        iterator new_start = data_allocator::allocate(n);
        iterator new_finish = uninitialized_copy(begin(), end(), new_start);
        std::cout << *begin() << std::endl;
        deallocate();
        start = new_start;
        finish = new_finish;
        mem_end = new_start + n;
    }

    template<class T, class Alloc>
    void vector<T, Alloc>::shrink_yo_fit() {
        iterator new_start = data_allocator::allocate(size());
        iterator new_finish = uninitialized_copy(start, finish, new_start);
        deallocate();
        start = new_start;
        finish = new_finish;
        mem_end = new_finish;
    }

    template<class T, class Alloc>
    typename vector<T, Alloc>::iterator vector<T, Alloc>::begin() {
        return start;
    }

    template<class T, class Alloc>
    typename vector<T, Alloc>::iterator vector<T, Alloc>::end() {
        return finish;
    }

    template<class T, class Alloc>
    bool vector<T, Alloc>::operator==(const vector &v) const {
        if (v.size() != size()) return false;
        auto p1 = begin();
        auto p2 = v.begin();
        for (; p1 != finish; ++ p1, ++ p2)
            if (*p1 != *p2) return false;
        return true;
    }

    template<class T, class Alloc>
    bool vector<T, Alloc>::operator!=(const vector &v) const {
        return (this == v) == false;
    }

    template<class T, class Alloc>
    typename vector<T, Alloc>::reference vector<T, Alloc>::front() {
        return *begin();
    }

    template<class T, class Alloc>
    typename vector<T, Alloc>::reference vector<T, Alloc>::back() {
        return *(end()-1);
    }

    template<class T, class Alloc>
    typename vector<T, Alloc>::reference vector<T, Alloc>::operator[](const vector::size_type &n) {
        return *(begin() + n);
    }

    template<class T, class Alloc>
    typename vector<T, Alloc>::pointer vector<T, Alloc>::data() {
        return begin();
    }

    template<class T, class Alloc>
    void vector<T, Alloc>::push_back(const value_type &val) {
        if (finish != mem_end) {
            construct(finish, val);
            ++ finish;
        }
        else {
            insert_aux(finish, val);
        }
    }

    template<class T, class Alloc>
    void vector<T, Alloc>::pop_back() {

    }

    template<class T, class Alloc>
    void vector<T, Alloc>::clear() {
        erase(begin(), end());
    }

    template<class T, class Alloc>
    void vector<T, Alloc>::insert_aux(vector::iterator position, const T &x) {
        if (finish != mem_end) {    //仍有备用空间
            construct(finish, *(finish-1));
            ++ finish;
            T x_copy = x;
            copy_backward(position, finish - 2, finish - 1);
            *position = x_copy;
        }
        else { //无备用空间，扩大并重新分配
            const size_type old_size = size();
            const size_type len = old_size == 0 ? 1 : old_size * 2;
            iterator new_start = data_allocator::allocate(len); //重新分配
            iterator new_finish = uninitialized_copy(start, position, new_start); //把position之前的copy
            construct(new_finish, x);
            ++ new_finish;
            new_finish = uninitialized_copy(position, finish, new_finish); //把position之后的copy

            destroy(begin(), end());
            deallocate();

            start = new_start;
            finish = new_finish;
            mem_end = new_start + len;
        }

    }

    //迭代器相关

}
#endif //MY_TINY_STL_VECTOR_H
