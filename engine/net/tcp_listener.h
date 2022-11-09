#pragma once
#include <string>
#include <asio.hpp>
#include <asio/ip/tcp.hpp>
#include <logger/logger.h>
#include <utils/noncopyable.hpp>
#include <utils/platform.hpp>
#include <utils/snowflake.hpp>
#include <net/decl.h>
#include <net/tcp_session.h>
#include <net/context_pool.h>
#include <net/context.h>

template<class this_parser>
class EngineAPI tcp_listener : public basic_listener
{
    //static_assert(std::is_convertible_v<this_parser*, basic_parser*>, "this_parser must be inherit basic_parser!");
public:
    explicit tcp_listener(context_pool_ptr context_pool, snowflake_ptr_mt id_generate, data_handler_ptr data_handler);
    ~tcp_listener();
    bool listen(const std::string& ip, int port);
protected:
    void close();
    bool is_listening();
    void do_accept();
    void accept_handler(session_ptr session, const std::error_code& ec);
private:
    asio_tcp_acceptor_ptr acceptor_;
    context_pool_ptr context_pool_;
    context_ptr accept_context_;
    snowflake_ptr_mt  id_generate_;
    data_handler_ptr data_handler_;
};

template<class this_parser>
tcp_listener<this_parser>::tcp_listener(context_pool_ptr context_pool, snowflake_ptr_mt id_generate, data_handler_ptr data_handler)
    :context_pool_(context_pool)
    , id_generate_(id_generate)
    , data_handler_(data_handler)
{
    accept_context_ = context_pool_->get(0);
    acceptor_ = std::make_shared<asio::ip::tcp::acceptor>(accept_context_->get());
}

template<class this_parser>
tcp_listener<this_parser>::~tcp_listener()
{
    close();
}

template<class this_parser>
bool tcp_listener<this_parser>::listen(const std::string& ip, int port)
{
    if (is_listening())
    {
        Log_Warn("tcp_server::listener server is listening");
        return true;
    }
    std::error_code error_code;
    asio::ip::tcp::resolver resolver(accept_context_->get());
    auto endpoint = resolver.resolve(ip, std::to_string(port), error_code);
    if (error_code)
    {
        close();
        Log_Error("tcp_server::listener resolver error_code : {} , message : {}", error_code.value(), error_code.message());
        return false;
    }
    auto end_point = endpoint.begin()->endpoint();
    acceptor_->open(end_point.protocol(), error_code);
    if (error_code)
    {
        Log_Error("tcp_server::listener open  error_code : {} , message : {}", error_code.value(), error_code.message());
        return false;
    }
    acceptor_->set_option(asio::ip::tcp::acceptor::reuse_address(true));
    acceptor_->set_option(asio::ip::tcp::no_delay(true));
    acceptor_->non_blocking(true);
    acceptor_->bind(end_point, error_code);
    if (error_code)
    {
        close();
        Log_Error("tcp_server::listener bind  error_code : {} , message : {}", error_code.value(), error_code.message());
        return false;
    }
    acceptor_->listen(128, error_code);
    if (error_code)
    {
        close();
        Log_Error("tcp_server::listener listen  error_code : {} , message : {}", error_code.value(), error_code.message());
        return false;
    }
    Log_Warn("tcp_server::listener server listen success ! ip = {} ,port = {}",ip,port);
    do_accept();
    return true;
}

template<class this_parser>
void tcp_listener<this_parser>::close()
{
    try
    {
        if (acceptor_->is_open())
        {
            acceptor_->close();
        }
    }
    catch (const std::system_error& sys_error)
    {
        const std::error_code& error_code = sys_error.code();
        Log_Error("tcp_server::close error_code : {} , message : {}", error_code.value(), error_code.message());
    }
}

template<class this_parser>
bool tcp_listener<this_parser>::is_listening()
{
    return acceptor_->is_open();
}

template<class this_parser>
void tcp_listener<this_parser>::do_accept()
{
    auto context = context_pool_->next();
    int64_t session_id = id_generate_->next();
    auto session = std::make_shared<tcp_session<this_parser>>(context->get(), session_id, data_handler_);
    acceptor_->async_accept(session->socket(), std::bind(&tcp_listener::accept_handler, this, session, std::placeholders::_1));
}

template<class this_parser>
void tcp_listener<this_parser>::accept_handler(session_ptr session, const std::error_code& ec)
{
    if (!is_listening())
        return;
    if (ec)
    {
        close();
        Log_Error("tcp_server::accept_handler error_code : {} , message : {}", ec.value(), ec.message());
        return;
    }
    session->start();
    Log_Info("tcp_server::accept_handler accept success ! sid = {}", session->sid());
    do_accept();
}
