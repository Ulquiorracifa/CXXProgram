#pragma once
#include <vector>
#include <memory>
#include <asio.hpp>
#include <utils/noncopyable.hpp>
#include <utils/platform.hpp>
#include <net/decl.h>

class EngineAPI context_pool : public std::enable_shared_from_this<context_pool>,private noncopyable
{
public:
	explicit context_pool(size_t thread_count = std::thread::hardware_concurrency() * 2 + 1);
	~context_pool();
	context_ptr next();
	context_ptr get(size_t index);
	void stop();
private:
	std::vector<context_ptr> contexts_;
	size_t thread_count_;
	int64_t thread_context_index_;
};
