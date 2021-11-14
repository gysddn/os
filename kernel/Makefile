KERNEL        :=$(OUT)/kernel.o
OUTS          :=$(OUT)/kernel/kernel.o $(OUT)/kernel/serial.o $(OUT)/kernel/vga.o $(OUT)/kernel/io.o $(OUT)/kernel/util.o
CFLAGS        :=-c -ffreestanding -masm=att -nostdlib -nostdinc -I../libc/include
LFLAGS        :=-r

all: $(KERNEL)

$(KERNEL): $(OUTS)
	$(LD) $(LFLAGS) $^ -o $@

$(OUT)/kernel/%.o: %.c
	@ mkdir -p $(OUT)/kernel
	$(CC) $(CFLAGS) -I. -o $@ $<

$(OUT)/kernel/%.o: %.S
	@ mkdir -p $(OUT)/kernel
	$(CC) $(CFLAGS) -I. -o $@ $<

clean:
	rm -rf $(OUT)/kernel

.PHONY: all clean
