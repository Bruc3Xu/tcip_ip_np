.PHONY: clean

hello_client: hello_client.o
	$(CC) $^ -o $@

hello_server: hello_server.o
	$(CC) $^ -o $@

%.o:%.c
	$(CC) -c $^ -o $@

all: hello_client hello_server

clean:
	rm -f hello_client hello_server *.o