#include "common.hpp"
#include "thermometer.hpp"

INITIALIZE_EASYLOGGINGPP

int
main()
{
  const std::string PREFIX = "/local-home";
  const std::string LOCATION = "/location/bedroom";
  const std::string DTYPE = "temperature";
  const std::string DB = "./test.db";

  auto f = std::make_shared<ndn::Face>("127.0.0.1");
  // auto node = std::make_shared<nacapp::Node>(PREFIX, f);
  // auto nacProd = std::make_shared<ndn::gep::Producer>(PREFIX, DTYPE, *f, DB);
  nacapp::thermometer::Thermometer t{PREFIX, LOCATION, f};
  t.run();
  return 0;
}