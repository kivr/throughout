int bt_wait_for_connection_on_psm(int psm)
{
	struct sockaddr_l2 addr;
	int s, client;

	memset(&addr, 0, sizeof(addr));

	s = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_L2CAP);

	addr.l2_family = AF_BLUETOOTH;
	addr.l2_psm = psm;
	addr.l2_bdaddr = *BDADDR_ANY;

	bind(s, (struct sockaddr*)&addr, sizeof(addr));
	listen(s, 1);
	
	client = accept(s, NULL, NULL);

	return client;
}
