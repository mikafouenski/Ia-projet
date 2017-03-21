CC=gcc

CCFLAGS= -Wall -I./inc -g3 -std=c11
LDFLAGS=

SRC=$(wildcard src/*.c)
OBJ=$(SRC:.c=.o)

EXEC=logmod

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) -o $(EXEC) $^ $(LDFLAGS)

%.o: %.c
	$(CC) $(CCFLAGS) -o $@ -c $<

clean:
	rm -fv src/*.o
	rm -fv $(EXEC)
