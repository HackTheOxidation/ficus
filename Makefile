include config.mk

SOURCES=ficus.c http.c handler.c config.c security.c
OBJECTS = $(SOURCES:.c=.o)
DEPS =$(SOURCES:.c=.d)
EXE = ficus
CC=gcc

ifeq ($(build), debug)
	CFLAGS = -I. -g -pedantic -Werror -Wall -std=c11
  BUILD_DIR=deps/debug
  BIN_DIR=bin/debug
else
	CFLAGS = -I. -O3 -std=c11
  BUILD_DIR=deps/release
  BIN_DIR=bin/release
endif

OBJECTS := $(addprefix ./$(BUILD_DIR)/, $(OBJECTS))
DEPS := $(addprefix ./$(BUILD_DIR)/, $(DEPS))
EXE := $(addprefix ./$(BIN_DIR)/, $(EXE))

all: $(EXE)

$(EXE): $(DEPS) $(OBJECTS)	# << Check the $(DEPS) new dependency
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) -ltls -o $@ $(OBJECTS)

$(BUILD_DIR)/%.o: %.c
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -ltls -o $@ -c $<

$(BUILD_DIR)/%.d: %.c
	@mkdir -p $(BUILD_DIR)
	$(CC) -MT$@ -MM $(CFLAGS) $< > $@
	$(CC) -MT$(@:.d=.o) -MM $(CFLAGS) $< >> $@

ifneq ($(MAKECMDGOALS),clean)
-include $(DEPS)
endif

clean:
	rm -rf deps
	rm -rf bin
	rm -f ficus-${VERSION}.tar.gz

dist: clean
	mkdir -p ficus-${VERSION}
	cp -R LICENSE Makefile README.md config.mk *.h *.c\
		ficus.1 ficus.default.conf ficus-${VERSION}
	tar -cf ficus-${VERSION}.tar ficus-${VERSION}
	gzip ficus-${VERSION}.tar
	rm -rf ficus-${VERSION}

install: all
	mkdir -p ${DESTDIR}${PREFIX}/bin
	cp -f $(EXE) ${DESTDIR}${PREFIX}/bin
	chmod 755 ${DESTDIR}${PREFIX}/bin/ficus
	cp -f ficus.def.conf /etc/ficus.conf
	mkdir -p ${DESTDIR}${MANPREFIX}/man1
	sed "s/VERSION/${VERSION}/g" < ficus.1 > ${DESTDIR}${MANPREFIX}/man1/ficus.1
	chmod 644 ${DESTDIR}${MANPREFIX}/man1/ficus.1
	mkdir -p /var/www/html

uninstall:
	rm -f ${DESTDIR}${PREFIX}/bin/ficus\
		${DESTDIR}${MANPREFIX}/man1/ficus.1

.PHONY: all clean dist install uninstall
