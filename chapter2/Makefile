.PHONY: clean

tcp_client: tcp_client.o
	$(CC) $^ -o $@

tcp_server: tcp_server.o
	$(CC) $^ -o $@

%.o:%.c
	$(CC) -c $^ -o $@

all: tcp_client tcp_server

clean:
	rm -f tcp_client tcp_server *.o