#include "tcpserverlib.h"
#include "tcpclientlib.h"
#include "gtest/gtest.h"

TEST(NetworkLibTest, TCPServerCreationPass) {
    hft::networklib::TCPServer server1{"8080"};
    EXPECT_TRUE(server1.isRunning());
    hft::networklib::TCPServer server2{"8480"};
    EXPECT_TRUE(server2.isRunning());
    hft::networklib::TCPServer server3{"8070"};
    EXPECT_TRUE(server3.isRunning());
    

}

TEST(NetworkLibTest, TCPServerCreationFailBind) {
    hft::networklib::TCPServer server5{"8030"};
    hft::networklib::TCPServer server6{"8090"};
    EXPECT_EXIT(hft::networklib::TCPServer server7{"8030"}, testing::ExitedWithCode(EXIT_FAILURE),"bind error: Address already in use");
    EXPECT_DEATH(hft::networklib::TCPServer server8{"8090"}, "bind error: Address already in use");
    //EXPECT_EXIT(hft::networklib::TCPServer server4{"8030"}, testing::ExitedWithCode(EXIT_FAILURE),"Failure");
}

void testFunc() {
        exit(EXIT_FAILURE);
}

TEST(NetworkLibTest, TCPClientConnectionPass) {
    hft::networklib::TCPServer server9{"8089", 4};
    std::thread t1(&hft::networklib::TCPServer::acceptConnection, std::ref(server9));
    std::this_thread::sleep_for(std::chrono::seconds(5));
    hft::networklib::TCPClient client1{"192.168.4.106", "8089"};
    //std::this_thread::sleep_for(std::chrono::seconds(5));
    hft::networklib::TCPClient client2{"192.168.4.106", "8089"};
    //std::this_thread::sleep_for(std::chrono::seconds(5));
    hft::networklib::TCPClient client3{"192.168.4.106", "8089"};
    std::this_thread::sleep_for(std::chrono::seconds(5));
    client1.sendMsg("Hello from client1");
    client2.sendMsg("Hello from client2");
    client3.sendMsg("Hello from client3");
    //std::this_thread::sleep_for(std::chrono::seconds(5));
    t1.join();
    //hft::networklib::TCPClient client4{"192.168.4.106", "8089"};
    //hft::networklib::TCPClient client5{"192.168.4.106", "8089"};
    
    

    /*ASSERT_TRUE(server9.isRunning());
    server9.acceptConnection();
    ASSERT_TRUE(client1.isConnectionInitiated());
    ASSERT_TRUE(client2.isConnectionInitiated());
    ASSERT_TRUE(client3.isConnectionInitiated());
    ASSERT_TRUE(client4.isConnectionInitiated());
    ASSERT_TRUE(client5.isConnectionInitiated());*/
    //EXPECT_DEATH(hft::networklib::TCPClient client6("192.168.4.106", "8089"),  "Failed to connect: Connection timed out");
    //EXPECT_EXIT(hft::networklib::TCPClient client7{"192.168.4.106", "8089"}, testing::ExitedWithCode(EXIT_FAILURE),"Failed to connect: Connection timed out");
    

}


int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}