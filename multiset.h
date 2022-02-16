//
// Created by 32416 on 2022/2/16.
//

#ifndef MY_TINY_STL_MULTISET_H
#define MY_TINY_STL_MULTISET_H
#include "functional.h"
#include "algorithm.h"
#include "rb_tree.h"

namespace STL {
    template <class Key, class Compare=less<Key>>
    class multiset {
    public:
        typedef Key         key_type;
        typedef Key         value_type;
        typedef Compare     key_compare;
        typedef Compare     value_compare;

    private:
        template<class T>
        struct identity : public unary_function<T, T> {
            const T& operator()(const T& x) const { return x; }
        };
        typedef rb_tree<value_type,  key_compare> rep_type;
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
        multiset() = default;

        template <class InputIterator>
        multiset(InputIterator first, InputIterator last):tree(){ tree.insert_multi(first, last); }
        multiset(const multiset& rhs): tree(rhs.tree_){}
        multiset& operator=(const multiset& rhs) {
            tree = rhs.tree_;
            return *this;
        }

        // 相关接口
        key_compare      key_comp()      const { return tree.key_comp(); }
        value_compare    value_comp()    const { return tree.key_comp(); }
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
            return tree.emplace_multi(std::forward<Args>(args)...);
        }

        template <class ...Args>
        iterator emplace_hint(iterator hint, Args&& ...args)
        {
            return tree.emplace_multi_use_hint(hint, std::forward<Args>(args)...);
        }

        std::pair<iterator, bool> insert(const value_type& value)
        {
            return tree.insert_multi(value);
        }

        iterator insert(iterator hint, const value_type& value)
        {
            return tree.insert_multi(hint, value);
        }

        template <class InputIterator>
        void insert(InputIterator first, InputIterator last)
        {
            tree.insert_multi(first, last);
        }

        void      erase(iterator position)             { tree.erase(position); }
        size_type erase(const key_type& key)           { return tree.erase_multi(key); }
        void      erase(iterator first, iterator last) { tree.erase(first, last); }

        void      clear() { tree.clear(); }

        // multiset 相关操作

        iterator       find(const key_type& key)              { return tree.find(key); }

        size_type      count(const key_type& key)       const { return tree.count_multi(key); }

        iterator       lower_bound(const key_type& key)       { return tree.lower_bound(key); }

        iterator       upper_bound(const key_type& key)       { return tree.upper_bound(key); }

        std::pair<iterator, iterator>
        equal_range(const key_type& key)
        { return tree.equal_range_multi(key); }

        std::pair<iterator, iterator>
        equal_range(const key_type& key) const
        { return tree.equal_range_multi(key); }

        void swap(multiset& rhs) noexcept
        { tree.swap(rhs.tree_); }

    public:
        friend bool operator==(const multiset& lhs, const multiset& rhs) { return lhs.tree_ == rhs.tree_; }
        friend bool operator< (const multiset& lhs, const multiset& rhs) { return lhs.tree_ < rhs.tree_; }
    };

// 重载 mystl 的 swap
    template <class Key, class Compare>
    void swap(multiset<Key, Compare>& lhs, multiset<Key, Compare>& rhs) noexcept
    {
        lhs.swap(rhs);
    }

}
#endif //MY_TINY_STL_MULTISET_H
