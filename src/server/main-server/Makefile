server:	main.c
		gcc -o server main.c function.c -I/usr/include/json-c -L/usr/lib -ljson-c `mysql_config --cflags --libs` `pkg-config --cflags --libs gstreamer-rtsp-server-1.0 gstreamer-1.0`
