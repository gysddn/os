KERNEL        :=$(OUT)/kernel.o
LIBC          :=$(OUT)/libc.o
OUTS          :=$(OUT)/kernel/kernel.o $(OUT)/kernel/serial.o $(OUT)/kernel/vga.o $(OUT)/kernel/io.o $(OUT)/kernel/util.o
CFLAGS        :=-c -ffreestanding -masm=att -nostdlib -nostdinc -I$(ROOTDIR)/include
LFLAGS        :=-r

all: $(KERNEL)

# I really don't know how this works but it does, 'make' is very well aware of these
# variables but the rules to make them were in a seperate process which is long dead
# at this point, all I can guess is that 'make' looks at these files and "can" in fact
# find them becase libc was built prior so it doesn't search for rules to make them instead
# just checks the time and compiles our target if it becomes outdated.
$(OUTS): $(STDINC) $(STDLIB) io.h serial.h types.h util.h vga.h

$(KERNEL): $(OUTS)
	$(LD) $(LFLAGS) $^ $(LIBC) -o $@

$(OUT)/kernel/%.o: %.c
	@ mkdir -p $(OUT)/kernel
	$(CC) $(CFLAGS) -I. -o $@ $<

$(OUT)/kernel/%.o: %.S
	@ mkdir -p $(OUT)/kernel
	$(CC) $(CFLAGS) -I. -o $@ $<

clean:
	rm -rf $(OUT)/kernel

.PHONY: all clean
