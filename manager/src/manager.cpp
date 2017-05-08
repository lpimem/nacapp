#include "manager.hpp"

namespace nacapp {

string Manager::getGroupFullName(const Name &group, const Name &dataType) {
  Name name(group);
  name.append(NAME_COMPONENT_READ);
  name.append(dataType);
  return name.toUri();
}

void Manager::addIdentity(const Name &entity, const Data &certData) {
  auto certificate = make_shared<Certificate>(certData);
  validateCertificate(entity, certificate);
  m_identities[entity.toUri()] = certificate;
}

shared_ptr<Certificate> Manager::getIdentity(const Name &entity) {
  string key = entity.toUri();
  if (m_identities.find(key) != m_identities.end()) {
    return m_identities[key];
  }
  return NULL;
}

void Manager::validateCertificate(const Name &identity,
                                  shared_ptr<Certificate> cert) {
  auto certIdentity = cert->getIdentity();
  if (certIdentity != identity) {
    throw "validateCertificate: identity not match";
  }
  if (!cert->isValid()) {
    throw "validateCertificate: certificate is already expired";
  }
}

void Manager::removeIdentity(const Name &identity) {
  LOG(DEBUG) << "removing identity " << identity.toUri();
  m_identities.erase(identity.toUri());
}

void Manager::grantAccess(const Name &entity, const Name &dataType,
                          const NamedInterval &namedInterval) {
  string schedule = namedInterval.getName().toUri();
  schedule = createSchedule(dataType, namedInterval);
  addGroupMember(dataType, entity, schedule);
}

NamedInterval defaultSchedule(const Name &dataType) {
  ptime now = second_clock::local_time();
  ptime future(now + years(1));
  auto days = (future.date() - now.date()).days();
  RepetitiveInterval oneYearFromNow(now, future, 0, 24, days,
                                    RepetitiveInterval::RepeatUnit::DAY);
  NamedInterval ni(SCHEDULE_NAME_EVERY_DAY, oneYearFromNow);
  return ni;
}

void Manager::grantAccess(const Name &entity, const Name &dataType) {
  NamedInterval namedInterval = defaultSchedule(dataType);
  grantAccess(entity, dataType, namedInterval);
}

void Manager::revokeAccess(const Name &entity, const Name &dataType) {
  removeGroupMember(dataType, entity);
}

shared_ptr<GroupManager> Manager::getGroup(const Name &entity,
                                           const Name &dataType) {
  const string groupFullName = getGroupFullName(m_prefix, dataType);
  if (m_groups.find(groupFullName) == m_groups.end()) {
    return NULL;
  }
  return m_groups[groupFullName];
}

shared_ptr<GroupManager> Manager::createGroup(const Name &dataType) {
  string groupFullName = getGroupFullName(m_prefix, dataType);
  if (m_groups.find(groupFullName) != m_groups.end()) {
    return m_groups[groupFullName];
  }
  auto groupManager =
      make_shared<GroupManager>(groupFullName, dataType, DB_PATH, 1024, 1);
  m_groups[groupFullName] = groupManager;
  return groupManager;
}

void Manager::deleteGroup(const Name &dataType) {
  string groupFullName = getGroupFullName(m_prefix, dataType);
  m_groups.erase(groupFullName);
}

/**
 * TODO: wrong if identity wasn't authenticated.
 *        cant't find API in keyring.
 *        app need to manage this information?
 * Note: interest handler should parse dataType and chech if it should be
 * allowed to proceed.
 */
void Manager::addGroupMember(const Name &dataType, const Name &identity,
                             const string &scheduleName) {
  shared_ptr<GroupManager> groupManager = createGroup(dataType);
  Certificate cert = getEntityCert(identity);
  groupManager->addMember(scheduleName, cert);
}

void Manager::removeGroupMember(const Name &dataType, const Name &identity) {
  shared_ptr<GroupManager> groupManager = createGroup(dataType);
  Certificate cert = getEntityCert(identity);
  groupManager->removeMember(identity);
}

string Manager::createSchedule(const Name &dataType,
                               const NamedInterval &namedInterval) {
  const string dataTypeStr = dataType.toUri();
  if (m_groups.find(dataTypeStr) == m_groups.end()) {
    throw "no matching group.";
  }
  const string iname = namedInterval.getName().toUri();
  if (m_group_schedules.find(iname) == m_group_schedules.end()) {
    shared_ptr<GroupManager> group = m_groups[dataTypeStr];
    Schedule schedule;
    schedule.addWhiteInterval(namedInterval.getInterval());
    group->addSchedule(iname, schedule);
  }
  return iname;
}

Certificate Manager::getEntityCert(const Name &entity) {
  Identity id = AppKeyChain.createIdentity(entity);
  return id.getDefaultKey().getDefaultCertificate();
}

} // nacapp