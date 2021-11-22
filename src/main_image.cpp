#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#include <png++/image.hpp>
#include <png++/rgba_pixel.hpp>

int main(int argc, char* argv[]) {

    /// Для перезаписи PNG c глубиной цвета - 32 (RGBA, где A -  alpha канал)
    png::image<png::rgba_pixel> image("image/KV-1.png");
    image.write("image/output.png");



    /// Попробовать перевести IMAGE в HEX и закодировать AES CBC

    return 0;
}
