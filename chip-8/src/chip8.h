#ifndef CHIP8_H_
#define CHIP8_H_

#include <SDL2/SDL.h>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <stdint.h>
#include <string.h>

#define TOTAL_PIXELS (WIDTH * HEIGHT)
#define RAM_SIZE 4096
#define FONTSTART 0x50
#define FONTSETSIZE 80
#define SCALE 16
#define WIDTH (64 * SCALE)
#define HEIGHT (32 * SCALE)

class chip8 {

  // private:
public:
  uint8_t mem[RAM_SIZE];

  uint8_t sound_timer;

  uint16_t pc;
  uint16_t stack[16];
  uint8_t sp;
  uint16_t index_;

  uint8_t V[0x10];
  uint16_t opcode;

  uint8_t fontset[80] = {
      0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
      0x20, 0x60, 0x20, 0x20, 0x70, // 1
      0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
      0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
      0x90, 0x90, 0xF0, 0x10, 0x10, // 4
      0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
      0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
      0xF0, 0x10, 0x20, 0x40, 0x40, // 7
      0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
      0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
      0xF0, 0x90, 0xF0, 0x90, 0x90, // A
      0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
      0xF0, 0x80, 0x80, 0x80, 0xF0, // C
      0xE0, 0x90, 0x90, 0x90, 0xE0, // D
      0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
      0xF0, 0x80, 0xF0, 0x80, 0x80  // F
  };

  SDL_Renderer *renderer;
  SDL_Window *window;
  SDL_Texture *screen;

  // change all to protected after finishing this is very unsafe
  uint8_t keyboard[16];
  uint8_t delay_timer;
  bool drawFlag = false;
  uint8_t display[TOTAL_PIXELS];
  int loadROM(std::string fileName, chip8 *cpu);
  int initCPU(chip8 *cpu);
  int readInstruction(chip8 *cpu);

  chip8();
  ~chip8();
};

#endif
