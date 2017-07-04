#ifndef UTIL_HPP
#define UTIL_HPP

#include "common.hpp"

namespace nacapp {

namespace names {

/** 
 * Find the prefix of the name which appears before the first @p token
 * If token is not found, return an empty name.
 */
extern Name
extractPrefix(const Name& name, const ndn::name::Component& token);

/**
 * Find the suffix of the name which appears behind the first @p token.
 * Return empty name if token is not found 
 * Return at most @p n components as the suffix, 
 *    if @p n is -1, return till the end.
 */
extern Name
extractSuffix(const Name& name, const ndn::name::Component& token, size_t n = Name::npos);
}

namespace data {

extern void
setFreshnessPeriodIfNotSet(Data& d, time::milliseconds period);

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
