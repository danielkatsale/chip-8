#include "chip8.h"
#include <SDL3/SDL_render.h>

chip8::chip8()
{
<<<<<<< HEAD
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

  for (int i = 0; i < FONTSETSIZE; i++)
  {
    mem[FONTSTART + i] = fontset[i];
  }
=======
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

	for (int i = 0; i < FONTSETSIZE; i++)
	{
		mem[FONTSTART + i] = fontset[i];
	}
>>>>>>> fc1a0af (Finished readInstruction for chip8 emulation)
}
chip8::~chip8() {}

// read in rom;put into memory;fetch and decode opcode until
// termination;
int chip8::loadROM(std::string romFileName)
{

<<<<<<< HEAD
  std::ifstream romFile(romFileName, std::ios::in | std::ios::binary);
  std::ifstream::pos_type pos = romFile.tellg();
  int length = pos;

  char *buffer = new char[length];
  SDL_RenderClear(renderer);
  screen = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888,
                             SDL_TEXTUREACCESS_STREAMING, 16 * 64, 16 * 32);

  while (!romFile.eof())
  {
    romFile.seekg(0, std::ios::beg);
    romFile.read(buffer, length);
  }

  uint8_t *buffer2 = (uint8_t *)buffer;

  if ((0xFFF - 0x200) > length)
  {
    for (int i = 0; i < length; i++)
    {
      mem[i + 0x200] = buffer[i];
    }
  }

  return 0;
=======
	std::ifstream romFile(romFileName, std::ios::in | std::ios::binary);
	std::ifstream::pos_type pos = romFile.tellg();
	int length = pos;

	char *buffer = new char[length];
	SDL_RenderClear(renderer);
	screen = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888,
							   SDL_TEXTUREACCESS_STREAMING, 16 * 64, 16 * 32);

	while (!romFile.eof())
	{
		romFile.seekg(0, std::ios::beg);
		romFile.read(buffer, length);
	}

	uint8_t *buffer2 = (uint8_t *)buffer;

	if ((0xFFF - 0x200) > length)
	{
		for (int i = 0; i < length; i++)
		{
			mem[i + 0x200] = buffer[i];
		}
	}

	romFile.close();
	free(buffer2);

	return 0;
>>>>>>> fc1a0af (Finished readInstruction for chip8 emulation)
}





int chip8::readInstruction(chip8 *cpu)
{

<<<<<<< HEAD
  cpu->opcode = mem[cpu->pc] << 8 | mem[cpu->pc + 1];

  switch (cpu->opcode & 0xF000)
  {

  case 0x0000: // opcode1NNN(cpu->opcode); maybe add functions for each
               // opcode/instruction?

    switch (cpu->opcode & 0x000F)
    {

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
    if (cpu->V[(cpu->opcode & 0x0F00) >> 8] == cpu->opcode & 0x00FF)
      cpu->pc += 2;
    break;
  case 0x7000:
    //if (cpu->V[(cpu->opcode & 0x0F00) >> 8] == cpu->opcode & 0x00FF)
      cpu->V[(cpu->opcode & 0x0F00) >> 8] += cpu->opcode & 0x00FF;
      cpu->pc+=2;
      break;

  default:
    break;
  }

  return 0;
}
=======
	cpu->opcode = mem[cpu->pc] << 8 | mem[cpu->pc + 1];

	switch (cpu->opcode & 0xF000)
	{

	case 0x0000: // opcode1NNN(cpu->opcode); maybe add functions for each
				 // opcode/instruction?

		switch (cpu->opcode & 0x000F)
		{

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
		switch (cpu->opcode & 0x000F)
		{

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

		if (cpu->V[(cpu->opcode & 0x0F00) >> 8] != (cpu->V[(cpu->opcode & 0x00F0) >> 4]))
			pc += 2;

		pc += 2;

		break;

	case 0xA000:
		cpu->index_ = cpu->opcode & 0x0FFF;
		cpu->pc += 2;
		break;
	case 0xB000:
		cpu->pc = (cpu->opcode & 0x0FFF) + cpu->V[0];
		break;
	case 0xC000:
		cpu->V[(cpu->opcode & 0x0F00) >> 8] = (rand() % 0xFF) & (cpu->opcode & 0x00FF);
		cpu->pc += 2;
		break;
	case 0xD000:
	{

		uint16_t x = cpu->V[(cpu->opcode & 0x0F00) >> 8];
		uint16_t y = cpu->V[(cpu->opcode & 0x00F0) >> 4];
		uint16_t height = cpu->opcode & 0x000F;
		uint16_t pixel;

		for (int y_cord = 0; y_cord < height; y_cord++)
		{
			pixel = mem[cpu->index_ + y_cord];
			for (int x_cord = 0; x_cord < 8; x_cord++)
			{
				if ((pixel & (0x80 >> x_cord)) != 0)
				{
					if (gfx[(x + x_cord + ((y + y_cord) * 64))] == 1)
					{
						cpu->V[0xF] = 1;
					}
					gfx[x + x_cord + ((y + y_cord) * 64)] ^= 1;
				}
			}
		}
		cpu->drawFlag = true;
		cpu->pc += 2;
	}
	break;
	case 0xE000:
		switch (cpu->opcode & 0x00FF)
		{
		case 0x009E:
			if (keyboard[cpu->V[(cpu->opcode & 0x0F00) >> 8]] != 0)
				pc += 4;
			else
				pc += 2;

			break;
		case 0x00A1:
			if (keyboard[cpu->V[(cpu->opcode & 0x0F00) >> 8]] == 0)
				pc += 4;
			else
				pc += 2;

			break;

		default:
			std::cout << "Unknown cpu->opcode [0xE000]: " << cpu->opcode;
		}
		break;
	case 0xF000:
		switch (cpu->opcode & 0x00FF)
		{
		case 0x0007:
			cpu->V[(cpu->opcode & 0x0F00) >> 8] = cpu->delay_timer;
			cpu->pc += 2;
			break;
		case 0x000A:
		{

			for (int i = 0; i < 16; ++i)
			{
				if (keyboard[i] != 0)
				{
					cpu->V[(cpu->opcode & 0x0F00) >> 8] = i;
					
				}
			}

			cpu->pc += 2;
		}
		break;

		case 0x0015:
			cpu->delay_timer = cpu->V[(cpu->opcode & 0x0F00) >> 8];
			cpu->pc += 2;
			break;
		case 0x0018:
			cpu->sound_timer = cpu->V[(cpu->opcode & 0x0F00) >> 8];
			cpu->pc += 2;
			break;
		case 0x001E:
			if (cpu->index_ + cpu->V[(cpu->opcode & 0x0F00) >> 8] > 0xFFF)

				cpu->V[0xF] = 1;
			else
				cpu->V[0xF] = 0;

			cpu->index_ += cpu->V[(cpu->opcode & 0x0F00) >> 8];
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
			mem[cpu->index_] = cpu->V[(cpu->opcode & 0x0F00) >> 8] / 100;
			mem[cpu->index_ + 1] = (cpu->V[(cpu->opcode & 0x0F00) >> 8] / 10) % 10;
			mem[cpu->index_ + 2] = (cpu->V[(cpu->opcode & 0x0F00) >> 8] % 100) % 10;
			cpu->pc += 2;
			break;

		case 0x0055:
			for (int i = 0; i <= ((cpu->opcode & 0x0F00) >> 8); i++)
				mem[cpu->index_ + i] = cpu->V[i];

			cpu->pc += 2;

			break;

		case 0x0065:
			for (int i = 0; i <= ((cpu->opcode & 0x0F00) >> 8); i++)
				cpu->V[i] = mem[cpu->index_ + i];

			cpu->pc += 2;

			break;

		default:
			std::cout << "Error: unknown cpu->opcode [0xF000] " << cpu->opcode;
		}
		break;

	default:
		std::cout << "Error: unknown cpu->opcode " << cpu->opcode;
	}
	if (cpu->delay_timer > 0)
		--cpu->delay_timer;
	if (cpu->sound_timer > 0)
	{
		if (cpu->sound_timer == 1)
			std::cout << "BEEEP!\n";
		--cpu->sound_timer;
	}

	return 0;
}
>>>>>>> fc1a0af (Finished readInstruction for chip8 emulation)
