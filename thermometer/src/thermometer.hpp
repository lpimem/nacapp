#ifndef THERMOMETER_HPP
#define THERMOMETER_HPP

#include "common.hpp"

namespace nacapp {
namespace thermometer {


/**
 * Notes:
 *     Name : /local-home/location/.../temperature
 *            \_________  ___________/\____  _____/
 *                      \/                 \/
 *                   Prefix             DataType
 *             
 */
class Thermometer
{
public:
  Thermometer(string localPrefix,
              string location,
              string dataType,
              shared_ptr<Face> f,
              const string& producerDbPath,
              shared_ptr<KeyChain> kc);

public:
  void
  run();

private:
  void
  registerPrefixes();

  bool
  onGetTemperature(const Interest& interest,
                   const Name& args,
                   shared_ptr<Data> data,
                   InterestShower show,
                   PutData put);

  bool
  onGetContentKey(const Interest& interest,
                  const Name& args,
                  shared_ptr<Data> data,
                  InterestShower show,
                  PutData put);

  void
  setPrefix(Name prefix, Name location);

private:
  int
  readTemp();

private:
  void
  onNACProduceError(const ndn::gep::ErrorCode& code, const std::string& msg);

  void
  onContentKeyEncrypted(PutData put, const Interest& interest, const std::vector<Data>& d);

  shared_ptr<Data>
  searchCKey(const Name& ckey);

private:
  std::string m_location;
  std::string m_dataType;
  unique_ptr<Node> m_node;
  shared_ptr<Producer> m_producer;
  map<Name, shared_ptr<Data>> m_ckeys;
  // Producer m_producer
}; // class Thermometer
} // namespace thermometer
} // namespace nacapp
#endif /* THERMOMETER_HPP */
