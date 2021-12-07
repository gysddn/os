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
ROOTDIR        :=$(BUILD_DIR)/root
ISOFILE       :=$(BUILD_DIR)/os.iso

STDINC        :=$(ROOTDIR)/include/stdbool.h \
                $(ROOTDIR)/include/stddef.h \
                $(ROOTDIR)/include/stdint.h \
                $(ROOTDIR)/include/stdnoreturn.h \
                $(ROOTDIR)/include/string.h \
                $(ROOTDIR)/include/result.h
STDLIB        :=$(OUT)/libc.o

GRUB          :=$(ROOTDIR)/boot/grub/grub.cfg
KERNEL_FILE   :=$(ROOTDIR)/boot/kernel

export

all: $(ISOFILE)

$(ISOFILE): $(KERNEL_FILE) $(GRUB)
	grub-mkrescue -o $@  $(ROOTDIR)

$(GRUB): $(PWD)/boot/grub.cfg
	mkdir -p $(ROOTDIR)/boot/grub
	cp $< $@

$(KERNEL_FILE): compile
	mkdir -p $(ROOTDIR)/boot
	$(LD) -T$(LINKER_SCRIPT) $(OUT)/boot/boot.o $(OUT)/kernel.o -o $@

libc:
	$(MAKE) --directory libc

compile: libc
	$(MAKE) --directory boot
	$(MAKE) --directory kernel

clean:
	rm -rf $(BUILD_DIR)

.PHONY: all clean compile libc
