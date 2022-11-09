#pragma once

typedef enum
{
	max_recv_buffer_size = 1024 * 8,
}net_enum;

typedef enum
{
	parse_ok = 0,
	parse_wait = 1,
	parse_close = 2,
	parse_error = 3,
}parser_code;

typedef enum
{
	tcp = 0,
	udp = 1,
	http = 2,
}socket_type;

typedef enum 
{
	none = 0,
	connected = 1,
	closeed = 2,
}msg_base;
