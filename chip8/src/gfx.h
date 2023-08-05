#include "chip8.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_oldnames.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_video.h>
#include <cstdio>
#include <ostream>
#include <stdlib.h>

class gfx : private chip8 {

public:
  void gfxLoop(const char *filename, chip8 *cpu);
  void gfxDraw(chip8 *cpu);
  void gfxInit();
  void gfxClean();

private:
  SDL_Renderer *renderer;
  SDL_Window *window;
  SDL_Surface *image;
  SDL_Texture *screen;
  SDL_Event event;
  bool quit = false;
  bool paused = false;
};
