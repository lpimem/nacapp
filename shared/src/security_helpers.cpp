//
// Created by Lei Pi on 2/26/17.
//

#include <iterator>
#include <vector>


#include "security_helpers.hpp"
#include "easylogging/easylogging++.h"
#include <ndn-cxx/util/sha256.hpp>
#include <ndn-cxx/util/string-helper.hpp>

namespace nacapp {
namespace sec {

using namespace ndn::security::transform;

// TODO: generate a random iv properly
const uint8_t const_iv[] =
  {0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07};

ConstBufferPtr
compute_sha256(const uint8_t* buffer, const size_t size)
{
  ndn::util::Sha256 digest;
  digest.update(buffer, size);
  return digest.computeDigest();
}

ConstBufferPtr
encrypt_aes(const uint8_t* plainText, const int textLength, const uint8_t* key, const int keyLength)
{
  // TODO: random iv here
  const int ivLength = sizeof(const_iv);
  // encrypt
  ConstBufferPtr cipher = encrypt_aes(plainText, textLength, key, keyLength, const_iv, ivLength);
  // append iv
  const int cipherSize = cipher->size();
  std::shared_ptr<ndn::Buffer> b = std::make_shared<ndn::Buffer>(0);
  std::vector<u_int8_t> r(cipherSize + ivLength);
  // u_int8_t r[cipherSize + ivLength];
  for (int i = 0; i < cipherSize; i++) {
    r[i] = *(cipher->get() + i);
    b->push_back(r[i]);
  }
  for (auto i = 0; i < ivLength; i++) {
    r[cipherSize + i] = *(const_iv + i);
    b->push_back(r[cipherSize + i]);
  }
  return b;
}

ConstBufferPtr
encrypt_aes(const uint8_t* plainText,
            const int textLength,
            const uint8_t* key,
            const int keyLength,
            const uint8_t* iv,
            const int ivLength)
{
  ndn::OBufferStream os;
  bufferSource(plainText, textLength) >> blockCipher(ndn::BlockCipherAlgorithm::AES_CBC,
                                                     ndn::CipherOperator::ENCRYPT,
                                                     key,
                                                     keyLength,
                                                     iv,
                                                     ivLength) >>
    streamSink(os);

  return os.buf();
}

ConstBufferPtr
encrypt_aes(const std::string& plainText, ConstBufferPtr key)
{
  return encrypt_aes(reinterpret_cast<const uint8_t*>(plainText.data()),
                     plainText.size(),
                     key->get(),
                     key->size());
}

ConstBufferPtr
decrypt_aes(const uint8_t* cipherText,
            const int cipherLength,
            const uint8_t* key,
            const int keyLength,
            const uint8_t* iv)
{
  try {
    const int ivLength = keyLength;

    std::cout << "[decrypt_aes] SECRET KEY: ";
    ndn::printHex(std::cout, key, keyLength, true);
    std::cout << std::endl;
    std::cout << "[decrypt_aes] IV: ";
    ndn::printHex(std::cout, iv, ivLength, true);
    std::cout << std::endl;
    std::cout << "[decrypt_aes] CIPHER: ";
    ndn::printHex(std::cout, cipherText, cipherLength, true);
    std::cout << std::endl;

    ndn::OBufferStream os;
    bufferSource(cipherText, cipherLength) >> blockCipher(ndn::BlockCipherAlgorithm::AES_CBC,
                                                          ndn::CipherOperator::DECRYPT,
                                                          key,
                                                          keyLength,
                                                          iv,
                                                          ivLength) >>
      streamSink(os);
    return os.buf();
  }
  catch (std::exception const& ex) {
    std::cerr << "Decryption error: " << ex.what() << std::endl;
    throw ex;
  }
}

ConstBufferPtr
decrypt_aes(const uint8_t* cipherText, int textLength, const uint8_t* key, int keyLength)
{
  const uint8_t* iv = extract_iv(cipherText, textLength, keyLength);
  const size_t cipherSize = textLength - keyLength;
  LOG(INFO) << "CIPHER SIZE: " << cipherSize;
  LOG(INFO) << "KEY LENGTH SIZE: " << keyLength;
  return decrypt_aes(cipherText, cipherSize, key, keyLength, iv);
}

const uint8_t*
extract_iv(const uint8_t* text, int textLength, int keyLength)
{
  const uint8_t* iv = text + (textLength - keyLength);
  return iv;
}

ConstBufferPtr
decrypt_aes(const Data& data, ConstBufferPtr key)
{
  const uint8_t* cipher = data.getContent().value();
  const uint8_t* k = key->get();
  const int keyLength = key->size();
  const int textLength = data.getContent().value_size();
  LOG(INFO) << "decrypt_aes (Data) : data: " << ndn::toHex(cipher, textLength, true);
  return decrypt_aes(cipher, textLength, k, keyLength);
}

} // namespace sec
} // namespace nacapp
