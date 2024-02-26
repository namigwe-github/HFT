#include <thread>
#include <utility>
#include <iostream>
//#include <pthread.h>
#include <atomic>

constexpr size_t NUMTHREADS = 2;


namespace hft{
    namespace threadlib {
        inline auto setThreadCore(const int& coreid) {
            cpu_set_t cpuset;
            CPU_ZERO(&cpuset);
            for(size_t i = 0; i <= coreid; i++) {
                CPU_SET(i,&cpuset);
            }
            //CPU_SET(coreid,&cpuset);
            return pthread_setaffinity_np(pthread_self(),sizeof cpuset, &cpuset);
        }

        template<typename Func, typename ...Args>
        inline bool createThread(const int& core,Func && func, Args &&... args) noexcept {
            std::atomic<bool> threadExecuted = false;
            std::thread t1;
            if (core >= 0 && core < NUMTHREADS && !setThreadCore(core)) {
                t1 = std::thread{std::forward<Func>(func),std::forward<Args>(args)...};
                if (t1.joinable()) { 
                    std::cout << "Thread created\n";
                    t1.join();
                    threadExecuted = true;
                    std::cout << "Thread destroyed\n";
                    return threadExecuted;
                } else {
                    t1.join();
                    std::cerr << "Thread creation failed, thread destroyed \n";
                    exit(EXIT_FAILURE);
                }
                
            }

            return threadExecuted;
        }
    }
}

