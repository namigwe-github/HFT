#include <queue>
#include <atomic>
#include <thread>


//Single producer single consumer lock free queue
namespace hft {
    namespace datastructures {
        template <typename T>
        class LFQueue {
            public:
                LFQueue(){}

                void push(const T& value) noexcept {
                    while(lock.test_and_set(std::memory_order_acquire)){ std::this_thread::yield();}
                    queue.push(value);
                    /*while(1){std::cout << "blocked by" << value << "\n";
                    std::this_thread::sleep_for(std::chrono::milliseconds(100));}*/
                    lock.clear(std::memory_order_release);
                }

                T pop() {
                    while(empty() || lock.test_and_set(std::memory_order_acquire)){ std::this_thread::yield();}
                    T value = queue.front();
                    queue.pop();
                    //std::cout << "popped\n ";
                    lock.clear(std::memory_order_release);
                    return value;
                }

                /*T normalpop() {
                    while(empty()){}
                    T value = queue.front();
                    queue.pop();
                    //std::cout << "popped\n ";
                    return value;
                }

                void normalpush(const T& value) noexcept {
                    queue.push(value);
                    while(1){}
                }

                T front() {
                    return queue.front();
                }*/

                bool empty() {
                    return queue.empty();
                }

                size_t size() {
                    return queue.size();
                }

            private:
                std::queue<T> queue{};
                std::atomic_flag lock = ATOMIC_FLAG_INIT;
        };
    }
}