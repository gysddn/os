KERNEL        :=$(OUT)/kernel.o
OUTS          :=$(OUT)/kernel/kernel.o $(OUT)/kernel/serial.o $(OUT)/kernel/vga.o $(OUT)/kernel/io.o $(OUT)/kernel/util.o
CFLAGS        :=-c -ffreestanding -masm=att -nostdlib -nostdinc
LFLAGS        :=-r

all: $(KERNEL)

$(OUTS): io.h serial.h types.h util.h vga.h

$(KERNEL): $(OUTS)
	$(LD) $(LFLAGS) $^ -o $@

$(OUT)/kernel/%.o: %.c
	@ mkdir -p $(OUT)/kernel
	$(CC) $(CFLAGS) -I. -Iinclude -o $@ $<

$(OUT)/kernel/%.o: %.S
	@ mkdir -p $(OUT)/kernel
	$(CC) $(CFLAGS) -I. -Iinclude -o $@ $<

clean:
	rm -rf $(OUT)/kernel

.PHONY: all clean
