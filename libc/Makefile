all: $(STDINC) $(STDLIB)

$(ROOTDIR)/include/%.h: include/%.h
	@ mkdir -p $(ROOTDIR)/include
	cp $< $@

$(STDLIB): $(STDINC)
	$(MAKE) --directory lib

.PHONY: all
