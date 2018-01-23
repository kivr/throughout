PROG = sdp_dbus_register
SRCS = $(wildcard gdbus/*.c) $(PROG).c

$(PROG): $(patsubst %.c,%.o,$(SRCS))
	gcc -o $@ $^ `pkg-config --libs dbus-1 glib-2.0`
	
%.o: %.c
	gcc -c -o $@ $< -g3 `pkg-config --cflags dbus-1 glib-2.0` -I.

.PHONY : clean
clean:
	rm *.o gdbus/*.o
	rm $(PROG)
