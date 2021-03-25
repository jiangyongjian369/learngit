CC=gcc
AR=$(CC)ar
EXEC=9-1-s
OBJS=9-1-s.o
all:$(EXEC)
$(EXEC):$(OBJS)
	$(CC) -o $@ $(OBJS) -lm
clean:
	-rm -f $(EXEC) *.elf *.gdb *.o
