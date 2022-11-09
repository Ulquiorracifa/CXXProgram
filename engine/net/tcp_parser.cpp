#include <net/tcp_parser.h>
#include <net/header.h>
#include <net/buffer_pool.h>

parser_code tcp_parser::recv_and_decode(const session_ptr& session, const data_handler_ptr& data_handler, char* data, size_t length)
{
	buffer_stream_.append(data, length);
	while (buffer_stream_.readable_length() >= sizeof(uint32_t))
	{
		uint32_t total_len = *reinterpret_cast<uint32_t*>(buffer_stream_.read_addr());
		if (buffer_stream_.readable_length() < total_len)
		{
			return parser_code::parse_ok;
		}
		auto buffer = buffer_pool_manager::instance().alloc(total_len);
		if (!buffer)
		{
			return parser_code::parse_error;
		}
		buffer->set_header_length(sizeof(packet_header));
		buffer->copy_from(buffer_stream_.read_addr(), total_len);
		buffer_stream_.offset_read(total_len);
		buffer_stream_.retrieve();
		if (data_handler)
		{
			data_handler->on_message(session, buffer);
		}
	}
	return parser_code::parse_ok;
} 

/**********************************************************************************************************/


parser_code tcp_string_parse::recv_and_decode(const session_ptr& session, const data_handler_ptr& data_handler, char* data, size_t length)
{
	return parser_code::parse_ok;
}
