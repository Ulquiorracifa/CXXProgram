#include <redis/redis_connect.h>
#include <redis/redis_command.h>
#include <redis/redis_result.h>
#include <logger/logger.h>
#ifdef OS_PLATFORM_WINDOWS
#include <Windows.h>
#endif
namespace engine::redis
{
	redis_connect::redis_connect(std::string& ip, int32_t port)
		:ip_(ip)
		, port_(port)
		, index_(0)
		, redis_context_(nullptr)
	{

	}

	redis_connect::redis_connect(std::string& ip, int32_t port, int32_t index)
		:ip_(ip)
		, port_(port)
		, index_(index)
		, redis_context_(nullptr)
	{

	}
	redis_connect::~redis_connect()
	{
		close();
	}

	bool redis_connect::connect()
	{
		close();
		struct timeval timeout = { 1, 500000 };
		redisContext* redis_context = redisConnectWithTimeout(ip_.c_str(), port_, timeout);
		if (redis_context->err)
		{
			close();
			Log_Error("dbredisconnect::connect redis connect failed! ip = {}, port = {} ,err = {}", ip_.c_str(), port_, redis_context->errstr);
			return false;
		}
		redis_context_ = redis_context;
		redisReply* redis_reply = (redisReply*)redisCommand(redis_context, "select %d", index_);
		if (redis_reply == nullptr)
		{
			close();
			Log_Error("dbredisconnect::connect:  ip = {}, port = {}, errno = {}, error ={}", ip_.c_str(), port_, redis_context_->err, redis_context_->errstr);
			return false;
		}
		if (redis_reply->type == REDIS_REPLY_ERROR)
		{
			close();
			Log_Error("dbredisconnect::connect redis select index failed! ip = {}, port = {} ,err = {}", ip_.c_str(), port_, redis_reply->str);
			freeReplyObject(redis_reply);
			return false;
		}
		freeReplyObject(redis_reply);
		return true;
	}

	bool redis_connect::is_connect()
	{
		return redis_context_ != nullptr;
	}

	bool redis_connect::close()
	{
		if (redis_context_)
		{
			redisFree(redis_context_);
			redis_context_ = nullptr;
		}
		return true;
	}

	int32_t redis_connect::get_err()
	{
		if (redis_context_)
		{
			return redis_context_->err;
		}
		return 1;
	}

	const char* redis_connect::get_err_str()
	{
		if (!redis_context_)
			return "redis_context_ is null";
		return redis_context_->errstr;
	}

	bool redis_connect::ping()
	{
		if (!redis_context_)
			return false;

		redisReply* redis_reply = (redisReply*)redisCommand(redis_context_, "ping");

		if (!redis_reply)
		{
			Log_Error("dbredisconnect::ping: errno={}, error={}", redis_context_->err, redis_context_->errstr);
			return false;
		}
		if (!(redis_reply->type == REDIS_REPLY_STATUS && strcmp(redis_reply->str, "pong") == 0))
		{
			Log_Error("dbredisconnect::ping: errno={}, error={}", redis_context_->err, redis_reply->str);
			freeReplyObject(redis_reply);
			return false;
		}
		freeReplyObject(redis_reply);
		return true;
	}

	bool redis_connect::excute(std::shared_ptr<redis_command> command)
	{
		if (!redis_context_ || !command)
			return false;
		std::vector<char*> temp;
		redisReply* redis_reply = nullptr;

		if (command->length() > 1)
		{
			redis_reply = (redisReply*)redisCommandArgv(redis_context_, command->length(), command->argv(temp), command->argv_len());
		}
		else
		{
			redis_reply = (redisReply*)redisCommand(redis_context_, command->to_string());
		}

		if (redis_context_->err)
		{
			close();
			Log_Error("dbredisconnect::execute: errno={}, error={}", redis_context_->err, redis_context_->errstr);
			return false;
		}
		if (!redis_reply)
		{
			return false;
		}
		//if (!(redis_reply->str || redis_reply->elements > 0))
		//{
		//	freereplyobject(redis_reply);
		//	return false;
		//}
		if (redis_reply->type == REDIS_REPLY_NIL || redis_reply->type == REDIS_REPLY_ERROR)
		{
			freeReplyObject(redis_reply);
			return false;
		}
		freeReplyObject(redis_reply);
		return true;
	}

	bool redis_connect::excute(const char* format, ...)
	{
		if (!redis_context_)
			return false;
		va_list arglist;
		va_start(arglist, format);
		redisReply* redis_reply = (redisReply*)redisCommand(redis_context_, format, arglist);
		va_end(arglist);
		if (redis_context_->err)
		{
			close();
			Log_Error("dbredisconnect::execute: errno={}, error={}", redis_context_->err, redis_context_->errstr);
			return false;
		}
		if (!redis_reply)
		{
			return false;
		}
		//if (!(redis_reply->str || redis_reply->elements > 0))
		//{
		//	freereplyobject(redis_reply);
		//	return false;
		//}
		if (redis_reply->type == REDIS_REPLY_NIL || redis_reply->type == REDIS_REPLY_ERROR)
		{
			freeReplyObject(redis_reply);
			return false;
		}
		freeReplyObject(redis_reply);
		return true;
	}

	bool redis_connect::query(std::shared_ptr<redis_command> command, std::shared_ptr<redis_result> result)
	{
		if (!redis_context_ || !result || !command)
			return false;
		std::vector<char*> temp;
		redisReply* redis_reply = nullptr;

		if (command->length() > 1)
		{
			redis_reply = (redisReply*)redisCommandArgv(redis_context_, command->length(), command->argv(temp), command->argv_len());
		}
		else
		{
			redis_reply = (redisReply*)redisCommand(redis_context_, command->to_string());
		}

		if (redis_context_->err)
		{
			close();
			Log_Error("dbredisconnect::execute: errno={}, error={}", redis_context_->err, redis_context_->errstr);
			return false;
		}
		result->set_result(redis_reply);
		return true;
	}

	bool redis_connect::query(std::shared_ptr<redis_result> result, const char* format,  ...)
	{
		if (!redis_context_ || !result)
			return false;
		va_list arglist;
		va_start(arglist, format);
		redisReply* redis_reply = (redisReply*)redisCommand(redis_context_, format, arglist);
		va_end(arglist);
		if (redis_context_->err)
		{
			close();
			Log_Error("dbredisconnect::execute: errno={}, error={}", redis_context_->err, redis_context_->errstr);
			return false;
		}
		result->set_result(redis_reply);
		return true;
	}
}

