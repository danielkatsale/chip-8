#include <SDL3/SDL.h>
<<<<<<< HEAD
=======
#include <stdlib.h>
#include <cstdio>
>>>>>>> fc1a0af (Finished readInstruction for chip8 emulation)

class gfx {

private:
  gfx();
  ~gfx();

<<<<<<< HEAD
public:
=======
  SDL_Renderer *renderer;
  SDL_Window *window;
  SDL_Surface *image;
  SDL_Texture *screen;
  SDL_Event event;
  bool quit = false;
  bool paused = false;


public:



>>>>>>> fc1a0af (Finished readInstruction for chip8 emulation)
};
