#include <cctype>
#include <iomanip>
#include <sstream>
#include <string>

#include "util.hpp"

namespace nacapp {

namespace names {


Name
extractPrefix(const Name& name, const ndn::name::Component& token)
{
  size_t pos = 0;
  for (; pos < name.size(); pos++) {
    ndn::name::Component c = name.get(pos);
    if (c == token) {
      break;
    }
  }
  // not found
  if (pos == name.size()) {
    pos = 0;
  }
  return name.getPrefix(pos);
}

extern Name
extractSuffix(const Name& name, const ndn::name::Component& token, size_t n)
{
  size_t pos = 0;
  for (; pos < name.size(); pos++) {
    ndn::name::Component c = name.get(pos);
    if (c == token) {
      break;
    }
  }
  return name.getSubName(pos + 1, n);
}
} // names

namespace data {

void
setFreshnessPeriodIfNotSet(Data& d, time::milliseconds period)
{
  if (d.getFreshnessPeriod() < time::milliseconds::zero()) {
    d.setFreshnessPeriod(period);
  }
}

string
getAsString(const Data& data)
{
  const uint8_t* bytes = data.getContent().value();
  const int len = data.getContent().value_size();
  std::string s(reinterpret_cast<char const*>(bytes), len);
  return s;
}

PublicKey
getAsPublicKey(const Data& data)
{
  PublicKey key(data.getContent().value(), data.getContent().value_size());
  return key;
}

void
setStringContent(shared_ptr<Data> data, const string& content)
{
  data->setContent(reinterpret_cast<const uint8_t*>(content.c_str()), content.length());
}

} // data

namespace strings {

string
uriEncode(const string rawUri)
{
  std::ostringstream os;
  os.fill('0');
  os << std::hex;
  for (auto i : rawUri) {
    if (std::isalnum(i) || i == '-' || i == '_' || i == '.' || i == '~') {
      os << i;
      continue;
    }
    os << std::uppercase;
    os << '%' << std::setw(2) << int((unsigned char)i);
    os << std::nouppercase;
  }
  return os.str();
}

string
uriDecode(const string encodedUri)
{
  std::ostringstream os;
  for (string::size_type i = 0; i < encodedUri.length(); i++) {
    if (encodedUri[i] == '%') {
      auto hex = encodedUri.substr(i + 1, 2);
      os << static_cast<char>(std::stoi(hex, NULL, 16));
      i += 2;
      continue;
    }
    os << encodedUri[i];
  }
  return os.str();
}
} // strings
}