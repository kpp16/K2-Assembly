#pragma once

#include <cstdint>
#include <vector>
#include <string>
#include <iostream>
#include <sstream>
#include <iomanip>

const uint16_t START_ADDR = 0x000;
const uint16_t END_ADDR = 0x400;

class Emulator {
    uint16_t memory[4096] {};
    uint8_t registers[12] {};
    uint16_t RM {};
    uint8_t RD {};
    uint8_t RS {};
    uint16_t RF {};
    uint16_t PC {};
    uint8_t SP{};

    uint16_t stack[16] {};
    uint16_t instruction {};

    int instructionSize;

    std::vector<uint16_t> extractHexValues(const char* buffer) {
        std::vector<uint16_t> hexValues;
        std::stringstream ss(buffer);

        std::string line;
        while (std::getline(ss, line)) {
            std::istringstream iss(line);
            uint16_t hexValue;
            while (iss >> std::hex >> hexValue) {
                hexValues.push_back(hexValue);
            }
        }

        return hexValues;
    }

    std::string uint16ToHexString(uint16_t value) {
        std::stringstream ss;
        ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(value);
        return ss.str();
    }    

    void opcode_LD();
    void opcode_MV();
    void opcode_ADD();
    void opcode_SUB();
    void opcode_MULT();
    void opcode_DIV();
    void opcode_MOD();
    void opcode_SKP();
    void opcode_SNE();
    void opcode_J();
    void opcode_CALL();
    void opcode_RET();
    void opcode_LA();
    void opcode_SRA();
    void opcode_SLA();
    void opcode_WA();

public:
    Emulator();

    void loadROM(const char* fileName);
    void cycle();
    void emulate();

    void printAllRegisters();

};