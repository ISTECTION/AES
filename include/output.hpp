#ifndef _OUTPUT_HPP_
#define _OUTPUT_HPP_

#define ENDLINE std::cout << std::endl

#include <iostream>
#include <vector>
#include <iomanip>

void printhex(const std::vector<uint8_t> &HEX, bool isUpper     = false,
                                               bool isSeparator = false) {

    #define UPPERCASE (isUpper     ? std::uppercase : std::nouppercase)
    #define SEPARATOR (isSeparator ? (i == 0 ? "" : " " ) : "" )

    for(size_t i = 0; i < HEX.size(); i++)
        std::cout << std::hex
                  << UPPERCASE
                  << SEPARATOR
                  << std::setw(2)
                  << std::setfill('0')
                  << static_cast<uint16_t>(HEX[i]);

    ENDLINE;
}

void print(const std::vector<uint8_t> &HEX) {

    for(size_t i = 0; i < HEX.size(); i++)
        std::cout << std::dec << static_cast<char>(HEX[i]);

    ENDLINE;
}
#endif // _OUTPUT_HPP_