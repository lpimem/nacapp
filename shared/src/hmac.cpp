#include "security/transform/hmac-filter.hpp"

#include "encoding/buffer-stream.hpp"
#include "security/transform/buffer-source.hpp"
#include "security/transform/step-source.hpp"
#include "security/transform/stream-sink.hpp"

#include "hmac.hpp"

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
  OBufferStream os;
  bufferSource(&plain->front(), plain->size()) >>
    hmacFilter(DigestAlgorithm::SHA256, &key->front(), key->size()) >> streamSink(os);

  return os.buf();
}
