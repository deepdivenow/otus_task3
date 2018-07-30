#include <iostream>
#include <map>

//#include "alloc.h"
#include "alloc_fwd.h"
#include "simple_vector.h"

int factorial(int n) {
    if(n < 1) return 1;
    return factorial(n-1) * n;
}

// Простой шаблон
//template<typename Key, typename Value>
//std::ostream& operator<< (std::ostream& stream, const std::map<Key,Value>& m) {
//    for (auto i : m){
//        stream << i << std::endl;
//    }
//    return stream;
//}

// Указать дефолтовые значения
//template<typename Key, typename Value,
//        typename S = std::less<Key>,
//        typename A = std::allocator<std::pair<const Key,Value>>>
//std::ostream& operator<< (std::ostream& stream, const std::map<Key,Value,S,A>& m) {
//    for (auto i : m){
//        stream << i << std::endl;
//    }
//    return stream;
//}

// Использовать параметр пак
template<typename ...Args>
std::ostream& operator<< (std::ostream& stream, const std::map<Args...>& m) {
    for (auto i : m){
        stream << i << std::endl;
    }
    return stream;
}

template <typename T1, typename T2>
std::ostream& operator<<(std::ostream& stream, const std::pair<T1,T2> p){
    stream << p.first << " " << p.second;
    return stream;
}

template<typename ...Args>
void set_map(std::map<Args...>& m) {
    for (size_t i=0; i<10; i++){
        m[i]=factorial(i);
    }
    return;
}

template<typename ...Args>
void set_simple_vector(simple_vector<Args...>& v) {
    for (size_t i=0; i<10; i++){
        v.pushback(i);
    }
    return;
}

template<typename ...Args>
std::ostream& operator<< (std::ostream& stream, const simple_vector<Args...>& v) {
    for (std::size_t i=0; i<v.get_size(); i++){
        stream << v[i] << std::endl;
    }
    return stream;
}

int main() {
//    auto v = std::vector<int, forward_allocator<int>>{};
//    // v.reserve(5);
//    for (int i = 0; i < 64; ++i) {
//        std::cout << "vector size = " << v.size() << std::endl;
//        v.emplace_back(i);
//        std::cout << std::endl;
//    }
//
//    for (auto i: v) {
//        std::cout << i << std::endl;
//    }

    {
        auto m = std::map<int, int>{};
        set_map(m);
        std::cout << m;
    }
    {
        auto m = std::map<int, int, std::less<int>, forward_allocator<std::pair<const int, int>>>{};
        set_map(m);
        std::cout << m;
    }

    {
        simple_vector<int> sv;
        set_simple_vector(sv);
        std::cout << sv;
    }
    {
        simple_vector<int,forward_allocator<int>> sv;
        set_simple_vector(sv);
        std::cout << sv;
    }
    return 0;
}