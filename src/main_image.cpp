#include "PKCS7/pkcs7_padding.h"
#include "Base64/base64.hpp"
#include "algorithm.hpp"
#include "utilites.hpp"
#include "output.hpp"
#include "AES/aes.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <array>

#include <png++/image.hpp>
#include <png++/rgba_pixel.hpp>

void getRGBA_Array(const png::image<png::rgba_pixel> &, std::vector<uint8_t> &);
void setIDAT_PNG  (png::image<png::rgba_pixel> &, const std::vector<uint8_t> &);

int main(int argc, char* argv[]) {

    png::image<png::rgba_pixel> image("image/Team-photo.png");
    std::vector<uint8_t> pixels;
    getRGBA_Array(image, pixels);

    std::cout << "Enter password: ";
    std::string password = enterPassword();
    std::vector<uint8_t> key = sha256(password);

    size_t size = pixels.size();
    std::cout << "Size: " << size << std::endl;
    {
        size_t nsize = {
            size % 16 ?
                size + (16 - size % 16) :
                size
        };
        std::cout << "n-Size: " << nsize << std::endl;

        pixels.resize(nsize);
    }

    struct AES_ctx ctx;
    std::vector<uint8_t> IV = generateIV();

    AES_init_ctx_iv(&ctx, &key[0], &IV[0]);
    AES_CBC_encrypt_buffer(&ctx, &pixels[0], pixels.size());
    setIDAT_PNG(image, pixels);
    image.write("image/Team-photo_en.png");

    std::vector<uint8_t> hmac = calcHMAC_SHA256(key, pixels);

    AES_ctx_set_iv(&ctx, &IV[0]);
    AES_CBC_decrypt_buffer(&ctx, &pixels[0], pixels.size());
    setIDAT_PNG(image, pixels);
    image.write("image/Team-photo_de.png");
    return 0;
}

void getRGBA_Array(const png::image<png::rgba_pixel> &im, std::vector<uint8_t> &pix) {
    pix.resize(im.get_width() * im.get_height() * 4);

    for (size_t x = 0; x < im.get_width() ; x++)
    for (size_t y = 0; y < im.get_height(); y++) {

        pix[(x * im.get_width() + y) * 4 + 0] = im[x][y].red  ;
        pix[(x * im.get_width() + y) * 4 + 1] = im[x][y].green;
        pix[(x * im.get_width() + y) * 4 + 2] = im[x][y].blue ;
        pix[(x * im.get_width() + y) * 4 + 3] = im[x][y].alpha;

    }
}

void setIDAT_PNG(png::image<png::rgba_pixel> &im, const std::vector<uint8_t> &pix) {

    for (size_t x = 0; x < im.get_width() ; x++)
    for (size_t y = 0; y < im.get_height(); y++) {

        im[x][y].red   = pix[(x * im.get_width() + y) * 4 + 0];
        im[x][y].green = pix[(x * im.get_width() + y) * 4 + 1];
        im[x][y].blue  = pix[(x * im.get_width() + y) * 4 + 2];
        im[x][y].alpha = pix[(x * im.get_width() + y) * 4 + 3];

    }
}