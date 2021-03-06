

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
                //???????????????????????????
                cur += n;
            }
            else {
                //??????
                difference_type node_offset =
                        offset > 0 ? offset / difference_type(buf_size())
                        : - difference_type((-offset - 1) / buf_size()) - 1;
                set_node(node + node_offset);
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
        //????????????????????????
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

        //?????????????????????

        //??????????????????
        deque() : start(), finish(), map(0), map_size(0){
            fill_initialize(0,T());
        }
        //??????n???val
        deque(int n, const value_type& val) : start(), finish(), map(0), map_size(0){
            fill_initialize(n, val); //????????????????????????
        }

        void push_back(const value_type& val) {
            //???????????????????????????
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
            if (start.node != finish.node) { //??????????????????????????????
                destroy(start.cur, start.last);
                destroy(finish.first, finish.cur);
                deallocate_node(finish.first); //???????????????
            }
            else {
                destroy(start.cur, finish.cur); //?????????????????????
            }
            finish = start;
        }

        iterator erase(iterator pos) {
            iterator next = pos;
            ++next;
            difference_type index = pos - start; //????????????????????????
            if (index < (size() >> 1)) { //????????????????????????????????????????????????????????????
                copy_backward(start, pos, next);
                pop_front();
            }
            else { //???????????????
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
                difference_type n = last - first;   //??????????????????
                difference_type ele_before = first - start; //????????????????????????
                if (ele_before < (size() - n) / 2) {
                    copy_backward(start, first, last);
                    iterator new_start = start + n;
                    destroy(start, new_start);
                    //???????????????????????????
                    for (map_pointer cur = start.node; cur < new_start.node; ++ cur) {
                        deallocate_node(*cur);
                    }
                    start = new_start;
                }
                else {
                    copy(last, finish, first);
                    iterator new_finish = finish - n;
                    destroy(new_finish, finish);
                    for (map_pointer cur = new_finish.node + 1; cur <= finish.node; ++ cur) {
                        deallocate_node(*cur);
                    }
                    finish = new_finish;
                }
                return start + ele_before;
            }
        }
        iterator insert(iterator pos, const value_type& val) {
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
        //??????n?????????
        void creat_map_and_node(size_t num_elements) {
            size_type num_nodes = num_elements / buf_size() + 1;

            map_size = max(size_type(2), num_nodes + 2); //+2???????????????????????????????????????
            map = map_allocator::allocate(map_size);

            //???start???finish?????????????????????????????????????????????????????????????????????
            map_pointer nstart = map + (map_size - num_nodes) / 2;
            map_pointer nfinish = nstart + num_nodes - 1;

            //?????????map_node????????????
            for (map_pointer cur = nstart; cur <= nfinish; ++ cur)
                *cur = allocate_node();

            //??????start???finish?????????
            start.set_node(nstart);
            finish.set_node(nfinish);
            start.cur = start.first;
            finish.cur = finish.first + num_elements % buf_size();
        }
        //?????????????????????
        void fill_initialize(size_type n, const value_type& val) {
            creat_map_and_node(n);
            for (map_pointer cur = start.node; cur < finish.node; ++ cur)
                uninitialized_fill(*cur, *cur + buf_size(), val);
            uninitialized_fill(finish.first, finish.cur, val);
        }
        //push_back????????????
        void push_back_aux(const value_type& val) {
            reserve_map_at_back();
            *(finish.node + 1) = allocate_node();
            construct(finish.cur, val); //??????

            //??????finish?????????
            finish.set_node(finish.node + 1);
            finish.cur = finish.first;
        }
        //push_front???????????????
        void push_front_aux(const value_type& val) {
            reserve_map_at_front();
            *(start.node - 1) = allocate_node();

            //??????finish?????????
            start.set_node(start.node - 1);
            start.cur = start.last - 1;
            construct(start.cur, val); //??????
        }
        //pop_back ???????????????
        void pop_back_aux() {
            deallocate_node(finish.first);
            finish.set_node(finish.node - 1);
            finish.cur = finish.last - 1;
            destroy(finish.cur);
        }
        //pop_front????????????
        void pop_front_aux() {
            destroy(start.cur);

            deallocate_node(start.first);
            start.set_node(start.node + 1);
            start.cur = start.first;
        }
        //insert??????
        iterator insert_aux(iterator pos, const value_type & val) {
            difference_type index = pos - start;
            if (index < size() / 2) { //??????????????????????????????
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
        //???back???????????????????????????map
        void reserve_map_at_back(size_type nodes_to_add=1) {
            if (nodes_to_add + 1 > map_size - (finish.node - map))
                //map??????????????????????????????????????????map
                reallocate_map(nodes_to_add, false);
        }
        //???front???????????????????????????map
        void reserve_map_at_front(size_type nodes_to_add=1) {
            if (nodes_to_add + 1 > start.node - map)
                //map??????????????????????????????????????????map
                reallocate_map(nodes_to_add, true);
        }
        //????????????map
        void reallocate_map(size_type nodes_to_add, bool add_at_front) {
            size_type old_num_nodes = finish.node - start.node + 1;
            size_type new_num_nodes = old_num_nodes + nodes_to_add;

            map_pointer new_nstart;     //?????? or ?????? ?????????????????????????????????????????????
            if (map_size > 2 * new_num_nodes) {
                //???????????????map??????????????????
                new_nstart = map + (map_size - new_num_nodes) / 2 + (add_at_front ? nodes_to_add : 0);
                if (new_nstart < start.node) {
                    copy(start.node, finish.node + 1, new_nstart);
                }
                else {
                    copy_backward(start.node, finish.node + 1, new_nstart + old_num_nodes);
                }
            }
            else {  //????????????
                size_type new_map_size = map_size + max(map_size, nodes_to_add) + 2;
                map_pointer new_map = map_allocator ::allocate(new_map_size);
                new_nstart = new_map + (new_map_size - new_num_nodes) / 2
                        + (add_at_front ? nodes_to_add : 0);
                //??????map????????????
                copy(start.node, finish.node + 1, new_nstart);
                //?????????map
                map_allocator ::deallocate(map, map_size);
                //?????????map
                map = new_map;
                map_size = new_map_size;
            }
            //???????????????
            start.set_node(new_nstart);
            finish.set_node(new_nstart + old_num_nodes - 1);

        }
        //???node????????????
        pointer allocate_node() {
            return data_allocator::allocate(buf_size());
        }
        //???node???????????????
        void deallocate_node(pointer x) {
            data_allocator::deallocate(x, buf_size());
        }
    };
}
#endif //MY_TINY_STL_DEQUE_H
