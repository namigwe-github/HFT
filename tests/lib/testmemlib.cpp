#include <gtest/gtest.h>
#include <memlib.h>
#include <algorithm>

TEST(MemPool, Allocate){
    hft::memory::MemPool<int> pool(10);
    //hft::memory::MemPool<int>::MemBlock* v = pool.allocate(7,9,5,10,2);
    std::cout << "Here\n";
    //int* v = pool.allocate(7,9,5,10,2);
    std::vector<int> vec = {7,9,5,10,2};
    pool.printPool();
    std::for_each(vec.begin(), vec.end(), [&pool](int i){
            //std::cout << *(pool.allocate(i)) << "\n";
            pool.deallocate((pool.allocate(i)));
        });
    /*for(size_t i=0; i<pool.getSize(); i++) {
        std::cout << *v <<"\n";
    }*/
    pool.printPool();
    std::cout << "Rebuilt again\n";

}



int main(int argc, char **argv){
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}