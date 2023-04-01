#include "CPU.h"
#include "Memory.h"

int main()
{
    Memory memory("memorie.txt");
    CPU cpu;

    cpu.run_cycles(memory);

    return 0;
}
