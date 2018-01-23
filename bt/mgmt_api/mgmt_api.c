#include <mgmt_api.h>
#include <sys/socket.h>

mgmt_api_ctx *mgmt_api_connect()
{
	mgmt_api_ctx *result = NULL;

	int s = socket(AF_BLUETOOTH, SOCK_RAW | SOCK_CLOEXEC | SOCK_NONBLOCK,
			 BTPROTO_HCI);
	if (s >= 0)
	{
		s.bind();
	}

	return result;
}

void mgmt_api_power(mgmt_api_ctx*, bool);

void mgmt_api_set_name(mgmt_api_ctx*, const char*);

void mgmt_api_set_class(mgmt_api_ctx*, uint8_t major, uint8_t minor);

void mgmt_api_set_link_key(mgmt_api_ctx*, bdaddr_t bdaddr, uint8_t val[16]);
