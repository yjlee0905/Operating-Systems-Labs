//
// Created by Yunjeon Lee on 2021/06/04.
//

#ifndef OPERATING_SYSTEMS_LABS_HEADER_H
#define OPERATING_SYSTEMS_LABS_HEADER_H

#include <vector>

using namespace std;

struct MemoryInfo {
    int addr;
    int moduleNum;
    int errcode = 0; // default: no error
    string symForE;
};

struct Rule7WarningInfo {
    int moduleNum;
    string unusedSymbol;
};

struct Symbol {
    basic_string<char> name; // TODO change type
    int definedAddr;
    int absoluteAddr;
    int baseAddr;
    bool isMultiDefined = false; // rule 2
    bool isUsed = false; // rule 4
    // information for module belongs to
    int moduleNo;
    int moduleSize;
};

struct TokenInfo {
    char* token;
    int line;
    int offset;
};

struct DefinitionList {
    int defCount;
    vector<pair<string, int> > defCountPairs;
};

struct UseList {
    int useCount;
    vector<string> symbols;
};

struct ProgramText {
    int codeCount;
    vector<pair<char, int> > codeCountPairs;
};

void __parseError (int errcode, int linenum, int lineoffset) { // TODO check parameters
    static char* errstr[] = {
            "NUM_EXPECTED",                 // Number expect, anything >= 2^30 is not a number either
            "SYM_EXPECTED",                 // Symbol Expected
            "ADDR_EXPECTED",                // Addressing Expected which is A/E/I/R
            "SYM_TOO_LONG",                 // Symbol Name is too long
            "TOO_MANY_DEF_IN_MODULE",       // > 16
            "TOO_MANY_USE_IN_MODULE",       // > 16
            "TOO_MANY_INSTR",               // total num_instr exceeds memory size (512)
    };
    printf("Parse Error line %d offset %d: %s\n", linenum, lineoffset, errstr[errcode]);
}

void __printErrorMsg (int errcode) {
    switch (errcode) {
        case 6:
            cout << " Error: External address exceeds length of uselist; treated as immediate" << endl;
            break;
        case 8:
            cout << " Error: Absolute address exceeds machine size; zero used" << endl;
            break;
        case 9:
            cout << " Error: Relative address exceeds module size; zero used" << endl;
            break;
        case 10:
            cout << " Error: Illegal immediate value; treated as 9999" << endl;
            break;
        case 11:
            cout << " Error: Illegal opcode; treated as 9999" << endl;
    }
}

#endif //OPERATING_SYSTEMS_LABS_HEADER_H