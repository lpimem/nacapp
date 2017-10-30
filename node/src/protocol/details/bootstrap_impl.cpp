#include "bootstrap_impl.hpp"

#include <ndn-cxx/util/sha256.hpp>
#include <sstream>


namespace nacapp {
namespace protocol {
namespace impl {

shared_ptr<Buffer>
generateDevicePublicKey(DeviceConfig& cfg)
{
  return NULL;
}

ndn::ConstBufferPtr
generateSecret(const std::string& pin)
{
  const uint8_t* content = reinterpret_cast<const uint8_t*>(pin.c_str());
  ndn::ConstBufferPtr result = nacapp::sec::compute_sha256(content, pin.length());
  return make_shared<Buffer>(result->buf(), 16);
  // return make_shared<Buffer>(ndn::crypto::sha256(content, pin.length())->get(), 16);
}

bool
authenticateInterest(const ndn::Interest& interest, ndn::ConstBufferPtr& key)
{
  // TODO: interests's signature not verified.
  return true;
}

void
parseGwPubKey(shared_ptr<BtSession> session, const ndn::Data& data, shared_ptr<DeviceConfig> config)
{
  LOG(INFO) << "start parsing gw pk";
  ndn::ConstBufferPtr secretKey = generateSecret(session->getCurrentState()->getConfig()->getPin());
  ndn::ConstBufferPtr content = nacapp::sec::decrypt_aes(data, secretKey);
  const std::string msg(reinterpret_cast<const char*>(content->get()), content->size());
  std::vector<std::string> parts = split(msg, '|');
  if (parts.size() < 3) {
    std::string error = "gw pubkey packet format error [" + msg + "]";
    LOG(ERROR) << error;
    throw BootstrapError(error);
  }
  std::string pubkey = parts[0];
  std::string devId = parts[1];
  std::string r1 = parts[2];

  if (devId != config->getId()) {
    std::string errmsg = "gw pubkey packet is invalid: dev id dismatch. "
                         "(Expecting: " +
                         config->getId() + ", got: " + devId;
    LOG(ERROR) << errmsg;
    throw BootstrapError(errmsg);
  }
  LOG(INFO) << "got pubkey: " << pubkey;
  shared_ptr<Buffer> pkBuf = ndn::fromHex(pubkey);
  config->setGwPubKey(*pkBuf);
  session->getCurrentState()->setR1(r1);
}

ndn::ConstBufferPtr
encryptDeviceKey(shared_ptr<BtSession> session, shared_ptr<DeviceConfig> cfg)
{
  ndn::ConstBufferPtr secretKey = generateSecret(session->getCurrentState()->getConfig()->getPin());
  const Buffer deviceKey = cfg->getDevicePubKey();
  const std::string r1 = session->getCurrentState()->getR1();
  const std::string r2 = randomHex();
  session->getCurrentState()->setR2(r2);
  std::string msg = toHex(deviceKey, true) + "|" + r1 + "|" + r2;
  return sec::encrypt_aes(msg, secretKey);
}

void
publishDeviceKey(shared_ptr<Face> face,
                 shared_ptr<ndn::KeyChain> kc,
                 shared_ptr<BtSession> session,
                 shared_ptr<DeviceConfig> cfg,
                 const Name& name)
{
  ndn::ConstBufferPtr cipher = encryptDeviceKey(session, cfg);
  auto data = make_shared<Data>();
  data->setName(name);
  data->setFreshnessPeriod(time::milliseconds(2000));
  data->setContent(cipher);
  kc->sign(*data);
  face->put(*data);
}


Step3Payload
parseStep3Payload(shared_ptr<BtSession> session, const ndn::Data& data)
{
  std::string pin = session->getCurrentState()->getConfig()->getPin();
  std::cout << "pin: " << pin << std::endl;
  std::cout << "----> " << data.getContent().value_size() << std::endl;
  ndn::ConstBufferPtr secretKey = generateSecret(pin);
  ndn::ConstBufferPtr content = sec::decrypt_aes(data, secretKey);
  const std::string msg = reinterpret_cast<const char*>(content->get());
  std::vector<std::string> parts = split(msg, '|');
  if (parts.size() < 3) {
    std::stringstream ss;
    ss << "Invalid step 3 payload : expecting 3 parts, got " << parts.size();
    throw ss.str();
  }
  return Step3Payload(parts[0], parts[1]);
}

void
validateR2(shared_ptr<BtSession> session, const Step3Payload& payload)
{
  if (session->getCurrentState()->getR2() != payload.R2) {
    std::stringstream ss;
    ss << "R2 dismatch! " << std::endl
       << "\t Expecting:\t" << session->getCurrentState()->getR2() << std::endl
       << "\t Got:\t\t:" << payload.R2 << std::endl;
    throw ss.str();
  }
}

void
parseDeviceCertificate(shared_ptr<BtSession> session, const Step3Payload& payload)
{
  std::string certStr = payload.certEncoded;
  shared_ptr<Buffer> buf = ndn::fromHex(payload.certEncoded);
  auto cert = std::make_shared<Certificate>(Block(buf));
  session->getCurrentState()->getConfig()->deviceCert = cert;
  return;
}

} // namespace impl
} // namespace protocol
} // namespace nacapp
