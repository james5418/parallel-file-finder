CC=gcc
CCFLAGS=-I include -O3 -Wall -lpthread
OBJS=objs/arg_handler.o objs/KMP.o objs/list.o objs/main.o objs/ringbuf.o objs/worker.o
EXE=pp-find

.PHONY: all clean

all: $(EXE)

$(EXE): $(OBJS)
	$(CC) -o $@ $^ $(CCFLAGS)

objs/%.o: src/%.c
	$(CC) -o $@ -c $^ $(CCFLAGS)

clean:
	-rm $(EXE)
	-rm $(OBJS)
