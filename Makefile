##
# eframe
#
# @file
# @version 0.1
simulator: simulator.cc
	 NIX_LDFLAGS="$$(pkg-config --libs gtkmm-4.0) $$NIX_LDFLAGS" \
		g++ simulator.cc \
		$$(pkg-config --cflags \
			cairo \
			gtkmm-4.0 \
			curlpp \
			asio \
			libzmq) \
		-isystem $$CROW_NIX/include \
		-isystem $$CPP_STDLIB_CXXABI/include/c++/v1 \
		$$NIX_CFLAGS_COMPILE \
		-std=c++23 -Wall \
		-o sim-window

sim: simulator
	./sim-window

graphicscc: graphics.cc
	clang++ graphics.cc $$(pkg-config --cflags cairo) $$NIX_CFLAGS_COMPILE -std=c++23 -o graphicscc

graphics: graphics.c
	cc graphics.c $$(pkg-config --cflags cairo) $$NIX_CFLAGS_COMPILE -o -std=gnu23 graphics

c_main:
	NIX_LDFLAGS="$$(pkg-config --libs cairo libcurl) $$NIX_LDFLAGS"
		clang c_application.c c_fetch_data.c \
		$$(pkg-config --cflags cairo libcurl) \
		$$(pkg-config --libs cairo libcurl) \
		$$NIX_CFLAGS_COMPILE \
		-std=c17 -Wall \
		-o c_main

.PHONY: bear
bear:
	$(MAKE) clean
	bear -- $(MAKE)

.PHONY: bear_c
bear_c:
	$(MAKE) clean
	bear -- $(MAKE) c_main

.PHONY: clean
clean:
	$(RM) sim-window graphicscc graphics c_main

# end
