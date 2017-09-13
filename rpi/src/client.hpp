#include <ndn-cxx/common.hpp>
#include <ndn-cxx/face.hpp>
#include <ndn-cxx/security/key-chain.hpp>
#include <ndn-cxx/util/scheduler.hpp>

#include "temp.hpp"
#include "motor.hpp"
#include "led.hpp"

namespace ndn
{
namespace smarthome
{

//class Temp;
//class Motor;
//class Led;

class Client : noncopyable
{
public:
  Client(std::string prefix);

  void
  run();

private:
  void
  onInterest(const InterestFilter &filter, const Interest &interest);

  void
  onRegisterFailed(const Name &prefix, const std::string &reason);

  void
  doRecieveStatus(Name &name);

  void
  doRecieveGetTemp(Name &name);

  void
  doRecieveSetTemp(Name &name, int target);

  void
  doRecieveMotor(Name &name, int on);

  void
  doRecieveLed(Name &name, int on);

  void
  doUserClientReg(Name &name, std::string u);

  void
  sendData(Name &dataName, std::string content);

private:
  std::string m_interestPrefix;
  Face m_face;
  KeyChain m_keyChain;
  Temp temp;
  Motor motor;
  Led led;
};

} // namespace smarthome
} // namespace ndn
