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

#include <openssl/aes.h>

int main(int argc, char* argv[]) {

    std::ifstream fin;
    argc == 2 ? fin.open(argv[1])
              : throw std::runtime_error("argc != 2"       );
    if ( !fin ) throw std::runtime_error("File is not open");

    std::vector<uint8_t> text = read(fin);
    fin.close();

    std::cout << "Enter password: ";
    std::string password = enterPassword();

    std::cout << "1. SHA-256" << std::endl;
    std::cout << "2. MD5"     << std::endl;
    std::cout << "Hash key: ";
    char hash = std::cin.get();
    std::vector<uint8_t> key;

    switch (hash)
    {
        case '1':
            key = sha256(password);
            break;
        case '2':
            key = md5(password);
            break;
        default:
            throw std::runtime_error("Incorrect input");
    }

    size_t size = text.size();
    {
        size_t nsize = {
            size % 16 ?
                size + (16 - size % 16) :
                size
        };
        text.resize(nsize);
    }
    pkcs7_padding_pad_buffer(&text[0], size, text.size(), 16);
    if (!pkcs7_padding_valid(&text[0], size, text.size(), 16))
        throw std::runtime_error("The data is not valid");

    struct AES_ctx ctx;
    std::vector<uint8_t> IV = generateIV();
    print(base64_encode(key));
    print(base64_encode(IV));


    AES_init_ctx_iv(&ctx, &key[0], &IV[0]);
    AES_CBC_encrypt_buffer(&ctx, &text[0], text.size());

    print(base64_encode(text));

    AES_ctx_set_iv(&ctx, &IV[0]);
    AES_CBC_decrypt_buffer(&ctx, &text[0], text.size());

    size_t actualDataLength =
        pkcs7_padding_data_length(&text[0], text.size(), 16);

    print(text);
    return 0;
}