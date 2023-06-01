#include <iostream>
#include "include/Emulator.hpp"

int main(int, char**) {
    Emulator emulator;

    emulator.loadROM("a.out");

    emulator.emulate();

    emulator.printAllRegisters();

}
