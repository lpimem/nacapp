#include "bootstrap-impl.hpp"

#include <ndn-cxx/util/string-helper.hpp>
#include <boost/algorithm/string/predicate.hpp>

#include "../../../../shared/src/buffer-helper.hpp"
#include "../../../../shared/src/hmac.hpp"
#include "../../../../shared/src/ndn-util.hpp"
#include "../../../../shared/src/string_helpers.hpp"


namespace nacapp {
namespace bootstrap {
namespace impl {


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

shared_ptr<Certificate>
extractOwnerCert(const Data& d, std::string& dname)
{
  auto content = nacapp::data::getAsString(d);
  auto parts = split(content, '|');
  auto dnameAndCert = split(parts[0], '^');
  dname = dnameAndCert[0];
  auto certHex = dnameAndCert[1];

  ndn::ConstBufferPtr buf = ndn::fromHex(certHex);
  shared_ptr<Certificate> cert = make_shared<Certificate>(Block{buf});
  return std::dynamic_pointer_cast<Certificate>(cert);
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
  auto block = deviceCertUnsigned->getContent();
  std::string hex = ndn::toHex(block.value(), block.value_size(), true);
  auto hash = sign_hmac(fromString(sharedSecret), fromString(hex));
  auto data_block = deviceCertUnsigned->wireEncode();
  std::string certHex = ndn::toHex(data_block.value(), data_block.value_size(), true);
  std::string newContent = certHex + "|" + ndn::toHex(*hash);
  auto ret = std::make_shared<Data>(deviceCertUnsigned->getName());
  ndn::ConstBufferPtr contentBuf = nacapp::fromString(newContent);
  ret->setContent(contentBuf);
  auto meta = ret->getMetaInfo();
  meta.setFreshnessPeriod(time::milliseconds(1000));
  return ret;
}

void
serveDeviceUnsignedCert(std::shared_ptr<Node> node,
                        const Name& wellknown,
                        const std::string& deviceId,
                        const std::string& sharedSecret,
                        const Name& dname,
                        std::shared_ptr<Certificate> deviceCertUnsigned,
                        OnDeviceCertSigned onDeviceCertSigned,
                        OnFail onFail)
{
  shared_ptr<int> retry = std::make_shared<int>();
  shared_ptr<int> retryMax = std::make_shared<int>();
  *retry = 0;
  *retryMax = 10;
  node->route("/",
              [&](const Interest& interest,
                  const Name& args,
                  shared_ptr<Data> data,
                  InterestShower show,
                  PutData put) {
                if (validateOwnerRequest(interest, args, sharedSecret)) {
                  auto signedCert = hmacSignUnsignedCert(deviceCertUnsigned, deviceId, sharedSecret);
                  put(signedCert);
                  fetchDeviceCert(node, wellknown, deviceId, dname, onDeviceCertSigned, onFail);
                  return true;
                }
                else {
                  std::string message = "Invalid request";
                  if ((*retry)++ > *retryMax) {
                    message += " - Max Retry Reached";
                    onFail(message);
                  }
                  nacapp::data::setStringContent(*data, message);
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
  interestName.append("cert");
  interestName.append(deviceId);
  interestName.append(randomNo);
  std::string input = deviceId + "|" + randomNo;
  auto hash = sign_hmac(fromString(sharedSecret), fromString(input));
  std::string hex = ndn::toHex(*hash);
  interestName.append(hex);
  return interestName;
}

void
fetchDeviceCert(std::shared_ptr<Node> node,
                const Name& wellknown,
                const std::string& deviceId,
                const Name& dname,
                OnDeviceCertSigned onDeviceCertSigned,
                OnFail onFail)
{
  Name btCertName(wellknown);
  btCertName.append("owner").append(deviceId).append("cert");
  node->showInterest(Interest{btCertName}, [&](const Data& d) {
    auto certBlock = d.getContent();
    auto cert = std::make_shared<Certificate>(certBlock);
    onDeviceCertSigned(cert);
  });
}

std::shared_ptr<Certificate>
generateIdentity(std::shared_ptr<ndn::KeyChain> keychain, ndn::Name name)
{
  auto id = keychain->createIdentity(name);
  return std::make_shared<Certificate>(id.getDefaultKey().getDefaultCertificate());
}

void
startBootstrap(const Name& wellknown,
               const std::string& deviceId,
               const std::string& sharedSecret,
               std::shared_ptr<ndn::KeyChain> keychain,
               std::shared_ptr<Node> node,
               OnOwnerCert onOwnerCert,
               OnDeviceCertSigned onDeviceCertSigned,
               OnFail onFail)
{
  const std::string randomNo = randomHex();
  Name interestName = constructBootstrapName(wellknown, randomNo, deviceId, sharedSecret);
  Interest interest(interestName);
  node->showInterest(interest, [&](const Data& d) {
    if (!validateOwnerCertResp(d, randomNo, sharedSecret)) {
      onFail("owner certificate is not valid.");
      return;
    }
    std::string dname;
    auto ownerCert = extractOwnerCert(d, dname);
    std::shared_ptr<Certificate> deviceCertUnsigned = generateIdentity(keychain, dname);
    onOwnerCert(ownerCert);
    serveDeviceUnsignedCert(node,
                            wellknown,
                            deviceId,
                            sharedSecret,
                            dname,
                            deviceCertUnsigned,
                            onDeviceCertSigned,
                            onFail);
  });
}


void
start(std::shared_ptr<Node> node,
      std::shared_ptr<Session> session,
      const Config& cfg,
      OnOwnerCert onOwnerCert,
      OnDeviceCertSigned onDeviceCertSigned,
      OnFail onFail)
{
  startBootstrap(cfg.wellknown,
                 cfg.pairId,
                 cfg.pairCode,
                 session->keychain,
                 node,
                 onOwnerCert,
                 onDeviceCertSigned,
                 onFail);
}

void
setTrustAnchor(std::shared_ptr<ndn::security::v2::Validator> validator,
               std::shared_ptr<Node> node,
               std::shared_ptr<Certificate> anchor)
{
  if (nullptr != validator) {
    validator->loadAnchor("", Certificate(*anchor));
  }
  else {
    node->setTrustAnchor(Certificate(*anchor));
  }
}


void
setDeviceCert(std::shared_ptr<KeyChain> keychain,
              std::shared_ptr<Identity> id,
              std::shared_ptr<Certificate> cert)
{
  auto keyBuf = cert->getPublicKey();
  auto keyName = cert->getKeyName();
  auto key = id->getDefaultKey();
  keychain->setDefaultCertificate(key, Certificate(*cert));
}

} // namespace impl
} // namespace bootstrap
} // namespace nacapp