#include <iostream>
#include <fstream>
#include <cstdlib>

#include "Header.h"
#include "Readers.h"

using namespace std;

void passOne();
void passTwo();
int getIdxFromSymbolTable(string key);
void printSymbolTable();
void printMemoryMap();
void printWarningInRule4();
void checkRule7(ProgramText programText, UseList useList);
void buildMemoryMap(ProgramText programText, UseList useList);
void checkIsUsed(string symbol);
MemoryInfo processAddrMode_R (pair<char, int> codeCountPair, int moduleSize);
MemoryInfo processAddrMode_E (pair<char, int> codeCountPair, UseList useList);
MemoryInfo processAddrMode_I (pair<char, int> codeCountPair);
MemoryInfo processAddrMode_A (pair<char, int> codeCountPair);
TokenInfo getToken();
DefinitionList parseDefinitionList();
UseList parseUseList();
ProgramText parseProgramText();

const int MEMORY_SIZE = 512;

// For the file read
string fileName;
ifstream input;
char *mark;
char line[1024];
int lineNum = 0;
int lineOffset;
int preLineOffset; // for the offset in last line
int markLen = 0;

int totalInstruction = 0;

// Symbol Table
vector<Symbol> symbolTable;
vector<MemoryInfo> memoryMap;
vector<Rule7WarningInfo> rule7WarningInfos;
int moduleNum = 0;
int baseAddr = 0;

int main() {
    fileName = "/Users/yjeonlee/Desktop/OS_labs/Operating-Systems-Labs/lab1_Linker/inputs/input-1";
    //fileName = argv[1];
    passOne();
    passTwo();
    return 0;
}

void passOne() {
    input.open(fileName.c_str());
    if (!input) {
        cerr << "Cannot open file!" << endl;
        exit(1);
    }

    while (!input.eof()) {
        moduleNum++;

        DefinitionList defList = parseDefinitionList();
        UseList useList = parseUseList();
        ProgramText programText = parseProgramText();

        for (int i=0; i<defList.defCount; i++) {
            Symbol s;
            s.name = defList.defCountPairs.at(i).first;
            s.definedAddr = defList.defCountPairs.at(i).second;
            s.absoluteAddr = baseAddr + defList.defCountPairs.at(i).second;
            s.baseAddr = baseAddr;
            s.moduleNo = moduleNum;
            s.moduleSize = programText.codeCount;

            int idx = getIdxFromSymbolTable(defList.defCountPairs.at(i).first);
            if (idx == -1) {
                symbolTable.push_back(s);
            } else {
                symbolTable.at(idx).isMultiDefined = true;
            }
        }
        baseAddr += programText.codeCount;
    }
    printSymbolTable();
    input.close();
}

void passTwo() {
    input.open(fileName.c_str());
    if (!input) {
        cerr << "Cannot open file!" << endl;
        exit(1);
    }

    lineNum = 0;
    baseAddr = 0;
    moduleNum = 0;
    while (!input.eof()) {
        moduleNum++;

        DefinitionList defList = parseDefinitionList();
        UseList useList = parseUseList();
        ProgramText programText = parseProgramText();

        buildMemoryMap(programText, useList);
        // rule 7
        checkRule7(programText, useList);
        baseAddr += programText.codeCount;
    }
    printMemoryMap();
    printWarningInRule4();
    input.close();
}

void printMemoryMap() {
    cout << endl;
    cout << "Memory Map" << endl;

    for (int i=0; i<memoryMap.size(); i++) {
        if (memoryMap.at(i).errcode == 0) {
            printf("%03d: %04d\n", i, memoryMap.at(i).addr);
        } else if (memoryMap.at(i).errcode == 3) {
            printf("%03d: %04d", i, memoryMap.at(i).addr);
            cout << " Error: " << memoryMap.at(i).symForE << " is not defined; zero used" << endl;
        } else {
            printf("%03d: %04d", i, memoryMap.at(i).addr);
            __printErrorMsg(memoryMap.at(i).errcode);
        }

        if (((i+1) < memoryMap.size()) &&
            (((memoryMap.at(i).moduleNum+1) == memoryMap.at(i+1).moduleNum))
            || (i == (memoryMap.size()-1))) {
            for (int j=0; j<rule7WarningInfos.size(); j++) {
                if (memoryMap.at(i).moduleNum == rule7WarningInfos.at(j).moduleNum) {
                    cout << "Warning: Module " << rule7WarningInfos.at(j).moduleNum << ": " << rule7WarningInfos.at(j).unusedSymbol << " appeared in the uselist but was not actually used" << endl;
                }
            }
        }
    }
}

void printWarningInRule4() {
    cout << endl;
    for (int i=0; i<symbolTable.size(); i++) {
        if (!symbolTable.at(i).isUsed) {
            cout << "Warning: Module " << symbolTable.at(i).moduleNo << ": " << symbolTable.at(i).name << " was defined but never used" << endl;
        }
    }
}

void checkRule7(ProgramText programText, UseList useList) {
    int check[20] = {0,}; // max useList length is 16
    for (int i=0; i<programText.codeCount; i++) {
        if (programText.codeCountPairs.at(i).first == 'E') {
            int instr = programText.codeCountPairs.at(i).second;
            int operand = instr % 1000;
            check[operand] = 1;
        }
    }

    for (int i=0; i<useList.useCount; i++) {
        if (check[i]==0) {
            Rule7WarningInfo warningInfo;
            warningInfo.moduleNum = moduleNum;
            warningInfo.unusedSymbol = useList.symbols.at(i);
            rule7WarningInfos.push_back(warningInfo);
        }
    }
}

void checkIsUsed(string symbol) {
    int idx = getIdxFromSymbolTable(symbol);
    if (idx != -1) {
        symbolTable.at(idx).isUsed = true;
    }
}

void buildMemoryMap(ProgramText programText, UseList useList) {
    for (int i=0; i<programText.codeCount; i++) {
        MemoryInfo memInfo;
        char addrMode = programText.codeCountPairs.at(i).first;

        switch (addrMode) {
            case 'R':
                memInfo = processAddrMode_R(programText.codeCountPairs.at(i), programText.codeCount);
                break;
            case 'E':
                memInfo = processAddrMode_E(programText.codeCountPairs.at(i), useList);
                break;
            case 'I': // an immediate operand is unchanged.
                memInfo = processAddrMode_I(programText.codeCountPairs.at(i));
                break;
            case 'A':
                memInfo = processAddrMode_A(programText.codeCountPairs.at(i));
                break;
        }
        memoryMap.push_back(memInfo);
    }
}

MemoryInfo processAddrMode_R (pair<char, int> codeCountPair, int moduleSize) {
    MemoryInfo processed;
    int instr = codeCountPair.second;
    int opcode = instr / 1000;
    int operand = instr % 1000;

    // rule 11
    // what if rule 9 and rule 11 conflicts?
    // if I apply rule 11, it becomes 9999 and still need to check rule 9?
    if (opcode >= 10) {
        processed.addr = 9999;
        processed.errcode = 11;
        processed.moduleNum = moduleNum;
    } else {
        // TODO check moduleSize -> instruction size?
        if (operand > moduleSize) {
            processed.addr = baseAddr + opcode * 1000;
            processed.errcode = 9;
            processed.moduleNum = moduleNum;
        } else {
            processed.addr = baseAddr + instr;
            processed.errcode = 0;
            processed.moduleNum = moduleNum;
        }
    }
    return processed;
}

MemoryInfo processAddrMode_E (pair<char, int> codeCountPair, UseList useList) {
    MemoryInfo processed;
    int instr = codeCountPair.second;
    int opcode = instr / 1000;
    int operand = instr % 1000;

    if (opcode >= 10) {
        processed.addr = 9999;
        processed.errcode = 11;
        processed.moduleNum = moduleNum;
    } else if (operand >= useList.useCount) {
        processed.addr = instr;
        processed.errcode = 6;
        processed.moduleNum = moduleNum;
    } else {
        string key = useList.symbols.at(operand);
        // rule 4- checking
        checkIsUsed(key);
        // rule 3
        int idx = getIdxFromSymbolTable(key);
        if (idx == -1) {
            processed.addr = opcode * 1000;
            processed.errcode = 3;
            processed.moduleNum = moduleNum;
            processed.symForE = key;
        } else {
            processed.addr = opcode * 1000 + symbolTable.at(idx).absoluteAddr ;
            processed.errcode = 0;
            processed.moduleNum = moduleNum;
        }
    }
    return processed;
}

MemoryInfo processAddrMode_I (pair<char, int> codeCountPair) {
    MemoryInfo processed;
    int instr = codeCountPair.second;

    if (instr >= 10000) {
        processed.addr = 9999;
        processed.errcode = 10;
        processed.moduleNum = moduleNum;
    } else {
        processed.addr = codeCountPair.second;
        processed.errcode = 0;
        processed.moduleNum = moduleNum;
    }
    return processed;
}

MemoryInfo processAddrMode_A (pair<char, int> codeCountPair) {
    MemoryInfo processed;
    int instr = codeCountPair.second;
    int opcode = instr / 1000;
    int operand = instr % 1000;

    if (operand > MEMORY_SIZE) {
        processed.addr = opcode * 1000;
        processed.errcode = 8;
        processed.moduleNum = moduleNum;
    } else {
        processed.addr = instr;
        processed.errcode = 0;
        processed.moduleNum = moduleNum;
    }
    return processed;
}

int getIdxFromSymbolTable(string key) {
    for (int i=0; i<symbolTable.size(); i++) {
        if (symbolTable.at(i).name == key) {
            return i;
        }
    }
    return -1;
}

void printSymbolTable() {
    // rule 5
    for (int i=0; i<symbolTable.size(); i++) {
        if (symbolTable.at(i).definedAddr > symbolTable.at(i).moduleSize-1) {
            cout << "Warning: Module " << symbolTable.at(i).moduleNo << ": " << symbolTable.at(i).name << " too big "
                 << symbolTable.at(i).definedAddr << " (max=" << symbolTable.at(i).moduleSize-1 << ") assume zero relative" << endl;

            symbolTable.at(i).absoluteAddr = symbolTable.at(i).baseAddr;
        }
    }

    cout << "Symbol Table" << endl;
    for (int i=0; i<symbolTable.size(); i++) {
        if (symbolTable.at(i).isMultiDefined) {
            cout << symbolTable.at(i).name << "=" << symbolTable.at(i).absoluteAddr <<
                 " Error: This variable is multiple times defined; first value used" << endl;
        } else {
            cout << symbolTable.at(i).name << "=" << symbolTable.at(i).absoluteAddr << endl;
        }
    }
}

TokenInfo getToken() {
    TokenInfo result;

    if (mark != NULL) {
        result.token = mark;
        result.line = lineNum;

        lineOffset = mark - line + 1;
        result.offset = lineOffset;
        //cout << "1 lineNum: " << lineNum << "   offset: " << lineOffset << endl;
        markLen = strlen(mark);
        mark = strtok(NULL, delim);
    } else {
        input.getline(line, 1024);
        preLineOffset = lineOffset;
        lineOffset = 1;
        lineNum++;

        if (strlen(line) != 0) {
            mark = strtok(line, delim);
            lineOffset = mark - line + 1;
            //cout << "2 lineNum: " << lineNum << "   offset: " << lineOffset << endl;
            result.token = mark;
            result.line = lineNum;
            result.offset = lineOffset;

            markLen = strlen(mark);
            mark = strtok(NULL, delim);
        } else {
            if (!input.eof()) {
                markLen = 0;
                return getToken();
            } else {
                // end of file
                result.token = "EOF";
                // final position in the file
                result.line = lineNum-1;
                //cout << "preLineOffset: " << preLineOffset << " markLen: " << markLen << endl;
                result.offset = preLineOffset + markLen;
                input.close();
            }
        }
    }
    return result;
}

DefinitionList parseDefinitionList() {
    DefinitionList parsed;
    TokenInfo tokenInfo = getToken();
    int defCount = readInt(tokenInfo);

    if (defCount > 16) {
        __parseError(4, tokenInfo.line, tokenInfo.offset);
        exit(1);
    }

    parsed.defCount = defCount;
    for (int i=0; i < parsed.defCount; i++) {
        pair<string, int> defCountPair;
        defCountPair.first = readSymbol(getToken());
        defCountPair.second = readInt(getToken());
        parsed.defCountPairs.push_back(defCountPair);
    }
    return parsed;
}

UseList parseUseList() {
    UseList parsed;
    TokenInfo tokenInfo = getToken();
    int useCount = readInt(tokenInfo);

    if (useCount > 16) {
        __parseError(5, tokenInfo.line, tokenInfo.offset);
        exit(1);
    }

    parsed.useCount = useCount;
    for (int i=0; i < parsed.useCount; i++) {
        char* symbol = readSymbol(getToken());
        parsed.symbols.push_back(symbol);
    }
    return parsed;
}

ProgramText parseProgramText() {
    ProgramText parsed;
    TokenInfo tokenInfo = getToken();
    int codeCount = readInt(tokenInfo);
    totalInstruction += codeCount;

    if (totalInstruction > MEMORY_SIZE) {
        __parseError(6, tokenInfo.line, tokenInfo.offset);
        exit(1);
    }

    parsed.codeCount = codeCount;
    for (int i=0; i < parsed.codeCount; i++) {
        pair<char, int> codeCountPair;
        codeCountPair.first = readIAER(getToken());
        codeCountPair.second = readInt(getToken());
        parsed.codeCountPairs.push_back(codeCountPair);
    }
    return parsed;
}