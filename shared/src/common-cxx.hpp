#ifndef SHARED_COMMON_CXX_HPP
#define SHARED_COMMON_CXX_HPP

#include <map>
#include <set>
#include <string>
#include <vector>

#include <boost/date_time/posix_time/posix_time.hpp>

#include "../../shared/src/easylogging/easylogging++.h"

namespace nacapp {
using std::size_t;
using std::string;
using std::vector;
using std::map;
using std::set;

using std::shared_ptr;
using std::unique_ptr;
using std::weak_ptr;
using std::bad_weak_ptr;
using std::make_shared;
using std::enable_shared_from_this;

using std::function;
using std::bind;
using std::ref;
using std::cref;

using std::static_pointer_cast;
using std::dynamic_pointer_cast;
using std::const_pointer_cast;

using boost::gregorian::date;
using boost::gregorian::years;
using boost::posix_time::ptime;
using boost::posix_time::second_clock;
}

#endif