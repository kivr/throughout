#include <unistd.h>
#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/l2cap.h>
#include "bt_utils.h"

int bt_hid_init()
{
	int control = bt_wait_for_connection_on_psm(17);
	int interrupt = bt_wait_for_connection_on_psm(19);

	return interrupt;
}

int bt_hid_get_report(int socket, uint8_t *report, size_t size)
{
	return read(socket, report, size);
}

int main(int argc, char *argv[])
{
	int socket = bt_hid_init();

	printf("%d\n", socket);

	return 0;
}
