#ifndef MANAGER_HPP
#define MANAGER_HPP

#include "common.hpp"
#include "named-schedule.hpp"

namespace nacapp {

/**
 * A manager controls access to all data under its namespace.
 */
class Manager {

public:
  Manager(const Name &prefix) : m_prefix(prefix) {}

public:
  /**
   * add identity with given cert packet
   */
  void addIdentity(const Name &entity, const Data &cert);

  shared_ptr<Certificate> getIdentity(const Name &entity);

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
  void grantAccess(const Name &entity, const Name &dataType,
                   const NamedInterval &schedule);

  void grantAccess(const Name &entity, const Name &dataType);

  /**
  * revoke @p entity access to @p dataType
  */
  void revokeAccess(const Name &entity, const Name &dataType);

  /**
   * @return the group manager of @p entity for @p dataType
   */
  shared_ptr<GroupManager> getGroup(const Name &entity, const Name &dataType);

private:
  shared_ptr<GroupManager> createGroup(const Name &dataType);

  void deleteGroup(const Name &dataType);

  void addGroupMember(const Name &dataType, const Name &identity,
                      const string &scheduleName);

  void removeGroupMember(const Name &dataType, const Name &identity);

  /**
   * Creates schedule for @p dataType only when no schedule is found for
   * @p namedInterval.getName().
   * @throws string error if no group for @p dataType could be found.
   * @return schedule name
   */
  string createSchedule(const Name &dataType,
                        const NamedInterval &namedInterval);

  NamedInterval defaultSchedule(const Name &dataType);

  Certificate getEntityCert(const Name &entity);

  static string getGroupFullName(const Name &group, const Name &dataType);

private:
  void validateCertificate(const Name &name, shared_ptr<Certificate> cert);

private:
  const Name m_prefix;
  std::map<string, shared_ptr<Certificate>> m_identities;
  std::map<string, shared_ptr<Data>> m_dkey_cache;
  std::map<string, shared_ptr<GroupManager>> m_groups;
  std::map<string, set<string>> m_group_schedules;
};

} // nacapp
#endif /* MANAGER_HPP */
