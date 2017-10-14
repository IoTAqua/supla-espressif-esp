SIGTOOL_TARGET = sigtool
SIGTOOL_OBJS = sigtool.o

SIGTOOL_CFLAGS += -std=gnu99 -Wall -Wextra -Werror -Wno-pointer-sign
SIGTOOL_CFLAGS += $(shell pkg-config --cflags nettle)
SIGTOOL_CFLAGS += $(shell pkg-config --cflags hogweed)

SIGTOOL_LDFLAGS += -lgmp
SIGTOOL_LDFLAGS += $(shell pkg-config --libs nettle)
SIGTOOL_LDFLAGS += $(shell pkg-config --libs hogweed)

ifeq ("$(V)","1")
Q :=
vecho := @true
else
Q := @
vecho := @echo
endif

.PHONY: all
all: $(SIGTOOL_TARGET)

$(SIGTOOL_TARGET): $(SIGTOOL_OBJS)
	$(vecho) "LD $@"
	$(Q) $(CC) $(SIGTOOL_LDFLAGS) -o $@ $^

%.o: %.c
	$(vecho) "CC $<"
	$(Q) $(CC) $(SIGTOOL_CFLAGS) -c -o $@ $<
	
clean:
	$(Q) rm -f $(SIGTOOL_OBJS) $(SIGTOOL_TARGET)
