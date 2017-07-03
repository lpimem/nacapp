#ifndef SERVICE_HPP
#define SERVICE_HPP

#include <ndn-cxx/key-locator.hpp>

#include "../../node/src/handlers.hpp"

#include "manager.hpp"
#include "validator/data_type_filter.hpp"

namespace nacapp {

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
  Service(const Name& prefix, shared_ptr<KeyChain> kc)
    : m_prefix(prefix)
    , m_keychain(kc)
    , m_manager(make_shared<Manager>(prefix, kc))
  {
  }

public:
  /* ********** Group Cryptography Handlers ********** */


  // <prefix>/READ/<data-type>/E-Key | D-Key[/for/<user>]?Exclude=<timestamp>, *
  // Path : /READ/<data-type>/E-Key | D-Key
  // Args : /<user>/<timestamp>
  bool
  onRead(const Interest& interest,
         const Name& args,
         shared_ptr<Data> data,
         InterestShower show,
         PutData put);


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
  bool
  onGetIdentityKey(const Interest& interest,
                   const Name& args,
                   shared_ptr<Data> data,
                   InterestShower show,
                   PutData put);

  /* ********** Management Hanlders ********** */

  /** <prefix>/MANAGEMENT/identity/add/<identity-name>
      Manager should sign an identity's key.
      Interest must be signed by a trusted key.
      The trusted key should differ among entities.
      Manager should remember this key at least until the identity's key is served.
      Path: /MANAGEMENT/identity/add
      Args: <identity-name>
  */
  bool
  onAddIdentity(const Interest& interest,
                const Name& args,
                shared_ptr<Data> data,
                InterestShower show,
                PutData put);

  // <prefix>/MANAGEMENT/identity/remove/<identity-name>
  bool
  onRemoveIdentity(const Interest& interest,
                   const Name& args,
                   shared_ptr<Data> data,
                   InterestShower show,
                   PutData put);

  // <prefix>/MANAGEMENT/access/grant/URIEncode(<identity-name>)/URIEncode(<data-type>)
  // path: /MANAGEMENT/access/grant
  // args:
  //       /URIEncode(<identity-name>)
  //       /URIEncode(<data-type>)
  //       /grant type
  //       /grant arguments (/start/end/startHour/endHour)
  bool
  onGrant(const Interest& interest,
          const Name& args,
          shared_ptr<Data> data,
          InterestShower show,
          PutData put);

  // <prefix>/MANAGEMENT/access/revoke/URIEncode(<identity-name>)/URIEncode(<data-type>)
  bool
  onRevoke(const Interest& interest,
           const Name& args,
           shared_ptr<Data> data,
           InterestShower show,
           PutData put);

public:
  shared_ptr<Manager>
  getManager()
  {
    return m_manager;
  }

private:
  bool
  onGetEKey(const Interest& interest,
            const Name& datatype,
            const Name& args,
            shared_ptr<Data> data,
            InterestShower show,
            PutData put);

  bool
  onGetDKey(const Interest& interest,
            const Name& datatype,
            const Name& args,
            shared_ptr<Data> data,
            InterestShower show,
            PutData put);

  void
  grant(const Name& identity,
        const Name& dataType,
        string startDate,
        string endDate,
        string startHour,
        string endHour);

private:
  Buffer
  parseIdentityPubKey(const Data& key);
  Certificate
  signPubkey(const Buffer& key);
  void
  authenticateManagementInterest(const Interest& interest, const Name args);

private:
  const Name& m_prefix;
  shared_ptr<KeyChain> m_keychain;
  shared_ptr<Manager> m_manager;
};

} // nacapp

#endif /* SERVICE_HPP */