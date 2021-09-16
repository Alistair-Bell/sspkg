include config.mk

SOURCES          = src/main.c src/args.c src/db.c src/perms.c
OBJECTS          = ${SOURCES:.c=.o}
OUT              = sspkg

# Merge cc flags.
CONFIG_CC_FLAGS  := ${CONFIG_CC_FLAGS} ${CONFIG_OS}

.c.o:
	$(CC) -c ${CONFIG_CC_FLAGS} $< -o $@

all: ${OUT} ${BS_OUT}

${OUT}: ${OBJECTS}
	$(CC) -o $@ ${OBJECTS}

install: all
	install -m755 -s ${OUT} /usr/local/bin

uninstall:
	rm /usr/local/bin/${OUT}

clean:
	rm ${OUT}
	rm src/*.o
