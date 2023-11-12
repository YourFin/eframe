#include "simulator.h"

#include "SDL2/SDL.h"
#include "SDL2/SDL_error.h"
#include "SDL_pixels.h"
#include "SDL_surface.h"
#include "SDL_video.h"
#include "cairo.h"
#include "imgui/imgui.h"
#include <cstddef>
#include <cstdio>
#include <iostream>
#include <new>
#include <optional>

extern "C" {
#include "SDL2/SDL.h"
#include "SDL2/SDL_error.h"
}

namespace sdl {
enum error { INIT_ISSUE };

std::optional<error> init() {
  printf("Yo");

  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    printf("Couldn't init SDL: %s\n", SDL_GetError());
    return INIT_ISSUE;
  }

  SDL_Window *window =
      SDL_CreateWindow("SDL Test", SDL_WINDOWPOS_UNDEFINED,
                       SDL_WINDOWPOS_UNDEFINED, 500, 500, SDL_WINDOW_SHOWN);
  if (window == std::nullptr_t()) {
    printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
    return INIT_ISSUE;
  }

  SDL_Surface *screenSurface = SDL_GetWindowSurface(window);

  SDL_FillRect(screenSurface, std::nullptr_t(),
               SDL_MapRGB(screenSurface->format, 0xFF, 0xFF, 0xFF));

  SDL_UpdateWindowSurface(window);

  // Hack to get window to stay up
  SDL_Event e;
  bool quit = false;
  while (quit == false) {
    while (SDL_PollEvent(&e)) {
      if (e.type == SDL_QUIT) {
        printf("Recieved quit event...");
        quit = true;
      }
    }
  }
  SDL_DestroyWindow(window);
  SDL_Quit();

  return std::nullopt;
}
} // namespace sdl

int main() {
  if (sdl::init().has_value()) {
    return 1;
  }
  return 0;
}
