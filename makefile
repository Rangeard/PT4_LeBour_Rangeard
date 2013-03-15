CC=gcc
CFLAGS=-W -Wall -lSDL
LDFLAGS=-lSDL
EXEC=jdv
<<<<<<< HEAD
C=src/c/
H=src/h/
B=bin/
AFF=0

all: $(EXEC)

jdv: jdv.o $(B)jdv_util.o $(B)jdv_sdl.o
	$(CC) -o bin/$@ bin/$^ $(LDFLAGS)
	
jdv.o: $(C)jdv.c $(H)jdv_util.h $(H)jdv_sdl.h 
	$(CC) -o bin/$@ -c $< $(CFLAGS)

jdv_util.o: $(C)jdv_util.c $(H)jdv_util.h
	$(CC) -o bin/$@ -c $< $(CFLAGS)

jdv_sdl.o: $(C)jdv_sdl.c $(H)jdv_util.h
	$(CC) -o bin/$@ -c $< $(CFLAGS)

clean:
	rm -rf $(B)*.o $(B)jdv $(B)jdv_aff

mrproper: clean
	rm -rf $(B)$(EXEC)

db :
	$(CC) -g jdv.c -o bin/jdv_db $(CFLAGS)
=======

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
>>>>>>> 6ef8b0ae6813e862adbf98f1226167649cbf3a58
