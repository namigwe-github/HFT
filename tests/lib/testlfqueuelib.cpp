#include <gtest/gtest.h>
#include <lfqueuelib.h>
#include <iostream>
#include <thread>


std::thread t1;
std::thread t2;

TEST(LFQueueTest, TestPushPop) {
    std::cout << "CPP Version: " << __cplusplus << std::endl;
    std::cout << "TestPushPop" << std::endl;
    hft::datastructures::LFQueue<int> q;
    auto func = [&q](){    
        q.push(10);
        q.push(34);
        q.push(23);
        q.push(45);
        q.push(56);
        q.push(67);
    };
    auto funcpop = [&q](){
        int a = q.pop();
        int b = q.pop();
        int c = q.pop();
        int d = q.pop();
        int e = q.pop();
        int f = q.pop();
        ASSERT_EQ(a, 10);
        ASSERT_EQ(b, 34);
        ASSERT_EQ(c, 23);
        ASSERT_EQ(d, 45);
        ASSERT_EQ(e, 56);
        ASSERT_EQ(f, 67);
        int i = 1;
        while(1);
    };
    t1 = std::thread{func};
    t2 = std::thread{funcpop};
    //t2(funcpop);
    t1.join();
    t2.join();

    auto func2 = [&q](){
        for(int i = 0; i < 1000; i++){
            q.push(i);
        }
    };
    auto funcpop2 = [&q](){
        for(int i = 0; i < 1000; i++){
            int a = q.pop();
            ASSERT_EQ(a, i);
        }
    };
    std::thread t3(func2);
    std::thread t4(funcpop2);
    t3.join();
    t4.join();

}



int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
