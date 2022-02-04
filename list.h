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
            list_iterator tmp = *this;
            ++ *this;
            return tmp;
        }
        list_iterator& operator--() {
            node = (link_type)((*node).pre);
            return *this;
        }
        list_iterator operator--(int) {
            list_iterator tmp = *this;
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
    protected:
        link_type node;
        typedef allocator<list_node> list_node_allocator;
    public:
        //各种构造和析构
        list() {
            empty_initialize();
        }

        explicit list(size_type n, const value_type& val = value_type()) {
            empty_initialize();
            for (int i = 0; i < n; ++ i) push_back(val);
        }

        list(list& lis) {
            empty_initialize();
            iterator first = lis.begin();
            iterator last = lis.end();
            while (first != last) {
                push_back(*first);
                first ++;
            }
        }

        list& operator = (const list& rhs) {
            empty_initialize();
            iterator first = rhs.begin();
            iterator last = rhs.end();
            while (first != last) {
                push_back(*first);
                first ++;
            }
        }

        ~list() {
            clear();
            delete_node(node);
        }

        //元素访问
        reference front() { return *begin(); }
        reference back() { return *(--end()); }
        iterator begin() {return static_cast<link_type>(node->nxt);}
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
            iterator first = begin();
            iterator last = end();
            while (first != last) {
                iterator tmp = first;
                if (*tmp == val) erase(tmp);
                first ++;
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

        void swap(list& lis) {
            link_type tmp = lis.node;
            lis.node = node;
            node = tmp;
        }

        void unique() {
            iterator first = begin();
            iterator last = end();
            if (first == last) return ; //skip empty

            iterator nxt = first;
            while (++ nxt != last) {
                if (*nxt == *first)
                    erase(nxt);
                else
                    first = nxt;
                nxt = first;
            }
        }
        void merge(list & lis) {
            iterator first1 = begin();
            iterator first2 = lis.begin();
            iterator last1 = begin();
            iterator last2 = lis.begin();

            while (first1 != last1 && first2 != last2) {
                if (*first2 < *first1) {
                    iterator nxt = first2;
                    transfer(first1, first2, ++nxt);
                    first2 = nxt;
                }
                else ++ first1;
            }
            if (first2 != last2) transfer(last1, first2, last2);

        }

        void reverse() {
            //小于等于1个的时候不进行翻转
            if (size() <= 1) return ;
            iterator first = begin();
            iterator last = end();
            while (first != last) {
                //std::cout << "run" << std::endl;
                iterator old = first;
                ++ first;
                transfer(begin(), old, first);
            }
        }

        iterator erase(iterator position) {
            auto pre_node = static_cast<link_type>(position.node->pre);
            auto nxt_node = static_cast<link_type>(position.node->nxt);
            pre_node->nxt = nxt_node;
            nxt_node->pre = pre_node;

            delete_node(position.node);
            return nxt_node;
        }
        iterator erase(iterator first, iterator last) {
            while (first != last) {
                iterator tmp = first++;
                erase(tmp);
            }
        }
        //将other整个放到position之前
        void splice(iterator position, list& other) {
            if (!other.empty())
                transfer(position, other.begin(), other.end());
        }
        //将 i 放到position之前
        void splice(iterator position, list& , iterator i) {
            iterator j = i;
            ++ j;
            if (position == i || position == j) return ;
            transfer(position, i, j);
        }
        void splice(iterator position, list& , iterator first, iterator last) {
            if (first != last)
                transfer(position, first, last);
        }

        void sort() {

        }

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
        size_type size()const {
            auto cur = static_cast<link_type>(node->nxt);
            size_type len = 0;
            while (cur != node)
                ++ len,
                cur = static_cast<link_type>(cur->nxt);
            return len;
        };
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

        void transfer(iterator position, iterator first, iterator last) {
            // == last的话就不用动了
            if (position != last) {
                static_cast<link_type>(last.node->pre)->nxt = position.node;
                static_cast<link_type>(first.node->pre)->nxt = last.node;
                static_cast<link_type>(position.node->pre)->nxt = first.node;
                auto tmp = static_cast<link_type>(position.node->pre);
                position.node->pre = last.node->pre;
                last.node->pre = first.node->pre;
                first.node->pre = tmp;
            }

        }

        void empty_initialize() {   //创建一个空list
            node = list_node_allocator::allocate();
            node->nxt = node;
            node->pre = node;
        }
    };



}

#endif //MY_TINY_STL_LIST_H

