include config.mk

SOURCES          = src/main.c src/args.c src/db.c src/perms.c
OBJECTS          = ${SOURCES:.c=.o}
OUT              = sspkg

BS_SOURCES       = src/bootstrap.c src/db.c src/perms.c
BS_OBJECTS       = ${BS_SOURCES:.c=.o}
BS_OUT           = sspkg-bootstrap

# Merge cc flags.
CONFIG_CC_FLAGS  := ${CONFIG_CC_FLAGS} ${CONFIG_OS}

.c.o:
	$(CC) -c ${CONFIG_CC_FLAGS} $< -o $@

all: ${OUT} ${BS_OUT}

${OUT}: ${OBJECTS}
	$(CC) -o $@ ${OBJECTS}

${BS_OUT}: ${BS_OBJECTS}
	$(CC) -o $@ ${BS_OBJECTS}

clean:
	rm ${OUT}
	rm ${BS_OUT}
	rm src/*.o
