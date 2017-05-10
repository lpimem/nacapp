#ifndef SERVICE_HPP
#define SERVICE_HPP

#include <ndn-cxx/key-locator.hpp>

#include "manager.hpp"
#include "validator/data_type_filter.hpp"
#include "../../node/src/handlers.hpp"

namespace nacapp
{

/**
 * NDN Service endpoint
 *
 * namespace
 * name   = <prefix>/<domain>/<args>
 * prefix = (\/[^\s\/]+)+  // TODO: should recheck with tlv specification
 * domain = READ        |
 *          MANAGEMENT  |
 *          IDENTITY    |
 *          SERVICE
 * args   = (\/[^\s\/]+)+
 */
class Service
{

public:
  Service(const Name &prefix) : m_prefix(prefix), m_manager(prefix) {}

public:
  /* ********** Group Cryptography Handlers ********** */

  // <prefix>/READ/<data-type>/E-Key/for/<user>/<timestamp>
  // Path : /READ/<data-type>/E-Key/for
  // Args : /<user>/<timestamp>
  void onGetEKey(const Interest &interest,
                 const Name args,
                 shared_ptr<Data> data,
                 InterestShower show);

  // <prefix>/READ/<data-type>/D-Key/for/<user>/<timestamp>
  // Path : /READ/<data-type>/D-Key/for
  // Args : /<user>/<timestamp>
  void onGetDKey(const Interest &interest,
                 const Name args,
                 shared_ptr<Data> data,
                 InterestShower show);

  /* ********** Identity Hanlders ********** */

  // <prefix>/IDENTITY/for/<entity-name>
  /**

  Serve the certificate for an identity signed by the manager.

  Interest should be signed by a trusted key, which
  should be the same with the one used to generate
  the identity.
  For example:
    sign-key = HASH(<short-passwd>, <entity-name>, nonce)

  Path: /IDENTITY/for
  Args: <entity-name>

  */
  void onGetIdentityKey(const Interest &interest,
                        const Name args,
                        shared_ptr<Data> data,
                        InterestShower show);

  /* ********** Management Hanlders ********** */

  /** <prefix>/MANAGEMENT/identity/add/<identity-name>

  Manager should sign an identity's key.
  
  Interest must be signed by a trusted key.
  The trusted key should differ among entities.
  Manager should remember this key at least until the identity's key is served.

  Path: /MANAGEMENT/identity/add
  Args: <identity-name>
  */
  void onAddIdentity(const Interest &interest,
                     const Name args,
                     shared_ptr<Data> data,
                     InterestShower show);

  // <prefix>/MANAGEMENT/identity/remove/<identity-name>
  void onRemoveIdentity(const Interest &interest,
                        const Name args,
                        shared_ptr<Data> data,
                        InterestShower show);

  // <prefix>/MANAGEMENT/access/grant/BASE64Encode(<identity-name>)/BASE64Encode(<data-type>)
  // path: /MANAGEMENT/access/grant
  // args:
  //       /BASE64Encode(<identity-name>)
  //       /BASE64Encode(<data-type>)
  //       /grant type
  //       /grant arguments (/start/end/startHour/endHour)
  void onGrant(const Interest &interest,
               const Name args,
               shared_ptr<Data> data,
               InterestShower show);

  // <prefix>/MANAGEMENT/access/revoke/BASE64Encode(<identity-name>)/BASE64Encode(<data-type>)
  void onRevoke(const Interest &interest,
                const Name args,
                shared_ptr<Data> data,
                InterestShower show);

private:
  void grant(const Name identity,
             const Name dataType,
             string startDate,
             string endDate,
             string startHour,
             string endHour);

private:
  Buffer parseIdentityPubKey(const Data &key);
  Certificate signPubkey(const Buffer &key);
  void authenticateManagementInterest(const Interest &interest, const Name args);

private:
  const Name &m_prefix;
  Manager m_manager;
};

} // nacapp

#endif /* SERVICE_HPP */