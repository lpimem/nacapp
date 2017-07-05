#ifndef COMMON_HPP
#define COMMON_HPP

#include <boost/date_time/posix_time/posix_time.hpp>

#include <ndn-cxx/common.hpp>
#include <ndn-cxx/face.hpp>
#include <ndn-cxx/util/signal.hpp>

#include <ndn-group-encrypt/group-manager.hpp>
#include <ndn-group-encrypt/schedule.hpp>

#include "../../shared/src/easylogging/easylogging++.h"

#include "keychain.hpp"

namespace nacapp {
using std::size_t;
using std::string;

using std::shared_ptr;
using std::unique_ptr;
using std::weak_ptr;
using std::bad_weak_ptr;
using std::make_shared;
using std::enable_shared_from_this;

using std::static_pointer_cast;
using std::dynamic_pointer_cast;
using std::const_pointer_cast;

using boost::gregorian::date;
using boost::gregorian::years;
using boost::posix_time::ptime;
using boost::posix_time::second_clock;

using std::function;
using std::bind;
using std::ref;
using std::cref;

using ndn::Interest;
using ndn::Data;
using ndn::Name;
using ndn::Exclude;
using ndn::Block;


using ndn::security::v2::Certificate;
using ndn::security::pib::Identity;
using ndn::security::pib::Key;
using ndn::security::SafeBag;

using ndn::gep::GroupManager;
using ndn::gep::Schedule;
using ndn::gep::RepetitiveInterval;

namespace name = ndn::name;
namespace time = ndn::time;
namespace signal = ndn::util::signal;

const ndn::name::Component NAME_COMPONENT_FOR("FOR");
const ndn::name::Component NAME_COMPONENT_READ("READ");
const ndn::name::Component NAME_COMPONENT_SAMPLE("SAMPLE");
const ndn::name::Component NAME_COMPONENT_ACCESS("ACCESS");
const ndn::name::Component NAME_COMPONENT_E_KEY("E-KEY");
const ndn::name::Component NAME_COMPONENT_D_KEY("D-KEY");
const ndn::name::Component NAME_COMPONENT_C_KEY("C-KEY");
const ndn::name::Component NAME_COMPONENT_IDENTITY("IDENTITY");

const string DB_PATH{"./manager-db"};
const string SCHEDULE_NAME_EVERY_DAY{"everyday"};

} // nacapp

#endif /* COMMON_HPP */