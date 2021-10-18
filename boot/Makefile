BOOT          :=$(OUT)/boot/boot.o

all: $(BOOT)

$(OUT)/boot/%.o: %.s
	@ mkdir -p $(OUT)/boot
	$(AS) $(AFLAGS) -o $@ $<

clean:
	rm -rf $(OUT)/boot

.PHONY: all clean
