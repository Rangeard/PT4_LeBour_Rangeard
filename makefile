CC=gcc
CFLAGS=-lpthreads -g
SDLFLAGS=-lSDL
OGLFLAGS=-lSDL -lGL -lGLU -DOGL
EXEC=jdv
C=src/c/
H=src/h/
B=bin/


all: $(EXEC)

jdv: $(B)jdv.o $(B)jdv_util.o $(B)jdv_sdl.o $(B)jdv_ogl.o
	$(CC) -o $(B)$@ $^ $(OGLFLAGS)
	
$(B)jdv.o: $(C)jdv.c $(H)jdv_util.h $(H)jdv_sdl.h $(H)jdv_ogl.h 
	$(CC) -o $@ -c $< $(CFLAGS) $(OGLFLAGS)

$(B)jdv_util.o: $(C)jdv_util.c $(H)jdv_util.h
	$(CC) -o $@ -c $< $(CFLAGS)

$(B)jdv_sdl.o: $(C)jdv_sdl.c $(H)jdv_util.h
	$(CC) -o $@ -c $< $(CFLAGS) $(SDLFLAGS)

$(B)jdv_ogl.o: $(C)jdv_ogl.c $(H)jdv_util.h
	$(CC) -o $@ -c $< $(CFLAGS) $(OGLFLAGS)

clean:
	rm -rf $(B)*.o $(B)jdv $(B)jdv_aff

mrproper: clean
	rm -rf $(B)$(EXEC)
	
