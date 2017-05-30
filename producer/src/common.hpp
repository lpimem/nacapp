#ifndef COMMON_HPP
#define COMMON_HPP

#include <ndn-group-encrypt/producer.hpp>

#include "../../shared/src/common-cxx.hpp"
#include "../../shared/src/common-nac.hpp"
#include "../../shared/src/common-ndn.hpp"

namespace nacapp {
namespace producer {

using ndn::gep::Producer;


const ndn::name::Component NAME_COMPONENT_SAMPLES("SAMPLES");

} // producer
} // nacapp

#endif /* COMMON_HPP */
