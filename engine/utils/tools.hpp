#include <sstream>
#include <string.h>
#include <string>
#include <chrono>
#include <cmath>
#include <random>
#ifdef _WIN32
#  include <stdlib.h>
#  include <windows.h>
#  define DIRSEPA '\\'
#else
#  include <unistd.h>
#  define DIRSEPA '/'
#endif
#include <utils/platform.hpp>

namespace engine::utils 
{
    static const int MAXBUFSIZE = 1024;
    inline std::string __hex_dump_one_line(const std::string& prefix, const std::string& line, size_t width)
    {
        static thread_local std::ostringstream ostrm;
        ostrm.clear();
        ostrm.str("");
        ostrm << prefix;
        // hex
        for (size_t i = 0; i < line.size(); i++) 
        {
            char hex_text[16] = "";
            snprintf(hex_text, sizeof(hex_text), "%02X ", (unsigned char)(line[i]));
            ostrm << hex_text;
            if (i == 7 || i == 15 || i == 23) 
            {
                ostrm << ' ';
            }
        }
        // the space between hex and text
        for (size_t spacer = line.size(); spacer < width; spacer++)
            ostrm << "   ";
        if (line.size() <= 8)
        ostrm << "   ";
        else if (line.size() <= 16)
        ostrm << "  ";
        else if (line.size() <= 24)
        ostrm << " ";
        ostrm << ": ";
        // text
        for (size_t i = 0; i < line.size(); i++) 
        {
            if (line[i] < 32)
                ostrm << '.';
            else
                ostrm << char(line[i]);
        }
        ostrm << std::endl;
        return ostrm.str();
    }

    inline std::string to_hex_dump_text(const std::string& _str, size_t width, const std::string& prefix)
    {
        static thread_local std::ostringstream ostrm;
        ostrm.clear();
        ostrm.str("");
        std::string str = _str;
        while (str.size() > width) 
        {
            std::string line(str.c_str(), width);
            ostrm << __hex_dump_one_line(prefix, line, width);
            str = str.substr(width);
        }
        if (str.size() > 0)
        {
            ostrm << __hex_dump_one_line(prefix, str, width);
        }
        return ostrm.str();
    }

    inline std::string get_app_directory()
    {
        std::string full_name;
        char        path[MAXBUFSIZE] = {0};
#ifdef _WIN32
        LPTSTR full_path = (LPTSTR)&path;
        if (::GetModuleFileName(NULL, full_path, MAXBUFSIZE)) 
        {
            full_name.assign(path);
        }
        std::replace(full_name.begin(), full_name.end(), DIRSEPA, '/');
#else
        int count = readlink("/proc/self/exe", path, MAXBUFSIZE);
        if (count > 0 && count < MAXBUFSIZE) 
        {
            path[count] = '\0';
            full_name.assign(path);
        } 
        else 
        {
            char  cmdline[MAXBUFSIZE] = {0};
            FILE* fp                  = nullptr;
            sprintf(cmdline, "ls -lt /proc/%d | grep exe | awk '{print $NF}'", getpid());
            if ((fp = popen(cmdline, "r"))) 
            {
                if (fgets(path, MAXBUFSIZE, fp))
                {
                    size_t len = strlen(path);
                    if (len > 0) 
                    {
                        if ('\n' == path[--len])
                        path[len] = '\0';
                        full_name.assign(path);
                    }
                }
                pclose(fp);
            }
        }
#endif
      return full_name.substr(0, full_name.find_last_of("/"));
    }

    EngineAPI int64_t get_sec_time()
    {
        return std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    }

    EngineAPI int64_t get_mill_time()
    {
        return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    }

    EngineAPI int64_t get_micro_time()
    {
        return std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    }

    EngineAPI int64_t get_nano_time()
    {
        return std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    }

    EngineAPI int64_t get_random_number(int64_t min, int64_t max)
    {
        static thread_local std::random_device rd;
        static thread_local std::mt19937_64    rng(rd());  // Generates random 64-bit integers
        std::uniform_int_distribution<int64_t> dist{ min, max };
        return dist(rng);
    }

    EngineAPI float get_random_float(float min, float max)
    {
        static thread_local std::random_device rd;
        static thread_local std::mt19937_64    rng(rd());
        std::uniform_real_distribution<float>  dist{ min, max };
        return dist(rng);
    }  // namespace utils
}
