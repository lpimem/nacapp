
#include "manager.hpp"

namespace nacapp {

Identity Manager::addIdentity(const Name &entity) {
  Identity identity = AppKeyChain.createIdentity(entity);
  return identity;
}

Data Manager::getCertificate(const Name &identity) {
  Data d;
  return d;
}

void Manager::removeIdentity(const Name &identity) {}

/**
* grant @p entity access to @p dataType
*
* Details:
*   - Create a GroupManager /<m_prefix>/READ/<dataType> if not exists
*   - Create a schedule 7x24 with access if not exists
*   - Add @p entity's certificate to schedule.
*/
void Manager::grantAccess(const Name &entity, const Name &dataType) {}

/**
* revoke @p entity access to @p dataType
*/
void Manager::revokeAccess(const Name &entity, const Name &dataType) {}

void Manager::createGroup(const Name &group, const Name &dataType) {}

void Manager::deleteGroup(const Name &group, const Name &dataType) {}

void Manager::addGroupMember(const Name &group, const Name &entity) {}

void Manager::removeGroupMember(const Name &group, const Name &entity) {}

/**
 * @return schedule name
 */
string Manager::createSchedule(const GroupManager &group,
                               const Name &dataType) {}

} // nacapp