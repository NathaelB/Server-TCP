CC = gcc
CFLAGS = -W -Wall -ansi -pedantic
LDFLAGS=
EXEC=clean mrproper server client
COMPILE=$(CC) -o $@ $^ -W -Wall -ansi -pedantic
#COMPILE_EXEC=$(CC) -o $@ -c $< $(CFLAGS)
all: $(EXEC)

server: server.c
	$(COMPILE)

client: client.o
	$(COMPILE)


clean:
	rm -rf *.o

mrproper: clean
	rm -rf $(EXEC)