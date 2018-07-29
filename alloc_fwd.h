//
// Created by dro on 24.07.18.
//

#pragma once

#include <iostream>
#include <vector>
#include <bitset>
#include <algorithm>
//maximum
#define ELEMENTS_IN_BLOCK 64


struct header{
    size_t tail;
    std::bitset<ELEMENTS_IN_BLOCK> bitmap;
    void* pointer;
};

template<typename T>
struct forward_allocator {
    using value_type = T;
    using pointer = T*;
    using const_pointer = const T*;
    using reference = T&;
    using const_reference = const T&;
    std::vector<header> block;


    T* allocate(std::size_t n) {
        if(block.empty()) block.reserve(16);
        //Allocate blocks more then default block size
        if(n>ELEMENTS_IN_BLOCK){
            auto p = std::malloc(sizeof(T)*n);
            if (!p)
                throw std::bad_alloc();
            block.push_back(header{n,ELEMENTS_IN_BLOCK,p});
            return reinterpret_cast<T *>(p);
        }
        //Try allocate in extising blocks
        for (auto& bl : block){
            if(ELEMENTS_IN_BLOCK==bl.tail) continue;
            if(ELEMENTS_IN_BLOCK-bl.tail < n) continue;

            for (size_t j=bl.tail; j<bl.tail+n; j++) {
                bl.bitmap.set(j, true);
            }
            auto p=static_cast<T*>(bl.pointer)+bl.tail;
            bl.tail+=n;
            return reinterpret_cast<T *>(p);
        }

        //Allocate in new block
        auto p = std::malloc(sizeof(T) * ELEMENTS_IN_BLOCK);
        if (!p)
            throw std::bad_alloc();
        block.push_back(header{n, n, p});
        return reinterpret_cast<T *>(p);

    }

    void deallocate(T *p, std::size_t n) {
        bool have_clean_pages=false;
        for (auto& bl : block){
            auto first_elem=static_cast<T*>(bl.pointer);
            if (first_elem == bl.pointer && bl.tail>ELEMENTS_IN_BLOCK){ // Delete big block
                std::free(bl.pointer);
                bl.pointer= nullptr;
                bl.tail=0;
                bl.bitmap=0;
                have_clean_pages=true;
                break;
            }
            if (first_elem<=p && p<first_elem+ELEMENTS_IN_BLOCK){
                size_t index = (p-first_elem);
                for(size_t i=index;i<index+n;i++){
                    bl.bitmap.set(i,false);
                }
                if (bl.bitmap.none()){
                    std::free(bl.pointer);
                    bl.pointer= nullptr;
                    bl.tail=0;
                    have_clean_pages=true;
                } else {
                    return; // Exit becouse nothing to do
                }
                break;
            }
;
        }
        if (have_clean_pages){
            auto it = std::partition(block.begin(),block.end(),[](auto v){return v.pointer;});
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
