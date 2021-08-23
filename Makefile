include config.mk

SOURCES          = src/main.c src/args.c src/db.c
OBJECTS          = ${SOURCES:.c=.o}
OUT              = sspkg

# Merge cc flags.
CONFIG_CC_FLAGS  := ${CONFIG_CC_FLAGS} ${CONFIG_OS}

.c.o:
	$(CC) -c ${CONFIG_CC_FLAGS} $< -o $@

all: ${OUT}

${OUT}: ${OBJECTS}
	$(CC) -o $@ ${OBJECTS}


clean:
	rm ${OUT}
	rm src/*.o
