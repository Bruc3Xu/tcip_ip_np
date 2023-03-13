.PHONY: clean

gethostbyname: gethostbyname.o
	$(CC) $^ -o $@

gethostbyaddr: gethostbyaddr.o
	$(CC) $^ -o $@

%.o:%.c
	$(CC) -c $^ -o $@

all: gethostbyname gethostbyaddr

clean:
	rm -f gethostbyname gethostbyaddr *.o *.dat