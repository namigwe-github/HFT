#include <vector>


namespace hft{
    namespace memory{

        template <typename T>
        class MemPool{
        public:
            struct MemBlock{
                T data;
                bool isFree = true;
            };

            MemPool(size_t size): m_size(size), freeIndex(0){
                pool = std::vector<MemBlock>(size);
            }
            ~MemPool(){}
            template <typename... Args>
            T* allocate(Args... args){
                T* allocmem = reinterpret_cast<T*>(&(pool[freeIndex]));
                auto vec = {args...};
                bool temp;
                for(auto v:vec) {
                    temp = pool[freeIndex].isFree;
                    if (temp){
                        pool[freeIndex].data = v;
                        pool[freeIndex].isFree = false;
                        if (freeIndex == (m_size)) break;
                        freeIndex++;
                    } else {
                        while(!freeIndex) 
                    }
                }

                return allocmem;

            }
            void deallocate(T* ptr){
                auto ind = reinterpret_cast<MemBlock*>(ptr) - &(pool[0]);
                std::cout << "Index to deallocate " << ind << "\n";
                pool[ind].isFree = true;
            }

            size_t getSize() {
                return m_size;
            }

            void printPool() {
                if(m_size > 0) {
                    int i=0;
                    std::for_each(pool.begin(), pool.end(), [&i](const MemBlock& block){
                        std::cout<< std::boolalpha << "Block "<< i << " is "<<block.isFree << " val:" << block.data<< "\n";
                        i++;
                    });
                }
            }

            
        private:
            std::vector<MemBlock> pool;
            void* m_pool;
            size_t m_size;
            size_t freeIndex;
        
        };
    }
}