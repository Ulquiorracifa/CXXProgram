#include <net/server_base.h>
#include <net/tcp_parser.h>
#include <net/tcp_session.h>
#include <net/tcp_listener.h>
#include <net/context_pool.h>
#include <net/context.h>
server_base::server_base(net_options& options_, message_dispatch dispatcher)
	:options_(std::move(options_))
	,dispatcher_(dispatcher)
{
	context_pool_ = std::make_shared<context_pool>(options_.threads);
	id_generate_ = std::make_shared<snowflake_mt>();
}

server_base::~server_base()
{

}

bool server_base::listen(const std::string& ip, int port, socket_type type)
{
	listener_ptr acceptor = nullptr;
	if (type == socket_type::tcp)
	{
		acceptor = std::make_shared<tcp_listener<tcp_parser>>(context_pool_, id_generate_, this->shared_from_this());
	}
	if (!acceptor)
	{
		return false;
	}
	if (!acceptor->listen(ip, port))
	{
		//error
		return false;
	}
	acceptors_.push_back(acceptor);
	return true;
}

int64_t server_base::connect(const std::string& ip, int port, socket_type type)
{
	session_ptr session = nullptr;
	if (type == socket_type::tcp)
	{
		auto data_handler = this->shared_from_this();
		auto context = context_pool_->next();
		int64_t session_id = id_generate_->next();
		session = std::make_shared<tcp_session<tcp_parser>>(context->get(), session_id, this->shared_from_this());
	}
	if (!session)
	{
		return 0;
	}
	if (!session->connect(ip, port))
	{
		return 0;
	}
	return session->sid();
}

void server_base::update(uint64_t tick)
{
	net_packet_ptr net_pkg;
	while (msg_queue_.try_dequeue(net_pkg))
	{
		if (dispatcher_)
		{
			dispatcher_(net_pkg);
		}
	}
}

void server_base::send(const session_ptr& session, uint32_t msgid, const char* data, size_t data_len)
{
	size_t header_length = sizeof(packet_header);
	size_t total_length = header_length + data_len;
	auto buffer = buffer_pool_manager::instance().alloc(total_length);
	buffer->set_total_length(total_length);
	buffer->set_header_length(header_length);
	packet_header* header = reinterpret_cast<packet_header*>(buffer->total_buffer());
	header->length = total_length;
	header->msgid = msgid;
	std::memcpy(buffer->body_buffer(), data, data_len);
	session->send(buffer->total_buffer(), buffer->total_length());
	//auto session = get_session(sid);
	//if (session)
	//{
	//	session->send(buffer->total_buffer(), buffer->total_length());
	//	//session->send(buffer);
	//}
}

void server_base::on_connect(const session_ptr& session)
{
	spin_lock_.lock();
	sessions_[session->sid()] = session;
	spin_lock_.unlock();
	msg_queue_.enqueue(std::make_shared<net_packet>(session, msg_base::connected, nullptr));
}

void server_base::on_close(const session_ptr& session)
{
	spin_lock_.lock();
	sessions_.erase(session->sid());
	spin_lock_.unlock();
	msg_queue_.enqueue(std::make_shared<net_packet>(session, msg_base::closeed, nullptr));
}

void server_base::on_message(const session_ptr& session, const buffer_ptr& buffer)
{
	packet_header* header = reinterpret_cast<packet_header*>(buffer->total_buffer());
	msg_queue_.enqueue(std::make_shared<net_packet>(session, header->msgid, buffer));
}

session_ptr server_base::get_session(int64_t sid)
{
	session_ptr session = nullptr;
	std::unique_lock < spin_lock> guard(spin_lock_);
	auto iter = sessions_.find(sid);
	if (iter != sessions_.end())
	{
		session = iter->second;
	}
	guard.unlock();
	return session;
}
