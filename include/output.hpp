#ifndef _OUTPUT_HPP_
#define _OUTPUT_HPP_

#define ENDLINE std::cout << std::endl

#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <cstring>
#include <vector>


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

void print(const unsigned char* HEX) {

    for(size_t i = 0; i < std::strlen((char*)HEX); i++)
        std::cout << std::dec << HEX[i];

    ENDLINE;
}


void writeFile(std::string _path, const std::vector<uint8_t> &data) {

    std::ostringstream ostream;
    for(size_t i = 0; i < data.size(); i++)
        ostream << std::hex
                << std::setw(2)
                << std::nouppercase
                << std::setfill('0')
                << static_cast<uint16_t>(data[i]);

    std::ofstream fout(_path);
    fout << ostream.str();
    fout.close();
}
#endif // _OUTPUT_HPP_