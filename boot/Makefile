BOOT          :=$(OUT)/boot/boot.o
CFLAGS        :=-c

all: $(BOOT)

$(OUT)/boot/%.o: %.s
	@ mkdir -p $(OUT)/boot
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -rf $(OUT)/boot

.PHONY: all clean
