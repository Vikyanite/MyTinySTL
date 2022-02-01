#include <iostream>
#include "allocator.h"
#include <vector>
#include "vector.h"
void allocatorTest() { //测试自己写的allocator与std::vector的交互
    std::vector<int, STL::allocator<int> > v1;
    for (int i = 0; i < 5; ++ i) v1.push_back(i);
    for (auto &i : v1)
        std::cout << i << " ";
    std::cout << std::endl;
    v1.pop_back();
    v1.push_back(20);
    for (auto &i : v1)
        std::cout << i << " ";
    std::cout << std::endl;
}
template<class T>
void print(T &v) {
    std::cout << "in: ";
    for (auto &i : v) std::cout << i << " ";
    std::cout << std::endl;
}

void vectorTest() {
    //构造相关
    STL::vector<int> v1(1, 1);
    ///迭代器相关
    std::cout << *v1.begin() << " " << *(v1.end()-1) << std::endl;
    ///大小容量相关
    std::cout << v1.capacity() << " " << v1.size() << std:: endl; //test for capacity(), size();

    //test for reserve();
    v1.reserve(2);
    std::cout << "capacity: " << v1.capacity() << " size: " << v1.size() << std:: endl;
    std::cout << *v1.begin() << std::endl;
    print(v1);

    //resize()

    v1.resize(3, 4);
    std::cout << "capacity: " << v1.capacity() << " size: " << v1.size() << std:: endl;
    print(v1);

}

int main() {
    //allocatorTest(); //clear
    vectorTest();
    return 0;
}
