#include "chip8.h"
#include <SDL3/SDL_render.h>

chip8::chip8() {
  pc = 0x200;
  /*for (int i = 0; i < 4096; i++) {
    mem[i] = 0;
  }*/
  /*for (int i = 0; i < 64 * 32; i++) {
    gfx[i] = 0;
  }*/
  memset(mem, 0, RAM_SIZE);
  memset(gfx, 0, TOTAL_PIXELS);
  memset(V, 0, 16);
  memset(stack, 0, 16);
  opcode = 0;

  /*V[0x0] = 0;
  V[0x1] = 0;
  V[0x2] = 0;
  V[0x3] = 0;
  V[0x4] = 0;
  V[0x5] = 0;
  V[0x6] = 0;
  V[0x7] = 0;
  V[0x8] = 0;
  V[0x9] = 0;
  V[0xA] = 0;
  V[0xB] = 0;
  V[0xC] = 0;
  V[0xD] = 0;
  V[0xE] = 0;
  V[0xF] = 0;
*/
  index_ = 0;
  sp = 0;
  delay_timer = 0;
  sound_timer = 0;

  for (int i = 0; i < FONTSETSIZE; i++) {
    mem[FONTSTART + i] = fontset[i];
  }
}
chip8::~chip8() {}

// read in rom;put into memory;fetch and decode opcode until
// termination;
int chip8::loadROM(std::string romFileName) {

  std::ifstream romFile(romFileName, std::ios::in | std::ios::binary);
  std::ifstream::pos_type pos = romFile.tellg();
  int length = pos;

  char *buffer = new char[length];
  SDL_RenderClear(renderer);
  screen = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888,
                             SDL_TEXTUREACCESS_STREAMING, 16 * 64, 16 * 32);

  while (!romFile.eof()) {
    romFile.seekg(0, std::ios::beg);
    romFile.read(buffer, length);
  }

  uint8_t *buffer2 = (uint8_t *)buffer;

  if ((0xFFF - 0x200) > length) {
    for (int i = 0; i < length; i++) {
      mem[i + 0x200] = buffer[i];
    }
  }

  return 0;
}

int chip8::readInstruction(chip8 *cpu) {

  cpu->opcode = mem[cpu->pc] << 8 | mem[cpu->pc + 1];

  switch (cpu->opcode & 0xF000) {

  case 0x0000: // opcode1NNN(cpu->opcode); maybe add functions for each
               // opcode/instruction?

    switch (cpu->opcode & 0x000F) {

    case 0x0000:
      memset(cpu->gfx, 0, TOTAL_PIXELS);
      drawFlag = true;
      cpu->pc += 2;
      break;

    case 0x000E:
      cpu->pc = stack[cpu->sp]; // maybe fix this to be proper pointers
      --cpu->sp;
      cpu->pc += 2;
      break;

    default:
      printf("Unknown cpu->opcode [0x0000]: 0x%X\n", cpu->opcode);
    }
    break;

  case 0x1000:
    cpu->pc = cpu->opcode & 0x0FFF;
    break;

  case 0x2000:
    stack[cpu->sp] = cpu->pc;
    cpu->sp++;
    cpu->pc = cpu->opcode & 0x0FFF;
    break;

  case 0x3000:
    if (cpu->V[(cpu->opcode & 0x0F00) >> 8] == (cpu->opcode & 0x00FF))
      cpu->pc += 2;

  case 0x4000:
    if (cpu->V[(cpu->opcode & 0x0F00) >> 8] != (cpu->opcode & 0x00FF))
      cpu->pc += 2;
    break;
  case 0x5000:
    if (cpu->V[(cpu->opcode & 0x0F00) >> 8] == V[(cpu->opcode & 0x00F0) >> 4])
      cpu->pc += 2;
    break;
  case 0x6000:
    if (cpu->V[(cpu->opcode & 0x0F00) >> 8] == (cpu->opcode & 0x00FF))
      cpu->pc += 2;
    break;
  case 0x7000:
    // if (cpu->V[(cpu->opcode & 0x0F00) >> 8] == cpu->opcode & 0x00FF)
    cpu->V[(cpu->opcode & 0x0F00) >> 8] += cpu->opcode & 0x00FF;
    cpu->pc += 2;
    break;
  case 0x8000:
    switch (cpu->opcode & 0x000F) {

    case 0x0000:
      cpu->V[(cpu->opcode & 0xF00) >> 8] = cpu->V[(cpu->opcode & 0x00F0) >> 4];
      cpu->pc += 2;
      break;

    case 0x0001:
      cpu->V[(cpu->opcode & 0xF00) >> 8] |= cpu->V[(cpu->opcode & 0x00F0) >> 4];
      cpu->pc += 2;
      break;
    case 0x0002:
      cpu->V[(cpu->opcode & 0xF00) >> 8] &= cpu->V[(cpu->opcode & 0x00F0) >> 4];
      cpu->pc += 2;
      break;
    case 0x0003:
      cpu->V[(cpu->opcode & 0x0F00) >> 8] ^=
          cpu->V[(cpu->opcode & 0x00F0) >> 4];
      cpu->pc += 2;
      break;
    case 0x0004:
      if (cpu->V[(cpu->opcode & 0x00F0) >> 4] >
          (0xFF - cpu->V[(cpu->opcode & 0x0F00) >> 8]))
        cpu->V[0xF] = 1;
      else
        cpu->V[0xF] = 0;

      cpu->V[(cpu->opcode & 0x0F00) >> 8] +=
          cpu->V[(cpu->opcode & 0x00F0) >> 4];
      cpu->pc += 2;

      break;
    case 0x0005:
      if (cpu->V[(cpu->opcode & 0x00F0) >> 4] >
          (cpu->V[(cpu->opcode & 0x0F00) >> 8]))
        cpu->V[0xF] = 0;
      else
        cpu->V[0xF] = 1;

      cpu->V[(cpu->opcode & 0x0F00) >> 8] -=
          cpu->V[(cpu->opcode & 0x00F0) >> 4];
      cpu->pc += 2;

      break;
    case 0x0006:
      cpu->V[0XF] = cpu->V[(cpu->opcode & 0x00F0) >> 8] &
                    0x1; // mask least significant bit out to grab it
      cpu->V[(cpu->opcode & 0x0F00) >> 8] >>= 1;

      cpu->pc += 2;

      break;
    case 0x0007:
      if (cpu->V[(cpu->opcode & 0x0F00) >> 8] >
          (cpu->V[(cpu->opcode & 0x00F0) >> 4]))
        cpu->V[0xF] = 0;
      else
        cpu->V[0xF] = 1;

      cpu->V[(cpu->opcode & 0x0F00) >> 8] =
          cpu->V[(cpu->opcode & 0x00F0) >> 4] -
          cpu->V[(cpu->opcode & 0x0F00) >> 8];
      cpu->pc += 2;

      break;

    case 0x000E:
      cpu->V[0xF] = cpu->V[(cpu->opcode & 0x00F0) >> 8] >>
                    7; // sets to most significant bit before shifting by 1
      cpu->V[(cpu->opcode & 0x0F00) >> 8] <<= 1;

      cpu->pc += 2;

      break;

    default:

      break;
    }
    // if (cpu->V[(cpu->opcode & 0x0F00) >> 8] == cpu->opcode & 0x00FF)
    // cpu->V[(cpu->opcode & 0x0F00) >> 8] += cpu->opcode & 0x00FF;
    // cpu->pc+=2;
    break;

  case 0x9000:
    break;

  default:
    break;
  }

  return 0;
}
