#include "PKCS7/pkcs7_padding.h"
#include "Base64/base64.hpp"
#include "algorithm.hpp"
#include "utilites.hpp"
#include "output.hpp"
#include "AES/aes.h"

#include <exception>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <array>
#include <map>

#include <png++/image.hpp>
#include <png++/rgba_pixel.hpp>

void getRGBA_Array(const png::image<png::rgba_pixel> &, std::vector<uint8_t> &);
void setIDAT_PNG  (png::image<png::rgba_pixel> &, const std::vector<uint8_t> &);


enum class CODE { ENCRYPT, DECRYPT };
std::map<std::string, CODE> code_m = {
    { "-e", CODE::ENCRYPT },
    { "-d", CODE::DECRYPT }
};

enum class MODE { ECB_MODE, CBC_MODE };
std::map<std::string, MODE> mode_m = {
    { "-ecb", MODE::ECB_MODE },
    { "-cbc", MODE::CBC_MODE }
};


int main(int argc, char* argv[]) {

    if (argc != 4)
        throw std::runtime_error("argc != 4");

    std::string code = argv[1];
    std::string path = argv[2];
    std::string mode = argv[3];

    std::transform(code.begin(), code.end(), code.begin(), tolower);
    std::transform(mode.begin(), mode.end(), mode.begin(), tolower);

    std::string path_info = path.substr(0, path.size() - (code_m[code] == CODE::ENCRYPT ? 4 : 11)) + '/';
    /// Добавить create_directory


    struct AES_ctx ctx;
    switch (code_m[code])
    {
        case CODE::ENCRYPT:
        {
            png::image<png::rgba_pixel> image(path);
            std::vector<uint8_t> pixels;
            getRGBA_Array(image, pixels);

            std::cout << "Enter password: ";
            std::string password = enterPassword();
            std::vector<uint8_t> key = sha256(password);

            switch (mode_m[mode])
            {
                case MODE::ECB_MODE:
                {
                    AES_ECB_encrypt(&ctx, &pixels[0]);
                    setIDAT_PNG(image, pixels);

                    image.write(path_info + "en-cbc.png");
                    break;
                }
                case MODE::CBC_MODE:
                {
                    std::vector<uint8_t> IV = generateIV();
                    writeFile(path_info + "IV.txt", IV);

                    AES_init_ctx_iv(&ctx, &key[0], &IV[0]);
                    AES_CBC_encrypt_buffer(&ctx, &pixels[0], pixels.size());
                    setIDAT_PNG(image, pixels);

                    std::vector<uint8_t> hmac = calcHMAC_SHA256(key, pixels);
                    writeFile(path_info + "HMAC.txt", IV);

                    image.write(path_info + "en-cbc.png");
                    break;
                }
                default:
                    throw std::runtime_error("Incorrect option");
            }
            break;
        }
        case CODE::DECRYPT:
        {
            png::image<png::rgba_pixel> image(path);
            std::vector<uint8_t> pixels;
            getRGBA_Array(image, pixels);

            std::cout << "Enter password: ";
            std::string password = enterPassword();
            std::vector<uint8_t> key = sha256(password);

            std::cout << "Enter IV: ";
            std::vector<uint8_t> IV = enterIV();

            switch (mode_m[mode])
            {
                case MODE::ECB_MODE:
                {


                    break;
                }
                case MODE::CBC_MODE:
                {
                    AES_init_ctx_iv(&ctx, &key[0], &IV[0]);
                    AES_CBC_decrypt_buffer(&ctx, &pixels[0], pixels.size());
                    setIDAT_PNG(image, pixels);


                    image.write(path_info + "de-cbc.png");
                    break;
                }
                default:
                    throw std::runtime_error("Incorrect option");
            }
            break;
        }
        default:
            throw std::runtime_error("Incorrect option");
    }


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