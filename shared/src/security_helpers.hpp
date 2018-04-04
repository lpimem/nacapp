//
// Created by Lei Pi on 2/26/17.
//

#ifndef HOMESEC_SECURITY_HELPERS_HPP
#define HOMESEC_SECURITY_HELPERS_HPP

#include <ndn-cxx/encoding/buffer-stream.hpp>
#include <ndn-cxx/face.hpp>
#include <ndn-cxx/security/transform/block-cipher.hpp>
#include <ndn-cxx/security/transform/buffer-source.hpp>
#include <ndn-cxx/security/transform/stream-sink.hpp>
#include <ndn-cxx/util/string-helper.hpp>

namespace nacapp {
namespace sec {
using ndn::ConstBufferPtr;
using ndn::Data;

extern ConstBufferPtr
compute_sha256(const uint8_t* buffer, const size_t size);

ConstBufferPtr
encrypt_aes(const uint8_t* plainText, const int textLength, const uint8_t* key, const int keyLength);

ConstBufferPtr
encrypt_aes(const uint8_t* plainText,
            const int textLength,
            const uint8_t* key,
            const int keyLength,
            const uint8_t* iv,
            const int ivLength);

ConstBufferPtr
encrypt_aes(const std::string& plainText, ConstBufferPtr key);

ConstBufferPtr
decrypt_aes(const uint8_t* cipherText,
            const int cipherLength,
            const uint8_t* key,
            const int keyLength,
            const uint8_t* iv);

ConstBufferPtr
decrypt_aes(const uint8_t* cipherText, int textLength, const uint8_t* key, int keyLength);

const uint8_t*
extract_iv(const uint8_t* text, int textLength, int keyLength);

ConstBufferPtr
decrypt_aes(const Data& data, ConstBufferPtr key);


// /* ---------- ---------- ---------- ---------- */
// std::string
// randomHex()
// {
//   srand(time(NULL));
//   int random = rand();
//   std::stringstream stream;
//   stream << std::hex << random;
//   return stream.str();
// }

} // namespace sec
} // namespace nacapp

#endif //HOMESEC_SECURITY_HELPERS_HPP
