#pragma once
#include <string>
#include <unordered_map>
#include <queue>
#include <concurrentqueue/concurrentqueue.h>
#include <utils/snowflake.hpp>
#include <utils/spin_lock.hpp>
#include <utils/platform.hpp>
#include <net/decl.h>
#include <net/config.h>
#include <net/header.h>
#include <net/buffer_pool.h>

struct net_options
{
	bool tcp_delay{ true };
	bool sync{ false };
	bool reuse{ true };
	uint32_t backlogs = 512;								// listening queue size
	size_t threads = 1; // std::thread::hardware_concurrency()  ; // iocp/epoll worker threads number
	uint32_t send_buffer_size = 16 * 1024;
	uint32_t recv_buffer_size = 14 * 1024;
	uint32_t recv_size = 8 * 1024;
};

class EngineAPI server_base : public basic_data_handler
{
public:
	server_base(net_options& options_, message_dispatch dispatcher);
	~server_base();
	bool listen(const std::string& ip, int port, socket_type type);
	int64_t connect(const std::string& ip, int port, socket_type type);
	void update(uint64_t tick);
	void send(const session_ptr& session, uint32_t msgid, const char* data, size_t data_len);

	virtual void on_connect(const session_ptr& session);
	virtual void on_close(const session_ptr& session);
	virtual void on_message(const session_ptr& session, const buffer_ptr& buffer);
private:
	session_ptr get_session(int64_t sid);
private:
	net_options options_;
	context_pool_ptr context_pool_;
	snowflake_ptr_mt id_generate_;
	std::vector<listener_ptr> acceptors_;
	std::unordered_map<int64_t, session_ptr> sessions_;
	spin_lock spin_lock_;
	moodycamel::ConcurrentQueue<net_packet_ptr> msg_queue_;
	message_dispatch dispatcher_;
};
