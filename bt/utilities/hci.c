#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/hci_lib.h>
#include <stdio.h>

int main()
{
	int dd;
	char name[100];
	int err;
	bdaddr_t addr;

	dd = hci_open_dev(0);
	err = hci_read_local_name(dd, 100, name, 0);
	hci_close_dev(dd);

	printf("%s\n", name);
	printf("%d\n", err);
	printf("%d\n", dd);

	return 0;
}
