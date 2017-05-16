#ifndef SHARED_COMMON_NDN_HPP
#define SHARED_COMMON_NDN_HPP

#include <ndn-cxx/common.hpp>
#include <ndn-cxx/encoding/tlv.hpp>
#include <ndn-cxx/face.hpp>
#include <ndn-cxx/util/signal.hpp>

namespace nacapp {

using ndn::make_unique;

using ndn::Interest;
using ndn::Data;
using ndn::Buffer;
using ndn::Block;
using ndn::Name;
using ndn::Exclude;
using ndn::Block;
using ndn::Face;

using ndn::security::v1::PublicKey;
using ndn::security::v2::Certificate;
using ndn::security::v2::KeyChain;
using ndn::security::pib::Identity;
using ndn::security::pib::Key;
using ndn::security::SafeBag;

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
}

#endif // nacapp