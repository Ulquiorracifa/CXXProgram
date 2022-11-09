#pragma once
#include <vector>
#include <memory>
#include <string>
#include <utils/platform.hpp>
namespace engine::redis
{
	class EngineAPI redis_command :std::enable_shared_from_this<redis_command>
	{
	public:
		redis_command();
		virtual ~redis_command();
		/*-----------------------PUSH START----------------------*/
		void push_int8(int8_t value);
		void push_uint8(uint8_t value);
		void push_int16(int16_t value);
		void push_uint16(uint16_t value);
		void push_int32(int32_t value);
		void push_uint32(uint32_t value);
		void push_int64(int64_t value);
		void push_uint64(uint64_t value);
		void push_float(float value);
		void push_double(double value);
		void push_string(const std::string value);
		void push_string(const char* format, ...);
		void push_blob(const char* p, size_t len);
		template<class T>
		void push_blob(T t)
		{
			const char* data = (char*)&t;
			append(data, sizeof(T));
		}
		/*-----------------------PUSH END----------------------*/

		size_t length();
		const char** argv(std::vector<char*>& vec);
		const size_t* argv_len();
		const char* to_string();
	private:
		void append(const char* p, size_t len);
		template<class T>
		void push(T t)
		{
			std::string data = std::to_string(t);
			append(data.c_str(), data.size());
		}
	private:
		std::vector<std::vector<char>> params_;
		std::vector<size_t> lens_;
	};
}

