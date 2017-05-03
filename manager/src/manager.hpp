#ifndef MANAGER_HPP
#define MANAGER_HPP

#include "common.hpp"

namespace nacapp {

/**
 * A manager controls access to all data under its namespace.
 */
class Manager {

public:
  Manager(string prefix) : m_prefix(prefix) {}

public:
  /**
   * Manager creates Identity-key-certificate tuple for @p entity
   * The created tuple is stored in PIB[1], and can be retrieved from
   * keychain[2].
   *
   * [1]
   * https://github.com/named-data/ndn-cxx/blob/master/src/security/pib/pib.hpp#L36
   * [2]
   * https://github.com/named-data/ndn-cxx/blob/master/src/security/v2/key-chain.hpp#L41
   */
  Identity addIdentity(const Name &entity);

  /**
  * Returns the certificate of an identity
  */
  Data getCertificate(const Name &identity);

  /**
  * Delete certificate from local store. Should call revokeAccess for
  * all dataTypes.
  */
  void removeIdentity(const Name &identity);

  /**
  * grant @p entity access to @p dataType
  *
  * Details:
  *   - Create a GroupManager /<m_prefix>/READ/<dataType> if not exists
  *   - Create a schedule 7x24 with access if not exists
  *   - Add @p entity's certificate to schedule.
  */
  void grantAccess(const Name &entity, const Name &dataType);

  /**
  * revoke @p entity access to @p dataType
  */
  void revokeAccess(const Name &entity, const Name &dataType);

private:
  void createGroup(const Name &group, const Name &dataType);

  void deleteGroup(const Name &group, const Name &dataType);

  void addGroupMember(const Name &group, const Name &entity);

  void removeGroupMember(const Name &group, const Name &entity);

  /**
   * @return schedule name
   */
  string createSchedule(const GroupManager &group, const Name &dataType);

private:
  Name m_prefix;
  std::map<string, Data *> m_certs;
  std::map<string, GroupManager *> m_groups;
};

} // nacapp
#endif /* MANAGER_HPP */
