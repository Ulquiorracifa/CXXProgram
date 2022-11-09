#include <net/context_pool.h>
#include <net/context.h>
context_pool::context_pool(size_t thread_count/* = std::thread::hardware_concurrency() * 2 + 1*/)
	:thread_count_(0),
	thread_context_index_(0)
{
	size_t cpu_core_num = static_cast<size_t>(std::thread::hardware_concurrency() * 2 + 1);
	if (thread_count <= 0 || thread_count > cpu_core_num)
	{
		thread_count = cpu_core_num;
	}

	for (size_t i = 0; i < thread_count; ++i)
	{
		auto ctx = std::make_shared<context>();
		contexts_.emplace_back(ctx);
	}
	thread_count_ = contexts_.size();
}

context_pool::~context_pool()
{
	stop();
}

context_ptr context_pool::next()
{
	size_t index = (++thread_context_index_) % thread_count_;
	if (thread_context_index_ >= std::numeric_limits<int64_t>::max())
	{
		thread_context_index_ = 0;
	}
	return contexts_.at(index);
}

context_ptr context_pool::get(size_t index)
{
	if (index < 0 || index >= contexts_.size())
	{
		return nullptr;
	}
	return contexts_.at(index);
}

void context_pool::stop()
{
	for (auto& iter : contexts_) {
		iter->stop();
	}
	contexts_.clear();
}
