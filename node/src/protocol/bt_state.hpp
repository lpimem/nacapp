#ifndef BT_STATE_HPP
#define BT_STATE_HPP

#include "bt_common.hpp"
#include "bt_session.hpp"


namespace nacapp {
namespace protocol {

class BtSession;

enum class BtStage {
  FetchingGwPubkey = 1,
  PublishingSelfPubkey = 2,
  FetchingGwAuth = 3,
  FailFetchingGwPubkey = 401,
  FailPublishingSelfPubkey = 402,
  FailFetchingGwAuth = 403
};

/**
 * A naive implementation of the bootstrap state machine.
 */
class BtState
{
public:
  BtState(shared_ptr<Face> face,
          shared_ptr<KeyChain> keychain,
          shared_ptr<BtSession> session,
          shared_ptr<DeviceConfig> config,
          BtStage stg)
    : m_face(face)
    , m_keychain(keychain)
    , m_config(config)
    , m_stage(stg)
    , m_session(session)
    , m_processing(false)
  {
  }

  BtStage
  getStage() const;

  void
  setStage(BtStage stage);

  shared_ptr<DeviceConfig>
  getConfig() const;

  void
  setConfig(shared_ptr<DeviceConfig> config);

  shared_ptr<BtSession>
  getSession();

  std::string
  getR1() const;

  void
  setR1(const std::string& r1);

  std::string
  getR2() const;

  void
  setR2(const std::string& r2);

  virtual void
  next()
  {
  }

protected:
  bool
  isProcessing()
  {
    return m_processing;
  }

  void
  flagProcessing()
  {
    m_processing = true;
  }

  void
  flagProcessed()
  {
    m_processing = false;
  }

  bool
  isInSession();

protected:
  shared_ptr<Face> m_face;
  shared_ptr<KeyChain> m_keychain;
  shared_ptr<DeviceConfig> m_config;

private:
  BtStage m_stage;
  shared_ptr<BtSession> m_session;
  std::string r1;
  std::string r2;
  bool m_processing;
};

} // namespace protocol
} // namespace nacapp


#endif /* BT_STATE_HPP */
