APPNAME="./src/main.c"

default:
	gcc -g $(APPNAME) ./src/mappyal.c -L/usr/local/lib -L/usr/X11R6/lib -lalleg -lm -lpthread -lXxf86dga -lXxf86vm -ldl
