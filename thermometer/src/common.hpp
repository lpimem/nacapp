#ifndef COMMON_HPP
#define COMMON_HPP

#include <ndn-group-encrypt/producer.hpp>

#include "../../shared/src/common-cxx.hpp"
#include "../../shared/src/common-nac.hpp"
#include "../../shared/src/common-ndn.hpp"
#include "../../shared/src/easylogging/easylogging++.h"
#include "../../shared/src/keychain.hpp"

#include "../../node/src/node.hpp"


namespace nacapp {
namespace thermometer {

using ndn::gep::Producer;
using nacapp::Node;

const ndn::name::Component NAME_COMPONENT_SAMPLES("SAMPLES");
const ndn::name::Component NAME_COMPONENT_TEMPERATURE("temperature");

// second
const int QUERY_TIME_DURATION = 10;

} // producer
} // nacapp

#endif /* COMMON_HPP */
