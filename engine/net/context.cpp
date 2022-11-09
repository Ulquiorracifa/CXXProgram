#include <net/context.h>
context::context()
{
	thread_ = std::thread([this]() {
		auto work = asio::make_work_guard(io_context_);
		io_context_.run();
		});
}

context::~context()
{
	stop();
}

void context::stop()
{
	if (thread_.joinable())
	{
		io_context_.stop();
		thread_.join();
	}
}

asio::io_context& context::get()
{
	return io_context_;
}
