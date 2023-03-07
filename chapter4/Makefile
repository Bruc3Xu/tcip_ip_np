.PHONY: clean

echo_client: echo_client.o
	$(CC) $^ -o $@

echo_server: echo_server.o
	$(CC) $^ -o $@

%.o:%.c
	$(CC) -c $^ -o $@

all: echo_client echo_server

clean:
	rm -f echo_client echo_server *.o

rebuild: clean all