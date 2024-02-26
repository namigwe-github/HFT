#include "gtest/gtest.h"
#include "loggerlib.h"

TEST(LoggerTest, TestLog) {
    auto logtest = []() {
        hft::Logger logger("test");
        logger.log("Int value %", 3);
        logger.log("Float value %", 3.14f);
        logger.log("Double value %", 3.14);
        logger.log("Char value %", 'c');
        logger.log("C String value %", "string");
        logger.log("C++ String %", std::string("string"));
        logger.log(std::string("c++ string arg"));
        logger.log("Multiple values % % % % %", 1, 2, 3, 4, 5);
        logger.log("Multiple values % % % % %", 1.1, 2.2, 3.3, 4.4, 5.5);
        logger.log("Multiple values % % % % %", 'a', 'b', 'c', 'd', 'e');
    };
    ASSERT_NO_THROW(logtest());
    std::cout<< "Log test passed" << std::endl;
}


int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}