#include "gfx.h"

void gfx::gfxInit() {
  if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    std::cout << stderr << "SDL failed to initialize: " << SDL_GetError()
              << std::endl;

  window = SDL_CreateWindow("CHIP-8", 640, 480, 0);
  renderer = SDL_CreateRenderer(
      window, NULL, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  SDL_SetRenderLogicalPresentation(renderer, 64, 32,
                                   SDL_LOGICAL_PRESENTATION_INTEGER_SCALE,
                                   SDL_SCALEMODE_LINEAR);
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

  image = SDL_LoadBMP("logo.bmp");
  if (image != NULL) {
    screen = SDL_CreateTextureFromSurface(renderer, image);
    SDL_DestroySurface(image);
    SDL_RenderClear(renderer);
    SDL_RenderTexture(renderer, screen, NULL, NULL);
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
      case SDL_EVENT_QUIT:
        gfxClean();
        quit = 1;
        break;

      case SDL_EVENT_KEY_DOWN:
        switch (event.key.keysym.sym) {
        case SDLK_ESCAPE:
          quit = 1;
          break;
        case SDLK_F1:
          initCPU(cpu);
          loadROM(filename);
          readInstruction(cpu);
          gfxLoop(filename, cpu);
          gfxClean();
          break;

        case SDLK_n:
          readInstruction(cpu);
          gfxLoop(filename, cpu);
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
          keyboard[0] = 1;
          break;
        case SDLK_1:
          keyboard[1] = 1;
          break;
        case SDLK_2:
          keyboard[2] = 1;
          break;
        case SDLK_3:
          keyboard[3] = 1;
          break;
        case SDLK_q:
          keyboard[4] = 1;
          break;
        case SDLK_w:
          keyboard[5] = 1;
          break;
        case SDLK_e:
          keyboard[6] = 1;
          break;
        case SDLK_a:
          keyboard[7] = 1;
          break;
        case SDLK_s:
          keyboard[8] = 1;
          break;
        case SDLK_d:
          keyboard[9] = 1;
          break;
        case SDLK_z:
          keyboard[10] = 1;
          break;
        case SDLK_c:
          keyboard[11] = 1;
          break;
        case SDLK_4:
          keyboard[12] = 1;
          break;
        case SDLK_r:
          keyboard[13] = 1;
          break;
        case SDLK_f:
          keyboard[14] = 1;
          break;
        case SDLK_v:
          keyboard[15] = 1;
          break;
        }
        break;

      case SDL_EVENT_KEY_UP:
        switch (event.key.keysym.sym) {
        case SDLK_x:
          keyboard[0] = 0;
          break;
        case SDLK_1:
          keyboard[1] = 0;
          break;
        case SDLK_2:
          keyboard[2] = 0;
          break;
        case SDLK_3:
          keyboard[3] = 0;
          break;
        case SDLK_q:
          keyboard[4] = 0;
          break;
        case SDLK_w:
          keyboard[5] = 0;
          break;
        case SDLK_e:
          keyboard[6] = 0;
          break;
        case SDLK_a:
          keyboard[7] = 0;
          break;
        case SDLK_s:
          keyboard[8] = 0;
          break;
        case SDLK_d:
          keyboard[9] = 0;
          break;
        case SDLK_z:
          keyboard[10] = 0;
          break;
        case SDLK_c:
          keyboard[11] = 0;
          break;
        case SDLK_4:
          keyboard[12] = 0;
          break;
        case SDLK_r:
          keyboard[13] = 0;
          break;
        case SDLK_f:
          keyboard[14] = 0;
          break;
        case SDLK_v:
          keyboard[15] = 0;
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

    if (delay_timer > 0)
      --delay_timer;

    if (!paused)
      readInstruction(cpu);
    if (drawFlag)
      gfxDraw();
  }
}
void gfx::gfxDraw() {

  SDL_FRect r;
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
      if (display[(x) + ((y) + WIDTH)] == 1) {
        r.x = x;
        r.y = y;
        SDL_RenderFillRect(renderer, &r);
      }
    }
  }

  SDL_RenderPresent(renderer);
  drawFlag = false;
}

void gfx::gfxClean() {

  SDL_DestroyTexture(screen);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
}
