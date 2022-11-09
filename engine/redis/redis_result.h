#pragma once
#include <memory>
#include <hiredis/hiredis.h>
#include <utils/platform.hpp>
namespace engine::redis
{
	class EngineAPI redis_result :std::enable_shared_from_this<redis_result>
	{
	public:
		redis_result();
		~redis_result();
		void set_result(redisReply* redis_reply);
		bool is_empty();
		int32_t get_row_count();
		bool fetch();
		bool get_bool(bool& value);
		bool get_int8(int8_t& value);
		bool get_uint8(uint8_t& value);
		bool get_int32(int32_t& value);
		bool get_uint32(uint32_t& value);
		bool get_int64(int64_t& value);
		bool get_uint64(uint64_t& value);
		bool get_float(float& value);
		bool get_double(double& value);
	private:
		const char* get_data(int& len);
		const char* get_data();
	private:
		redisReply* redis_reply_;
		int32_t pos_;
	};
}
