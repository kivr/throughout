PROG = throughout
SRCS = usb/usb_hid.c bt/bt_hid.c bt/utils/bt_utils.c $(PROG).c

$(PROG): $(patsubst %.c,%.o,$(SRCS))
	gcc -o $@ $^ `pkg-config --libs libusb-1.0 glib-2.0` -lbluetooth -lpthread
	
%.o: %.c
	gcc -c -o $@ $< -g3 $(DEFS) `pkg-config --cflags libusb-1.0 glib-2.0` -I.

.PHONY : clean
clean:
	rm $(patsubst %.c,%.o,$(SRCS))
	rm $(PROG)

.PHONY : apple
apple: DEFS := -DAPPLE_KEYBOARD
apple: $(PROG)
