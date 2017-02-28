SRC=./src/main.c ./src/mappyal.c
CFLAGS=-L/usr/local/lib -L/usr/X11R6/lib -lalleg -lm -lpthread -lXxf86dga -lXxf86vm -ldl
NAME=FlappyBirdSpaceClone

default:
	gcc -g $(SRC) -o $(NAME) $(CFLAGS)

clean:
	rm $(NAME)