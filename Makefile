.PHONY:clean
CC=gcc
CFLAGS=-Wall -g
BIN=main
OBJS=main.o httpd.o condtion.o threadpool.o 
$(BIN):$(OBJS)
	$(CC) $(CFLAGS) $^ -o $@ -lpthread -lrt
%.o:%.c
	$(CC) $(CFLAGS) -c $< -o $@ 

clean:
	rm -f *.o $(BIN)
