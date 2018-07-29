//
// Created by dro on 29.07.18.
//

#pragma once

template<typename T, typename A = std::allocator<T>>
class simple_vector {
public:
    simple_vector() :size(0),alloc_size(0),area(nullptr) {}

    ~simple_vector(){
        // destroy old elements
        for (std::size_t i=0;i<size;i++){
            alloc.destroy(area+i);
        }
        //deallocate old area
        if ( area ) alloc.deallocate(area,alloc_size);
    }

    void resize(const std::size_t new_alloc_size){
        if (new_alloc_size<= size) return;
        T* new_area = alloc.allocate(new_alloc_size);

        // copy all elements to new area
        for (std::size_t i=0;i<size;i++){
            alloc.construct(new_area+i, T(*area+i));
        }
        // destroy old elements
        for (std::size_t i=0;i<size;i++){
            alloc.destroy(area+i);
        }
        //deallocate old area
        if ( area ) alloc.deallocate(area,alloc_size);
        alloc_size=new_alloc_size;
        area = new_area;
    }

    void pushback(T elem){
        if (alloc_size < size+1) resize_policy();
        auto pointer = area+size;
        *pointer = elem;
        size++;
    }

    T operator[] ( std::size_t pos ){
        if (pos>=size){
            std::logic_error("Position is invalid");
        }
        auto pointer=area+pos;
        return *pointer;
    }

    std::size_t get_size(){
        return size;
    }

private:
    void resize_policy(){
        if (size == 0){
            resize(2);
        } else {
            resize(size*2);
        }
        return;
    }

private:
    std::size_t size;
    std::size_t alloc_size;
    T* area;
    A alloc;

};

