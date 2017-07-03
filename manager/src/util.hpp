#ifndef UTIL_HPP
#define UTIL_HPP

#include "common.hpp"

namespace nacapp {

namespace data {

extern string
getAsString(const Data& data);

extern PublicKey
getAsPublicKey(const Data& data);

extern void
setStringContent(shared_ptr<Data> data, const string& content);

} // data

namespace strings {

extern string
uriEncode(const string rawUri);

extern string
uriDecode(const string encodedUri);

} // strings

} // nacapp
#endif /* UTIL_HPP */
