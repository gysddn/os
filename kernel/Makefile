KERNEL        :=$(OUT)/kernel/kernel.o
CFLAGS        :=-c -ffreestanding

all: $(KERNEL)

$(OUT)/kernel/%.o: %.c
	@ mkdir -p $(OUT)/kernel
	$(CC) $(CFLAGS) -I. -o $@ $<

clean:
	rm -rf $(OUT)/kernel

.PHONY: all clean
