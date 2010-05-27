
CC = g++
CFLAGS  = -Wall
OUT  = run
LIBS = -lGL -lSDL

compile=${CC} ${CFLAGS}
link=${compile} -o ${OUT} ${LIBS}

${OUT} : glpp.o draw_shape.o Playfield.o Collision.o Gunman.o makefile
	${link} ${LIBS} Gunman.o Bullet.h main.cpp Collision.o Playfield.o glpp.o draw_shape.o 

Gunman.o : Gunman.cpp Gunman.h makefile
	${compile} -c Gunman.cpp

Collision.o : Collision.cpp Collision.h makefile
	${compile} -c Collision.cpp

Playfield.o : Playfield.h Playfield.cpp makefile
	${compile} -c Playfield.cpp

draw_shape.o : draw_shape.h draw_shape.cpp makefile
	${compile} -c draw_shape.cpp 

glpp.o : glpp.cpp glpp.h makefile
	${compile} -c glpp.cpp

clean : 
	rm *.o

.PHONY : clean
