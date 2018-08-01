//
// Created by dro on 25.06.18.
//

#include "gtest/gtest.h"
#include "alloc_fwd.h"
#include "simple_vector.h"
#include <vector>
#include <map>
#include <string>

TEST (AllocFWD, VectorAllocInt){
    std::vector<int,forward_allocator<int>> vec;
    for (std::size_t i=0;i<64;i++){
        vec.push_back(i*i);
    }
    for (std::size_t i=0;i<64;i++){
        EXPECT_EQ(vec[i],i*i);
    }
}

TEST (AllocFWD, VectorAllocStr){
    std::vector<std::string,forward_allocator<std::string>> vec;
    for (std::size_t i=0;i<64;i++){
        vec.push_back(std::to_string(i*i));
    }
    for (std::size_t i=0;i<64;i++){
        EXPECT_EQ(vec[i],std::to_string(i*i));
    }
    EXPECT_EQ(vec[63],std::to_string(63*63));
}

TEST (AllocFWD, MapAllocInt){
    auto test_map=std::map<int,int,std::less<int>,forward_allocator<std::pair<const int,int>>> {};
    for (std::size_t i=0;i<64;i++){
        test_map[i]=(i*i);
    }
    for (std::size_t i=0;i<64;i++){
        EXPECT_EQ(test_map[i],i*i);
    }
}

TEST (AllocFWD, MapAllocStr){
    auto test_map = std::map<int,std::string,std::less<int>,forward_allocator<std::pair<const int,std::string>>> {};
    for (std::size_t i=0;i<64;i++){
        test_map[i]=(std::to_string(i*i));
    }
    for (std::size_t i=0;i<64;i++){
        EXPECT_EQ(test_map[i],std::to_string(i*i));
    }
    EXPECT_EQ(test_map[63],std::to_string(63*63));
}

//Test allocation more then default block size
TEST (AllocFWD, VectorAllocIntHuge){
    std::vector<int,forward_allocator<int>> vec;
    for (std::size_t i=0;i<ELEMENTS_IN_BLOCK*2;i++){
        vec.push_back(i*i);
    }
    for (std::size_t i=0;i<ELEMENTS_IN_BLOCK*2;i++){
        EXPECT_EQ(vec[i],i*i);
    }
}

TEST (SimpleVector, ResizeInt){
    std::vector<int> array={1,3,7,11,13,17,19,23,31,37};
    simple_vector<int> sv;
    for (auto i : array){
        sv.pushback(i);
    }
    sv.resize(1024);
    for (std::size_t i=0;i<sv.get_size();i++){
        EXPECT_EQ(array[i],sv[i]);
    }
}

TEST (SimpleVector, ResizeStr){
    std::vector<std::string> array={"1","3","7","11","13","17","19","23","31","37"};
    simple_vector<std::string> sv;
    for (auto i : array){
        sv.pushback(i);
    }
    sv.resize(1024);
    for (std::size_t i=0;i<sv.get_size();i++){
        EXPECT_EQ(array[i],sv[i]);
    }
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}