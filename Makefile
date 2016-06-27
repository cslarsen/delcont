TARGETS := aux.o cont.o cont
CFLAGS := -W -Wall -g -O0
NASM := nasm

OS := $(shell uname)
	ifeq ($(OS),Linux)
		NASMFLAGS := -felf64
	else
		NASMFLAGS := -fmacho64
	endif

%.o: %.asm
		$(NASM) $(NASMFLAGS) $^ -o$@

check: all
	./cont

all: $(TARGETS)

cont: cont.o aux.o

clean:
	rm -f $(TARGETS)
