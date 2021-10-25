KERNEL        :=$(OUT)/kernel/kernel.o $(OUT)/kernel/vga.o
CFLAGS        :=-c -ffreestanding -masm=att

all: $(KERNEL)

$(OUT)/kernel/%.o: %.c
	@ mkdir -p $(OUT)/kernel
	$(CC) $(CFLAGS) -I. -o $@ $<

clean:
	rm -rf $(OUT)/kernel

.PHONY: all clean
