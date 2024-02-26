#include <fstream>
#include <filesystem>
//#include <chrono>
#include <ctime>

namespace hft {
    class Logger {
    public:
        Logger(const std::string& filename){   
            if (!std::filesystem::exists("./logs")){
                std::filesystem::create_directory("./logs");
                std::cout << "Log directory created" << std::endl;
            }
            std::string file = "./logs/" + filename + "_" + get_time() + ".log";
            m_file.open(file);
        }

        ~Logger(){
            m_file.close();
        }

        void log(const char* message){
            m_file << message << std::endl;
        }

        void log(const std::string& message){
            m_file << message << std::endl;
        }

        template<typename T, typename ... Args>
        void log(const char* format, T value, Args... Fargs){
            for (; *format != '\0'; format++)
            {
                if (*format == '%')
                {
                    m_file << value;
                    log(format + 1, Fargs...); // recursive call
                    return;
                }
                m_file << *format;
            }
        }
    private:
        std::ofstream m_file;

        std::string get_time(){
            std::time_t time = std::time({});
            char mbstr[20];
            if (std::strftime(mbstr, sizeof(mbstr), "%F_%T", std::localtime(&time)))
                return std::string(mbstr);
        }    

    };
}