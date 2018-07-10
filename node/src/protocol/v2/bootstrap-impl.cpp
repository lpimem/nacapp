#include "bootstrap-impl.hpp"

#include <ndn-cxx/util/string-helper.hpp>
#include <boost/algorithm/string/predicate.hpp>

#include "../../../../shared/src/buffer-helper.hpp"
#include "../../../../shared/src/hmac.hpp"
#include "../../../../shared/src/ndn-util.hpp"
#include "../../../../shared/src/string_helpers.hpp"


namespace nacapp {
namespace bootstrap {


bool
verifyHash(std::string content, std::string key, std::string hash)
{
  auto expected = sign_hmac(fromString(key), fromString(content));
  std::string hex = ndn::toHex(*expected);
  std::cout << hex << std::endl;
  std::cout << hash << std::endl;
  return boost::iequals(hex, hash);
}

bool
validateOwnerCertResp(const Data& d, std::string expected_r, std::string pin)
{
  auto content = nacapp::data::getAsString(d);
  auto parts = split(content, '|');
  auto cert = parts[0];
  auto r = parts[1];
  auto hash = parts[2];

  return r == expected_r && verifyHash(cert + '|' + r, pin, hash);
}

bool
validateOwnerRequest(const Interest& interest, const Name& args, std::string pin)
{
  auto interestName = interest.getName();
  auto name = interestName.getPrefix(interestName.size() - 1);
  auto hash = args.get(2).toUri();
  return verifyHash(name.toUri(), pin, hash);
}

void
serveDeviceUnsignedCert(std::shared_ptr<Node> node,
                        const std::string& deviceId,
                        const std::string& pin,
                        std::shared_ptr<Certificate> deviceCertUnsigned,
                        OnStatusChange onSuccess,
                        OnStatusChange onFailure)
{
  node->route(deviceCertUnsigned->getName().toUri(),
              [&](const Interest& interest,
                  const Name& args,
                  shared_ptr<Data> data,
                  InterestShower show,
                  PutData put) {
                if (validateOwnerRequest(interest, args, pin)) {
                  put(deviceCertUnsigned);
                  return true;
                }
                else {
                  nacapp::data::setStringContent(*data, "Invalid request");
                }
                return false;
              });
}

void
startBootstrap(const Name& ownerName,
               const std::string& deviceId,
               const std::string& pin,
               std::shared_ptr<Certificate> deviceCertUnsigned,
               std::shared_ptr<Node> node,
               OnStatusChange onSuccess,
               OnStatusChange onFailure)
{
  Name interestName(ownerName);
  const std::string r1 = randomHex();
  interestName.append(deviceId);
  interestName.append(r1);
  Interest interest(interestName);
  node->showInterest(interest, [&](const Data& d) {
    if (!validateOwnerCertResp(d, r1, pin)) {
      onFailure(node);
      return;
    }
    serveDeviceUnsignedCert(node, deviceId, pin, deviceCertUnsigned, onSuccess, onFailure);
  });
}

} // namespace bootstrap
} // namespace nacapp