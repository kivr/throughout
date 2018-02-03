#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/l2cap.h>

int bt_wait_for_connection_on_psm(int *serverSocket, int psm)
{
	struct sockaddr_l2 addr;
	int client;
    int enable = 1;

	memset(&addr, 0, sizeof(addr));

    if (*serverSocket < 0)
    {
        *serverSocket = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_L2CAP);

        addr.l2_family = AF_BLUETOOTH;
        addr.l2_psm = psm;
        addr.l2_bdaddr = *BDADDR_ANY;

        bind(*serverSocket, (struct sockaddr*)&addr, sizeof(addr));
        listen(*serverSocket, 1);
    }
	
	client = accept(*serverSocket, NULL, NULL);

	return client;
}
