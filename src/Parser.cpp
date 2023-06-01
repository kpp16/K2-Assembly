#include "../include/Parser.hpp"
#include <iostream>

const void Parser::openFile(std::string &filePath) {
    std::ifstream input(filePath);

    input.seekg(0, std::ios::end);
    size_t size = input.tellg();
    std::string buffer(size, ' ');
    input.seekg(0);
    input.read(&buffer[0], size);

    programData = buffer;    
}

Parser::Parser(std::string &filePath) {
    openFile(filePath);
    currentIdx = 0;
    ld   = std::make_unique<ListNode>(Types::OPCODE, "LD");
    mv   = std::make_unique<ListNode>(Types::OPCODE, "MV");
    add   = std::make_unique<ListNode>(Types::OPCODE, "ADD");
    sub   = std::make_unique<ListNode>(Types::OPCODE, "SUB");
    mult   = std::make_unique<ListNode>(Types::OPCODE, "MULT");
    div   = std::make_unique<ListNode>(Types::OPCODE, "DIV");
    mod   = std::make_unique<ListNode>(Types::OPCODE, "MOD");
    skp   = std::make_unique<ListNode>(Types::OPCODE, "SKP");
    sne   = std::make_unique<ListNode>(Types::OPCODE, "SNE");
    j   = std::make_unique<ListNode>(Types::OPCODE, "J");
    call   = std::make_unique<ListNode>(Types::OPCODE, "CALL");
    ret   = std::make_unique<ListNode>(Types::OPCODE, "RET");
    la   = std::make_unique<ListNode>(Types::OPCODE, "LA");
    sra   = std::make_unique<ListNode>(Types::OPCODE, "SRA");
    sla   = std::make_unique<ListNode>(Types::OPCODE, "SLA");
    wa   = std::make_unique<ListNode>(Types::OPCODE, "WA");
        

    insert(ld, "", Types::REGISTER);
    insert(ld, "", Types::NUMBER, Types::ADDRESS);

    insert(mv, "", Types::REGISTER);
    insert(mv, "", Types::REGISTER);
    
    insert(add, "", Types::REGISTER);
    insert(add, "", Types::REGISTER);

    insert(sub, "", Types::REGISTER);
    insert(sub, "", Types::REGISTER);

    insert(mult, "", Types::REGISTER);
    insert(mult, "", Types::REGISTER);

    insert(div, "", Types::REGISTER);
    insert(div, "", Types::REGISTER);

    insert(mod, "", Types::REGISTER);
    insert(mod, "", Types::REGISTER);

    insert(skp, "", Types::REGISTER);
    insert(skp, "", Types::REGISTER);

    insert(sne, "", Types::REGISTER);
    insert(sne, "", Types::REGISTER);

    insert(j, "", Types::ADDRESS);

    insert(call, "", Types::ADDRESS);

    insert(la, "", Types::ADDRESS);

    insert(sra, "", Types::REGISTER);
    insert(sla, "", Types::REGISTER);

    insert(wa, "", Types::REGISTER);
    insert(wa, "", Types::ADDRESS);    
}

std::string Parser::getNextToken() {
    while ((programData[currentIdx] == ' ' ||
           programData[currentIdx] == '\n') &&
           currentIdx < programData.size()) {

        currentIdx++;
    }

    if (currentIdx >= programData.size()) {
        return "~EOF~";
    }

    std::string res = "";

    while (programData[currentIdx] != ' ' &&
           programData[currentIdx] != '\n' &&
           currentIdx < programData.size()) {

        res += programData[currentIdx];
        currentIdx++;
    }

    return res;
}

const Types Parser::getTypeFromToken(std::string &token) {

    if (token == "~EOF~") {
        return Types::EndOFFile;
    }

    if (registers.find(token) != registers.end()) {
        return Types::REGISTER;
    } else if (opcodes.find(token) != opcodes.end()) {
        return Types::OPCODE;
    } else if (is_hex_notation(token)) {
        return Types::ADDRESS;
    } else if (is_number(token)) {
        return Types::NUMBER; // also return Address because of hex conversion later
    } else {
        return Types::INVALID_TYPE;
    }
}

const std::string Parser::handleOpcode(const std::string &token) {

    std::map<std::string, ListNode*> opcodeSyntaxTable = {

        {"LD", ld.get()},
        {"MV", mv.get()},
        {"ADD", add.get()},
        {"SUB", sub.get()},
        {"MULT", mult.get()},
        {"DIV", div.get()},
        {"MOD", mod.get()},
        {"SKP", skp.get()},
        {"SNE", sne.get()},
        {"J", j.get()},
        {"CALL", call.get()},
        {"RET", ret.get()},
        {"LA", la.get()},
        {"SRA", sra.get()},
        {"SLA", sla.get()},
        {"WA", wa.get()}
    };

    std::string currentNum = "0x";

    ListNode*cur = opcodeSyntaxTable[token];

    std::string movingToken = token;
    Types movingType = getTypeFromToken(movingToken);

    while (cur != nullptr) {
        std::string curToken = cur->value;
        Types curType = cur->type;
        Types curType2 = cur->type2;

        if ((curToken != "" && curToken == movingToken && curType == movingType) || 
            (curToken == "" && (curType == movingType || curType2 == movingType))) {

            if (movingType == Types::NUMBER) {
                currentNum += uint8ToHex(std::stoi(movingToken));
            } else if (movingType == Types::ADDRESS) {
                movingToken = movingToken.substr(2);
                currentNum += movingToken;
            } else if (movingType == Types::OPCODE){
                auto num = uint8ToHex(opcodeTable[movingToken]);
                currentNum += num;
            } else if (movingType == Types::REGISTER) {
                auto num = uint8ToHex(registerTable[movingToken]);
                currentNum += num;
            } else {
                throw std::runtime_error("Invalid syntax: Invalid keyword/symbol");
            }

            if (cur->next == nullptr) {
                break;
            }

            movingToken = getNextToken();
            movingType = getTypeFromToken(movingToken);
            cur = cur->next.get();

        } else {
            throw std::runtime_error("Invalid Syntax: Opcode process error");
        }
    }

    return currentNum;
}

const void Parser::interpret() {
    std::string token = "";
    std::string finalNum = "";

    while ((token = getNextToken()) != "") {
        Types type = getTypeFromToken(token);

        if (type == Types::OPCODE) {
            finalNum += handleOpcode(token) + "\n";
        } else if (type == Types::EndOFFile) {
            break;
        } else {
            throw std::runtime_error("Invalid syntax: Not a valid Opcode");
        }
    }

    std::cout << "Final string:\n" << finalNum << std::endl;
}