#ifndef BT_NAMING_HPP
#define BT_NAMING_HPP

#include "../common.hpp"

namespace nacapp {
namespace protocol {


inline Name
nameOwnerPublicKey(Name ownerPrefix, const string& deviceId)
{
  return ownerPrefix.append("for").append(deviceId);
}

inline Name
nameOwnerAttestation(Name ownerPrefix, const string& deviceId)
{
  return ownerPrefix.append("auth").append("for").append(deviceId);
}

inline Name
nameDevicePublicKey(Name devicePrefix)
{
  return devicePrefix.append("pubkey");
}
} // namespace protocol
} // namespace nacapp
#endif //BT_NAMING_HPP
