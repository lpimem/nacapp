#include "manager.hpp"

namespace nacapp {

string Manager::getGroupFullName(const Name &group, const Name &dataType) {
  Name name(group);
  name.append(NAME_COMPONENT_READ);
  name.append(dataType);
  return name.toUri();
}

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

void Manager::grantAccess(const Name &entity, const Name &dataType) {
  addGroupMember(m_prefix, dataType, entity);
}

void Manager::revokeAccess(const Name &entity, const Name &dataType) {}

shared_ptr<GroupManager> Manager::createGroup(const Name &group,
                                              const Name &dataType) {
  string groupFullName = getGroupFullName(group, dataType);
  if (m_groups.find(groupFullName) != m_groups.end()) {
    return m_groups[groupFullName];
  }
  auto groupManager =
      make_shared<GroupManager>(group, dataType, DB_PATH, 1024, 1);
  createSchedule(groupManager);
  m_groups[groupFullName] = groupManager;
  return groupManager;
}

void Manager::deleteGroup(const Name &group, const Name &dataType) {
  string groupFullName = getGroupFullName(group, dataType);
  m_groups.remove(groupFullName);
}

/**
 * TODO: wrong if identity wasn't authenticated.
 *        cant't find API in keyring.
 *        app need to manage this information?
 * Note: interest handler should parse dataType and chech if it should be
 * allowed to proceed.
 */
void Manager::addGroupMember(const Name &group, const Name &dataType,
                             const Name &identity) {
  shared_ptr<GroupManager> groupManager = createGroup(group, dataType);
  Certificate cert = getEntityCert(identity);
  groupManager->addMember(SCHEDULE_NAME_EVERY_DAY, cert);
}

void Manager::removeGroupMember(const Name &group, const Name &dataType,
                                const Name &identity) {
  shared_ptr<GroupManager> groupManager = createGroup(group, dataType);
  Certificate cert = getEntityCert(identity);
  groupManager->removeMember(identity);
}

/**
 * Create a schedule for unlimited access within one year.
 * TODO: renew after expires.
 */
void Manager::createSchedule(shared_ptr<GroupManager> group) {
  Schedule schedule;
  ptime now = second_clock::local_time();
  ptime future(now + years(1));
  auto days = (future.date() - now.date()).days();
  RepetitiveInterval oneYearFromNow(now, future, 0, 24, days,
                                    RepetitiveInterval::RepeatUnit::DAY);
  schedule.addWhiteInterval(oneYearFromNow);
  group->addSchedule(SCHEDULE_NAME_EVERY_DAY, schedule);
}

Certificate Manager::getEntityCert(const Name &entity) {
  Identity id = AppKeyChain.createIdentity(entity);
  return id.getDefaultKey().getDefaultCertificate();
}

} // nacapp