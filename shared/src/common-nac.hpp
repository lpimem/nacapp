#ifndef SHARED_COMMON_NAC_HPP
#define SHARED_COMMON_NAC_HPP

#include <ndn-group-encrypt/group-manager.hpp>
#include <ndn-group-encrypt/schedule.hpp>

namespace nacapp {
using ndn::gep::GroupManager;
using ndn::gep::Schedule;
using ndn::gep::RepetitiveInterval;
using ndn::gep::TimeStamp;

const string SCHEDULE_NAME_EVERY_DAY{"everyday"};

} // nacapp
#endif