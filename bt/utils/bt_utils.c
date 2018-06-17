#include <sys/socket.h>
#include <unistd.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/l2cap.h>

int bt_bind_on_psm(int psm)
{
	struct sockaddr_l2 addr;
    int enable = 1;
    int serverSocket;

	memset(&addr, 0, sizeof(addr));

    serverSocket = socket(PF_BLUETOOTH, SOCK_STREAM, BTPROTO_L2CAP);

    addr.l2_family = AF_BLUETOOTH;
    addr.l2_psm = psm;
    addr.l2_bdaddr = *BDADDR_ANY;

    bind(serverSocket, (struct sockaddr*)&addr, sizeof(addr));
    listen(serverSocket, 1);

    return serverSocket;
}

int bt_wait_for_connection_on_psm(int *serverSocket, int psm)
{
    int client;

    if (*serverSocket < 0)
    {
        *serverSocket = bt_bind_on_psm(psm);
    }

	client = accept(*serverSocket, NULL, NULL);

    return client;
}

int bt_connect_to_psm(const char *s_address, int psm)
{
    struct sockaddr_l2 address;
    int s, status;

    memset(&address, 0, sizeof(address));

    s = socket(PF_BLUETOOTH, SOCK_STREAM, BTPROTO_L2CAP);

    address.l2_family = AF_BLUETOOTH;
    address.l2_psm = psm;
    str2ba(s_address, &address.l2_bdaddr);

    status = connect(s, (struct sockaddr*)&address, sizeof(address));

    if (status != 0)
    {
        close(s);
        s = -1;
    }

    return s;
}
