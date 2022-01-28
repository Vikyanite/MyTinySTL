#ifndef MY_TINY_STL_ALLOC_H
#define MY_TINY_STL_ALLOC_H
#include <cstdlib>

namespace  STL {
    /*
     * 空间配置器，以字节为单位去分配内存空间
     */
    class alloc {
    private:
        enum{ __ALIGN =  8 }; //小区块上调边界
        enum{ __MAX_BYTES = 128 };//小型区块上界
        enum{ __NFREELISTS = __MAX_BYTES / __ALIGN }; // free_list个数
        enum{ __NOBJS = 20 }; //每次增加的节点数
        static char* start_free; //内存池起点
        static char* end_free; //内存池终点
        static size_t heap_size;

        union obj { //free list的节点
            union obj* nxt;
            char data[1];
        };
        static obj* free_list[__NFREELISTS];

        //将bytes上调至的倍数
        static size_t ROUND_UP(const size_t & bytes) ;
        //根据区块大小，决定使用第n号free_list，从0起
        static size_t FREELIST_INDEX(const size_t & bytes) ;
        //配置可容纳nobjs个大小为size的区块
        static char *chunk_alloc(const size_t & size, size_t &nobjs);

        //返回一个大小为n的对象，并可能加入大小为n的其他区块到free_list
        static void* refill(const size_t & byte) ;

    public:
        static void* allocate(const size_t & bytes) ;
        static void deallocate(void *ptr, const size_t & bytes) ;
        static void* reallocate(void *ptr, const size_t & old_sz, const size_t & new_sz) ;

        alloc() = default;
        ~alloc() = default;
    };
}
#endif //MY_TINY_STL_ALLOC_H
