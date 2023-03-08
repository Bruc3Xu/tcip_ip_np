.PHONY: clean

op_client: op_client.o
	$(CC) $^ -o $@

op_server: op_server.o
	$(CC) $^ -o $@

%.o:%.c
	$(CC) -c $^ -o $@

all: op_client op_server

clean:
	rm -f op_client op_server *.o

rebuild: clean all