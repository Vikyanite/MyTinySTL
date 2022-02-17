
#ifndef MY_TINY_STL_UNORDERED_MAP_H
#define MY_TINY_STL_UNORDERED_MAP_H
#include "hashtable.h"

namespace STL {

    template<class Key, class T, class Hash = STL::hash <Key>, class KeyEqual = STL::equal_to <Key>>
    class unordered_map {
    private:
        // 使用 hashtable 作为底层机制
        typedef hashtable<std::pair<const Key, T>, Hash, KeyEqual> base_type;
        base_type ht_;

    public:
        // 使用 hashtable 的型别

        typedef typename base_type::allocator_type allocator_type;
        typedef typename base_type::key_type key_type;
        typedef typename base_type::mapped_type mapped_type;
        typedef typename base_type::value_type value_type;
        typedef typename base_type::hasher hasher;
        typedef typename base_type::key_equal key_equal;

        typedef typename base_type::size_type size_type;
        typedef typename base_type::difference_type difference_type;
        typedef typename base_type::pointer pointer;
        typedef typename base_type::reference reference;


        typedef typename base_type::iterator iterator;


        allocator_type get_allocator() const { return ht_.get_allocator(); }

    public:
        // 构造、复制、移动、析构函数

        unordered_map()
                : ht_(100, Hash(), KeyEqual()) {
        }

        explicit unordered_map(size_type bucket_count,
                               const Hash &hash = Hash(),
                               const KeyEqual &equal = KeyEqual())
                : ht_(bucket_count, hash, equal) {
        }

        template<class InputIterator>
        unordered_map(InputIterator first, InputIterator last,
                      const size_type bucket_count = 100,
                      const Hash &hash = Hash(),
                      const KeyEqual &equal = KeyEqual())
                : ht_(STL::max(bucket_count, static_cast<size_type>(STL::distance(first, last))), hash, equal) {
            for (; first != last; ++first)
                ht_.insert_unique_noresize(*first);
        }

        unordered_map(std::initializer_list<value_type> ilist,
                      const size_type bucket_count = 100,
                      const Hash &hash = Hash(),
                      const KeyEqual &equal = KeyEqual())
                : ht_(STL::max(bucket_count, static_cast<size_type>(ilist.size())), hash, equal) {
            for (auto first = ilist.begin(), last = ilist.end(); first != last; ++first)
                ht_.insert_unique_noresize(*first);
        }

        unordered_map(const unordered_map &rhs)
                : ht_(rhs.ht_) {
        }


        unordered_map &operator=(const unordered_map &rhs) {
            ht_ = rhs.ht_;
            return *this;
        }


        ~unordered_map() = default;

        // 迭代器相关

        iterator begin() noexcept { return ht_.begin(); }


        iterator end() noexcept { return ht_.end(); }


        // 容量相关

        bool empty() const noexcept { return ht_.empty(); }

        size_type size() const noexcept { return ht_.size(); }

        size_type max_size() const noexcept { return ht_.max_size(); }

        // 修改容器操作

        // insert

        std::pair<iterator, bool> insert(const value_type &value) { return ht_.insert_unique(value); }

        template<class InputIterator>
        void insert(InputIterator first, InputIterator last) { ht_.insert_unique(first, last); }

        // erase / clear

        void erase(iterator it) { ht_.erase(it); }

        size_type erase(const key_type &key) { return ht_.erase_unique(key); }

        void clear() { ht_.clear(); }

        void swap(unordered_map &other) noexcept { ht_.swap(other.ht_); }

        // 查找相关
        mapped_type &operator[](const key_type &key) {
            iterator it = ht_.find(key);
            if (it.node == nullptr)
                it = insert(std::make_pair(key, T{})).first;
            return it->second;
        }

        size_type count(const key_type &key) const { return ht_.count(key); }

        iterator find(const key_type &key) { return ht_.find(key); }

        pair<iterator, iterator> equal_range(const key_type &key) { return ht_.equal_range_unique(key); }


        // bucket interface
        size_type bucket_count() const noexcept { return ht_.bucket_count(); }

        size_type max_bucket_count() const noexcept { return ht_.max_bucket_count(); }

        size_type bucket_size(size_type n) const noexcept { return ht_.bucket_size(n); }

        size_type bucket(const key_type &key) const { return ht_.bucket(key); }

        // hash policy

        void rehash(size_type count) { ht_.rehash(count); }

        void reserve(size_type count) { ht_.reserve(count); }




    };


// 重载 mystl 的 swap
    template<class Key, class T, class Hash, class KeyEqual>
    void swap(unordered_map<Key, T, Hash, KeyEqual> &lhs,
              unordered_map<Key, T, Hash, KeyEqual> &rhs) {
        lhs.swap(rhs);
    }
}

#endif //MY_TINY_STL_UNORDERED_MAP_H
