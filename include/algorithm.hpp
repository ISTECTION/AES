#ifndef __ALGORITHM_H__
#define __ALGORITHM_H__

#include <algorithm>
#include <iomanip>
#include <sstream>
#include <vector>
#include <string>

#include <openssl/sha.h>
#include <openssl/md5.h>
#include <openssl/rand.h>
#include <openssl/hmac.h>


std::vector<uint8_t> sha256(const std::string &str) {
    std::vector<uint8_t> hash(SHA256_DIGEST_LENGTH);
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, str.c_str(), str.size());
    SHA256_Final(&hash[0], &sha256);
    // std::ostringstream ostream;
    // for(int i = 0; i < SHA256_DIGEST_LENGTH; i++)
    //     ostream << std::hex
    //             << std::setw(2)
    //             << std::setfill('0')
    //             << static_cast<int>(hash[i]);
    return hash;
}

std::vector<uint8_t> md5(const std::string &str) {
    std::vector<uint8_t> hash(MD5_DIGEST_LENGTH);
    MD5_CTX md5;
    MD5_Init(&md5);
    MD5_Update(&md5, str.c_str(), str.size());
    MD5_Final(&hash[0], &md5);
    // std::ostringstream ostream;
    // for(int i = 0; i < MD5_DIGEST_LENGTH; i++)
    //     ostream << std::hex
    //             << std::setw(2)
    //             << std::setfill('0')
    //             << static_cast<int>(hash[i]);
    return hash;
}

std::vector<uint8_t> calcHMAC_SHA256(std::vector<uint8_t> key,
                                     std::vector<uint8_t> &data) {

    std::vector<uint8_t> hmac(SHA256_DIGEST_LENGTH);
    unsigned int hmac_len;

    HMAC (
        EVP_sha256(),
        &key[0],
        static_cast<int>(key.size()),
        reinterpret_cast<unsigned char const*>(&data[0]),
        static_cast<int>(data.size()),
        &hmac[0],
        &hmac_len
    );

    return hmac;
}

std::vector<uint8_t> generateIV() {
    std::vector<uint8_t> init_vector(16);
    RAND_bytes(&init_vector[0], 16);
    return init_vector;
}

#endif // _ALGORITHM_HPP_