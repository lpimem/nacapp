#ifndef UTIL_HPP
#define UTIL_HPP

#include "common.hpp"

namespace nacapp
{

namespace data
{

extern string getAsString(const Data &data);

extern PublicKey getAsPublicKey(const Data &data);

} // data

namespace strings
{
extern string uriEncode(const string rawUri);
extern string uriDecode(const string encodedUri);
} // strings
} // nacapp
#endif /* UTIL_HPP */
