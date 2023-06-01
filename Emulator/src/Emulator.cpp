#include "../include/Emulator.hpp"
#include <fstream>

Emulator::Emulator() {
    PC = START_ADDR;
}

void Emulator::loadROM(const char* fileName) {
    std::ifstream file(fileName);

    if (file.is_open()) {
        

        file.seekg(0, std::ios::end); // go to the begining of file and fill the buffer
        size_t size = file.tellg();
        char* buffer = new char[size];
        file.seekg(0);
        file.read(&buffer[0], size);

        file.close();

        auto hexVals = extractHexValues(buffer);

        for (int i = 0; i < hexVals.size(); i++) {
            memory[START_ADDR + i] = hexVals[i];
        }

        instructionSize = hexVals.size();

        delete []buffer;


        std::cout << "ROM loaded successfully!" << std::endl;
    }
}


void Emulator::opcode_LD() {
    uint8_t Rx = (instruction & 0x0F00) >> 8;
    uint8_t imm = (instruction & 0x00FF);

    registers[Rx] = imm;
}

void Emulator::opcode_MV() {
    uint8_t Rx = (instruction & 0x0F00) >> 8;
    uint8_t Ry = (instruction & 0x00F0) >> 4;

    registers[Rx] = registers[Ry];
}

void Emulator::opcode_ADD() {
    uint8_t Rx = (instruction & 0x0F00) >> 8;
    uint8_t Ry = (instruction & 0x00F0) >> 4;

    registers[Rx] += registers[Ry];
}

void Emulator::opcode_SUB() {
    uint8_t Rx = (instruction & 0x0F00) >> 8;
    uint8_t Ry = (instruction & 0x00F0) >> 4;

    registers[Rx] -= registers[Ry];
}

void Emulator::opcode_MULT() {
    uint8_t Rx = (instruction & 0x0F00) >> 8;
    uint8_t Ry = (instruction & 0x00F0) >> 4;

    registers[Rx] *= registers[Ry];
}

void Emulator::opcode_DIV() {
    uint8_t Rx = (instruction & 0x0F00) >> 8;
    uint8_t Ry = (instruction & 0x00F0) >> 4;

    registers[Rx] /= registers[Ry];
}

void Emulator::opcode_MOD() {
    uint8_t Rx = (instruction & 0x0F00) >> 8;
    uint8_t Ry = (instruction & 0x00F0) >> 4;

    RF = registers[Rx] % registers[Ry];
}

void Emulator::opcode_SKP() {
    uint8_t Rx = (instruction & 0x0F00) >> 8;
    uint8_t Ry = (instruction & 0x00F0) >> 4;

    if (Rx == Ry) {
        PC += 1;
    }
}

void Emulator::opcode_SNE() {
    uint8_t Rx = (instruction & 0x0F00) >> 8;
    uint8_t Ry = (instruction & 0x00F0) >> 4;

    if (Rx != Ry) {
        PC += 1;
    }
}

void Emulator::opcode_J() {
    uint16_t addr = (instruction & 0x0FFF);
    PC = addr;
}

void Emulator::opcode_CALL() {
    uint16_t addr = (instruction & 0x0FFF);
    stack[SP] = PC;
    ++SP;

    PC = addr;
}

void Emulator::opcode_RET() {
    PC = stack[--SP];
}

void Emulator::opcode_LA() {
    uint16_t addr = (instruction & 0x0FFF);
    RM = addr;
}

void Emulator::opcode_SLA() {
    uint8_t Rx = (instruction & 0x0F00) >> 8;
    uint8_t Ry = (instruction & 0x00F0) >> 4;

    registers[Rx] = registers[Rx] << registers[Ry];

    RF = (registers[Rx] >> 7) & 1;
}

void Emulator::opcode_SRA() {
    uint8_t Rx = (instruction & 0x0F00) >> 8;
    uint8_t Ry = (instruction & 0x00F0) >> 4;

    registers[Rx] = registers[Rx] >> registers[Ry];

    RF = (registers[Rx] >> 7) & 1;
}

void Emulator::opcode_WA() {
    uint8_t Rx = (instruction & 0x0F00) >> 8;
    uint8_t addr = (instruction & 0x00FF);

    memory[addr] = Rx;
}

void Emulator::cycle() {
    instruction = memory[PC];
    uint8_t opcode = (instruction & 0xF000) >> 12;

    switch (opcode)
    {
    case 0x0:
        opcode_LD();
        break;
    case 0x1:
        opcode_MV();
        break;
    case 0x2:
        opcode_ADD();
        break;
    case 0x3:
        opcode_SUB();
        break;
    case 0x4:
        opcode_MULT();
        break;
    case 0x5:
        opcode_DIV();
        break;
    case 0x6:
        opcode_MOD();
        break;
    case 0x7:
        opcode_SKP();
        break;
    case 0x8:
        opcode_SNE();
        break;
    case 0x9:
        opcode_J();
        break;
    case 0xA:
        opcode_CALL();
        break;
    case 0xB:
        opcode_RET();
        break;
    case 0xC:
        opcode_LA();
        break;
    case 0xD:
        opcode_SRA();
        break;
    case 0xE:
        opcode_SLA();
        break;
    case 0xF:
        opcode_WA();
        break;                                                                                                                    
    default:
        throw std::runtime_error("Invalid opcode!");
        break;
    }

    PC++;
}

void Emulator::printAllRegisters() {
    int i = 1;
    for (uint8_t num: registers) {
        std::cout << "R" << i << ": " << uint16ToHexString(num) << std::endl << "\n";
        i++;
    }
    std::cout << "RM" << ": " << uint16ToHexString(RM) << std::endl << "\n";
    std::cout << "RD" << ": " << uint16ToHexString(RD) << std::endl << "\n";
    std::cout << "RS" << ": " << uint16ToHexString(RS) << std::endl << "\n";
    std::cout << "RF" << ": " << uint16ToHexString(RF) << std::endl << "\n";
}

void Emulator::emulate() {
    for (int i = 0; i < instructionSize; i++) {
        cycle();
    }
}