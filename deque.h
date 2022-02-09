

#ifndef MY_TINY_STL_DEQUE_H
#define MY_TINY_STL_DEQUE_H

#include "allocator.h"
#include "construct.h"
#include "iterator.h"
#include "algorithm.h"

namespace STL {

    size_t deque_buf_size(size_t n, size_t sz) {
        return n != 0 ? n : (sz < 512 ? size_t(512/ sz) : size_t(1));
    }

    template<class T, size_t BufSiz=0>
    struct deque_iterator : public iterator<random_access_iterator_tag, T> {
        typedef T                  value_type;
        typedef value_type*        pointer;
        typedef T&                 reference;

        typedef size_t             size_type;
        typedef ptrdiff_t          difference_type;
        typedef T**                map_pointer;
        typedef deque_iterator     self;

        T* cur;
        T* first;
        T* last;
        map_pointer node;

        static size_type buf_size() { return deque_buf_size(BufSiz, sizeof(T));}

        void set_node(map_pointer new_node) {
            node = new_node;
            first = *new_node;
            last = first + difference_type(buf_size());
        }

        reference operator*() { return *cur;}
        pointer operator->() { return &(operator*());}
        difference_type operator-(self& x) {
            return difference_type(buf_size()) * (node - x.node - 1) + (cur - first) + (x.last - x.cur);
        }

        self& operator++() {
            ++cur;
            if (cur == last) {
                set_node(node + 1);
                cur = first;
            }
            return *this;
        }
        self operator++(int) {
            self tmp = *this;
            ++ *this;
            return tmp;
        }
        self& operator--() {
            if (cur == first) {
                set_node(node - 1);
                cur = last;
            }
            -- cur;
            return *this;
        }
        self operator--(int) {
            self tmp = *this;
            -- *this;
            return tmp;
        }

        self& operator+=(difference_type n) {
            difference_type offset = n + (cur - first);
            if (offset >= 0 && offset < difference_type(buf_size())) {
                //目标在同一缓存区内
                cur += n;
            }
            else {
                //不在
                difference_type node_offset =
                        offset > 0 ? offset / difference_type(buf_size()) :
                        -difference_type((-offset - 1) / buf_size()) - 1;
                cur = first + (offset - node_offset * difference_type(buf_size()));
            }
            return *this;
        }
        self operator+(difference_type n) {
            self tmp = *this;
            return tmp += n;
        }
        self& operator-=(difference_type n) {
            return *this += -n;
        }
        self operator-(difference_type n) {
            self tmp = *this;
            return tmp -= n;
        }

        reference operator[](difference_type n) {
            return *(*this + n);
        }

        bool operator==(self& x) {return x.cur == cur;}
        bool operator!=(self& x) {return !(x == *this);}
        bool operator<(self& x) {
            return node == x.node ? (cur < x.cur) : (node < x.node);
        }
        bool operator<=(self& x) {return x == *this || x < *this;}
    };

    template <class T, class Alloc=allocator<T>, size_t BufSiz=0>
    class deque {
    public:
        typedef T                           value_type;
        typedef deque_iterator<T, BufSiz>   iterator;
        typedef value_type&                 reference;
        typedef size_t                      size_type;
        typedef T*                          pointer;
        typedef ptrdiff_t                   difference_type;
        typedef allocator<T*>               map_allocator;
        typedef allocator<T>                data_allocator;
    protected:
        typedef pointer*                    map_pointer;

        iterator start;
        iterator finish;
        map_pointer map;
        size_type map_size;

    public:
        //元素访问，与大小
        iterator begin() {
            return start;
        }
        iterator end() {
            return finish;
        }
        reference operator[](size_type n) {
            return start[difference_type(n)];
        }
        reference front() {
            return *begin();
        }
        reference back() {
            return *(finish - 1);
        }
        size_type size() {
            return finish - start;
        }
        size_type max_size() {
            return size_type(-1);
        }
        bool empty() {
            return finish == start;
        }

        //内存与构造相关

        //默认构造函数
        deque() : start(), finish(), map(0), map_size(0){
            fill_initialize(0,T());
        }
        //预设n个val
        deque(int n, const value_type& val) : start(), finish(), map(0), map_size(0){
            fill_initialize(n, val); //调用内部封装函数
        }

        void push_back(const value_type& val) {
            //缓存区还有备用空间
            if (finish.cur != finish.last - 1) {
                construct(finish.cur, val);
                ++ finish.cur;
            }
            else push_back_aux(val);
        }
        void push_front(const value_type& val) {
            if (start.cur != start.first) {
                construct(start.cur - 1, val);
                -- start.cur;
            }
            else push_front_aux(val);
        }
        void pop_back() {
            if (finish.cur != finish.first) {
                destroy(-- finish.cur);
            }
            else {
                pop_back_aux();
            }
        }
        void pop_front() {
            if (start.cur != start.last - 1) {
                destroy(start.cur++);
            }
            else {
                pop_front_aux();
            }
        }

        void clear() {
            for (map_pointer node = start.node + 1; node < finish.node; ++ node) {
                destroy(*node, *node + buf_size());
                deallocate_node(*node);
            }
            if (start.node != finish.node) { //至少有头尾两个缓存区
                destroy(start.cur, start.last);
                destroy(finish.first, finish.cur);
                deallocate_node(finish.first); //仅回收尾部
            }
            else {
                destroy(start.cur, finish.cur); //仅析构，不释放
            }
            finish = start;
        }
        iterator erase(iterator pos) {
            iterator next = pos;
            ++next;
            difference_type index = pos - start; //清除点之前的点数
            if (index < (size() >> 1)) { //如果清除点前面的元素较少，就移动之前的点
                copy_backward(start, pos, next);
                pop_front();
            }
            else { //移动之后的
                copy(next, finish, pos);
                pop_back();
            }
            return start + index;
        }
        iterator erase(iterator first, iterator last) {
            if (first == start && finish == last) {
                clear();
                return finish;
            }
            else {
                difference_type n = last - first;   //清除区间长度
                difference_type ele_before = first - start; //清除区间前方长度
                if (ele_before < (size() - n) / 2) {
                    copy_backward(start, first, last);
                    iterator new_start = start + n;
                    destroy(start, new_start);
                    //将以下冗余空间释放
                    for (map_pointer cur = start.node; cur < new_start; ++ cur) {
                        deallocate_node(*cur);
                    }
                    start = new_start;
                }
                else {
                    copy(last, finish, first);
                    iterator new_finish = finish - n;
                    destroy(new_finish, finish);
                    for (map_pointer cur = new_finish + 1; cur <= finish.node; ++ cur) {
                        deallocate_node(*cur);
                    }
                    finish = new_finish;
                }
                return start + ele_before;
            }
        }
        iterator insert(iterator pos, value_type& val) {
            if (pos.cur == start.cur) {
                push_front(val);
                return start;
            }
            else if (pos.cur == finish.cur) {
                push_back(val);
                return finish - 1;
            }
            else {
                return insert_aux(pos, val);
            }
        }
    private:
        size_type buf_size() { return deque_buf_size(BufSiz, sizeof(T));}
        //创建n个结点
        void creat_map_and_node(size_t num_elements) {
            size_type num_nodes = num_elements / buf_size() + 1;

            map_size = max(size_type(2), num_nodes + 2); //+2是为了让前后都有扩充的空间
            map = map_allocator::allocate(map_size);

            //将start与finish设在缓存区中间的位置，这样两头可扩充大小就相等
            map_pointer nstart = map + (map_size - num_nodes) / 2;
            map_pointer nfinish = nstart + num_nodes - 1;

            //为每个map_node分配空间
            for (map_pointer cur = nstart; cur <= nfinish; ++ cur)
                *cur = allocate_node();

            //设置start与finish的状态
            start.set_node(nstart);
            finish.set_node(nfinish);
            start.cur = start.first;
            finish.cur = finish.first + num_elements % buf_size();
        }
        //初始化预设函数
        void fill_initialize(size_type n, const value_type& val) {
            creat_map_and_node(n);
            for (map_pointer cur = start.node; cur < finish.node; ++ cur)
                uninitialized_fill(*cur, *cur + buf_size(), val);
            uninitialized_fill(finish.first, finish.cur, val);
        }
        //push_back重分配款
        void push_back_aux(const value_type& val) {
            reserve_map_at_back();
            *(finish.node + 1) = allocate_node();
            construct(finish.cur, val); //构造

            //修改finish的信息
            finish.set_node(finish.node + 1);
            finish.cur = finish.first;
        }
        //push_front重新分配款
        void push_front_aux(const value_type& val) {
            reserve_map_at_front();
            *(start.node - 1) = allocate_node();

            //修改finish的信息
            start.set_node(start.node - 1);
            start.cur = start.last - 1;
            construct(start.cur, val); //构造
        }
        //pop_back 内存管理版
        void pop_back_aux() {
            deallocate_node(finish.first);
            finish.set_node(finish.node - 1);
            finish.cur = finish.last - 1;
            destroy(finish.cur);
        }
        //pop_front重分配款
        void pop_front_aux() {
            destroy(start.cur);

            deallocate_node(start.first);
            start.set_node(start.node + 1);
            start.cur = start.first;
        }
        //insert实现
        iterator insert_aux(iterator pos, value_type & val) {
            difference_type index = pos - start;
            if (index < size() / 2) { //插入点之前的元素较少
                push_front(front());
                copy(start + 2, start + index + 1, start + 1);
            }
            else {
                push_back(back());
                copy_backward(start + index, finish - 2, finish - 1);
            }
            *pos = val;
            return pos;
        }
        //若back满足条件就重新分配map
        void reserve_map_at_back(size_type nodes_to_add=1) {
            if (nodes_to_add + 1 > map_size - (finish.node - map))
                //map尾部的备用节点不足，就换一个map
                reallocate_map(nodes_to_add, false);
        }
        //若front满足条件就重新分配map
        void reserve_map_at_front(size_type nodes_to_add=1) {
            if (nodes_to_add + 1 > start.node - map)
                //map头部的备用节点不足，就换一个map
                reallocate_map(nodes_to_add, true);
        }
        //重新分配map
        void reallocate_map(size_type nodes_to_add, bool add_at_front) {
            size_type old_num_nodes = finish.node - start.node + 1;
            size_type new_num_nodes = old_num_nodes + nodes_to_add;

            map_pointer new_nstart;     //头部 or 尾部 剩的元素满足大于需要元素的两倍
            if (map_size > 2 * new_num_nodes) {
                //调整原来的map，不重新分配
                new_nstart = map + (map_size - new_num_nodes) / 2 + (add_at_front ? nodes_to_add : 0);
                if (new_nstart < start.node) {
                    copy(start.node, finish.node + 1, new_nstart);
                }
                else {
                    copy_backward(start.node, finish.node + 1, new_nstart + old_num_nodes);
                }
            }
            else {  //重新分配
                size_type new_map_size = map_size + max(map_size, nodes_to_add) + 2;
                map_pointer new_map = map_allocator ::allocate(new_map_size);
                new_nstart = new_map + (new_map_size - new_num_nodes) / 2
                        + (add_at_front ? nodes_to_add : 0);
                //把原map内容拷贝
                copy(start.node, finish.node + 1, new_nstart);
                //释放原map
                map_allocator ::deallocate(map, map_size);
                //设置新map
                map = new_map;
                map_size = new_map_size;
            }
            //重设迭代器
            start.set_node(new_nstart);
            finish.set_node(new_nstart + old_num_nodes - 1);

        }
        //给node分配空间
        pointer allocate_node() {
            return data_allocator::allocate(buf_size());
        }
        //把node的空间释放
        void deallocate_node(pointer x) {
            data_allocator::deallocate(x, buf_size());
        }
    };
}
#endif //MY_TINY_STL_DEQUE_H
