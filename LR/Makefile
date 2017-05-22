LDFLAGS = -lpng

ifeq ($(OS),WIN32)
LDFLAGS += -lz -static
endif

SRC = cpyshp.c readlevel.c savelevelbmp.c savelevelpng.c lr.c
TARGET = lr

all: $(TARGET)

OBJ = $(SRC:.c=.o)

$(TARGET): $(OBJ)
	$(CC) -o $@$(EXE) $(OBJ) $(LDFLAGS)

%.d: %.c
	$(CC) -MM $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	$(RM) $@.$$$$

-include $(SRC:.c=.d)

.PHONY: clean

clean:
	$(RM) *~ *.o *.d core $(TARGET)$(EXE)

