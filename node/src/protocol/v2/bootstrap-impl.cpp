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
validateOwnerCertResp(const Data& d, std::string expected_r, std::string sharedSecret)
{
  auto content = nacapp::data::getAsString(d);
  auto parts = split(content, '|');
  auto cert = parts[0];
  auto r = parts[1];
  auto hash = parts[2];

  return r == expected_r && verifyHash(cert + '|' + r, sharedSecret, hash);
}

bool
validateOwnerRequest(const Interest& interest, const Name& args, std::string sharedSecret)
{
  auto interestName = interest.getName();
  auto name = interestName.getPrefix(interestName.size() - 1);
  auto hash = args.get(1).toUri();
  return verifyHash(name.toUri(), sharedSecret, hash);
}

std::shared_ptr<Data>
hmacSignUnsignedCert(std::shared_ptr<Certificate> deviceCertUnsigned,
                     const std::string& deviceId,
                     const std::string& sharedSecret)
{
  // deviceCertUnsigned.content -> base64 -> hash -> concatenate -> new data

  return nullptr;
}

void
serveDeviceUnsignedCert(std::shared_ptr<Node> node,
                        const Name& wellknown,
                        const std::string& deviceId,
                        const std::string& sharedSecret,
                        std::shared_ptr<Certificate> deviceCertUnsigned,
                        OnStatusChange onSuccess,
                        OnStatusChange onFailure)
{
  node->route("/",
              [&](const Interest& interest,
                  const Name& args,
                  shared_ptr<Data> data,
                  InterestShower show,
                  PutData put) {
                if (validateOwnerRequest(interest, args, sharedSecret)) {
                  auto signedCert = hmacSignUnsignedCert(deviceCertUnsigned, deviceId, sharedSecret);
                  put(deviceCertUnsigned);
                  return true;
                }
                else {
                  nacapp::data::setStringContent(*data, "Invalid request");
                }
                return false;
              });
}

Name
constructBootstrapName(const Name& wellknown,
                       const std::string& randomNo,
                       const std::string& deviceId,
                       const std::string& sharedSecret)
{
  Name interestName(wellknown);
  interestName.append("owner");
  interestName.append(deviceId);
  interestName.append(randomNo);
  auto hash = sign_hmac(fromString(sharedSecret), fromString(interestName.toUri()));
  std::string hex = ndn::toHex(*hash);
  interestName.append(hex);
  return interestName;
}

void
startBootstrap(const Name& wellknown,
               const std::string& deviceId,
               const std::string& sharedSecret,
               std::shared_ptr<Certificate> deviceCertUnsigned,
               std::shared_ptr<Node> node,
               OnStatusChange onSuccess,
               OnStatusChange onFailure)
{
  const std::string randomNo = randomHex();
  Name interestName = constructBootstrapName(wellknown, randomNo, deviceId, sharedSecret);
  Interest interest(interestName);
  node->showInterest(interest, [&](const Data& d) {
    if (!validateOwnerCertResp(d, randomNo, sharedSecret)) {
      onFailure(node);
      return;
    }
    serveDeviceUnsignedCert(node,
                            wellknown,
                            deviceId,
                            sharedSecret,
                            deviceCertUnsigned,
                            onSuccess,
                            onFailure);
  });
}

} // namespace bootstrap
} // namespace nacapp