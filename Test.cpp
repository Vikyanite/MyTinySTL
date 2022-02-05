#include <iostream>
#include "allocator.h"
#include <vector>
#include "vector.h"
#include "list.h"

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
    std::cout << "size: " << v.size() << std::endl;
    std::cout << "in: ";
    for (auto &i : v) std::cout << i << " ";
    std::cout << std::endl;
}

void vectorTest() {
    //构造，复制 clear
    STL::vector<int> v1; //默认构造函数
    STL::vector<int> v2(v1); //复制构造函数
    STL::vector<int> v3(1, 1); //重载构造函数 1
    STL::vector<int> v4(2); //重载构造函数 2
    print(v1);
    print(v2);
    print(v3);
    print(v4);

    //比较操作符 clear
    std::cout << "v1 == v2 : " << (v1 == v2 ? "equal" : "not equal") << std::endl;
    std::cout << "v3 != v4 : " << (v3 != v4 ? "not equal" : "equal") << std::endl;

    //迭代器相关 clear
    std::cout << "test begin(), end() : " << *v3.begin() << " " << *(v3.end()-1) << std::endl;

    //容量大小相关 clear
    std::cout << "test empty(): " << v1.empty() << std::endl;
    std::cout << "test resize() " << std::endl;
    v1.resize(5, 3);
    print(v1);

    v2.reserve(3);
    print(v2);

    v2.shrink_yo_fit();
    print(v2);

    //元素访问 clear
    std::cout << "test for front(), back(), operator[], data: " <<
    v1.front() << " " << v1.back() << " " << v1[0] << " " << *v1.data() << std::endl;

    //元素调整
    v1.push_back(2);
    std::cout << "test for push_back: " << std::endl;
    print(v1);

    v1.pop_back();
    std::cout << "test for pop_back: " << std::endl;
    print(v1);

    v1.clear();
    std::cout << "test for clear: " << std::endl;
    print(v1);

    auto t = v1.insert(v1.begin(), 2);
    std::cout << "test for insert(position, value): " << std::endl;
    print(v1);
    std::cout << (t == v1.begin()) << std::endl;

    v1.insert(v1.begin(), 1, 1);
    v1.insert(v1.begin()+2, 1, 3);
    std::cout << "test for insert(position, n, value): " << std::endl;
    print(v1);

    v1.erase(v1.begin() + 1);
    std::cout << "test for erase(position): " << std::endl;
    print(v1);

    v1.erase(v1.begin() + 1, v1.begin() + 2);
    std::cout << "test for erase(first, last): " << std::endl;
    print(v1);
}

void listTest() {
    //构造函数测试
    STL::list<int> l1(3, 3);
    STL::list<int> l2;
    STL::list<int> l3(l1);
    STL::list<int> l4 = l1;

    l1.push_back(-1);
    print(l1);
    print(l2);
    print(l3);
    print(l4);


    //元素访问
    std::cout << "test for front, back : " << l1.front() << " " << l1.back() << std::endl;

    //元素操作
    l1.pop_front();
    l1.pop_back();
    l1.push_back(4);
    l1.push_front(1);
    std::cout << "test for push/pop _ front/back : " << std::endl;
    print(l1);

    std::cout << "test for remove : " << std::endl;
    l1.remove(-1);
    print(l1);

    std::cout << "test for swap : " << std::endl;
    l1.swap(l2);
    print(l1);
    print(l2);
    l1.swap(l2);
    std::cout << "test for swap : " << std::endl;
    print(l1);

    std::cout << "test for reverse : " << std::endl;
    l1.reverse();
    print(l1);

    std::cout << "test for sort : " << std::endl;
    l1.sort();
    print(l1);

    std::cout << "test for unique : " << std::endl;
    l1.unique();
    print(l1);

    std::cout << "test for merge : " << std::endl;
    l1.merge(l3);
    print(l1);
    print(l3);

    std::cout << "test for clear : " << std::endl;
    l1.clear();
    print(l1);


}

int main() {
    //allocatorTest(); //clear
    //vectorTest(); //clear
    listTest();
    return 0;
}
