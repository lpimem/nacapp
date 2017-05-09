#include "ndn-util.hpp"

namespace nacapp
{
namespace data
{

void setStringContent(Data &d, const std::string &content)
{
  d.setContent(reinterpret_cast<const uint8_t *>(content.c_str()),
               content.length());
}

}
}