#pragma once
#include <functional>
#include <memory>
#include <asio.hpp>
#include <utils/noncopyable.hpp>
#include <net/config.h>
namespace engine::net
{
	
};

using asio_tcp_acceptor_ptr = std::shared_ptr<asio::ip::tcp::acceptor>;
using asio_tcp_socket_ptr = std::shared_ptr<asio::ip::tcp::socket>;

class context;
using context_ptr = std::shared_ptr<context>;
class context_pool;
using context_pool_ptr = std::shared_ptr<context_pool>;

class basic_listener;
using listener_ptr = std::shared_ptr <basic_listener>;

class basic_session;
using session_ptr = std::shared_ptr<basic_session>;

class basic_data_buffer;
using buffer_ptr = std::shared_ptr <basic_data_buffer>;

class basic_data_handler;
using data_handler_ptr = std::shared_ptr <basic_data_handler>;

struct net_packet;
using net_packet_ptr = std::shared_ptr <net_packet>;

using async_connect = std::function<void()>;
using message_dispatch = std::function<bool(const net_packet_ptr&)>;


class basic_listener : public std::enable_shared_from_this<basic_listener>, noncopyable
{
public:
	~basic_listener() {}
	virtual bool listen(const std::string& ip, int port) = 0;
};


class basic_session : public std::enable_shared_from_this<basic_session>, noncopyable
{
public:
	virtual ~basic_session() {}
	virtual int64_t sid() = 0;
	virtual void start() = 0;
	virtual void send(const char* data, size_t len) = 0;
	virtual void send(const buffer_ptr& buffer) = 0;
	virtual bool connect(const std::string& ip, int port) = 0;
	virtual bool async_connect(const std::string& ip, int port) = 0;
};


class basic_data_buffer
{
public:
	virtual ~basic_data_buffer() {}
	virtual char* total_buffer() = 0;
	virtual size_t total_length() = 0;
	virtual void set_total_length(size_t total_length) = 0;
	virtual void set_header_length(size_t header_length) = 0;
	virtual char* body_buffer() = 0;
	virtual size_t body_length() = 0;
	virtual void release() = 0;
	virtual size_t copy_from(basic_data_buffer* src) = 0;
	virtual size_t copy_from(char* src, size_t len) = 0;
	virtual size_t copy_to(char* desc, size_t len) = 0;
};

class basic_data_handler :public std::enable_shared_from_this<basic_data_handler>, noncopyable
{
public:
	virtual void on_connect(const session_ptr&) = 0;
	virtual void on_close(const session_ptr&) = 0;
	virtual void on_message(const session_ptr&, const buffer_ptr&) = 0;
};

class basic_parser : public noncopyable
{
public:
	virtual parser_code recv_and_decode(const session_ptr& session,const data_handler_ptr& data_handler, char* data, size_t length) = 0;
};


