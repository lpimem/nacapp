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
const int DEFAULT_KEY_FRESH_PERIOD = 1;
const uint32_t DEFAULT_RSA_KEY_SIZE = 2048;

} // nacapp

#endif /* COMMON_HPP */