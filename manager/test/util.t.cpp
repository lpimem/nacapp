#include "test.common.hpp"
#include "../src/util.hpp"

namespace nacapp
{
namespace strings
{

const static std::string uriRaw = "/named-data/123456/%1.00ab";
const static std::string uriEncoded = "%2Fnamed-data%2F123456%2F%251.00ab";

TEST_CASE("URI Encode")
{
  std::string encoded = uriEncode(uriRaw);
  REQUIRE(uriEncoded == encoded);
}

TEST_CASE("URI Decode")
{
  std::string decoded = uriDecode(uriEncoded);
  REQUIRE(uriRaw == decoded);
}

} // strings
} // nacapp