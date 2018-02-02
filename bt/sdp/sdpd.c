#include <unistd.h>
#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/l2cap.h>
#include <../bt_utils.h>

int bt_sdp_start()
{
	char input[64];
	int socket = bt_wait_for_connection_on_psm(1);

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
