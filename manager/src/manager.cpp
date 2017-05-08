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

NamedInterval Manager::defaultSchedule(const Name &dataType) {
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

void Manager::grantAccess(const Name &entity, const Name &dataType,
                          const NamedInterval &namedInterval) {
  shared_ptr<GroupManager> groupManager = createGroup(dataType);
  string schedule = createSchedule(groupManager, namedInterval);
  addGroupMember(dataType, entity, schedule);
}

void Manager::revokeAccess(const Name &entity, const Name &dataType) {
  removeGroupMember(dataType, entity);
}

shared_ptr<GroupManager> Manager::getGroup(const Name &dataType) {
  const string groupFullName = getGroupFullName(m_prefix, dataType);
  if (m_groups.find(groupFullName) == m_groups.end()) {
    return NULL;
  }
  return m_groups[groupFullName];
}

shared_ptr<GroupManager> Manager::createGroup(const Name &dataType) {
  string groupFullName = getGroupFullName(m_prefix, dataType);
  if (m_groups.find(groupFullName) != m_groups.end()) {
    LOG(DEBUG) << groupFullName << "already created";
    return m_groups[groupFullName];
  }
  auto groupManager =
      make_shared<GroupManager>(groupFullName, dataType, DB_PATH, 1024, 1);
  m_groups[groupFullName] = groupManager;
  LOG(DEBUG) << groupFullName << " created";
  return groupManager;
}

void Manager::deleteGroup(const Name &dataType) {
  string groupFullName = getGroupFullName(m_prefix, dataType);
  m_groups.erase(groupFullName);
}

/**
 * Note: interest handler should parse dataType and chech if it should be
 * allowed to proceed.
 */
void Manager::addGroupMember(const Name &dataType, const Name &identity,
                             const string &scheduleName) {
  shared_ptr<GroupManager> groupManager = createGroup(dataType);
  Certificate cert = getEntityCert(identity);
  LOG(DEBUG) << "granting access for cert [1] " << cert.getFullName().toUri();
  groupManager->addMember(scheduleName, cert);
  LOG(DEBUG) << "granting access for cert [2] " << cert.getFullName().toUri();
}

void Manager::removeGroupMember(const Name &dataType, const Name &identity) {
  shared_ptr<GroupManager> groupManager = createGroup(dataType);
  Certificate cert = getEntityCert(identity);
  groupManager->removeMember(identity);
}

string Manager::createSchedule(shared_ptr<GroupManager> group,
                               const NamedInterval &namedInterval) {
  const string iname = namedInterval.getName().toUri();
  if (m_group_schedules.find(iname) == m_group_schedules.end()) {
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