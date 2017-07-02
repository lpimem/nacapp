#ifndef SHARED_COMMON_NAC_HPP
#define SHARED_COMMON_NAC_HPP

#include <ndn-cxx/security/security-common.hpp>

#include <ndn-group-encrypt/group-manager.hpp>
#include <ndn-group-encrypt/schedule.hpp>

namespace nacapp {
using ndn::gep::GroupManager;
using ndn::gep::Schedule;
using ndn::gep::RepetitiveInterval;
using ndn::gep::TimeStamp;

const std::string SCHEDULE_NAME_EVERY_DAY{"everyday"};
const uint32_t DEFAULT_RSA_KEY_SIZE = 2048;

} // nacapp
#endif