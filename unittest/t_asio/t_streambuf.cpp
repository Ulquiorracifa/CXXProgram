#include <cstdlib>
#include <iostream>
#include <streambuf>
#include <memory>
#include <utility>
#include <thread>
#include <chrono>
#include <time.h> 
#include <string>
#include <string.h>
#include <asio.hpp>
#include <logger/logger.cpp>
#include <spdlog/spdlog.h>


class t_asio_streambuf
{
public:
    virtual ~t_asio_streambuf() {}
    void excute()
    {
        int32_t i32 = std::numeric_limits<int32_t>::max();
        char cs[4];
        std::memcpy(cs, &i32, 4);
    /*    cs[0] = i32;
        cs[1] = i32 >> 8;
        cs[2] = i32 >> 16;
        cs[3] = i32 >> 24;*/

        int32_t ti32;
        std::memcpy(&ti32, cs, 4);
        std::cout << ti32 << std::endl;

        int32_t tt = cs[0] & 0xFF;
        tt |= cs[1] << 8 & 0xFF00;
        tt |= cs[2] << 16 & 0xFF0000;
        tt |= cs[3] << 24 &0xFF000000;
        std::cout << tt << std::endl;

        int32_t header = *reinterpret_cast<int32_t*>(cs);
        int32_t left = header << 8;
        int32_t right = left >> 8;
        std::cout << right << std::endl;
        auto length = header << 8 >> 8;
        std::cout << length << std::endl;
        asio::streambuf buf(512);
        std::ostream o_stream(&buf);

        o_stream << 12345678;
        for (int i = 0;i<512;++i) 
        {
            o_stream << i;
        }
       
        std::istream i_stream(&buf);
        std::string s;
        i_stream >> s;
        std::cout << s << std::endl;
        std::string s2 = "1";
        const char* chs = { "234" };
        s2.append(chs, 2);
        std::cout << s2 << std::endl;
    }
};
int main(int argc, char* argv[])
{
    printf("\n----- start -----\n");
    t_asio_streambuf* sln = new t_asio_streambuf();
    sln->excute();
    delete sln;
    printf("------ end ------\n");
    return 0;
}
