//
// Created by dro on 24.07.18.
//

#pragma once

#include <iostream>
#include <vector>
//#include <map>
#include <bitset>
#include <algorithm>
#define ELEMENTS_IN_BLOCK 1024


template<typename T>
struct bit_allocator {
    using value_type = T;
    using pointer = T*;
    using const_pointer = const T*;
    using reference = T&;
    using const_reference = const T&;
    std::vector<void*> block;

    constexpr size_t size_of_block(int n){
        return sizeof(T)*ELEMENTS_IN_BLOCK + sizeof(std::bitset<ELEMENTS_IN_BLOCK>);
    }

    T* allocate(std::size_t n) {
        if(block.empty()) block.reserve(16);
        //Try allocate in extising blocks
        for (auto bl : block){
            auto bs = static_cast<std::bitset<ELEMENTS_IN_BLOCK>*>(bl);
//            auto str = bs->to_string();
            if (bs->all()) continue;
            if(ELEMENTS_IN_BLOCK-bs->count()<n) continue;
            for(size_t i=0; i<ELEMENTS_IN_BLOCK; i++){
                if(!bs->test(i)){
                    auto next=bs->_Find_next(i);
                    if (next-i<n) {i=next; continue;}
                    for (size_t j=i; j<i+n; j++) {
                        bs->set(j, true);
                    }
                    auto first_elem=static_cast<T*>(bl+sizeof(*bs));
                    auto p=first_elem+i;
//                    std::cout << "bs:" << bs->to_string() << std::endl;
                    return reinterpret_cast<T *>(p);
                }
            }

        }

        //Allocate in new block
        auto p = std::malloc(size_of_block(n));
        if (!p)
            throw std::bad_alloc();
        block.push_back(p);
        std::bitset<ELEMENTS_IN_BLOCK>* bs = new((void*)p) std::bitset<ELEMENTS_IN_BLOCK>(0);
        for(size_t i=0;i<n;i++){
            bs->set(i,true);
        }
//        std::cout << "bs:" << bs->to_string() << std::endl;
        p+=sizeof(std::bitset<ELEMENTS_IN_BLOCK>);
        return reinterpret_cast<T *>(p);
    }

    void deallocate(T *p, std::size_t n) {
        bool have_clean_pages=false;
        for (auto& bl : block){
            auto bs = static_cast<std::bitset<ELEMENTS_IN_BLOCK>*>(bl);
            auto first_elem=static_cast<T*>(bl+sizeof(*bs));
            if (first_elem<=p && p<first_elem+ELEMENTS_IN_BLOCK){
                size_t index = (p-first_elem);
//                std::cout << "index:" << index << std::endl;
                for(size_t i=index;i<index+n;i++){
                    bs->set(i,false);
                }
//                std::cout << "bs:" << bs->to_string() << std::endl;
                if (bs->none()){
                    bs->~bitset<ELEMENTS_IN_BLOCK>();
                    std::free(bl);
                    bl=nullptr;
                    have_clean_pages=true;
                } else {
                    return; // Exit becouse nothing to do
                }
                break;
            }
;
        }
        if (have_clean_pages){
            auto it = std::partition(block.begin(),block.end(),[](auto v){return v;});
            block.erase(it,block.end());
        }
    }

    template<typename U, typename ...Args>
    void construct(U *p, Args &&...args) {
        new(p) U(std::forward<Args>(args)...);
    };

    void destroy(T *p) {
        p->~T();
    }

//  g++ Don't use this
//    template<typename U>
//    struct rebind {
//        using other = logging_allocator<U>;
//    };
//
//    logging_allocator() = default;
//    ~logging_allocator() = default;
//
//    template<typename U>
//    logging_allocator(const logging_allocator<U>&) {
//
//    }
};
