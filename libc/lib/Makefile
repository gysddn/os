OUTS          :=$(OUT)/libc/string.o
CFLAGS        :=-c -ffreestanding -masm=att -nostdlib -nostdinc -I$(ROOTDIR)/include
LFLAGS        :=-r

all: $(STDLIB)

$(STDLIB): $(OUTS)
	$(LD) $(LFLAGS) $^ -o $@

$(OUT)/libc/%.o: %.c
	@ mkdir -p $(OUT)/libc
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -rf $(OUT)/libc
	rm     $(OUT)/libc.o

.PHONY: all clean compile
