#include <gtest/gtest.h>
#include <memlib.h>
#include <algorithm>



TEST(MemPool, Allocate){
    hft::memory::MemPool<int> pool(10);
    ASSERT_EQ(pool.getFreeInds(), 10);
    
    
    std::vector<int> vec = {7,9,5,10,2,3,4,5,6,7,9,10,11,12,13,14,15,16,17,18,19,20};
    std::vector<int*> ptrs;
    
    
    std::for_each(vec.begin(), vec.end(), [&pool, &ptrs](int i){
            if(!pool.isPoolFull()) ptrs.push_back(pool.allocate(i));
        });

    int i = 0;
    std::for_each(ptrs.begin(), ptrs.end(), [&pool, &i](int* ptr){
            ASSERT_EQ(*ptr, pool[i]);
            i++;
        });
    ASSERT_EQ(pool.isPoolFull(), true);
    ASSERT_EQ(pool.getFreeInds(), 0);
    pool.printPool();
    std::cout << "\n";
}

TEST(MemPool, DeallocateAndReallocate){
    hft::memory::MemPool<int> pool(10);
 
    std::vector<int> vec = {7,9,5,10,2,3,4,5,6,7,9,10,11,12,13,14,15,16,17,18,19,20};
    std::vector<int*> ptrs;
    
    std::for_each(vec.begin(), vec.end(), [&pool, &ptrs](int i){
            if(!pool.isPoolFull()) ptrs.push_back(pool.allocate(i));
        });
    int a = 0;
    std::for_each(ptrs.begin(), ptrs.end(), [&pool, &a ](int* ptr){
            if(a%2 == 0) pool.deallocate(ptr);
            a++;
        });
    ASSERT_EQ(pool.getFreeInds(), 5);
    ASSERT_EQ(pool.isPoolFull(), false);

     vec = std::vector<int>{200, 300, 400, 500, 600, 700, 800, 900, 1000, 1100};
    std::for_each(vec.begin(), vec.end(), [&pool, &ptrs](int i){
            if(!pool.isPoolFull()) ptrs.push_back(pool.allocate(i));
        });

    ASSERT_EQ(ptrs[10], &pool[0]);
    ASSERT_EQ(ptrs[11], &pool[2]);
    ASSERT_EQ(ptrs[12], &pool[4]);
    ASSERT_EQ(ptrs[13], &pool[6]);
    ASSERT_EQ(ptrs[14], &pool[8]);
    

    ASSERT_EQ(pool.isPoolFull(), true);
    pool.printPool();
    std::cout << "\n";

}



int main(int argc, char **argv){
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}