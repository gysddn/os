# Main point of control for build tasks

PWD           :=$(shell pwd)
TOOCHAIN_BIN  :=/opt/i686-elf/bin
TARGET        :=i686-elf
AS            :=$(TOOCHAIN_BIN)/$(TARGET)-as
CC            :=$(TOOCHAIN_BIN)/$(TARGET)-gcc
LD            :=$(TOOCHAIN_BIN)/$(TARGET)-ld

LINKER_SCRIPT :=$(PWD)/linker.ld

BUILD_DIR     :=$(PWD)/build
OUT           :=$(BUILD_DIR)/out
ISODIR        :=$(BUILD_DIR)/iso
ISOFILE       :=$(BUILD_DIR)/os.iso

GRUB          :=$(ISODIR)/boot/grub/grub.cfg
KERNEL_FILE   :=$(ISODIR)/boot/kernel

export

all: $(ISOFILE)

$(ISOFILE): $(KERNEL_FILE) $(GRUB)
	grub-mkrescue -o $@  $(ISODIR)

$(GRUB): $(PWD)/boot/grub.cfg
	mkdir -p $(ISODIR)/boot/grub
	cp $< $@

$(KERNEL_FILE): compile
	mkdir -p $(ISODIR)/boot
	$(LD) -T$(LINKER_SCRIPT) $(OUT)/boot/boot.o $(OUT)/kernel.o -o $@

compile:
	$(MAKE) --directory boot
	$(MAKE) --directory kernel

clean:
	rm -rf $(BUILD_DIR)

.PHONY: all clean compile
