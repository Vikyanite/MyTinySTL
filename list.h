#ifndef MY_TINY_STL_LIST_H
#define MY_TINY_STL_LIST_H
#include "iterator.h"
#include "allocator.h"
#include "construct.h"

namespace STL {

    template<class T>
    struct list_node {
        typedef void* void_pointer;
        void_pointer pre;
        void_pointer nxt;
        T data;
    };

    template<class T>
    struct list_iterator : public iterator<bidirectional_iterator_tag, T>{

        typedef list_node<T>* link_type;
        link_type node; //迭代器内部指针，指向节点

        list_iterator(link_type x) : node(x) {}
        list_iterator() = default;

        bool operator ==(const list_iterator& x) const {
            return node == x.node;
        }
        bool operator !=(const list_iterator& x) const {
            return node != x.node;
        }
        T& operator *() {
            return node->data;
        }
        T& operator->() {
            return &(operator*());
        }
        list_iterator& operator++() {
            node = (link_type)((*node).nxt);
            return *this;
        }
        list_iterator operator++(int) {
            list_node<T> tmp = *this;
            -- *this;
            return tmp;
        }
        list_iterator& operator--() {
            node = (link_type)((*node).nxt);
            return *this;
        }
        list_iterator operator--(int) {
            list_node<T> tmp = *this;
            -- *this;
            return tmp;
        }

    };

    template<class T, class Alloc = allocator<list_node<T>> >
    class list {
    public:
        typedef list_node<T>            list_node;
        typedef T                       value_type;
        typedef list_iterator<T>        iterator;
        typedef list_iterator<const T>  const_iterator;
        typedef value_type&             reference;
        typedef size_t                  size_type;
        typedef list_node*              link_type;
    private:
        link_type node;
        size_type sz{};
        typedef allocator<list_node> list_node_allocator;
    public:
        //各种构造和析构
        list() {
            empty_initialize();
        }

        explicit list(size_type n, const value_type& val = value_type());

        list(const list& lis) ;

        list& operator = (const list& rhs);

        ~list() {
            clear();
            delete_node(node);
        }

        //元素访问
        reference front() { return *begin(); }
        reference back() { return *(--end()); }
        iterator begin() {return static_cast<link_type>((*node).nxt);}
        iterator end() {return node;}

        //元素操作
        void push_front(const value_type& val) {
            insert(begin(), val);
        }
        void pop_front() {
            erase(begin());
        }
        void push_back(const value_type& val) {
            insert(end(), val);
        }
        void pop_back() {
            erase(--end());
        }
        void remove(const value_type& val) {
            link_type cur = node->nxt; ///begin
            while (cur != node) {
                link_type tmp = cur;
                ++ cur;
                if (tmp->data == val) erase(tmp);
            }

        }

        iterator insert(iterator position, const value_type& val) {
            link_type tmp = new_node(val);
            tmp->nxt = position.node;
            tmp->pre = position.node->pre;

            static_cast<link_type>(position.node->pre)->nxt = tmp;
            position.node->pre = tmp;
            return tmp;
        }

        void swap(list& lis);
        void insert(iterator position, size_type n, const value_type& val);

        void unique();

        iterator erase(iterator position) {
            link_type pre_node = position.node->pre;
            link_type nxt_node = position.node->nxt;
            pre_node->nxt = nxt_node;
            nxt_node->pre = pre_node;

            delete_node(position);
            return nxt_node;
        }
        iterator erase(iterator first, iterator last);
        void splice(iterator position, list& other);
        void splice(iterator position, list& other, iterator i);
        void splice(iterator position, list& other, iterator first, iterator last);

        //容量相关
        void clear() {
            auto cur = (link_type)node->nxt;
            while (cur != node) {
                link_type tmp = cur;
                cur = (link_type)cur->nxt;
                delete_node(tmp);
            }
            node->nxt = node;
            node->pre = node;
        }

        bool empty() { return node->nxt == node; };
        size_type size()const { return sz;};
    private:
        //空间配置器相关

        //创建一个有值节点
        link_type new_node(const T& value) {
            link_type p = list_node_allocator::allocate();
            construct(&p->data, value);
            return p;
        }
        //删除一个有值节点
        void delete_node(link_type p) {
            destroy(&p->data);
            list_node_allocator ::deallocate(p);
        }
        void __insert(iterator position, size_type n, const T& val, std::true_type);


        void empty_initialize() {   //创建一个空list
            node = list_node_allocator::allocate();
            node->nxt = node;
            node->pre = node;
            sz = 0;
        }

    public:

    };



}

#endif //MY_TINY_STL_LIST_H

