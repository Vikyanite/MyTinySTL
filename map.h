
#ifndef MY_TINY_STL_MAP_H
#define MY_TINY_STL_MAP_H
#include "functional.h"
#include "alloc.h"
#include "rb_tree.h"
#include <algorithm>
namespace STL {
    template <class Key, class T, class Compare=less<Key>>
    class map {
    public:
        typedef Key                     key_type;
        typedef T                       mapped_type;
        typedef std::pair<const Key, T> value_type;
        typedef Compare                 key_compare;
        class value_compare : public binary_function<value_type, value_type, bool> {
            friend class map<Key, T, Compare> ;
        protected:
            Compare comp;
            value_compare(Compare c) : comp(c) { }
        public:
            bool operator()(const value_type& x, const value_type& y) const {
                return comp(x.first, y.first);
            }
        };

    private:
        typedef rb_tree<value_type, key_compare> rep_type;
        rep_type tree;

    public:
        // 使用 rb_tree 定义的型别
        typedef typename rep_type::node_type              node_type;
        typedef typename rep_type::pointer                pointer;
        typedef typename rep_type::reference              reference;
        typedef typename rep_type::iterator               iterator;
        typedef typename rep_type::size_type              size_type;
        typedef typename rep_type::difference_type        difference_type;
        typedef typename rep_type::allocator_type         allocator_type;
        map() = default;

        template <class InputIterator>
        map(InputIterator first, InputIterator last):tree(){ tree.insert_unique(first, last); }
        map(const map& rhs):tree(rhs.tree_){}
        map& operator=(const map& rhs) {
            tree = rhs.tree_;
            return *this;
        }

        // 相关接口
        key_compare      key_comp()      const { return tree.key_comp(); }
        value_compare    value_comp()    const { return value_compare(tree.key_comp()); }
        allocator_type   get_allocator() const { return tree.get_allocator(); }

        // 迭代器相关
        iterator               begin()
        { return tree.begin(); }

        iterator               end()           noexcept
        { return tree.end(); }

        // 容量相关
        bool                   empty()    const noexcept { return tree.empty(); }
        size_type              size()     const noexcept { return tree.size(); }
        size_type              max_size() const noexcept { return tree.max_size(); }

        // 插入删除操作

        template <class ...Args>
        std::pair<iterator, bool> emplace(Args&& ...args)
        {
            return tree.emplace_unique(std::forward<Args>(args)...);
        }

        std::pair<iterator, bool> insert(const value_type& value)
        {
            return tree.insert_unique(value);
        }

        iterator insert(iterator hint, const value_type& value)
        {
            return tree.insert_unique(hint, value);
        }

        template <class InputIterator>
        void insert(InputIterator first, InputIterator last)
        {
            tree.insert_unique(first, last);
        }

        void      erase(iterator position)             { tree.erase(position); }
        size_type erase(const key_type& key)           { return tree.erase_unique(key); }
        void      erase(iterator first, iterator last) { tree.erase(first, last); }

        void      clear() { tree.clear(); }
        T& operator[](const key_type& k) {
            return (*( (insert(value_type(k, T()))).first )).second;
        }
        // multiset 相关操作

        iterator       find(const key_type& key)              { return tree.find(key); }

        size_type      count(const key_type& key)       const { return tree.count_unique(key); }

        iterator       lower_bound(const key_type& key)       { return tree.lower_bound(key); }

        iterator       upper_bound(const key_type& key)       { return tree.upper_bound(key); }

        std::pair<iterator, iterator>
        equal_range(const key_type& key)
        { return tree.equal_range_unique(key); }

        std::pair<iterator, iterator>
        equal_range(const key_type& key) const
        { return tree.equal_range_unique(key); }

        void swap(map& rhs) noexcept
        { tree.swap(rhs.tree_); }

    };

// 重载 mystl 的 swap
    template <class Key, class Compare>
    void swap(map<Key, Compare>& lhs, map<Key, Compare>& rhs) noexcept
    {
        lhs.swap(rhs);
    }

};
#endif //MY_TINY_STL_MAP_H
