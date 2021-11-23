#include <exception>
#include <iostream>

#include <png++/rgba_pixel.hpp>
#include <png++/rgb_pixel.hpp>
#include <png++/image.hpp>

int main(int argc, char* argv[]) {

    if(argc == 2) {
        std::string name = argv[1];

        std::cout << "1. PNG24" << std::endl;
        std::cout << "2. PNG32"     << std::endl;
        std::cout << "Convert to: ";
        char choice = std::cin.get();

        switch (choice)
        {
            case '1':
            {
                png::image<png::rgb_pixel> image(name);
                const std::string png = "-png24";
                name.insert(name.end() - 4, png.begin(), png.end());
                image.write(name);
                break;
            }
            case '2':
            {
                png::image<png::rgba_pixel> image(name);
                const std::string png = "-png32";
                name.insert(name.end() - 4, png.begin(), png.end());
                image.write(name);
                break;
            }
            default:
                throw std::runtime_error("Incorrect choice");
        }
    } else
        throw std::runtime_error("argc != 2");

    return 0;
}