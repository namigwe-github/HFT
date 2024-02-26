#include "threadlib.h"
#include "gtest/gtest.h"


void threadFunc() {
    std::cout << "Thread function\n";
    using namespace std::literals::chrono_literals;
    std::this_thread::sleep_for(1s);
}

TEST(ThreadLibTest, ThreadExecutionPass) {
    EXPECT_TRUE(hft::threadlib::createThread(0,threadFunc));
    EXPECT_TRUE(hft::threadlib::createThread(1,threadFunc));
}

TEST(ThreadLibTest, ThreadExecutionFail) {
    EXPECT_FALSE(hft::threadlib::createThread(-1,threadFunc));
    EXPECT_FALSE(hft::threadlib::createThread(2,threadFunc));
    EXPECT_FALSE(hft::threadlib::createThread(20,threadFunc));
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
