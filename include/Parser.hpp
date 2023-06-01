#pragma once

#include <string>
#include <fstream>
#include <algorithm>
#include <iomanip>
#include <sstream>
#include <set>
#include <unordered_map>
#include <map>
#include <bitset>

enum Types {
    OPCODE,
    REGISTER,
    ADDRESS,
    NUMBER,
    INVALID_TYPE,
    EndOFFile,
};


class Parser {
    std::string programData;
    std::string interpretedData;
    size_t currentIdx;

    const void openFile(std::string &filePath);

    const bool is_hex_notation(std::string const& s) {
        return s.compare(0, 2, "0x") == 0
            && s.size() > 2
            && s.find_first_not_of("0123456789abcdefABCDEF", 2) == std::string::npos;
    }

    const bool is_number(const std::string &s) {
        return !s.empty() && std::find_if(s.begin(), 
            s.end(), [](unsigned char c) { return !std::isdigit(c); }) == s.end();
    }

    const std::string uint8ToBinaryString(uint8_t value) {
        std::bitset<8> bits(value);
        return bits.to_string();
    }

    const std::string hexToBinaryString(const std::string& hexString, Types type = Types::ADDRESS) {
        // Convert hexadecimal string to unsigned integer
        unsigned int intValue;
        std::stringstream ss;
        ss << std::hex << hexString;
        ss >> intValue;

        if (type == Types::REGISTER || type == Types::OPCODE) {
            std::bitset<4> bits(intValue);
            return bits.to_string();
        } else {
            std::bitset<8> bits(intValue);
            return bits.to_string();
        }
    }

    std::string binaryToHexString(const std::string& binaryString) {
        std::bitset<16> bits(binaryString);
        unsigned long long value = bits.to_ullong();

        // Convert unsigned long long to hexadecimal string
        std::stringstream ss;
        ss << std::hex << value;
        std::string hexString = ss.str();

        // Ensure the output string has the correct length
        if (hexString.length() % 2 != 0) {
            hexString.insert(0, "0");
        }

        return hexString;
    }    

    const std::string handleOpcode(const std::string &s);


    std::string uint8ToHex(uint8_t value) {
        std::stringstream stream;
        stream << std::setfill('0') << std::setw(2) << std::hex << static_cast<int>(value);
        return stream.str();
    }

    struct ListNode {
        Types type;
        Types type2;

        std::string value;
        std::vector<std::string> exclude = {"RM", "RD", "RS", "RF", "PC", "SP"};
        std::unique_ptr<ListNode> next;

        ListNode(Types inptype, std::string invalue, Types intype2 = Types::INVALID_TYPE): type(inptype), value(invalue) {
            next = nullptr;
            type2 = intype2;
        }

        ListNode() {
            value = "";
            next = nullptr;
            type = Types::INVALID_TYPE;
        }

        ~ListNode() {}
    };

    void insert(std::unique_ptr<ListNode> &head, std::string val,
        Types type, Types type2 = Types::INVALID_TYPE) {

        std::unique_ptr<ListNode> newNode = std::make_unique<ListNode>(type, val, type2);

        if (head == nullptr) {
            head = std::move(newNode);
        } else {
            ListNode* cur = head.get();
            while (cur->next != nullptr) {
                cur = cur->next.get();
            }
            cur->next = std::move(newNode);
        }
    }

    const std::set<std::string> opcodes = {
        "LD",
        "MV",
        "ADD",
        "SUB",
        "MULT",
        "DIV",
        "MOD",
        "SKP",
        "SNE",
        "J",
        "CALL",
        "RET",
        "LA",
        "SRA",
        "SLA",
        "WA"
    };

    std::map<std::string, uint8_t> opcodeTable = {
        {"LD", 0x0},
        {"MV", 0x1},
        {"ADD", 0x2},
        {"SUB", 0x3},
        {"MULT", 0x4},
        {"DIV", 0x5},
        {"MOD", 0x6},
        {"SKP", 0x7},
        {"SNE", 0x8},
        {"J", 0x9},
        {"CALL", 0xA},
        {"RET", 0xB},
        {"LA", 0xC},
        {"SRA", 0xD},
        {"SLA", 0xE},
        {"WA", 0xF}
    };

    std::set<std::string> registers = {
        "R1",
        "R2",
        "R3",
        "R4",
        "R5",
        "R6",
        "R7",
        "R8",
        "R9",
        "R10",
        "R11",
        "R12",
        "RD",
        "RS",
        "RF",
        "SP"
    };

    std::unordered_map<std::string, uint8_t> registerTable = {
        {"R1", 0x0},
        {"R2", 0x1},
        {"R3", 0x2},
        {"R4", 0x3},
        {"R5", 0x4},
        {"R6", 0x5},
        {"R7", 0x6},
        {"R8", 0x7},
        {"R9", 0x8},
        {"R10", 0x9},
        {"R11", 0xA},
        {"R12", 0xB},
        {"RD", 0xC},
        {"RS", 0xD},
        {"RF", 0xE},
        {"SP", 0xF},
    };    

    std::unique_ptr<ListNode> ld  ;
    std::unique_ptr<ListNode> mv  ;
    std::unique_ptr<ListNode> add ;
    std::unique_ptr<ListNode> sub ;
    std::unique_ptr<ListNode> mult;
    std::unique_ptr<ListNode> div ;
    std::unique_ptr<ListNode> mod ;
    std::unique_ptr<ListNode> skp ;
    std::unique_ptr<ListNode> sne ;
    std::unique_ptr<ListNode> j   ;
    std::unique_ptr<ListNode> call;
    std::unique_ptr<ListNode> ret ;
    std::unique_ptr<ListNode> la  ;
    std::unique_ptr<ListNode> sra ;
    std::unique_ptr<ListNode> sla ;
    std::unique_ptr<ListNode> wa  ;    


public:
    Parser(std::string &filePath);

    std::string getNextToken();

    const void interpret();

    const Types getTypeFromToken(std::string &token);

};