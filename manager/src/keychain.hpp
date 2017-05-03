#ifndef KEYCHAIN_HPP
#define KEYCHAIN_HPP

#include <ndn-cxx/security/key-chain.hpp>

namespace nacapp {
// todo: read configuration file instead
ndn::security::v2::KeyChain AppKeyChain("pib-memory", "tpm-memory");
}

#endif /* KEYCHAIN_HPP */
