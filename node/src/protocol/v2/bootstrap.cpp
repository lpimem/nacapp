#include "bootstrap.hpp"

#include "../../../../shared/src/hmac.hpp"
#include "../../../../shared/src/ndn-util.hpp"
#include "../../../../shared/src/string_helpers.hpp"

namespace nacapp {

namespace bootstrap {


bool
verifyHash(std::string content, std::string key, std::string hash)
{
  auto expected = sign_hmac(key, content);
  return expected == hash;
}

bool
validateOwnerCertResp(const Data& d, std::string expected_r, std::string pin)
{
  auto content = nacapp::data::getAsString(d);
  auto parts = split(content, '|');
  auto cert = parts[0];
  auto r = parts[1];
  auto hash = parts[2];

  return r == expected_r && verifyHash(cert, pin, hash);
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
serveDeviceUnsignedCert(std::shared_ptr<BootstrapHelper> helper,
                        OnStatusChange onSuccess,
                        OnStatusChange onFailure)
{
  auto node = helper->getNode();
  auto cert = helper->getDeviceCert();
  node->route(cert->getName().toUri(),
              [&](const Interest& interest,
                  const Name& args,
                  shared_ptr<Data> data,
                  InterestShower show,
                  PutData put) {
                if (validateOwnerRequest(interest, args, helper->getDevicePin())) {
                  put(cert);
                  return true;
                }
                else {
                  nacapp::data::setStringContent(*data, "Invalid request");
                }
                return false;
              });
}

void
startBootstrap(Name ownerName,
               std::string deviceID,
               std::shared_ptr<BootstrapHelper> helper,
               OnStatusChange onSuccess,
               OnStatusChange onFailure)
{
  const std::string r1 = randomHex();
  ownerName.append(deviceID);
  ownerName.append(r1);
  Interest startInterest(ownerName);
  auto node = helper->getNode();
  node->showInterest(startInterest, [&](const Data& d) {
    if (!validateOwnerCertResp(d, r1, helper->getDevicePin())) {
      onFailure(helper);
      return;
    }
    serveDeviceUnsignedCert(helper, onSuccess, onFailure);
  });
}

} // namespace bootstrap

/** 
 *  node -..-> owner 
 *    r1 + devid + hmac
 *  owner -> node 
 *    r1 + owner cert + r2
 *  node -> owner 
 *    r2 +  
 */
void
BootstrapHelper::start(Name ownerName,
                       std::string deviceId,
                       std::string pin,
                       OnStatusChange onSuccess,
                       OnStatusChange onFail)
{
  m_deviceId = deviceId;
  m_pin = pin;
  m_ownerName = ownerName;
  m_onSuccess = onSuccess;
  m_onFailure = onFail;
  m_status = BootstrapHelper::BTStatus::STARTED;
  bootstrap::startBootstrap(ownerName, deviceId, shared_from_this(), m_onSuccess, m_onFailure);
}

} // namespace nacapp