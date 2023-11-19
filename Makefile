##
# eframe
#
# @file
# @version 0.1
simulator: simulator.cc
	 NIX_LDFLAGS="$$(pkg-config --libs gtkmm-4.0) $$NIX_LDFLAGS" $(CXX) simulator.cc $$(pkg-config --cflags cairo gtkmm-4.0 curlpp asio libzmq) -isystem $$ZMQPP_NIX/include -isystem $$CROW_NIX/include $$NIX_CFLAGS_COMPILE -std=c++23 -Wall -o sim-window

sim: simulator
	./sim-window

graphicscc: graphics.cc
	g++ graphics.cc $$(pkg-config --cflags cairo) $$NIX_CFLAGS_COMPILE -std=c++23 -o graphicscc

graphics: graphics.c
	cc graphics.c $$(pkg-config --cflags cairo) $$NIX_CFLAGS_COMPILE -o -std=gnu23 graphics

.PHONY: bear
bear:
	$(MAKE) clean
	bear -- $(MAKE)

.PHONY: clean
clean:
	$(RM) sim-window graphicscc graphics

# end
