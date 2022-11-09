#include <redis/redis_command.h>
namespace engine::redis
{
	redis_command::redis_command()
	{
	}

	redis_command::~redis_command()
	{
	}

	void redis_command::push_int8(int8_t value)
	{
		push(value);
	}

	void redis_command::push_uint8(uint8_t value)
	{
		push(value);
	}

	void redis_command::push_int16(int16_t value)
	{
		push(value);
	}

	void redis_command::push_uint16(uint16_t value)
	{
		push(value);
	}

	void redis_command::push_int32(int32_t value)
	{
		push(value);
	}

	void redis_command::push_uint32(uint32_t value)
	{
		push(value);
	}

	void redis_command::push_int64(int64_t value)
	{
		push(value);
	}

	void redis_command::push_uint64(uint64_t value)
	{
		push(value);
	}

	void redis_command::push_float(float value)
	{
		push(value);
	}

	void redis_command::push_double(double value)
	{
		push(value);
	}

	void redis_command::push_string(const std::string value)
	{
		append(value.c_str(), value.size());
	}

	void redis_command::push_string(const char* format, ...)
	{
		char buff[512];
		va_list argList;
		va_start(argList, format);
		vsnprintf(buff, 512, format, argList);
		va_end(argList);
		append(buff, strlen(buff));
	}

	void redis_command::push_blob(const char* p, size_t len)
	{
		append(p, len);
	}

	void redis_command::append(const char* p, size_t len)
	{
		std::vector<char> vec;
		vec.insert(vec.begin(), p, p + len);

		params_.push_back(vec);
		lens_.push_back(len);
	}

	size_t redis_command::length()
	{
		return lens_.size();
	}
	const char** redis_command::argv(std::vector<char*>& vec)
	{
		for (size_t i = 0; i < params_.size(); ++i)
		{
			vec.push_back((char*)(params_[i].data()));
		}

		return (const char**)(vec.data());
	}
	const size_t* redis_command::argv_len()
	{
		return lens_.data();
	}

	const char* redis_command::to_string()
	{
		params_[0].push_back(0);
		const char* ret = (const char*)(params_[0].data());
		return ret;
	}
}


