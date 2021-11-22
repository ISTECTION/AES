#ifndef _UTILITES_HPP_
#define _UTILITES_HPP_

#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#include <conio.h>

///‿︵‿︵‿︵‿︵‿︵‿︵‿︵‿︵‿︵‿ UTILITES ‿︵‿︵‿︵‿︵‿︵‿︵‿︵‿︵‿︵‿ ///

std::vector<uint8_t> read(std::ifstream &fin) {
    std::string text, line;
    while (std::getline(fin, line))
        text += line;

    std::vector<uint8_t> T(text.size());
    for (size_t pos = 0; pos < text.size(); pos++)
        T[pos] = text[pos];
    return T;
}

std::string enterPassword() {
    std::string password;
    for (char c = _getch(); c != '\r'; c = _getch()) {
        password.push_back(c);
        _putch('*');
    }
    std::cout << std::endl;
    return password;
}

std::vector<uint8_t> toVector(const std::string &str) {
    std::vector<uint8_t> vec;
    vec.resize(str.size());

    for (size_t i = 0; i < vec.size(); i++)
        vec[i] = str[i];
    return vec;
}

///‿︵‿︵‿︵‿︵‿︵‿︵‿︵‿︵‿︵‿ UTILITES ‿︵‿︵‿︵‿︵‿︵‿︵‿︵‿︵‿︵‿ ///

#endif // _UTILITES_HPP_