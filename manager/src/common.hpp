#ifndef COMMON_HPP
#define COMMON_HPP

#include "../../shared/src/common-cxx.hpp"
#include "../../shared/src/common-nac.hpp"
#include "../../shared/src/common-ndn.hpp"
#include "../../shared/src/easylogging/easylogging++.h"

#include "keychain.hpp"

namespace nacapp {

using ndn::security::v2::KeyChain;

const string DB_PATH{"./manager-db"};

} // nacapp

#endif /* COMMON_HPP */