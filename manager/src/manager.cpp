#include "manager.hpp"

namespace nacapp {

Identity Manager::addIdentity(const Name &entity) {
  Identity identity = AppKeyChain.createIdentity(entity);
  return identity;
}

Data Manager::getSafeBag(const Name &identity) {
  Identity id = AppKeyChain.createIdentity(identity);
  Certificate cert = id.getDefaultKey().getDefaultCertificate();
  shared_ptr<SafeBag> safeBag = AppKeyChain.exportSafeBag(cert, NULL, 0);
  Data d(safeBag->wireEncode());
  Name bagName(m_prefix);
  bagName.append(NAME_COMPONENT_IDENTITY);
  bagName.append(identity);
  d.setName(bagName);
  return d;
}

void Manager::removeIdentity(const Name &identity) {
  Identity id = AppKeyChain.createIdentity(identity);
  AppKeyChain.deleteIdentity(id);
}

void Manager::grantAccess(const Name &entity, const Name &dataType) {}

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