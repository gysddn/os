NASM=nasm
NFLAGS=-g -f bin

.PHONY: clean compile

SRCS = $(wildcard *.asm)
BINS = $(patsubst %.asm,%.bin,$(SRCS))

compile: $(BINS)

%.bin: %.asm
	$(NASM) $(NFLAGS) -o $@ $<

clean:
	rm -f $(BINS)
