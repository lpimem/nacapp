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