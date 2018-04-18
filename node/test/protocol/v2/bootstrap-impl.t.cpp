
#include <string>

#include <ndn-cxx/util/string-helper.hpp>

#include "../../../../shared/src/buffer-helper.hpp"
#include "../../../../shared/src/common-cxx.hpp"
#include "../../../../shared/src/common-nac.hpp"
#include "../../../../shared/src/common-ndn.hpp"
#include "../../../../shared/src/hmac.hpp"

#include "../../../../shared/test/catch.hpp"

#include "../../../src/protocol/v2/bootstrap-impl.hpp"


namespace nacapp {
namespace bootstrap {

const std::string prefixText = "/local-home";
const std::string ownerNameText = prefixText + "/owner";
const std::string deviceIDText = "Make1-Model2-Version345";
const std::string devicePinText = "uns4fe";


TEST_CASE("hmac - match")
{
  const std::string content = "/local-home/bootstrap/owner/Make1-Model2-Version345/1923856";
  ndn::ConstBufferPtr expect =
    ndn::fromHex("c4231e0ae5717a7e8cad7837a78e7f5884b4e0553315f6c6dcad667011e586db");

  auto hash = sign_hmac(fromString(devicePinText), fromString(content));
  REQUIRE(*hash == *expect);
}

TEST_CASE("hmac - invalid pin")
{
  const std::string content = "/local-home/bootstrap/owner/Make1-Model2-Version345/1923856";
  ndn::ConstBufferPtr expect =
    ndn::fromHex("c4231e0ae5717a7e8cad7837a78e7f5884b4e0553315f6c6dcad667011e586db");

  auto hash = sign_hmac(fromString("invalid"), fromString(content));
  REQUIRE(*hash != *expect);
}

TEST_CASE("hmac - different content")
{
  const std::string content = "/prefix/local-home/bootstrap/owner/Make1-Model2-Version345/1923856";
  ndn::ConstBufferPtr expect =
    ndn::fromHex("c4231e0ae5717a7e8cad7837a78e7f5884b4e0553315f6c6dcad667011e586db");

  auto hash = sign_hmac(fromString(devicePinText), fromString(content));
  REQUIRE(*hash != *expect);
}

TEST_CASE("Verify Hash - Match")
{
  const std::string content = "/local-home/bootstrap/owner/Make1-Model2-Version345/1923856";
  const std::string expect = "c4231e0ae5717a7e8cad7837a78e7f5884b4e0553315f6c6dcad667011e586db";

  REQUIRE(verifyHash(content, devicePinText, expect));
}

// TEST_CASE("")

} // namespace bootstrap
} // namespace nacapp