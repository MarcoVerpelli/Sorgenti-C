all:
	$(CC) snowflake.c -o snowflake -I/usr/X11R6/include -I/usr/X11R6/include/X11 -L/usr/X11R6/lib -L/usr/X11R6/lib/X11 -lX11

.PHONY: clean

clean:
	$(RM) snowflake *~ *.o *.d core