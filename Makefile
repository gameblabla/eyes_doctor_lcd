CC = gcc

CFLAGS = -Os
LDFLAGS = -lSDL2
OUTPUT = eyes_doctor

SOURCES = main.c wrapper.c
OBJS = ${SOURCES:.c=.o}

${OUTPUT}:${OBJS}
	${CC} -o ${OUTPUT} ${SOURCES} ${CFLAGS} ${LDFLAGS} ${DEFINES} 
	
clean:
	rm *.o
	rm ${OUTPUT}
