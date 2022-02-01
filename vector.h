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
//        template<class InputIterator> vector(InputIterator first, InputIterator last) ;
        //vector(const vector &x);
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
        reference front();
        /*为什么这里返回的是引用？如果不是引用的话就相当于返回的是一个拷贝，
         这样的话就不能通过调用T& tmp = vector.front()去修改第一个元素了
        */
         reference back();
         reference operator[](const size_type& n);
         pointer data();
        //元素调整
        void push_back(const value_type& val);
        void pop_back();
        iterator erase(iterator position);
        iterator erase(iterator first, iterator last);
        void insert(iterator position, const size_type& n, const value_type& val);
        template<class InputIterator>
        void insert(iterator position, InputIterator first, InputIterator last);
        iterator insert(iterator position, const value_type& value);
        void clear();
        data_allocator get_alloc();

    private:





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

    }

    template<class T, class Alloc>
    void vector<T, Alloc>::reserve(const size_t &n) {

    }

    template<class T, class Alloc>
    void vector<T, Alloc>::shrink_yo_fit() {

    }

    template<class T, class Alloc>
    typename vector<T, Alloc>::iterator vector<T, Alloc>::begin() {
        return start;
    }

    template<class T, class Alloc>
    typename vector<T, Alloc>::iterator vector<T, Alloc>::end() {
        return finish;
    }

    //迭代器相关

}
#endif //MY_TINY_STL_VECTOR_H
