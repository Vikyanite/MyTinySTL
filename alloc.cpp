#include "alloc.h"
#include <iostream>
namespace STL {
    char* alloc::start_free = nullptr;
    char* alloc::end_free = nullptr;
    size_t alloc::heap_size = 0;

    alloc::obj* alloc::free_list[alloc::__NFREELISTS] = {
            nullptr, nullptr, nullptr, nullptr,
            nullptr, nullptr, nullptr, nullptr,
            nullptr,nullptr, nullptr, nullptr,
            nullptr, nullptr, nullptr, nullptr,
    };

    size_t alloc::ROUND_UP(const size_t & bytes) {
        return ((bytes + __ALIGN - 1) & ~(__ALIGN - 1));
    }

    size_t alloc::FREELIST_INDEX(const size_t & bytes) {
        return (((bytes) + __ALIGN - 1) / __ALIGN - 1);
    }
    void *alloc::allocate(const size_t &  n) {
        if (n > (size_t)__MAX_BYTES) {
            return malloc(n);
        }
        //从16个free_list中取一个适当大小的空间
        size_t index = FREELIST_INDEX(n);
        obj* my_free_list = free_list[index];
        //没有合适大小的空间， 去内存迟里取
        if (my_free_list == nullptr) {
            void* refill_p = refill(ROUND_UP(n));
            return refill_p;
        }
        //有可用空间，从free_list中取
        free_list[index] = my_free_list->nxt;
        return my_free_list;
    }
    void alloc::deallocate(void *ptr, const size_t & n) {
        if (n > __MAX_BYTES) { //大于128
            free(ptr);
        }
        //回收到对应free_list中
        size_t index = FREELIST_INDEX(n);
        obj* node = static_cast<obj*>(ptr);
        node->nxt = free_list[index];
        free_list[index] = node;
    }

    void *alloc::reallocate(void *ptr, const size_t & old_sz, const size_t & new_sz) {
        deallocate(ptr, old_sz);
        ptr = allocate(new_sz);
        return ptr;
    }

    void *alloc::refill(const size_t & n) {
        //n保证是8的倍数
        size_t nobjs = 20;
        char* chunk = chunk_alloc(n, nobjs);
        obj** my_free_list;
        obj* result;
        obj* current_obj, *next_obj;

        if (nobjs == 1) return chunk;

        my_free_list = free_list + FREELIST_INDEX(n);
        result = (obj*) (chunk);
        *my_free_list = next_obj = (obj* )(chunk + n);
        //将取多的空间放到对应free_list里
        for (int i = 1; ; ++ i) {
            current_obj = next_obj;
            next_obj = (obj* )((char *)next_obj + n);
            if (nobjs - 1 == i) {
                current_obj->nxt = nullptr;
                break;
            }
            else {
                current_obj->nxt = next_obj;
            }
        }
        return result;
    }

    char *alloc::chunk_alloc(const size_t & bytes, size_t &nobjs) {
        char* result;
        size_t need_bytes = bytes * nobjs;
        size_t bytes_left = end_free - start_free;

        if (bytes_left >= need_bytes) { //内存池剩余大小满足需要
            result = start_free;
            start_free = start_free + need_bytes;
            return result;
        }
        else if (bytes_left >= bytes) {  //只能满足一个以上的需要
            nobjs = bytes_left / bytes;
            need_bytes = nobjs * bytes;
            result = start_free;
            start_free += need_bytes;
            return result;
        }
        else { //内存池连一个都无法满足
            if (bytes_left > 0) {
                obj ** my_free_list = free_list + FREELIST_INDEX(bytes_left);
                ((obj *)start_free)->nxt = *my_free_list;
                *my_free_list = (obj *)start_free;
            }
            size_t bytes_to_get = 2 * need_bytes + ROUND_UP(heap_size >> 4);
            start_free = (char *) malloc(bytes_to_get);
            if (start_free == nullptr) { //malloc找不斗空间，寻找大小相近的空间
                obj** my_free_list;
                obj* ptr;
                for (size_t i = bytes; i <= __MAX_BYTES; i += __ALIGN) {
                    my_free_list = free_list + FREELIST_INDEX(i);
                    ptr = *my_free_list;
                    if (ptr != nullptr) {
                        *my_free_list = ptr->nxt;
                        start_free = (char *)ptr;
                        end_free = start_free + i;
                        return chunk_alloc(bytes, nobjs);
                    }
                }
                end_free = nullptr;
                throw std::bad_alloc();
            }
            heap_size += bytes_to_get;
            end_free = start_free + bytes_to_get;
            return chunk_alloc(bytes, nobjs);

        }
    }


}

