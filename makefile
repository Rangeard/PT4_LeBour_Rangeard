CC=gcc
CFLAGS=-W -Wall -lSDL
LDFLAGS=-lSDL
EXEC=jdv

all: $(EXEC)

jdv: jdv.o
	$(CC) -o $@ $^ $(LDFLAGS)

jdv.o: jdv.c
	$(CC) -o $@ -c $< $(CFLAGS)

clean:
	rm -rf *.o

mrproper: clean
	rm -rf $(EXEC)

db :
	$(CC) -g jdv.c -o jdv_db $(CFLAGS)
