#pragma once
#include <memory>
#include <string>
#include <hiredis/hiredis.h>
#include <utils/platform.hpp>
namespace engine::redis
{
	class redis_command;
	class redis_result;
	class EngineAPI redis_connect :std::enable_shared_from_this<redis_connect>
	{
	public:
		redis_connect(std::string& ip, int32_t port);
		redis_connect(std::string& ip, int32_t port, int32_t index);
		~redis_connect();

		bool connect();
		bool is_connect();
		bool close();
		int32_t get_err();
		const char* get_err_str();
		bool ping();
		bool excute(std::shared_ptr<redis_command> command);
		bool excute(const char* format, ...);
		bool query(std::shared_ptr<redis_command> command, std::shared_ptr<redis_result> result);
		bool query(std::shared_ptr<redis_result> result, const char* format,  ...);
	private:
		std::string ip_;
		int32_t port_;
		int32_t index_;
		redisContext* redis_context_;
	};
}
