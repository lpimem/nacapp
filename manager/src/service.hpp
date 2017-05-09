#ifndef SERVICE_HPP
#define SERVICE_HPP

#include "manager.hpp"
#include "validator/data_type_filter.hpp"

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
  Service(const Name &prefix) : m_prefix(prefix), m_manager(prefix) {}

public:
  /* ********** Group Cryptography Handlers ********** */

  // <prefix>/READ/<data-type>/E-Key/for/<user>/<start>/<end>
  // Path : /READ/<data-type>/E-Key/for
  // Args : /<user>/<timestamp>
  void onGetEKey(const Interest &interest, const Name args,
                 shared_ptr<Data> data);

  // <prefix>/READ/<data-type>/D-Key/<start>/<end>
  void onGetDKey(const Interest &interest, const Name args,
                 shared_ptr<Data> data);

  /* ********** Identity Hanlders ********** */

  // <prefix>/IDENTITY/for/<entity-name>
  /**

  Serve the certificate for an identity signed by the manager.

  Interest should be signed by a trusted key, which
  should be the same with the one used to generate
  the identity.

  For example:
    sign-key = HASH(<short-passwd>, <entity-name>, nonce)
  */
  void onGetIdentityKey(const Interest &interest, const Name args,
                        shared_ptr<Data> data);

  /* ********** Management Hanlders ********** */

  // <prefix>/MANAGEMENT/identity/add/<identity-name>
  /**
  Interest must be signed by a trusted key.
  The trusted key should differ among entities.
  Manager should remember this key at least until the identity's key is served.
  */
  void onAddIdentity(const Interest &interest, const Name args,
                     shared_ptr<Data> data);

  // <prefix>/MANAGEMENT/identity/remove/<identity-name>
  void onRemoveIdentity(const Interest &interest, const Name args,
                        shared_ptr<Data> data);

  // <prefix>/MANAGEMENT/access/grant/BASE64Encode(<identity-name>)/BASE64Encode(<data-type>)
  // path: /MANAGEMENT/access/grant
  // args:
  //       /BASE64Encode(<identity-name>)
  //       /BASE64Encode(<data-type>)
  //       /start
  //       /start

  void onGrantFixed(const Interest &interest, const Name args,
                    shared_ptr<Data> data);

  // <prefix>/MANAGEMENT/access/revoke/BASE64Encode(<identity-name>)/BASE64Encode(<data-type>)
  void onRevoke(const Interest &interest, const Name args,
                shared_ptr<Data> data);

private:
  void grant(const Name identity, const Name dataType, string start, string end,
             string interval);

private:
  const Name &m_prefix;
  Manager m_manager;
};

} // nacapp

#endif /* SERVICE_HPP */