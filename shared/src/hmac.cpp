#include "hmac.hpp"

#include <ndn-cxx/security/transform/hmac-filter.hpp>

#include <ndn-cxx/encoding/buffer-stream.hpp>
#include <ndn-cxx/security/transform/buffer-source.hpp>
#include <ndn-cxx/security/transform/step-source.hpp>
#include <ndn-cxx/security/transform/stream-sink.hpp>


string
sign_hmac(string key, string plain)
{
  string mac, encoded;
  try {
    HMAC<SHA256> hmac((byte*)key.c_str(), key.length());

    StringSource(plain,
                 true,
                 new HashFilter(hmac,
                                new StringSink(mac)) // HashFilter
    );                                               // StringSource
  }
  catch (const CryptoPP::Exception& e) {
    cerr << e.what() << endl;
  }

  encoded.clear();
  StringSource(mac,
               true,
               new Base64Encoder(new StringSink(encoded)) // Base64Encoder
  );                                                      // StringSource

  return encoded;
}

ndn::ConstBufferPtr
sign_hmac(ndn::ConstBufferPtr key, ndn::ConstBufferPtr plain)
{
  ndn::OBufferStream os;
  ndn::security::transform::bufferSource(&plain->front(), plain->size()) >>
    ndn::security::transform::hmacFilter(ndn::DigestAlgorithm::SHA256, &key->front(), key->size()) >>
    ndn::security::transform::streamSink(os);

  return os.buf();
}
