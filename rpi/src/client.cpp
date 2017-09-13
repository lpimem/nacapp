#include "client.hpp"

#include <set>
#include <vector>
#include <fstream>
#include <map>
#include <utility>
#include <algorithm>
#include <queue>
#include <string>

const std::string STATUS = "status";
const std::string TEMP = "temp";
const std::string MOTOR = "motor";
const std::string LED = "led";
const std::string REG = "reg";

namespace ndn
{
namespace smarthome
{
// TODO: Lei: Instead of calling this a "Client",
//  the name "Service" or "Gateway" is more precise.
Client::Client(std::string prefix)
{
  m_interestPrefix = prefix;
}

void Client::run()
{
  m_face.setInterestFilter(m_interestPrefix,
                           bind(&Client::onInterest, this, _1, _2),
                           RegisterPrefixSuccessCallback(),
                           bind(&Client::onRegisterFailed, this, _1, _2));

  m_face.processEvents();
}

void Client::onInterest(const InterestFilter &filter, const Interest &interest)
{
  Name name(interest.getName());
  std::string opt = name.get(3).toUri();
  if (opt == STATUS)
  {
    doRecieveStatus(name);
  }
  else if (opt == TEMP)
  {
    std::string ss = name.get(4).toUri();
    if (ss == "Get")
      doRecieveGetTemp(name);
    else
    {
      int t = stoi(name.get(5).toUri());
      doRecieveSetTemp(name, t);
    }
  }
  else if (opt == MOTOR)
  {
    int isOn = stoi(name.get(4).toUri());
    doRecieveMotor(name, isOn);
  }
  else if (opt == LED)
  {
    int isOn = stoi(name.get(4).toUri());
    doRecieveLed(name, isOn);
  }
  else if (opt == REG)
  {
    std::string userClientName = name.get(4).toUri();
    //doUserClientReg(userClientName);
  }
}

void
Client::onRegisterFailed(const Name &prefix, const std::string &reason)
{
  std::cerr << "ERROR: Failed to register prefix \""
            << prefix << "\" in local hub's daemon (" << reason << ")"
            << std::endl;
  m_face.shutdown();
}

void Client::doRecieveStatus(Name &name)
{
  //std::string content = std::to_string(temp.getTemp()) + ":" + std::to_string(motor.get()) + ":" + std::to_string(led.get());
  //sendData(name, content);
}

void Client::doRecieveGetTemp(Name &name)
{
  int t = temp.getTemp();
  sendData(name, to_string(t));
}

void Client::doRecieveSetTemp(Name &name, int target)
{
  int current = temp.getTemp();
  if (current > target){
    //motor.set(1);
  } else{
    //motor.set(0);
  }
}

void Client::doRecieveMotor(Name &name, int on)
{
  int suc = motor.set(on);
  sendData(name, to_string(suc));
}

void Client::doRecieveLed(Name &name, int on)
{
  std::cout << on << std::endl;
  int suc = led.set(on);
  sendData(name, to_string(suc));
}

void doUserClientReg(std::string u)
{
  return;
}

void Client::sendData(Name &dataName, std::string content)
{
  shared_ptr<Data> data = make_shared<Data>();
  data->setName(dataName);
  data->setFreshnessPeriod(time::seconds(10));
  data->setContent(reinterpret_cast<const uint8_t *>(content.c_str()), content.length());
  m_keyChain.sign(*data);
  std::cout << "Send Data Back: " << content << std::endl;
  m_face.put(*data);
}

} // namespace smarthome
} // namespace ndn

int main(int argc, char **argv)
{
  wiringPiSetup();
  std::cout << "new client and registering ndn prefix..." << std::endl;
  ndn::smarthome::Client client("/ndn/smarthome");
  try
  {
    std::cout << "starting client..." << std::endl;
    client.run();
  }
  catch (const std::exception &e)
  {
    std::cerr << "ERROR: " << e.what() << std::endl;
  }
  return 0;
}
