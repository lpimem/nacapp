//
// Created by Lei Pi on 2/26/17.
//

#include <iterator>
#include <sstream>
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

  // std::cout << "[encrypt_aes] SECRET KEY: ";
  // ndn::printHex(std::cout, key, keyLength, true);
  // std::cout << std::endl;
  // std::cout << "[encrypt_aes] IV: ";
  // ndn::printHex(std::cout, const_iv, ivLength, true);
  // std::cout << std::endl;
  // std::cout << "[encrypt_aes] PLAINTEXT: ";
  // ndn::printHex(std::cout, plainText, textLength, true);
  // std::cout << std::endl;


  // encrypt
  ConstBufferPtr cipher = encrypt_aes(plainText, textLength, key, keyLength, const_iv, ivLength);
  // append iv
  const int cipherSize = cipher->size();
  std::shared_ptr<ndn::Buffer> b = std::make_shared<ndn::Buffer>(0);
  std::vector<u_int8_t> r(cipherSize + ivLength);
  // u_int8_t r[cipherSize + ivLength];
  for (int i = 0; i < cipherSize; i++) {
    auto b_start = &cipher->front();
    r[i] = *(b_start + i);
    b->push_back(r[i]);
  }
  for (auto i = 0; i < ivLength; i++) {
    r[cipherSize + i] = *(const_iv + i);
    b->push_back(r[cipherSize + i]);
  }
  // std::cout << "[encrypt_aes] CIPHER: ";
  // ndn::printHex(std::cout, b->get(), b->size(), true);
  // std::cout << std::endl;
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
  auto key_buf = &key->front();
  return encrypt_aes(reinterpret_cast<const uint8_t*>(plainText.data()),
                     plainText.size(),
                     key_buf,
                     key->size());
}

ConstBufferPtr
decrypt_aes(const uint8_t* cipherText,
            const int cipherLength,
            const uint8_t* key,
            const int keyLength,
            const uint8_t* iv)
{
  const int ivLength = keyLength;
  try {
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
    std::stringstream ss;
    ss << "Decryption error: " << ex.what();
    std::string err = ss.str();
    std::cerr << err << std::endl;

    std::cout << "[decrypt_aes] SECRET KEY: ";
    ndn::printHex(std::cout, key, keyLength, true);
    std::cout << std::endl;
    std::cout << "[decrypt_aes] IV: ";
    ndn::printHex(std::cout, iv, ivLength, true);
    std::cout << std::endl;
    std::cout << "[decrypt_aes] CIPHER: ";
    ndn::printHex(std::cout, cipherText, cipherLength, true);
    std::cout << std::endl;
    std::cout.flush();

    throw err;
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
  const uint8_t* k = &key->front();
  const int keyLength = key->size();
  const int textLength = data.getContent().value_size();
  // LOG(INFO) << "decrypt_aes (Data) : data: " << ndn::toHex(cipher, textLength, true);
  return decrypt_aes(cipher, textLength, k, keyLength);
}

} // namespace sec
} // namespace nacapp
