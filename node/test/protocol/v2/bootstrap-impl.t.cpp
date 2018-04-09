
#include <string>

#include "../../../../shared/src/common-cxx.hpp"
#include "../../../../shared/src/common-nac.hpp"
#include "../../../../shared/src/common-ndn.hpp"
#include "../../../../shared/src/hmac.hpp"

#include "../../../src/protocol/v2/bootstrap-impl.hpp"


namespace nacapp {
namespace bootstrap {

const std::string prefixText = "/local-home";
const std::string ownerNameText = prefixText + "/owner";
const std::string deviceIDText = "Make1-Model2-Version345";
const std::stirng devicePinText = "uns4fe";


TEST_CASE("hmac")
{
  const std::string content = "/local-home/bootstrap/owner/Make1-Model2-Version345/1923856";
  const std::string expect = "c4231e0ae5717a7e8cad7837a78e7f5884b4e0553315f6c6dcad667011e586db";
  std::string hash = sign_hmac(devicePinText, content);
  REQUIRE(hash == expect);
}

} // namespace bootstrap
} // namespace nacapp