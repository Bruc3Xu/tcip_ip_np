.PHONY: clean

uecho_client: uecho_client.o
	$(CC) $^ -o $@

uecho_server: uecho_server.o
	$(CC) $^ -o $@

bound_host1: bound_host1.o
	$(CC) $^ -o $@

bound_host2: bound_host2.o
	$(CC) $^ -o $@

uecho_con_client: uecho_con_client.o
	$(CC) $^ -o $@

%.o:%.c
	$(CC) -c $^ -o $@

all: uecho_client uecho_server bound_host1 bound_host2 uecho_con_client

clean:
	rm -f uecho_client uecho_server bound_host1 bound_host2 uecho_con_client *.o

rebuild: clean all