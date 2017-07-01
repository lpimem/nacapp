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
  Thermometer(string localPrefix, string location, shared_ptr<Face> f, const string& producerDbPath);

public:
  void
  run();

private:
  void
  registerPrefixes();

  void
  onGetTemperature(const Interest& interest,
                   const Name& args,
                   shared_ptr<Data> data,
                   InterestShower show,
                   PutData put);

  void
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

private:
  std::string m_location;
  std::string m_dataType;
  unique_ptr<Node> m_node;
  unique_ptr<Producer> m_producer;
}; // class Thermometer
} // namespace thermometer
} // namespace nacapp
#endif /* THERMOMETER_HPP */
