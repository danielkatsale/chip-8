#include "chip8.h"
#include <SDL2/SDL_render.h>

chip8::chip8() {}
chip8::~chip8() {}

// read in rom;put into memory;fetch and decode opcode until
// termination;
//

int chip8::initCPU(chip8 *cpu) {
  cpu->pc = 0x200;
  /*for (int i = 0; i < 4096; i++) {
    mem[i] = 0;
  }*/
  /*for (int i = 0; i < 64 * 32; i++) {
    gfx[i] = 0;
  }*/
  memset(cpu->mem, 0, RAM_SIZE);
  memset(cpu->display, 0, TOTAL_PIXELS);
  memset(cpu->V, 0, 16);
  memset(cpu->stack, 0, 16);
  cpu->opcode = 0;

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
  cpu->index_ = 0;
  cpu->sp = 0;
  cpu->delay_timer = 0;
  cpu->sound_timer = 0;

  for (int i = 0; i < FONTSETSIZE; i++) {
    cpu->mem[FONTSTART + i] = cpu->fontset[i];
  }

  srand(time(0));

  return 0;
}
int chip8::loadROM(std::string romFileName, chip8 *cpu) {

  std::ifstream romFile(romFileName, std::ifstream::binary);
  romFile.seekg(0, std::ios::end);
  uint64_t length = romFile.tellg();
  romFile.seekg(0, std::ios::beg);

  SDL_RenderClear(renderer);
  screen = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888,
                             SDL_TEXTUREACCESS_STREAMING, WIDTH, HEIGHT);

  char *buffer = new char[length];

  if (!buffer) {
    std::cout << "buffer allocation failed!" << std::endl;
    exit(EXIT_FAILURE);
  }

  if (!romFile) {
    // romFile.seekg(0, std::ios::beg);
    // romFile.read(buffer, length);
    std::cout << "Couldn't read file" << std::endl;
    exit(EXIT_FAILURE);
  }
  romFile.read(buffer, length);
  std::streamsize result = romFile.gcount();

  if (result != length) {
    std::cout << "Game file loading failed!\n";
    std::cout << "Result: " << result << "!="
              << "Length of file:  " << length << std::endl;
    exit(EXIT_FAILURE);
  }
  std::cout << "Loading Game file to buffer...Sucess!\n";

  if ((0xFFF - 0x200) > length) {
    for (int i = 0; i < length; i++) {
      cpu->mem[i + 0x200] = buffer[i];
    }
  }

  romFile.close();
  delete[] buffer;

  return 0;
}

int chip8::readInstruction(chip8 *cpu) {

  cpu->opcode = cpu->mem[cpu->pc] << 8 | cpu->mem[cpu->pc + 1];

  std::cout << "opcode: " << std::hex << cpu->opcode << std::endl;

  switch (cpu->opcode & 0xF000) {

  case 0x0000: // opcode1NNN(cpu->opcode); maybe add functions for each
               // opcode/instruction?

    switch (cpu->opcode & 0x000F) {

    case 0x0000:
      memset(cpu->display, 0, TOTAL_PIXELS);
      cpu->drawFlag = true;
      cpu->pc += 2;
      break;

    case 0x000E:
      --cpu->sp;
      cpu->pc = cpu->stack[cpu->sp]; // maybe fix this to be proper pointers
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
    cpu->stack[cpu->sp] = cpu->pc;
    cpu->sp++;
    cpu->pc = cpu->opcode & 0x0FFF;
    break;

  case 0x3000:
    if (cpu->V[(cpu->opcode & 0x0F00) >> 8] == (cpu->opcode & 0x00FF))
      cpu->pc += 2;
    cpu->pc += 2;
    break;

  case 0x4000:
    if (cpu->V[(cpu->opcode & 0x0F00) >> 8] != (cpu->opcode & 0x00FF))
      cpu->pc += 2;
    cpu->pc += 2;
    break;
  case 0x5000:
    if (cpu->V[(cpu->opcode & 0x0F00) >> 8] == V[(cpu->opcode & 0x00F0) >> 4])
      cpu->pc += 2;
    cpu->pc += 2;
    break;
  case 0x6000:
    cpu->V[(cpu->opcode & 0x0F00) >> 8] = cpu->opcode & 0x00FF;
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
      cpu->V[(cpu->opcode & 0x0F00) >> 8] = cpu->V[(cpu->opcode & 0x00F0) >> 4];
      cpu->pc += 2;
      break;

    case 0x0001:
      cpu->V[(cpu->opcode & 0x0F00) >> 8] |=
          cpu->V[(cpu->opcode & 0x00F0) >> 4];
      cpu->pc += 2;
      break;
    case 0x0002:
      cpu->V[(cpu->opcode & 0x0F00) >> 8] &=
          cpu->V[(cpu->opcode & 0x00F0) >> 4];
      cpu->pc += 2;
      break;
    case 0x0003:
      cpu->V[(cpu->opcode & 0x0F00) >> 8] ^=
          cpu->V[(cpu->opcode & 0x00F0) >> 4];
      cpu->pc += 2;
      break;
    case 0x0004: {
      uint16_t sum = (cpu->V[(cpu->opcode & 0x0F00) >> 8] +
                      cpu->V[(cpu->opcode & 0x00F0) >> 4]);

      cpu->V[(cpu->opcode & 0x0F00) >> 8] = sum & 0xFF;
      cpu->V[0xF] = sum > 0xFF ? 1 : 0;
      cpu->pc += 2;

      break;
    }
    case 0x0005: {

      uint8_t VxValue = cpu->V[(cpu->opcode & 0x0F00) >> 8];
      uint8_t VyValue = cpu->V[(cpu->opcode & 0x00F0) >> 4];
      uint8_t result = VxValue - VyValue;

      cpu->V[0xF] = VxValue >= VyValue ? 1 : 0;

      cpu->V[(cpu->opcode & 0x0F00) >> 8] = result;

      cpu->pc += 2;
      break;
    }

    case 0x0006: {
      uint8_t masked_bit = cpu->V[(cpu->opcode & 0x0F00) >> 8] & 0x01;

      // mask least significant bit out to grab it
      cpu->V[(cpu->opcode & 0x0F00) >> 8] >>= 1;
      cpu->V[0xF] = masked_bit;
      cpu->pc += 2;

      break;
    }
    case 0x0007:

      cpu->V[(cpu->opcode & 0x0F00) >> 8] =
          cpu->V[(cpu->opcode & 0x00F0) >> 4] -
          cpu->V[(cpu->opcode & 0x0F00) >> 8];
      if (cpu->V[(cpu->opcode & 0x0F00) >> 8] <=
          (cpu->V[(cpu->opcode & 0x00F0) >> 4]))
        cpu->V[0xF] = 1;
      else
        cpu->V[0xF] = 0;

      cpu->pc += 2;

      break;

    case 0x000E: {

      // cpu->V[(cpu->opcode & 0x0F00) >> 8] = cpu->V[(cpu->opcode & 0x00F0) >>
      // 4];
      uint8_t shift = cpu->V[(cpu->opcode & 0x0F00) >> 8] >> 7;

      cpu->V[(cpu->opcode & 0x0F00) >> 8] <<= 1;
      cpu->V[0xF] = shift;
      cpu->pc += 2;

      break;
    }
    default:

      break;
    }
    // if (cpu->V[(cpu->opcode & 0x0F00) >> 8] == cpu->opcode & 0x00FF)
    // cpu->V[(cpu->opcode & 0x0F00) >> 8] += cpu->opcode & 0x00FF;
    // cpu->pc+=2;
    break;

  case 0x9000:

    if (cpu->V[(cpu->opcode & 0x0F00) >> 8] !=
        (cpu->V[(cpu->opcode & 0x00F0) >> 4]))
      cpu->pc += 2;

    cpu->pc += 2;

    break;

  case 0xA000:
    cpu->index_ = cpu->opcode & 0x0FFF;
    cpu->pc += 2;
    break;
  case 0xB000:
    cpu->pc = (cpu->opcode & 0x0FFF) + cpu->V[0];
    break;
  case 0xC000:
    cpu->V[(cpu->opcode & 0x0F00) >> 8] =
        (rand() % 0xFF) & (cpu->opcode & 0x00FF);
    cpu->pc += 2;
    break;
  case 0xD000: {

    uint8_t x = cpu->V[(cpu->opcode & 0x0F00) >> 8];
    uint8_t y = cpu->V[(cpu->opcode & 0x00F0) >> 4];
    uint8_t height = cpu->opcode & 0x000F;
    uint8_t pixel;

    cpu->V[0xF] = 0;

    for (int y_cord = 0; y_cord < height; y_cord++) {
      pixel = cpu->mem[cpu->index_ + y_cord];
      for (int x_cord = 0; x_cord < 8; x_cord++) {
        if ((pixel & (0x80 >> x_cord)) != 0) {
          if (cpu->display[(x + x_cord + ((y + y_cord) * WIDTH))] == 1) {
            cpu->V[0xF] = 1;
          }
          cpu->display[(x + x_cord + ((y + y_cord) * WIDTH))] ^= 1;
        }
      }
    }
    cpu->drawFlag = true;
    cpu->pc += 2;
  } break;
  case 0xE000:
    switch (cpu->opcode & 0x00FF) {
    case 0x009E:
      if (cpu->keyboard[cpu->V[(cpu->opcode & 0x0F00) >> 8]] != 0)
        cpu->pc += 4;
      else
        cpu->pc += 2;

      break;
    case 0x00A1:
      if (cpu->keyboard[cpu->V[(cpu->opcode & 0x0F00) >> 8]] == 0)
        cpu->pc += 4;
      else
        cpu->pc += 2;

      break;

    default:
      std::cout << "Unknown cpu->opcode [0xE000]: " << cpu->opcode;
    }
    break;
  case 0xF000:
    switch (cpu->opcode & 0x00FF) {

    case 0x0007:
      cpu->V[(cpu->opcode & 0x0F00) >> 8] = cpu->delay_timer;
      cpu->pc += 2;
      break;
    case 0x000A: {

      for (int i = 0; i < 16; ++i) {
        if (cpu->keyboard[i] != 0) {
          cpu->V[(cpu->opcode & 0x0F00) >> 8] = i;
        }
      }

      cpu->pc += 2;
    } break;

    case 0x0015:
      cpu->delay_timer = cpu->V[(cpu->opcode & 0x0F00) >> 8];
      cpu->pc += 2;
      break;

    case 0x0018:
      cpu->sound_timer = cpu->V[(cpu->opcode & 0x0F00) >> 8];
      cpu->pc += 2;
      break;

    case 0x001E:

      cpu->index_ += cpu->V[(cpu->opcode & 0x0F00) >> 8];

      if (cpu->index_ + cpu->V[(cpu->opcode & 0x0F00) >> 8] > 0xFFF)

        cpu->V[0xF] = 1;
      else
        cpu->V[0xF] = 0;

      cpu->pc += 2;
      break;

    case 0x0029:
      cpu->index_ = FONTSTART + cpu->V[(cpu->opcode & 0x0F00) >> 8] * 0x5;
      cpu->pc += 2;
      break;

    case 0x0030:
      cpu->index_ = cpu->V[(cpu->opcode & 0x0F00) >> 8] * 0x5;
      cpu->pc += 2;
      break;

    case 0x0033:
      cpu->mem[cpu->index_] = cpu->V[(cpu->opcode & 0x0F00) >> 8] / 100;
      cpu->mem[cpu->index_ + 1] =
          (cpu->V[(cpu->opcode & 0x0F00) >> 8] / 10) % 10;
      cpu->mem[cpu->index_ + 2] =
          (cpu->V[(cpu->opcode & 0x0F00) >> 8] % 100) % 10;
      cpu->pc += 2;
      break;

    case 0x0055:
      for (int i = 0; i <= ((cpu->opcode & 0x0F00) >> 8); i++)
        cpu->mem[cpu->index_ + i] = cpu->V[i];

      cpu->pc += 2;

      break;

    case 0x0065:
      for (int i = 0; i <= ((cpu->opcode & 0x0F00) >> 8); i++)
        cpu->V[i] = cpu->mem[cpu->index_ + i];

      cpu->pc += 2;

      break;

    default:
      std::cout << "Error: unknown cpu->opcode [0xF000]: " << std::hex
                << cpu->opcode << std::endl;
    }
    break;

  default:
    std::cout << "Error: unknown cpu->opcode " << cpu->opcode;
  }
  if (cpu->delay_timer > 0)
    --cpu->delay_timer;
  if (cpu->sound_timer > 0) {
    if (cpu->sound_timer == 1)
      std::cout << "BEEEP!\n";
    --cpu->sound_timer;
  }

  return 0;
}
