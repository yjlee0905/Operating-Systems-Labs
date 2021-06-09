//
// Created by Yunjeon Lee on 2021/06/04.
//

#ifndef OPERATING_SYSTEMS_LABS_READERS_H
#define OPERATING_SYSTEMS_LABS_READERS_H

#include <cstring>
#include <cmath>
#include "Header.h"

using namespace std;

const char delim[] = " \t\r\n\v\f";

bool isValidIntegers (char* num) {
    for (int i=0; i<strlen(num); i++) {
        if (num[i] < '0' || num[i] > '9') return false;
    }
    return true;
}

int readInt(TokenInfo tokenInfo) {
    if (tokenInfo.token == "EOF") return 0;
    if (isValidIntegers(tokenInfo.token)) {
        int parsedInt = 0;
        int digit = 1;
        for (int i = strlen(tokenInfo.token) - 1; i >= 0; i--) {
            parsedInt += (tokenInfo.token[i] - '0') * digit;
            digit *= 10;
        }
        // limit 2^30
        if (parsedInt < pow(2, 30)) {
            return parsedInt;
        }
    }
    __parseError(0, tokenInfo.line, tokenInfo.offset);
    exit(1);
}

bool isValidSymbol (TokenInfo tokenInfo) {
    if (!isalpha(tokenInfo.token[0])) return false;
    if (strlen(tokenInfo.token) > 16) {
        __parseError(3, tokenInfo.line, tokenInfo.offset);
        exit(1);
    }
    //if (strlen(tokenInfo.token) <0 || strlen(tokenInfo.token) > 16) return false;

    if (strlen(tokenInfo.token) > 1) {
        for (int i=1; i < strlen(tokenInfo.token); i++) {
            if (!isalpha(tokenInfo.token[i]) && !isdigit(tokenInfo.token[i])) return false;
        }
    }
    return true;
}

char* readSymbol(TokenInfo tokenInfo) {
    if (isValidSymbol(tokenInfo) && tokenInfo.token != "EOF") {
        return tokenInfo.token;
    }

    __parseError(1, tokenInfo.line, tokenInfo.offset);
    exit(1);
}

bool isValidAddressingMode (char* type) {
    if (strlen(type) != 1) return false;
    if (type[0] == 'R' || type[0] == 'E' || type[0] == 'I' || type[0] == 'A' ) return true;
    return false;
}

char readIAER(TokenInfo tokenInfo) {
    if (isValidAddressingMode(tokenInfo.token) && tokenInfo.token != "EOF") {
        return tokenInfo.token[0];
    }
    __parseError(2, tokenInfo.line, tokenInfo.offset);
    exit(1);
}

bool isFirstElemDelimiters (char* str) {
    for (int i=0; i<strlen(delim); i++) {
        if (str[0] == delim[i]) return true;
    }
    return false;
}

#endif //OPERATING_SYSTEMS_LABS_READERS_H