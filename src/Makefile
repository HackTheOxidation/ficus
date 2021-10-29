SOURCES=ficus.c http.c handler.c config.c
OBJECTS = $(SOURCES:.c=.o)
DEPS =$(SOURCES:.c=.d)
EXE = ficus
CC=gcc

ifeq ($(build), release)
	CFLAGS = -I. -O3 -std=c11
  BUILD_DIR=deps/release
  BIN_DIR=bin/release
else
	CFLAGS = -I. -g -pedantic -Werror -Wall -std=c11
  BUILD_DIR=deps/debug
  BIN_DIR=bin/debug
endif

OBJECTS := $(addprefix ./$(BUILD_DIR)/, $(OBJECTS))
DEPS := $(addprefix ./$(BUILD_DIR)/, $(DEPS))
EXE := $(addprefix ./$(BIN_DIR)/, $(EXE))


all: $(EXE)

$(EXE): $(DEPS) $(OBJECTS)	# << Check the $(DEPS) new dependency
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) -o $@ $(OBJECTS)

$(BUILD_DIR)/%.o: %.c
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -o $@ -c $<

$(BUILD_DIR)/%.d: %.c
	@mkdir -p $(BUILD_DIR)
	$(CC) -MT$@ -MM $(CFLAGS) $< > $@
	$(CC) -MT$(@:.d=.o) -MM $(CFLAGS) $< >> $@

ifneq ($(MAKECMDGOALS),clean)
-include $(DEPS)
endif


clean:
	rm -r deps
	rm -r bin
