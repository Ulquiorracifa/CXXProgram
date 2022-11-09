#pragma once
#include <stdint.h>
#include <net/decl.h>
#pragma pack(push)
#pragma pack(1)
struct packet_header
{
	uint32_t length;
	uint32_t msgid;
};

#pragma pack(pop)

struct net_packet
{
	net_packet(const session_ptr& p_sesion,uint32_t p_msgid,const buffer_ptr& p_buffer)
		: msgid(p_msgid)
		, buffer(p_buffer)
	{
		session = const_cast<session_ptr&>(p_sesion);
	}
	session_ptr session;
	uint32_t msgid;
	buffer_ptr buffer;
};
