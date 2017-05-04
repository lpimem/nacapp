#ifndef SERVICE_HPP
#define SERVICE_HPP

#include "common.hpp"
#include "manager.hpp"

namespace nacapp {

/**
 * NDN Service endpoint
 *
 * namespace
 * name   = <prefix>/<domain>/<args>
 * prefix = (\/[^\s\/]+)+      // regular expression
 * domain = READ        |
 *          MANAGEMENT  |
 *          IDENTITY    |
 *          SERVICE
 * args   = (\/[^\s\/]+)+
 */
class Service {

public:
  Service(const Name &prefix, shared_ptr<Manager> manager)
      : m_prefix(prefix), m_manager(manager) {}

public:
  void serveForever();

private:
  // route interests to handlers
  // should have some middle layer wrappers to pre-process interests and post-
  // process data.
  //
  // in post-processing, data are segmented into chuncks if too large.
  // business handler can only set the content, content type, and name
  void onInterest(const ndn::InterestFilter &filter, const Interest &interest);

  void registerPrefixes();

  void onRegisterPrefixFailed(const Name &prefix, const string reason);

  /* ********** Group Cryptography Handlers ********** */

  // <prefix>/READ/<data-type>/E-Key/<strat>/<end>
  void onGetEKey(const Interest &interest);

  // <prefix>/READ/<data-type>/D-Key/<strat>/<end>
  void onGetDKey(const Interest &interest);

  /* ********** Identity Hanlders ********** */

  // <prefix>/IDENTITY/for/<entity-name>
  /**
  Interest must be signed by a trusted key, which
  should be the same with the one used to generate
  the identity.

  For example:
    sign-key = HASH(<short-passwd>, <entity-name>, nonce)
  */
  void onGetIdentityKey(const Interest &interest);

  /* ********** Management Hanlders ********** */

  // <prefix>/MANAGEMENT/identity/add/<identity-name>
  /**
  Interest must be signed by a trusted key.
  The trusted key should differ among entities.
  Manager should remember this key at least until the identity's key is served.
  */
  void onAddIdentity(const Interest &interest);

  // <prefix>/MANAGEMENT/identity/remove/<identity-name>
  void onRemoveIdentity(const Interest &interest);

  // <prefix>/MANAGEMENT/access/grant/BASE64Encode(<identity-name>)/BASE64Encode(<data-type>)
  void onGrant(const Interest &interest);

  // <prefix>/MANAGEMENT/access/revoke/BASE64Encode(<identity-name>)/BASE64Encode(<data-type>)
  void onRevoke(const Interest &interest);

private:
  ndn::Face m_face;
  const Name &m_prefix;
  shared_ptr<Manager> m_manager;
};

} // nacapp

#endif /* SERVICE_HPP */