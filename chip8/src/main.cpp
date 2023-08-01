#include "chip8.h"
#include "gfx.h"
#include <iostream>
#include <string>

using namespace std;

char *mem[4096];

chip8 cpu;
gfx gfx;
int main(int argc, char **argv) {

  if (argc < 2) {
    cout << "Usage ./chip8 <path to rom>" << endl;
    exit(EXIT_FAILURE);
  }

  gfx.gfxInit();
  cpu.initCPU(&cpu);
  cpu.loadROM(argv[1]);
  cpu.readInstruction(&cpu);
  gfx.gfxLoop(argv[1], &cpu);
  gfx.gfxClean();

  return 0;
}
