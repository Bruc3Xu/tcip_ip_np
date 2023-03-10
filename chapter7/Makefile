.PHONY: clean

file_client: file_client.o
	$(CC) $^ -o $@

file_server: file_server.o
	$(CC) $^ -o $@

%.o:%.c
	$(CC) -c $^ -o $@

all: file_client file_server

clean:
	rm -f file_client file_server *.o *.dat