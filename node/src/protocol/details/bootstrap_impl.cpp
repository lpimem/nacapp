#include "bootstrap_impl.hpp"

#include <ndn-cxx/util/sha256.hpp>


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
parseGwPubKey(BtSession* session, const ndn::Data& data, shared_ptr<DeviceConfig> config)
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
encryptDeviceKey(BtSession* session, shared_ptr<DeviceConfig> cfg)
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
                 BtSession* session,
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

bool
validateR2(BtSession* session, const ndn::Data& data)
{
  ndn::ConstBufferPtr secretKey = generateSecret(session->getCurrentState()->getConfig()->getPin());
  ndn::ConstBufferPtr content = sec::decrypt_aes(data, secretKey);
  const std::string msg = reinterpret_cast<const char*>(content->get());
  std::vector<std::string> parts = split(msg, '|');
  return session->getCurrentState()->getR2() == parts[0];
}

} // namespace impl
} // namespace protocol
} // namespace nacapp
