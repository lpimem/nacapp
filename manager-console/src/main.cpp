#include "../../shared/src/common-cxx.hpp"
#include "../../shared/src/common-ndn.hpp"

using namespace nacapp;

const string CMD_ADD_IDENTITY = "MANAGEMENT/identity/add";
const string CMD_RM_IDENTITY = "MANAGEMENT/identity/remove";
const string CMD_GRANT = "MANAGEMENT/access/grant";
const string CMD_REVOKE = "MANAGEMENT/access/revoke";

class ControlClient
{
public:
  ControlClient(string prefix) : m_prefix(prefix) {}

private:
  string m_prefix;
};