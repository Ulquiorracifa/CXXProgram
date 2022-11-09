#pragma once
#include <memory>
#include <utils/platform.hpp>
#include <net/decl.h>
#include <net/config.h>
#include <net/buffer_stream.h>

class EngineAPI tcp_parser :public basic_parser
{
public:
	virtual parser_code recv_and_decode(const session_ptr& session, const data_handler_ptr& data_handler, char* data, size_t length);
private:
	buffer_stream buffer_stream_ = { 2048 };
};


class EngineAPI tcp_string_parse : public basic_parser
{
public:
	virtual parser_code recv_and_decode(const session_ptr& session, const data_handler_ptr& data_handler, char* data, size_t length);
private:
	size_t offset_ = {0};
};
