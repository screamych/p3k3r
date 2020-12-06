CFLAGS := -Wall -O2 -fomit-frame-pointer

SRC := swp-encoder.c crc32.c z85.c
OBJ := $(SRC:.c=.o)

all: swp-encoder

swp-encoder: $(OBJ) Makefile
	$(CC) -o $@ $(OBJ)

clean:
	@rm -f $(OBJ) swp-encoder

distclean: clean

install:
	install -m755 -D swp-encoder $(DESTDIR)/usr/bin/swp-encoder

.PHONY: all install clean distclean swp-encoder
