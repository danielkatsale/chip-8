#include "gfx.h"
#include <SDL2/SDL_video.h>
#include <SDL_hints.h>
#include <SDL_render.h>
#include <iostream>

void gfx::gfxInit() {
  if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    std::cerr << "SDL failed to initialize: " << SDL_GetError() << std::endl;
  SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");
  window = SDL_CreateWindow("CHIP-8", SDL_WINDOWPOS_UNDEFINED,
                            SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, 0);
  renderer = SDL_CreateRenderer(
      window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  SDL_RenderSetLogicalSize(renderer, 64, 32);
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

  image = SDL_LoadBMP("logo.bmp");
  if (image != NULL) {
    screen = SDL_CreateTextureFromSurface(renderer, image);
    SDL_FreeSurface(image);
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, screen, NULL, NULL);
    SDL_RenderPresent(renderer);
    SDL_Delay(2000);
  }
}
void gfx::gfxLoop(const char *filename, chip8 *cpu) {
  int delay = 4;
  std::cout << "Delay: " << delay << std::endl;
  while (!quit) {
    while (SDL_PollEvent(&event)) {
      switch (event.type) {

      case SDL_QUIT:
        gfxClean();
        quit = 1;
        break;

      case SDL_KEYDOWN:
        switch (event.key.keysym.sym) {
        case SDLK_ESCAPE:
          quit = 1;
          break;
        case SDLK_F1:
          cpu->initCPU(cpu);
          cpu->loadROM(filename, cpu);
          cpu->readInstruction(cpu);
          gfxLoop(filename, cpu);
          gfxClean();
          break;

        case SDLK_n:
          cpu->readInstruction(cpu);
          gfxLoop(filename, cpu);
          break;
        case SDLK_p:
          paused = !paused;
          break;
        case SDLK_F2:
          delay -= 1;
          std::cout << "Delay " << delay << std::endl;
          break;

        case SDLK_F3:
          delay += 1;
          std::cout << "Delay " << delay << std::endl;
          break;

        case SDLK_x:
          cpu->keyboard[0] = 1;
          break;
        case SDLK_1:
          cpu->keyboard[1] = 1;
          break;
        case SDLK_2:
          cpu->keyboard[2] = 1;
          break;
        case SDLK_3:
          cpu->keyboard[3] = 1;
          break;
        case SDLK_q:
          cpu->keyboard[4] = 1;
          break;
        case SDLK_w:
          cpu->keyboard[5] = 1;
          break;
        case SDLK_e:
          cpu->keyboard[6] = 1;
          break;
        case SDLK_a:
          cpu->keyboard[7] = 1;
          break;
        case SDLK_s:
          cpu->keyboard[8] = 1;
          break;
        case SDLK_d:
          cpu->keyboard[9] = 1;
          break;
        case SDLK_z:
          cpu->keyboard[10] = 1;
          break;
        case SDLK_c:
          cpu->keyboard[11] = 1;
          break;
        case SDLK_4:
          cpu->keyboard[12] = 1;
          break;
        case SDLK_r:
          cpu->keyboard[13] = 1;
          break;
        case SDLK_f:
          cpu->keyboard[14] = 1;
          break;
        case SDLK_v:
          cpu->keyboard[15] = 1;
          break;
        }
        break;

      case SDL_KEYUP:
        switch (event.key.keysym.sym) {
        case SDLK_x:
          cpu->keyboard[0] = 0;
          break;
        case SDLK_1:
          cpu->keyboard[1] = 0;
          break;
        case SDLK_2:
          cpu->keyboard[2] = 0;
          break;
        case SDLK_3:
          cpu->keyboard[3] = 0;
          break;
        case SDLK_q:
          cpu->keyboard[4] = 0;
          break;
        case SDLK_w:
          cpu->keyboard[5] = 0;
          break;
        case SDLK_e:
          cpu->keyboard[6] = 0;
          break;
        case SDLK_a:
          cpu->keyboard[7] = 0;
          break;
        case SDLK_s:
          cpu->keyboard[8] = 0;
          break;
        case SDLK_d:
          cpu->keyboard[9] = 0;
          break;
        case SDLK_z:
          cpu->keyboard[10] = 0;
          break;
        case SDLK_c:
          cpu->keyboard[11] = 0;
          break;
        case SDLK_4:
          cpu->keyboard[12] = 0;
          break;
        case SDLK_r:
          cpu->keyboard[13] = 0;
          break;
        case SDLK_f:
          cpu->keyboard[14] = 0;
          break;
        case SDLK_v:
          cpu->keyboard[15] = 0;
          break;
        }
        break;
      }
      break;
    }

    if (delay < 0) {
      delay = 0;
      std::cout << "Delay: " << delay << std::endl;
    } else
      SDL_Delay(delay);

    if (cpu->delay_timer > 0)
      --cpu->delay_timer;

    if (!paused)
      cpu->readInstruction(cpu);
    if (cpu->drawFlag)
      gfxDraw(cpu);
  }
}
void gfx::gfxDraw(chip8 *cpu) {

  SDL_Rect r;
  int x, y;

  r.x = 0;
  r.y = 0;
  r.w = 1;
  r.h = 1;

  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderClear(renderer);
  SDL_SetRenderDrawColor(renderer, 0, 255, 0, 0);
  for (x = 0; x < WIDTH; x++) {
    for (y = 0; y < HEIGHT; y++) {
      if (cpu->display[(x) + ((y)*WIDTH)] == 1) {
        r.x = x;
        r.y = y;
        SDL_RenderFillRect(renderer, &r);
      }
    }
  }

  SDL_RenderPresent(renderer);
  cpu->drawFlag = false;
}

void gfx::gfxClean() {

  SDL_DestroyTexture(screen);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
}
