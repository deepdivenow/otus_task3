#include <iostream>
#include <map>

//#include "alloc.h"
#include "alloc_fwd.h"
#include "simple_vector.h"

int factorial(int n) {
    if(n < 1) return 1;
    return factorial(n-1) * n;
}

template<typename T1, typename T2>
std::ostream& operator<< (std::ostream& stream, const std::map<T1,T2>& m) {
    for (auto i : m){
        stream << i << std::endl;
    }
    return stream;
}

template<typename T1, typename T2, typename T3,typename T4>
std::ostream& operator<< (std::ostream& stream, const std::map<T1,T2,T3,T4>& m) {
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

template<typename T>
void set_map(T& m) {
    for (size_t i=0; i<10; i++){
        m[i]=factorial(i);
    }
    return;
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
        std::cout << m << std::endl;
    }
    {
        auto m = std::map<int, int, std::less<int>, forward_allocator<std::pair<const int, int>>>{};
        set_map(m);
        std::cout << m << std::endl;
    }

    {
        simple_vector<int> sv;
        for (std::size_t i = 0; i < 10; i++) {
            sv.pushback(i);
        }
        for (std::size_t i = 0; i < 10; i++) {
            std::cout << sv[i] << std::endl;
        }
    }
    {
        simple_vector<int,forward_allocator<int>> sv;
        for (std::size_t i = 0; i < 10; i++) {
            sv.pushback(i);
        }
        for (std::size_t i = 0; i < 10; i++) {
            std::cout << sv[i] << std::endl;
        }
    }
    return 0;
}