CC = gcc
CFLAGS = --std=c99 -Wall -Wextra -D_POSIX_C_SOURCE=200809L
OBJD = obj
_OBJ = util.o config.o log.o netwk.o main.o
OBJ = $(patsubst %,$(OBJD)/%,$(_OBJ))
SRCD = src
INCS = $(shell find $(SRCD) -name '*.h')
LIBS = -Ilib/inih

$(OBJD)/%.o: $(SRCD)/%.c $(INCS)
	$(CC) -c -o $@ $< $(CFLAGS)

debug: $(OBJ)
	$(CC) -o quot-debug $^ $(CFLAGS) $(LIBS)
	
.PHONY: clean

clean:
	rm -rf $(OBJD)/*.o
