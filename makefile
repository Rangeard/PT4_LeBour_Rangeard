CC=gcc
CFLAGS=-W -Wall -lSDL
LDFLAGS=-lSDL
EXEC=jdv

all: $(EXEC)

jdv: jdv.o
	$(CC) -o bin/$@ bin/$^ $(LDFLAGS)

jdv.o: jdv.c
	$(CC) -o bin/$@ -c $< $(CFLAGS)

clean:
	rm -rf bin/*.o bin/jdv bin/jdv_aff

mrproper: clean
	rm -rf $(EXEC)

db :
	$(CC) -g jdv.c -o bin/jdv_db $(CFLAGS)

aff :
	$(CC) jdv.c -o bin/jdv_aff $(CFLAGS) -DAFF
