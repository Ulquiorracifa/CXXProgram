#include <redis/redis_result.h>
#include <cstdlib>
namespace engine::redis
{
	redis_result::redis_result()
		:redis_reply_(nullptr)
		, pos_(0)
	{

	}

	redis_result::~redis_result()
	{
		if (redis_reply_)
			freeReplyObject(redis_reply_);
	}

	void redis_result::set_result(redisReply* redis_reply)
	{
		redis_reply_ = redis_reply;
	}

	bool redis_result::is_empty()
	{
		if (!redis_reply_)
			return true;
		if (redis_reply_->type == REDIS_REPLY_NIL || redis_reply_->type == REDIS_REPLY_ERROR)
			return true;
		//if (!(redis_reply_->str || redis_reply_->elements > 0))
		//	return true;
		return false;
	}

	int32_t redis_result::get_row_count()
	{
		return redis_reply_->str ? 1 : redis_reply_->elements;
	}

	bool redis_result::fetch()
	{
		return pos_ < get_row_count();
	}

	bool redis_result::get_bool(bool& value)
	{
		if (!fetch())
			return false;
		value = (bool)std::atoi(get_data());
		return true;
	}

	bool redis_result::get_int8(int8_t& value)
	{
		if (!fetch())
			return false;
		value = (int8_t)std::atol(get_data());
		return true;
	}

	bool redis_result::get_uint8(uint8_t& value)
	{
		if (!fetch())
			return false;
		value = (uint8_t)std::atol(get_data());
		return true;
	}

	bool redis_result::get_int32(int32_t& value)
	{
		if (!fetch())
			return false;
		value = (int32_t)std::atol(get_data());
		return true;
	}

	bool redis_result::get_uint32(uint32_t& value)
	{
		if (!fetch())
			return false;
		value = (uint32_t)std::atol(get_data());
		return true;
	}

	bool redis_result::get_int64(int64_t& value)
	{
		if (!fetch())
			return false;
		value = (int64_t)std::atoll(get_data());
		return true;
	}

	bool redis_result::get_uint64(uint64_t& value)
	{
		if (!fetch())
			return false;
		value = (uint64_t)std::atoll(get_data());
		return true;
	}

	bool redis_result::get_float(float& value)
	{
		if (!fetch())
			return false;
		value = (float)std::atof(get_data());
		return true;
	}

	bool redis_result::get_double(double& value)
	{
		if (!fetch())
			return false;
		value = (double)std::atof(get_data());
		return true;
	}

	const char* redis_result::get_data(int& len)
	{
		if (pos_ >= get_row_count())
		{
			return nullptr;
		}
		const char* str;
		// key or list
		if (redis_reply_->str)
		{
			len = redis_reply_->len;
			str = redis_reply_->str;
		}
		else
		{
			len = redis_reply_->element[pos_]->len;
			str = redis_reply_->element[pos_]->str;
		}
		pos_++;
		return str;
	}

	const char* redis_result::get_data()
	{
		if (pos_ >= get_row_count())
		{
			return nullptr;
		}
		const char* str;
		if (redis_reply_->str)
		{
			str = redis_reply_->str;
		}
		else
		{
			str = redis_reply_->element[pos_]->str;
		}
		pos_++;
		return str;
	}
}

