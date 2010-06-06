
# NOTE:
# LDFLAGS are flags sent to the linker.
# CFLAGS are sent to the C compiler.

ifeq "$(OS)" "Windows_NT"
	LDFLAGS = -lmingw32 -lSDLmain -lSDL -lopengl32
	OUT     = run.exe
else
	# Linux
	LDFLAGS = -lGL -lSDL
	OUT     = run
endif


CC = g++

CFLAGS  += -Wall

compile = ${CC} ${CFLAGS} -c
link    = ${CC} ${CFLAGS} -o ${OUT}

${OUT} : .draw_shape.o .Playfield.o .Actor.o .Gunman.o .Collision.o main.cpp makefile
	# LDFLAGS must be at end for Windows compiling.
	${link} main.cpp -std=c++0x .Playfield.o .Actor.o .Gunman.o .Collision.o ${LDFLAGS}

.Collision.o : Collision.cpp Collision.h makefile
	${compile} Collision.cpp -o .Collision.o

.Gunman.o : Gunman.* Actor.h Collision.h makefile
	${compile} Gunman.cpp -o .Gunman.o

.Actor.o : Actor.cpp Actor.h makefile
	${compile} Actor.cpp -o .Actor.o

.Playfield.o : Playfield.h Playfield.cpp Actor.h Collision.h makefile
	${compile} Playfield.cpp -o .Playfield.o


