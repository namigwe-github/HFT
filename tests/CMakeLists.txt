


add_executable(testthreadlib lib/testthreadlib.cpp)
add_executable(testnetworklib lib/testnetworklib.cpp)
add_executable(testlfqueuelib lib/testlfqueuelib.cpp)
add_executable(testloggerlib lib/testloggerlib.cpp)
add_executable(testmemlib lib/testmemlib.cpp)

target_link_libraries(testthreadlib GTest::gtest_main)
target_link_libraries(testnetworklib GTest::gtest_main)
target_link_libraries(testlfqueuelib GTest::gtest_main)
target_link_libraries(testloggerlib GTest::gtest_main)
target_link_libraries(testmemlib GTest::gtest_main)

target_include_directories(testthreadlib PUBLIC ${CMAKE_SOURCE_DIR}/lib/thread)
target_include_directories(testnetworklib PUBLIC ${CMAKE_SOURCE_DIR}/lib/network)
target_include_directories(testlfqueuelib PUBLIC ${CMAKE_SOURCE_DIR}/lib/datastructures)
target_include_directories(testloggerlib PUBLIC ${CMAKE_SOURCE_DIR}/lib/logger)
target_include_directories(testmemlib PUBLIC ${CMAKE_SOURCE_DIR}/lib/memory)

include(GoogleTest)
gtest_discover_tests(testthreadlib)
gtest_discover_tests(testnetworklib)
gtest_discover_tests(testlfqueuelib)
gtest_discover_tests(testloggerlib)
gtest_discover_tests(testmemlib)