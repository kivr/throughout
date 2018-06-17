PROG = throughout
SRCS = bt/bt_hid.c bt/utils/bt_utils.c $(PROG).c
DEBUG = -g3

$(PROG): $(patsubst %.c,%.o,$(SRCS))
	gcc -o $@ $^ `pkg-config --libs glib-2.0` -lbluetooth
	
%.o: %.c
	gcc -c -o $@ $< $(DEBUG) $(DEFS) `pkg-config --cflags glib-2.0` -I.

bt/config_controller: bt/config_controller.o bt/mgmt_api/mgmt_api.o
	gcc $^ -o $@ -lbluetooth `pkg-config --libs glib-2.0`

bt/sdp/sdpd: bt/sdp/sdpd.o bt/utils/bt_utils.o
	gcc $^ -o $@ -lbluetooth

.PHONY : config
config: bt/config_controller

.PHONY : sdpd
sdpd: bt/sdp/sdpd

.PHONY : clean-sdpd
clean-sdpd:
	rm -f bt/sdp/sdpd

.PHONY : clean-config
clean-config:
	rm -f bt/config_controller.o
	rm -f bt/mgmt_api/mgmt_api.o
	rm -f bt/config_controller

.PHONY : clean
clean:
	rm -f $(patsubst %.c,%.o,$(SRCS))
	rm -f $(PROG)

.PHONY : apple
apple: DEFS := -DAPPLE_KEYBOARD
apple: $(PROG)
