##
# eframe
#
# @file
# @version 0.1
graphics: graphics.c
	cc graphics.c $$(pkg-config --cflags cairo) $$NIX_CFLAGS_COMPILE -o -std=gnu23 graphics

.PHONY: bear
bear:
	bear -- $(MAKE)

.PHONY: clean
clean:
	$(RM) graphics

# end
