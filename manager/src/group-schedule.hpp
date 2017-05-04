#ifndef GROUP_SCHEDULE_HPP
#define GROUP_SCHEDULE_HPP

#include "common.hpp"

namespace nacapp {

/**
 * Every group has only one schedule.
 */
class GroupSchedule {
public:
  GroupSchedule(shared_ptr<GroupManager> group, shared_ptr<Schedule> schedule)
      : m_group(group), m_schedule(schedule) {}

public:
  shared_ptr<GroupManager> getGroup() { return m_group; }
  shared_ptr<Schedule> getSchedule() { return m_schedule; }

private:
  shared_ptr<GroupManager> m_group;
  shared_ptr<Schedule> m_schedule;
};
} // nacapp

#endif /* GROUP_SCHEDULE_HPP */
