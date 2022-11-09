#pragma once
#include <vector>
#include <thread>
#include <asio.hpp>
#include <utils/noncopyable.hpp>
#include <utils/platform.hpp>
class EngineAPI context : public std::enable_shared_from_this<context>,private noncopyable
{
public:
	explicit context();
	~context();
	void stop();
	asio::io_context& get();
private:
	std::thread thread_;
	asio::io_context io_context_;
};
