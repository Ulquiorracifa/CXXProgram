
#pragma once 
#include <string> 
#include <vector>
#include <unordered_map>
#include <sstream>
#include <type_traits> 
#include <algorithm> 


namespace engine::net::utils {



    /*
 
    tcp://127.0.0.1:3333?key=value&

    uri       = protocol "://" host [":" port] [ "?" param *( "&" param ) ]
    protocol  = tcp/udp/ws/wss/kcp
    param     = key "=" value
    key       = *( "[^=]" )
    value     = *( "[^&:]" )
    */

    /*
    options: 
        bind_addr
        bind_port 
        multi  //multi address 
        rbuf_size
        sbuf_size 
        encryption
        auth_token
        async
        backlogs 
        delay
        threads 
 
    */

        template<class T> 
        struct type_caster
        {
            T operator()(const std::string & val) 
            {
                T rst; 
                std::stringstream ss ( val ); 
                ss  >> rst; 
                char c; 
                //https://stackoverflow.com/questions/194465/how-to-parse-a-string-to-an-int-in-c 
                if (ss.fail() || ss.get(c)) 
                {
                    return T(); 
                }
                return rst; 
            }
        };

        template<> 
        struct type_caster<const char *>
        {
            inline const char *  operator()(const std::string &val )
            {
                return val.c_str(); 
            }
        };


        template<> 
        struct type_caster<bool>
        {
            bool operator()(const std::string &val )
            {
                std::string str = val ;
                std::transform(str.begin(), str.end(),str.begin(), [](unsigned char c)
                    { 
                        return std::tolower(c);
                    });
                if (val == "false" || val == "0" || val == "no")
                {
                    return false; 
                }
                return true; 
            }
        };

        template<> 
        struct type_caster<float>
        {
            inline float operator()(const std::string &val ) 
            {
                return std::stof(val); 
            }
        };

        template<> 
        struct type_caster<double>
        {
            inline double operator()(const std::string &val ) 
            {
                return std::stod(val); 
            }
        };

        template<> 
        struct type_caster<long double>
        {
            inline long double operator()(const std::string &val ) 
            {
                return std::stold(val); 
            }
        };

        template<> 
        struct type_caster<std::string>
        {
            inline std::string operator()(const std::string &val )
            {
                return val; 
            }
        };


    class net_url
    {
    public:
        net_url(const std::string & url = "")
        {
            parse(url); 
        }
    
        net_url(const std::string & proto ,const  std::string & host , uint16_t port = 0)
        {
            protocol_ = proto; 
            host_ = host; 
            port_ = port; 
        }

        bool parse(const std::string &channel)
        { 
            reset(); 
            auto pos = channel.find("://");
            if (pos != std::string::npos){
                protocol_ = channel.substr(0, pos);
            }else {
                return false; 
            }
            pos += 2; //skip "//" after ":"
        
            auto p_pos = channel.find('?',pos); 
            if (p_pos == std::string::npos)
            {
                p_pos = channel.size(); 
            }
            std::string host_info = std::string (channel.begin() + pos +1 , channel.begin() + p_pos); 
            auto c_pos = host_info.find(":"); 
            if (c_pos != std::string::npos)
            {
                host_ = host_info.substr(0,c_pos); 
                auto p = host_info.substr(c_pos +1); 
                if (!p.empty())
                {
                    port_ = (uint16_t)std::stoi( p); 
                }
            
            }else {
                host_ = host_info;
            }

            bool is_key = true;
            std::string key ;
            std::string value;
            for(uint32_t i = p_pos +1;i <  channel.length();++i) 
            {
                if ( channel[i] == '=')
                {
                    is_key = !is_key;
                }else if (channel[i] == '&'  )
                {
                    params_[key] = value;
                    key.clear();
                    value.clear();
                    is_key = !is_key;
                }else
                {
                    if(is_key)
                    {
                        key.append(1,channel[i]);
                    } else 
                    {
                        value.append(1,channel[i]);
                    }
                }
            }
            if (!key.empty())
            {
                params_[key] = value;
            }
            return true;
        }

        std::string encode(){
            std::string url = protocol_ + "://"  + host_ + (port_ > 0 ?":"+std::to_string(port_):""); 
            if (!params_.empty())
            {
                url  += "?"; 
                for(const auto & param : params_)
                {
                    url += (param.first +"=" +param.second) +"&"; 
                }
            }
            return url; 
        }

        inline bool has(const std::string & key) const 
        {
            return params_.find(key) != params_.end(); 
        }
 
        void set(const std::string & key, const std::string & val)
        {
            params_[key] = val; 
        }

        std::string get(const std::string& key, const std::string & dft = "") const 
        {
            auto itr = params_.find(key); 
            if (itr!= params_.end())
            {
                return itr->second; 
            }
            return dft; 
        }
        template <class T> 
        T get(const std::string & key, T dft = T()) const
        {
            auto itr = params_.find(key); 
            if (itr!= params_.end())
            {
                return type_caster<T>{}(itr->second);
            }
            return dft; 
        }

        std::string host() const
        {
            return host_; 
        }

        uint16_t port() const 
        {
            return port_; 
        }

        std::string dump() const
        {
            std::string dumpStr = "protocol : " + protocol_ + "\n";
                        dumpStr += "host : " + host_ + "\n";       
                        dumpStr += "port : " + std::to_string(port_) + "\n";       
            for(auto & param : params_)
            {
                dumpStr +=  param.first + " : " + param.second +"\n";
            }
            return dumpStr; 
        }
        void reset()
        {
            params_.clear();
            protocol_.clear();
            host_.clear();
            port_ = 0;
        }

        private: 
            std::string protocol_;
            std::string host_;
            uint16_t    port_ = 0;
            std::unordered_map<std::string, std::string> params_; 
    };


} 
