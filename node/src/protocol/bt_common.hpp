#ifndef BT_COMMON
#define BT_COMMON

#include <string>

#include <ndn-cxx/face.hpp>
#include <ndn-cxx/interest-filter.hpp>
#include <ndn-cxx/util/string-helper.hpp>

#include "../../../shared/src/ndn-util.hpp"
#include "../../../shared/src/string_helpers.hpp"
#include "../common.hpp"

#include "bt_naming.hpp"
#include "device_config.hpp"

using ndn::InterestFilter;
using ndn::RegisteredPrefixId;
using ndn::NackCallback;
using ndn::RegisterPrefixSuccessCallback;

class BootstrapError : std::runtime_error
{
public:
  BootstrapError(const std::string& msg)
    : runtime_error(msg)
  {
  }
};

#endif /* BT_COMMON */