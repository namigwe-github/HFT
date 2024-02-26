#include <vector>


namespace hft{
    namespace memory{

        template <typename T>
        class MemPool{
        public:
            MemPool(size_t size): m_size(size), freeIndex(0), isFull(false), totalFreeInds(size){
                pool = std::vector<MemBlock>(size);
            }
            ~MemPool(){}
            template <typename... Args>
            T* allocate(Args... args){
                T* allocmem = reinterpret_cast<T*>(&(pool[freeIndex]));
                auto vec = {args...};
                bool temp;
                
                for(auto v:vec) {
                    if (totalFreeInds){
                        pool[freeIndex].data = v;
                        pool[freeIndex].isFree = false;
                        totalFreeInds--;
                        updateFreeIndex();
                    } else {
                        std::cout << "Memory pool full, element not added\n";
                        return nullptr;
                    }
                }
                 
                return allocmem;
            }

            void updateFreeIndex() {
                auto origFreeIndex = freeIndex;
                while(!pool[freeIndex].isFree) {
                    freeIndex++;
                    if (freeIndex == m_size) {
                        freeIndex = 0;
                    }

                    if (freeIndex == origFreeIndex){
                        std::cout << "Memory pool full\n";
                        isFull = true;
                        break;
                    }
                }
            }

            void deallocate(T* ptr){
                auto ind = reinterpret_cast<MemBlock*>(ptr) - &(pool[0]);

                //std::cout << "Index to deallocate " << ind << "\n";
                if (!pool[ind].isFree) {
                    pool[ind].isFree = true;
                    totalFreeInds++;
                    if(isFull) {
                        freeIndex = ind;
                        isFull = false;
                    }
                } else {
                    std::cout << "Memory already deallocated\n";
                }
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

            bool isPoolFull() {
                return isFull;
            }

            size_t getFreeInds() {
                return totalFreeInds;
            }

            T& operator[](size_t index){
                return pool[index].data;
            }
            
        private:
            struct MemBlock{
                T data;
                bool isFree = true;
            };

            std::vector<MemBlock> pool;
            void* m_pool;
            size_t m_size;
            size_t freeIndex;
            bool isFull;
            size_t totalFreeInds;

        };
    }
}