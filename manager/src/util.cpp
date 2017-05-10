#include <cctype>
#include <iomanip>
#include <sstream>
#include <string>

#include "util.hpp"

namespace nacapp
{
namespace data
{

string getAsString(const Data &data)
{
  const uint8_t *bytes = data.getContent().value();
  const int len = data.getContent().value_size();
  std::string s(reinterpret_cast<char const *>(bytes), len);
  return s;
}

PublicKey getAsPublicKey(const Data &data)
{
  PublicKey key(data.getContent().value(), data.getContent().value_size());
  return key;
}

} // data

namespace strings
{

string uriEncode(const string rawUri)
{
  std::ostringstream os;
  os.fill('0');
  os << std::hex;
  for (auto i : rawUri)
  {
    if (std::isalnum(i) || i == '-' || i == '_' || i == '.' || i == '~')
    {
      os << i;
      continue;
    }
    os << std::uppercase;
    os << '%' << std::setw(2) << int((unsigned char)i);
    os << std::nouppercase;
  }
  return os.str();
}

string uriDecode(const string encodedUri)
{
  std::ostringstream os;
  for (auto i = 0; i < encodedUri.length(); i++)
  {
    if (encodedUri[i] == '%')
    {
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