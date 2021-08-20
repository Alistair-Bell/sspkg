include config.mk

SOURCES          = src/main.c src/args.c
OBJECTS          = ${SOURCES:.c=.o}
OUT              = sspkg

.c.o:
	$(CC) -c ${CC_FLAGS} $< -o $@

all: ${OUT}

${OUT}: ${OBJECTS}
	$(CC) -o $@ ${OBJECTS}


clean: all
	rm ${OUT}
	rm src/*.o
